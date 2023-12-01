/**
 * @file LED.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the header definitions of
 * functions used to handle the LEDs of SafeBox
 * at an high level. Also contains the defines
 * used for LED colours and their meanings.
 * @version 0.1
 * @date 2023-10-26
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDE - //
#include "Outputs/LEDs/WS2812/WS2812.hpp" //// The LED used by SafeBox is a WS2812.
#include "Debug/Debug.hpp"

//#pragma region [LED_COLORS]
/// @brief RGB color that the LED will have when SafeBox is offline.
#define LED_COLOR_OFFLINE 0,0,0
/// @brief RGB color that the LED will have when SafeBox is initialising.
#define LED_COLOR_INITIALISING 255,255,255
/// @brief RGB color that the LED will have when SafeBox is waiting for XFactor to be online.
#define LED_COLOR_WAITING_FOR_COMMS 32,32,0
/// @brief RGB color that the LED will have when SafeBox is actively communicating.
#define LED_COLOR_COMMUNICATING 0,128,128
/// @brief RGB color that the LED will have when SafeBox is in an error.
#define LED_COLOR_ERROR 128,0,0
/// @brief RGB color that the LED will have when SafeBox is unlocked by the user
#define LED_COLOR_DISARMED 0,128,0
/// @brief RGB color that the LED will have when SafeBox is locked and operational.
#define LED_COLOR_ARMED 128,64,0
/// @brief RGB color that the LED will have when SafeBox going nuts over the alarm.
#define LED_COLOR_ALARM 255,0,0
/// @brief RGB color that the LED will have when SafeBox is waiting for a delivery.
#define LED_COLOR_WAITFORDELIVERY 64,0,64
/// @brief RGB color that the LED will have when SafeBox is waiting for the user to put their RFID card to save it.
#define LED_COLOR_NEW_CARD_SAVED 128,32,32
//#pragma endregion

//#pragma region [XFACTOR_LED_NUMBERS]
#define LED_WS2812_ARDUINO_PIN 13

#define LED_ID_STATUS_INDICATOR 0
//#pragma endregion

//#pragma region [FUNCTIONS]

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
bool LEDS_Init();

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
bool LEDS_SetColor(int LEDNumber, unsigned char red, unsigned char green, unsigned char blue);

//#pragma endregion