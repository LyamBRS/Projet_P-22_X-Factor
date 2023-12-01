/**
 * @file WS2812.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File that takes care of the header definitions
 * of functions that handles directly WS2812 LEDs
 * by using Arduino's basic functions.
 * @version 0.1
 * @date 2023-10-26
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDE - //
#include <Adafruit_NeoPixel.h>
#include "Debug/Debug.hpp"
#include "Communication/Bluetooth.hpp"

/**
 * @brief
 * Initialises one or multiple WS2812 by
 * specifying which pin of the Arduino is used to
 * talk with the LEDs.
 *
 * @param pinNumber
 * Which pin of the Arduino is used to talk with
 * the LEDs
 * @return true:
 * Successfully initialised a strip of WS2812s
 * @return false:
 * Failed to initialise the WS2812 at the
 * specified pin.
 */
bool WS2812_Init(int pinNumber);

/**
 * @brief Updates a specified WS2812 with a
 * specified color in the RGB format. You
 * must specify which LED in the strip needs to
 * be updated.
 *
 * @param pinNumber
 * Which pin of the Arduino is used to talk with
 * the LEDs
 * @param LEDNumber
 * Which LED in the strip of LEDs must be changed
 * @param red
 * Value ranging from 0 to 255 that indicates the
 * wanted intensity of the red LED.
 * @param green
 * Value ranging from 0 to 255 that indicates the
 * wanted intensity of the green LED.
 * @param blue
 * Value ranging from 0 to 255 that indicates the
 * wanted intensity of the blue LED.
 * @return true:
 * Successfully changed and updated the specified
 * LED with the specified color.
 * @return false:
 * Failed to change the LED with the specified
 * colors.
 */
bool WS2812_SetStaticColors(int pinNumber, int LEDNumber, unsigned char red, unsigned char green, unsigned char blue);