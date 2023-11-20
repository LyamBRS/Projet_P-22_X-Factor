/**
 * @file Motors.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains the definitions
 * of functions and variables used for basic
 * motor handling for ROBUS. These are
 * mainly redefinition of existing functions
 * and allows portability of the program.
 * @version 0.1
 * @date 2023-10-23
 * 
 * @copyright Copyright (c) 2023
 */

#pragma once
#include "LibRobus.h"
#include "Debug/Debug.hpp"

// Diameter of the wheel (cm)
#define DIAMETER_WHEEL_CM 7.7f 

// Calculate the circumference of the whell depending on its diameter
#define CIRCUMFERENCE_WHEEL_CM (3.1416*DIAMETER_WHEEL_CM)

// Diameter between the wheels ; used for turning //18.6f A et 19.2f B
#define DISTANCE_BT_WHEEL_CM 18.6f 

// Constant for calculating the arc. See documentation (formula) for more information
//The goal is to avoid dividing every time
#define ARC_CONSTANT_CM (DISTANCE_BT_WHEEL_CM/2.0f)


/**
 * @brief Function that resets
 * all the encoders of XFactor.
 * @return true:
 * Successfully reset all encoders
 * @return false:
 * Failed to reset all encoders, some
 * values are still read.
 */
bool ResetAllEncoders();

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
int GetAnEncoder(int motorNumber);

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
bool SetMotorSpeed(int motorNumber, float wantedSpeed);

/**
 * @brief Simple function that transforms
 * a number expressed in ticks into its
 * distance equivalensce in centimeters.
 * This should prevent you from manually
 * having to calculate this converstion
 * each time its needed.
 * @param ticks
 * The number of ticks that the encoder
 * has read.
 * @return float
 * value of the ticks converted to
 * centimeters. 
 */
float EncoderToCentimeters(int ticks);

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
 * value of the ticks converted to
 * centimeters. 
 */
float CentimetersToEncoder(float distance_cm);
