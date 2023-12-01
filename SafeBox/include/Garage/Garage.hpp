/**
 * @file Garage.hpp
 * @author your name (you@domain.com)
 * @brief
 * File containing the header definitions of
 * functions used to handle SafeBox's garage.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "Outputs/Motors/Servo/S3003.hpp"
#include "Sensors/Distance/GP2D12.hpp"
#include "Debug/Debug.hpp"

// - DEFINES - //
/// @brief PWM value of the open servo motor when the garage is opened
#define ANGLE_OPEN 75
/// @brief PWM value of the closed servo moor when the garage is closed
#define ANGLE_CLOSED 215

/// @brief Distance sensor arduino pin where the echo (returned pulse) is read.
#define GARAGE_ECHO_PIN 11
/// @brief Distance sensor arduino pin where the trigger for the pulse is sent.
#define GARAGE_TRIG_PIN 10

/// @brief the maximum value for the door to be considered closed
#define GARAGE_DISTANCE_VALUE_CLOSED 50
/// @brief the minimum value for the door to be considered opened
#define GARAGE_DISTANCE_VALUE_OPEN 90

#define GARAGE_IS_CLOSED_DEBUG_PIN 5

/**
 * @brief 
 * Returns wether the garage SHOULD be closed
 * or not.
 * @return true:
 * Should be opened.
 * @return false:
 * Should be closed.
 */
bool Garage_GetSupposedWantedStatus();

/**
 * @brief
 * Initialises the garage of SafeBox.
 * Initialises the servo motors and sensors used
 * to control the garage of SafeBox.
 * @return true:
 * Successfully initialised all the sensors and
 * motors
 * @return false
 */
bool Garage_Init();

/**
 * @brief
 * Function that attempts to open the garage of
 * SafeBox.
 * @return true:
 * Successfully opened the garage.
 * @return false:
 * Failed to open the garage.
 */
bool Garage_Open();

/**
 * @brief
 * Function that attempts to close the garage of
 * SafeBox.
 * @return true:
 * Successfully closed the garage.
 * @return false:
 * Failed to close the garage.
 */
bool Garage_Close();

/**
 * @brief
 * Function that analyses the distance sensor
 * that is inside SafeBox's garage and tells the
 * program if XFactor is inside.
 *
 * @return true:
 * XFactor is detected inside the garage.
 * @return false:
 * XFactor is not inside of the garage.
 */
bool Garage_XFactorInside();

/**
 * @brief
 * Function that analyses the distance sensor
 * that is inside SafeBox's garage and tells the
 * program if XFactor is inside.
 *
 * @return true:
 * The distance sensor sees the door as closed.
 * @return false:
 * XFactor is either inside or the door is closed.
 */
bool Garage_IsClosed();

/**
 * @brief Enables the debug light.
 * Is on if its closed and off
 * if its open.
 */
void Garage_ShowDebugLight();