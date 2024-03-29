/**
 * @file Movements.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains all the
 * high level functions used throughout
 * XFactor's movements. Any low end functions
 * should be in another file.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Movements/Movements.hpp"



// - GLOBAL VARIABLES - //
int32_t rightPulse = 0;
int32_t leftPulse  = 0;
int32_t previousRightPulse = 0;
int32_t previousLeftPulse  = 0;
double completionRatio = 0.0;

float gMaxSpeed = SPEED_MAX;

float rightMovement = 0.0f;
float rotationMovement = 0.0f;

int direction = 0;
float targetTicks = 0;

float currentSpeed = 0.0f;
float speedLeft    = 0.0f;

unsigned long previousInterval_ms = 0;

bool checkForSensors = true;
bool checkAlarmEnabled = true;
bool examineModeEnabled = false;

int distanceSensorCounter = 0;

//#pragma region Base_functions
/**
 * @brief
 * This function moves the robot on a specified
 * vector allowing it to keep track of where it
 * is relative to the box. The robot will start
 * by turning on itself to reach the wanted
 * radian value. It will then move the given
 * distance in a straight line after which the
 * vector will be saved in the buffer.
 * @param radians
 * The relative radians that the robot should turn
 * before it starts to move forwards. The radians
 * is relative to the current direction that the
 * robot is facing. Meaning 0 will always be nothing
 * and 3.14 will always be a 180 turn.
 * @param distance
 * The distance in centimeters that the robot should
 * make. In other words, the length of the vector.
 * @param saveVector
 * This parameter tells the function wether it should
 * try to save the vector in the vector buffer or not.
 * @return true: Successfully made the specified vector
 * @return false: Failed to move to the specified vector
 * or the vector cannot be saved in the buffer for X
 * reason.
 */
int MoveFromVector(float radians, float distance, bool saveVector, bool checkSensors, bool checkAlarm, bool examineMode, float maxSpeed)
{
    Debug_Start("MoveFromVector");
    checkForSensors = checkSensors;
    checkAlarmEnabled = checkAlarm;
    examineModeEnabled = examineMode;
    gMaxSpeed = maxSpeed;

    rightMovement    = 0;
    rotationMovement = 0;

    int turnStatus = MOVEMENT_COMPLETED;
    int moveStatus = MOVEMENT_COMPLETED;
    int status = MOVEMENT_COMPLETED;

    if (radians > PI)
    {
        radians = radians - 2 * PI;
    }
    else if (radians < -PI)
    {
        radians = radians + 2 * PI;
    }

    if(!ResetMovements())
    {
        Debug_Error("Movements", "MoveFromVector", "Failed to reset movements");
        return MOVEMENT_ERROR;
    }

    if (radians != 0)
    {
        turnStatus = Execute_Turning(radians);
        Debug_Information("Movements", "MoveFromVector", "turnStatus : " + String(turnStatus));
        if (turnStatus == MOVEMENT_ERROR)
        {
            Debug_Error("Movements", "MoveFromVector", "Failed to turn in radians");
            return turnStatus;
        }
        else if (turnStatus == ALARM_TRIGGERED)
        {
            return turnStatus;
        }
        status = turnStatus;
    }

    if (distance != 0 && turnStatus == MOVEMENT_COMPLETED)
    {
        moveStatus = Execute_Moving(distance, radians);
        Debug_Information("Movements", "MoveFromVector", "moveStatus : " + String(moveStatus));
        if (moveStatus == MOVEMENT_ERROR)
        {
            Debug_Error("Movements", "MoveFromVector", "Failed to go straight");
            return moveStatus;
        }
        else if (moveStatus == ALARM_TRIGGERED)
        {
            return moveStatus;
        }
        status = moveStatus;
    }

    if (saveVector){
        if(!UpdateSavedDistance(rightMovement))
        {
            Debug_Error("Movements", "MoveFromVector", "Failed to update distance");
            Debug_End();
            return MOVEMENT_ERROR;
        }
        if(!UpdateSavedRotation(rotationMovement))
        {
            Debug_Error("Movements", "MoveFromVector", "Failed to update rotations");
            Debug_End();
            return MOVEMENT_ERROR;
        }
        if(!SaveNewVector()) 
        {
            Debug_Error("Movements", "MoveFromVector", "Failed to save new vector");
            Debug_End();
            return MOVEMENT_ERROR;
        }
        UpdateSavedPosition();
    }
    
    return status;
}

/**
 * @brief
 * This function backtraces the robot by using
 * the saved vectors in the vector buffer.
 * Firstly, it will rotate the robot so that its
 * rotation is the exact opposite of the previous
 * vector in the buffer. After that, it will
 * start to backtrace for a specified amount of
 * vectors or until he vector table is empty.
 * Once the specified amount of vectors have been
 * backtraced, the robot will turn around on
 * itself again to face where the rotation value
 * of the last vector in the buffer.
 * @param AmountOfVectorsToBacktrace
 * How many vectors should the robot backtrace?
 * If the number is bigger than the amount of
 * saved vectors in the vector buffer, the
 * function will stop once the buffer is empty.
 * @return true:
 * Successfully backtraced the amount of
 * specified vectors
 * @return false:
 * Failed to backtraced the amount of
 * specified vectors due to possible errors or
 * because there is no more vectors to backtrace.
 */
bool BacktraceSomeVectors(int AmountOfVectorsToBacktrace)
{
    Debug_Start("BacktraceSomeVectors");
    for(int i = 0; i<AmountOfVectorsToBacktrace; i++){
        MovementVector backtraceVector = GetLastOppositeVector();
        Debug_Information("Movements.cpp", "BacktraceSomeVectors", "Rotation : " + String(backtraceVector.rotation_rad,2) + " Distance : " + String(backtraceVector.distance_cm, 2));
        MoveFromVector(BACKTRACE_VECTOR);
        RemoveLastVector();
    }
    Debug_End();
    return true;
}

/**
 * @brief Makes the robot turn on itself relative
 * to a given radian. 0 corresponds to the front
 * of the robot (AKA not turning), and 3.14
 * corresponds to a full 180 turn.
 * @param radians
 * The relative radians that the robot should turn
 * before it starts to move forwards. The radians
 * is relative to the current direction that the
 * robot is facing. Meaning 0 will always be nothing
 * and 3.14 will always be a 180 turn.
 * 
 * Giving it a negative value will make it turn
 * the opposite direction.
 * @return true
 * The robot successfully turned by X radians
 * @return false
 * The robot failed to turn by the given radian.
 */
bool TurnInRadians(float radians)
{
    Debug_Start("TurnInRadians");
    if (!ResetPID())
    {
        Debug_Error("Movements", "TurnInRadians", "Failed to reset PID");
        Debug_End();
        return false;
    }
    else if (!ResetAllEncoders())
    {
        Debug_Error("Movements", "TurnInRadians", "Failed to reset encoders");
        Debug_End();
        return false;
    }
    else if (!ResetParameters())
    {
        Debug_Error("Movements", "TurnInRadians", "Failed to reset parameters");
        Debug_End();
        return false;
    }

    targetTicks = CentimetersToEncoder(abs(radians)*ARC_CONSTANT_CM);

    if (radians <= 0) direction = TURN_RIGHT;
    else direction = TURN_LEFT;

    Debug_End();
    return true;
}

/**
 * @brief Makes the robot go in a
 * straight line until the specified
 * distance is reached.
 * @param distance
 * The distance in centimeters that the robot should
 * make. In other words, the length of the vector.
 * @return true
 * The robot successfully drove to the
 * wanted distance
 * @return false
 * The robot failed to reach the wanted distance.
 */
bool MoveStraight(float distance)
{
    Debug_Start("MoveStraight");
    if (!ResetPID())
    {
        Debug_Error("Movements", "MoveStraight", "Failed to reset PID");
        Debug_End();
        return false;
    }
    else if (!ResetAllEncoders())
    {
        Debug_Error("Movements", "MoveStraight", "Failed to reset all encoders");
        Debug_End();
        return false;
    }
    else if (!ResetParameters())
    {
        Debug_Error("Movements", "MoveStraight", "Failed to reset parameters");
        Debug_End();
        return false;
    }

    targetTicks = CentimetersToEncoder(abs(distance));

    if (distance >= 0) direction = MOVEMENT_FORWARD;
    else direction = MOVEMENT_BACKWARD;

    Debug_End();
    return true;
}

/**
 * @brief Calculate the wanted speed factor depending on how much
 * distance is left to move to reach its wanted destination.
 * This allows the robot to perform an acceleration when starting
 * its movement and a de acceleration when its nearing the end of
 * its distance.
 *
 * This is important to make the robot go faster than an instant
 * acceleration that would otherwise cause slip and potential
 * drifting, meaning loss in accuracy.
 * @param completionRatio
 * ratio going from 0 to 1 representing how much of the chosen
 * movement the robot has completed. Used to represent the x in
 * the parabolic function.
 * @param maximumSpeed
 * a ratio from 0 to 1 that indicates the maximum speed of
 * the wheels.
 * @return float
 * Ratio from 0 to 1 that should be multiplied to the
 * current PID value
 */
float Accelerate(float completionRatio, float maximumSpeed)
{
    double neededSpeed = 0;
    if ((completionRatio >= 0) && completionRatio <= 100)
    {
        neededSpeed = (pow(sin(completionRatio * 3.14),2) * maximumSpeed)+0.1;
        if(neededSpeed>maximumSpeed) neededSpeed = maximumSpeed;
        return neededSpeed;
    }
    else 
    {
        Debug_Error("Movements", "Accelerate", "Ratio is out of bounds: " + String(completionRatio, 2));
        return 0.0f;
    }
    return 0.0f;
}

/**
 * @brief Stops the robot no matter what.
 * This function should simply stop
 * the motors
 * @return true
 * Successfully stopped the robot
 * @return false
 * Something failed and the robot could not be stopped.
 */
bool Stop()
{
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT,  0);
    if (ResetAllEncoders()) return true;
    else
    {
        Debug_Error("Movements", "Stop", "Failed to reset encoders");
        return true;
    }
}

/**
 * @brief Function that must be called
 * before the robot starts to execute
 * its movements. It will reset the
 * PID as well as encoders. It should
 * also reset current rotation and
 * distance.
 * @return true:
 * Successfully reset movements.
 * @return false:
 * Failed to reset the movements.
 */
bool ResetMovements()
{
    if (ResetAllEncoders()){
        if (ResetPID()){
            return true;
        }
        else Debug_Error("Movements", "ResetMovements", "Failed to reset PID");
    }
    else Debug_Error("Movements", "ResetMovements", "Failed to reset encoders");
    return false;    
}

/**
 * @brief Function that must be called
 * before the robot starts to execute
 * every Execute. It will reset the
 * variables used during the movement.
 * @return true:
 * Successfully reset parameters.
 * @return false:
 * Failed to reset the parameters.
 */
bool ResetParameters()
{
    rightPulse = 0;
    leftPulse  = 0;
    previousRightPulse = 0;
    previousLeftPulse  = 0;
    completionRatio = 0.0;

    direction = 0;
    targetTicks = 0;

    currentSpeed = 0.0f;
    speedLeft    = 0.0f;

    previousInterval_ms = 0;
    return true;    
}
//#pragma endregion

//#pragma region Execution_Functions

/**
 * @brief Function that executes the
 * turning of the robot.
 * The program will block inside this
 * function until the robot has finished
 * turning or something prevents it from
 * continuing to turn.
 *
 * This function should periodically
 * execute PID and acceleration each
 * milliseconds. While it waits for that
 * millisecond, TurningEvent is called.
 *
 * @param targetRadians
 * Relative turn that the robot needs
 * to turn.
 * @return true:
 * Successfully executed the turning
 * sequence
 * @return false:
 * Failed to execute the turning sequence
 */
int Execute_Turning(float targetRadians)
{
    int32_t absoluteOfRightPulse = 0;
    if (!TurnInRadians(targetRadians))
    {
        Debug_Error("Movements", "Execute_Turning", "Could not get the target movement");
        return MOVEMENT_ERROR;
    }

    targetTicks = targetTicks*CONSTANT_RATIO_TURN;

    int status = MOVEMENT_COMPLETED;
    
    SetMotorSpeed(LEFT, (float)direction*-1.0f*currentSpeed);
    SetMotorSpeed(RIGHT, (float)direction*currentSpeed);
    
    while(completionRatio <= 1){
        if((millis()-previousInterval_ms)>PID_INTERVAL_MS){
            rightPulse = abs(ENCODER_Read(RIGHT));
            leftPulse  = abs(ENCODER_Read(LEFT));

            absoluteOfRightPulse = abs(rightPulse);
            completionRatio = ((float)absoluteOfRightPulse)/targetTicks;

            currentSpeed = Accelerate(completionRatio, gMaxSpeed / 2);

            speedLeft = PID(PID_MOVEMENT, (leftPulse-previousLeftPulse), (rightPulse-previousRightPulse), currentSpeed);

            SetMotorSpeed(LEFT, (float)direction*-1.0f*speedLeft);
            SetMotorSpeed(RIGHT, (float)direction*currentSpeed);
            previousLeftPulse  = leftPulse;
            previousRightPulse = rightPulse;
            previousInterval_ms = millis();
        }
    
        if (checkAlarmEnabled)
        {
            if (Alarm_VerifySensors())
            {
                Debug_Information("Movements.cpp", "Execute_Turning", "STATUS_ALARM_TRIGGERED");
                return ALARM_TRIGGERED;
            }
        }

        if (checkForSensors)
        {
            if(distanceSensorCounter == 0)
            {
                if (Package_Detected(FRONT_SENSOR, targetRadians, 0.0f) == PACKAGE_DETECTED)
                {
                    status = OBJECT_LOCATED_FRONT;
                    break;
                } 
                //distanceSensorCounter += 1;
            }
            /*else if(distanceSensorCounter == 1)
            {
                if (Package_Detected(LEFT_SENSOR, targetRadians, 0.0f) == 1)
                {
                    status = OBJECT_LOCATED_LEFT;
                    break;
                } 
                distanceSensorCounter += 1;
            }
            else if(distanceSensorCounter == 2)
            {
                if (Package_Detected(RIGHT_SENSOR, targetRadians, 0.0f) == 1)
                {
                    status = OBJECT_LOCATED_RIGHT;
                    break;
                } 
                distanceSensorCounter = 0;
            }*/
        }

        if (examineModeEnabled)
        {
            if(Package_Confirmed())
            {
                Debug_Information("Movements.cpp", "Execute_Turning", "STATUS_PACKAGE_DETECTED");
                status = PACKAGE_FOUND;
                break;
            }
        }
            
    }

    rotationMovement = -(EncoderToCentimeters((float)ENCODER_Read(RIGHT)))*ARC_TICK_TO_CM; // TO LOOK AT

    if(!Stop())
    {
        Debug_Error("Movements", "Execute_Turning", "Failed to stop");
        status = MOVEMENT_ERROR;
    }
    return status;
}

/**
 * @brief Function that executes the
 * moving of the robot.
 * The program will block inside this
 * function until the robot has finished
 * moving or something prevents it from
 * continuing to go straight.
 *
 * This function should periodically
 * execute @ref PID and @ref Acceleration
 * each milliseconds. While it waits for
 * that millisecond, @ref MovingEvent is
 * called.
 *
 * @param targetDistance
 * Relative distance that the robot needs
 * to move in a straight line. Can be
 * negative for reversing.
 * @return true:
 * Successfully executed the moving
 * sequence
 * @return false:
 * Failed to execute the moving sequence
 */
int Execute_Moving(float targetDistance, float targetRadians)
{
    Debug_Start("Execute_Moving");
    int32_t absoluteOfRightPulse = 0;

    if (!MoveStraight(targetDistance))
    {
        Debug_Error("Movements", "Execute_Turning", "Could not get the target movement");
        Debug_End();
        return false;
    }

    targetTicks = targetTicks*CONSTANT_RATIO_STRAIGHT;

    int status = MOVEMENT_COMPLETED;

    SetMotorSpeed(LEFT, (float)direction*currentSpeed);
    SetMotorSpeed(RIGHT, (float)direction*currentSpeed);
    
    while(completionRatio<1){
        // PID called each 10 milliseconds
        if((millis()-previousInterval_ms)>PID_INTERVAL_MS){
            rightPulse = abs(ENCODER_Read(RIGHT));
            leftPulse  = abs(ENCODER_Read(LEFT));

            absoluteOfRightPulse = abs(rightPulse);
            completionRatio = ((float)absoluteOfRightPulse)/targetTicks;

            currentSpeed = Accelerate(completionRatio, gMaxSpeed);

            speedLeft = PID(PID_MOVEMENT,
                            leftPulse-previousLeftPulse, 
                            rightPulse-previousRightPulse, 
                            currentSpeed);
            
            SetMotorSpeed(LEFT, (float)direction*speedLeft);
            SetMotorSpeed(RIGHT, (float)direction*currentSpeed);
            
            previousLeftPulse  = leftPulse;
            previousRightPulse = rightPulse;
            previousInterval_ms = millis();
        }

        if (checkForSensors)
        {
            if(distanceSensorCounter == 0)
            {
                if (Package_Detected(FRONT_SENSOR, targetRadians, completionRatio * targetDistance) == 1)
                {
                    status =  OBJECT_LOCATED_FRONT;
                    break;
                } 
                //distanceSensorCounter += 1;
            }
            /*else if(distanceSensorCounter == 1)
            {
                if (Package_Detected(LEFT_SENSOR, targetRadians, completionRatio * targetDistance) == 1)
                {
                    status = OBJECT_LOCATED_LEFT;
                    break;
                } 
                distanceSensorCounter += 1;
            }
            else if(distanceSensorCounter == 2)
            {
                if (Package_Detected(RIGHT_SENSOR, targetRadians, completionRatio * targetDistance) == 1)
                {
                    status = OBJECT_LOCATED_RIGHT;
                    break;
                } 
                distanceSensorCounter = 0;
            }*/
        }
        if (examineModeEnabled)
        {
            if(Package_Confirmed())
            {
                Debug_Information("Movements.cpp", "Execute_Moving", "STATUS_PACKAGE_DETECTED");
                status = PACKAGE_FOUND;
                break;
            }
        }

        if (checkAlarmEnabled)
        {
            if (completionRatio <= 0.85f && Alarm_VerifySensors())
            {
                Debug_Information("Movements.cpp", "Execute_Moving", "STATUS_ALARM_TRIGGERED");
                status = ALARM_TRIGGERED;
                break;
            }
        }
    }
    Debug_Information("Movements", "Execute_Moving", "Exited while loop");
    rightMovement = EncoderToCentimeters(abs((float)ENCODER_Read(RIGHT)));

    if (targetDistance < 0)
    {
        rightMovement = EncoderToCentimeters((float)ENCODER_Read(RIGHT));
    }

    if(!Stop())
    {
        Debug_Error("Movements", "Execute_Moving", "Failed to stop");
        status = MOVEMENT_ERROR;
    }

    Debug_End();
    return status;
}

//#pragma endregion