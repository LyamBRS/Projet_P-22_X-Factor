/**
 * @file Motors.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains the functions
 * and variables used for basic motor handling
 * for ROBUS. These are mainly redefinition of
 * existing functions and allows portability of
 * the program.
 * @version 0.1
 * @date 2023-10-23
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Outputs/Motors/DC/Motors.hpp"

/**
 * @brief Function that resets
 * all the encoders of XFactor.
 * @return true:
 * Successfully reset all encoders
 * @return false:
 * Failed to reset all encoders, some
 * values are still read.
 */
bool ResetAllEncoders()
{
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    return true;
}

/**
 * @brief Returns the encoder value
 * of the specified motor. Specified
 * motors can either be 0 or 1.
 * Anything above should return 0.
 * @param motorNumber
 * Number ranging from 0 to 1 which
 * specifies that it should return
 * either the LEFT motor or the RIGHT
 * motor.
 * @return int
 * The amount of ticks that the
 * specified encoder has read.
 */
int GetAnEncoder(int motorNumber)
{
    if (motorNumber == LEFT || motorNumber == RIGHT){
        return ENCODER_Read(motorNumber);
    }
    else {
        Debug_Error("Motors","GetAnEncoder","Motor called is neither Left (0) or Right (1).");
        return 0;
    }
}

/**
 * @brief Sets a specified motor
 * to a specified speed value ranging
 * from -1 to 1.
 * @param motorNumber
 * Number ranging from 0 to 1 which
 * specifies that it should return
 * either the LEFT motor or the RIGHT
 * motor.
 * @param wantedSpeed
 * Sets the specified motor to this
 * specified speed. Must be a float
 * number ranging from -1.0f to 1.0f
 * @return true:
 * Successfully changed the motor's
 * speed.
 * @return false:
 * Failed to change the motor's
 * speed.
 */
bool SetMotorSpeed(int motorNumber, float wantedSpeed)
{
    if (motorNumber == LEFT || motorNumber == RIGHT){
        if(wantedSpeed >= -1 && wantedSpeed <= 1){
            MOTOR_SetSpeed(motorNumber, wantedSpeed);
            return true;
        }
        else {
            Debug_Error("Motors","SetMotorSpeed","Speed not within the thresholds [-1,1] : " + String(wantedSpeed));
            return false;
        }
    }
    else {
        Debug_Error("Motors","SetMotorSpeed","Motor called is neither Left (0) or Right (1).");
        return false;
    }
    
}

/**
 * @brief Simple function that transforms
 * a number expressed in ticks into its
 * distance equivalence in centimeters.
 * This should prevent you from manually
 * having to calculate this conversion
 * each time its needed.
 * @param ticks
 * The number of ticks that the encoder
 * has read.
 * @return float
 * value of the ticks converted to
 * centimeters.
 */
float EncoderToCentimeters(int ticks)
{
    float distance_cm = ((float)ticks * CIRCUMFERENCE_WHEEL_CM / 3200);
    return distance_cm;
}

/**
 * @brief Simple function that transforms
 * a number expressed in centimeters into
 * its distance equivalence in ticks.
 * This should prevent you from manually
 * having to calculate this converstion
 * each time its needed.
 * @param distance_cm
 * The number of centimeters you want to 
 * reach
 * @return float
 * value of the centimeters converted to
 * ticks. 
 */
float CentimetersToEncoder(float distance_cm)
{
    float pulse = ((float)distance_cm / CIRCUMFERENCE_WHEEL_CM) * 3200.0f;
    return pulse;
}
