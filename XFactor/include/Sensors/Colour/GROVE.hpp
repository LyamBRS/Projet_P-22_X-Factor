/**
 * @file GROVE.hpp
 * @author Lyam BRS (lyam.brs@gmail.com)
 * @brief Header file used for the GROVE colour sensor.
 * @version 0.1
 * @date 2023-10-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

/**
 * @brief Initialises one GROVE sensor depending on given
 * pins.
 * @param redPin Arduino pins for the red input
 * @param greenPin Arduino pins for the green input
 * @param bluePin Arduino pins for the blue input
 */
void GROVE_Init(int redPin, int greenPin, int bluePin);

/**
 * @brief This function should use the specified colour
 * sensor pins and return a large number that represents
 * the hex value of the number. Numbers can be see as hex
 * and regular numbers automatically. It should combine
 * R,G,B into one big number.
 * 255,255,255 would be 255255255
 * 
 * @param redPin Arduino pins for the red input
 * @param greenPin Arduino pins for the green input
 * @param bluePin Arduino pins for the blue input
 * @return unsigned int of the hex colour returned by the sensor.
 */
unsigned int GROVE_GetColor(int redPin, int greenPin, int bluePin);

// I suggest making more functions. You could make one that makes the big number from the smaller number for example.
// The functions you make should start with GROVE_. variables as inputs should be lower camelCase.
