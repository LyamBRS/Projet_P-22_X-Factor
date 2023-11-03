/**
 * @file Accelerometer.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing header definition of functions
 * used to make a basic accelerometer work.
 * @version 0.1
 * @date 2023-10-26
 * @copyright Copyright (c) 2023
 */

#pragma once

/**
 * @brief
 * Initialises an accelerometer to be used on
 * XFactor. The name of the accelerometer will
 * define how these functions are called.
 * @param accelerometerPin
 * @return true:
 * Successfully initialised the accelerometer
 * using the specifed pins.
 * @return false:
 * Failed to initialise the accelerometer.
 */
bool Accelerometer_Init(int accelerometerPin);

/**
 * @brief
 * Function that returns the X value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @param accelerometerPin:
 * The pin of the accelerometer that was
 * initialised
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetX(int accelerometerPin);

/**
 * @brief
 * Function that returns the Y value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @param accelerometerPin:
 * The pin of the accelerometer that was
 * initialised
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetY(int accelerometerPin);

/**
 * @brief
 * Function that returns the Z value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @param accelerometerPin:
 * The pin of the accelerometer that was
 * initialised
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetZ(int accelerometerPin);

/**
 * @brief
 * Function that returns the compass value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @param accelerometerPin:
 * The pin of the accelerometer that was
 * initialised
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetCompass(int accelerometerPin);
