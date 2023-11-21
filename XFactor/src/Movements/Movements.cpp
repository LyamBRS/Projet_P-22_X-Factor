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
int rightPulse = 0;
int leftPulse  = 0;
int previousRightPulse = 0;
int previousLeftPulse  = 0;
double completionRatio = 0.0;

float rightMovement = 0.0f;
//float leftMovement = 0.0f;
float rotationMovement = 0.0f;

int direction = 0;
float targetTicks = 0;

float currentSpeed = 0.0f;
float speedLeft    = 0.0f;

unsigned long previousInterval_ms = 0;

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
bool MoveFromVector(float radians, float distance, bool saveVector)
{
    rightMovement    = 0;
    rotationMovement = 0;

    if(!ResetMovements())
    {
        Debug_Error("Movements", "MoveFromVector", "Failed to reset movements");
        return false;
    }
    if(!TurnInRadians(radians))
    {
        Debug_Error("Movements", "MoveFromVector", "Failed to turn in radians");
        return false;
    }
    if(!MoveStraight(distance))
    {
        Debug_Error("Movements", "MoveFromVector", "Failed to go straight");
        return false;
    }

    if (saveVector){
        if(!UpdateSavedDistance(rightMovement))
        {
            Debug_Error("Movements", "MoveFromVector", "Failed to update distance");
            return false;
        }
        if(!UpdateSavedRotation(rotationMovement))
        {
            Debug_Error("Movements", "MoveFromVector", "Failed to update rotations");
            return false;
        }
        if(!SaveNewVector()) 
        {
            Debug_Error("Movements", "MoveFromVector", "Failed to save new vector");
            return false;
        }
    }
    return true;
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
    
    return false;
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
    if (!ResetPID())
    {
        Debug_Error("Movements", "TurnInRadians", "Failed to reset PID");
        return false;
    }
    else if (!ResetAllEncoders())
    {
        Debug_Error("Movements", "TurnInRadians", "Failed to reset encoders");
        return false;
    }
    else if (!ResetParameters())
    {
        Debug_Error("Movements", "TurnInRadians", "Failed to reset parameters");
        return false;
    }

    targetTicks = CentimetersToEncoder(abs(radians)*ARC_CONSTANT_CM);

    if (radians >= 0) direction = TURN_RIGHT;
    else direction = TURN_LEFT;

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
    if (!ResetPID())
    {
        Debug_Error("Movements", "MoveStraight", "Failed to reset PID");
        return false;
    }
    else if (!ResetAllEncoders())
    {
        Debug_Error("Movements", "MoveStraight", "Failed to reset all encoders");
        return false;
    }
    else if (!ResetParameters())
    {
        Debug_Error("Movements", "MoveStraight", "Failed to reset parameters");
        return false;
    }

    targetTicks = CentimetersToEncoder(distance);

    if (distance >= 0) direction = MOVEMENT_FORWARD;
    else direction = MOVEMENT_BACKWARD;

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
    if (completionRatio > 0 && completionRatio < 100)
    {
        return ACCELERATION_CONSTANT*square(completionRatio-0.5)+maximumSpeed; 
    }
    else 
    {
        Debug_Error("Movements", "Accelerate", "Ratio is out of bounds");
        return 0;
    }
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
        return false;
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
            ResetPositions();
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
bool Execute_Turning(float targetRadians)
{
    SetMotorSpeed(LEFT, (float)direction*-1.0f*currentSpeed);
    SetMotorSpeed(RIGHT, (float)direction*currentSpeed);
    
    while(TurningEvent(completionRatio, direction)){
        if((millis()-previousInterval_ms)>PID_INTERVAL_MS){
            rightPulse = abs((float)ENCODER_Read(RIGHT));
            leftPulse  = abs((float)ENCODER_Read(LEFT));
            completionRatio = rightPulse/targetTicks;

            currentSpeed = Accelerate(completionRatio, SPEED_MAX);

            speedLeft = PID(PID_MOVEMENT, (leftPulse-previousLeftPulse), (rightPulse-previousRightPulse), currentSpeed);

            SetMotorSpeed(LEFT, (float)direction*-1.0f*speedLeft);
            SetMotorSpeed(RIGHT, (float)direction*currentSpeed);
            previousLeftPulse  = leftPulse;
            previousRightPulse = rightPulse;
            previousInterval_ms = millis();
        }
    }

    rotationMovement = (EncoderToCentimeters((float)ENCODER_Read(RIGHT)))*ARC_TICK_TO_CM;

    if(!Stop())
    {
        Debug_Error("Movements", "Execute_Turning", "Failed to stop");
        return false;
    }
    return true;
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
bool Execute_Moving(float targetDistance)
{ 
    SetMotorSpeed(LEFT, (float)direction*currentSpeed);
    SetMotorSpeed(RIGHT, (float)direction*currentSpeed);
    
    while(MovingEvent(completionRatio, direction)){
        if((millis()-previousInterval_ms)>PID_INTERVAL_MS){
            rightPulse = abs((float)ENCODER_Read(RIGHT));
            leftPulse  = abs((float)ENCODER_Read(LEFT));
            completionRatio = rightPulse/targetTicks;

            currentSpeed = Accelerate(completionRatio, SPEED_MAX);

            speedLeft = PID(PID_MOVEMENT, (leftPulse-previousLeftPulse), (rightPulse-previousRightPulse), currentSpeed);

            SetMotorSpeed(LEFT, (float)direction*speedLeft);
            SetMotorSpeed(RIGHT, (float)direction*currentSpeed);
            
            previousLeftPulse  = leftPulse;
            previousRightPulse = rightPulse;
            previousInterval_ms = millis();
        }
    }

    if(!Stop())
    {
        Debug_Error("Movements", "Execute_Moving", "Failed to stop");
        return false;
    }

    rightMovement = EncoderToCentimeters(abs((float)ENCODER_Read(RIGHT)));
    //leftMovement  += EncoderToCentimeters(abs((float)ENCODER_Read(LEFT)));
    //if (rightMovement != leftMovement) rotationMovement += (float)atan();

    return true;
}

//#pragma endregion