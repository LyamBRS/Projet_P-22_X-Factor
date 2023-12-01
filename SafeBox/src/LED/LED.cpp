/**
 * @file LED.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the functions used to handle
 * the LEDs of SafeBox at an high level.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "LED/LED.hpp"


/**
 * @brief
 * Function that initialises all the LEDs used
 * on SafeBox.
 *
 * @return true:
 * Successfully initialised all the LEDs.
 * @return false:
 * Failed to initialise all the LEDs.
 */
bool LEDS_Init()
{
    WS2812_Init(LED_WS2812_ARDUINO_PIN);
    return true;
}

/**
 * @brief
 * Function that sets the color of a specified
 * RGB LED to a specified colour in the RGB
 * format.
 * @param LEDNumber
 * The number associated with that LED. Ranges
 * from 0 to whatever the amount of LEDs is.
 * @param red
 * Value of the red LED ranging from 0 to 255.
 * @param green
 * Value of the green LED ranging from 0 to 255.
 * @param blue
 * Value of the blue LED ranging from 0 to 255.
 * @return true:
 * Successfully changed the specified LED's
 * colors to the new specified colors.
 * @return false:
 * Failed to changed the specified LED's colors
 * to the new specified colors.
 */
bool LEDS_SetColor(int LEDNumber, unsigned char red, unsigned char green, unsigned char blue)
{
    Debug_Start("LEDS_SetColor");
    WS2812_SetStaticColors(LED_WS2812_ARDUINO_PIN, LEDNumber, red, green, blue);
    Debug_End();
    return true;
}