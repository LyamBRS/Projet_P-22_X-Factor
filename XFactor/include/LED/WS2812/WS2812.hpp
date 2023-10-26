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

#include <Adafruit_NeoPixel.h>
#include <math.h>

bool WS2812_Innit(int pinNumber);

bool WS2812_SetStaticColors(int pinNumber, int LEDNumber, unsigned char red, unsigned char green, unsigned char blue);