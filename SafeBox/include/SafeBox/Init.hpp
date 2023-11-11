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
#include "XFactor/Status.hpp"
#include "LibRobus.h"

#include "Actions/Actions.hpp"
#include "Communication/Bluetooth.hpp"
#include "LED/LED.hpp"
#include "Alarm/Alarm.hpp"
#include "Package/Package.hpp"
#include "Lid/Lid.hpp"

/**
 * @brief
 * Initialises all the functions required to make
 * XFactor work. Should be the only function
 * present in void setup.
 *
 * @attention
 * (LEDS_Init), (Package_Init), (Alarm_Init), (Lid_Init)
 */
void SafeBox_Init();