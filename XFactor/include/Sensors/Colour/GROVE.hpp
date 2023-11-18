/**
 * @file GROVE.hpp
 * @author Lyam BRS (lyam.brs@gmail.com)
 * @brief Header file used for the GROVE colour sensor.
 * @version 0.1
 * @date 2023-10-19
 * @copyright Copyright (c) 2023
 */
#pragma once

#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Colour/Colour.hpp"
#include "Debug/Debug.hpp"

/**
 * @brief Initialises one GROVE sensor depending on given
 * pins.
 * 
 * @return true:
 * Successfully initialised the GROVE sensor.
 * @return false:
 * Failed to initialise the GROVE sensor.
 */
bool GROVE_Init();

/**
 * @brief This function should use the specified colour
 * sensor pins and return a large number that represents
 * the hex value of the number. Numbers can be see as hex
 * and regular numbers automatically. It should combine
 * R,G,B into one big number.
 * 255,255,255 would be 255255255
 *
 * 
 * @return unsigned int of the hex colour returned by the sensor.
 */
unsigned long GROVE_GetColor();
