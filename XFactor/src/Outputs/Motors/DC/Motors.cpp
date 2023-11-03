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
    return false;
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
    return 0;
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
    return false;
}

/**
 * @brief Simple function that transforms
 * a number expressed in ticks into its
 * distance equivalensce in centimeters.
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
    return 0.0f;
}
