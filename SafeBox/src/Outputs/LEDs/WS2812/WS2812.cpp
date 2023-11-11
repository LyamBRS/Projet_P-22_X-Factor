/**
 * @file WS2812.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File that takes care of the hardware functions
 * of WS2812 programmable RGB LEDs.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Outputs/LEDs/WS2812/WS2812.hpp"

Adafruit_NeoPixel pixels;

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
bool WS2812_Init(int pinNumber)
{
    pixels = Adafruit_NeoPixel(1, pinNumber, NEO_GRB + NEO_KHZ800);
    pixels.begin();
    pixels.clear();
    return true;
}

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
bool WS2812_SetStaticColors(int pinNumber, int LEDNumber, unsigned char red, unsigned char green, unsigned char blue)
{
    // Checking if the 300 microseconds of down time has been acheived or not.
    if(not pixels.canShow())
    {
        // Cannot change the color right now.
        return false;
    }

    pixels.setPixelColor(LEDNumber, pixels.Color(red, green, blue));
    pixels.show();

    return true;
}