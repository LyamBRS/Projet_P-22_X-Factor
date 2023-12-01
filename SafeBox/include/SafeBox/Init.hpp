/**
 * @file Initialisation.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Contains the main function that initialises
 * SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "SafeBox/Status.hpp"

#include "Actions/Actions.hpp"
#include "Communication/Bluetooth.hpp"
#include "Alarm/Alarm.hpp"
#include "Package/Package.hpp"
#include "Lid/Lid.hpp"
#include "Garage/Garage.hpp"
#include "Debug/Debug.hpp"
#include "LED/LED.hpp"

/**
 * @brief
 * Initialises all the functions required to make
 * SafeBox work. Should be the only function
 * present in void setup.
 *
 * @attention
 * (LEDS_Init), (Package_Init), (Alarm_Init), (Lid_Init), (Garage_Init)
 */
void SafeBox_Init();