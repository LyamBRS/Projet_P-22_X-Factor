/**
 * @file Initialisation.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Contains the main function that initialises
 * XFactor.
 * @version 0.1
 * @date 2023-11-01
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "XFactor/Status.hpp"
#include "LibRobus.h"

#include "Actions/Actions.hpp"
#include "LED/LED.hpp"
#include "Alarm/Alarm.hpp"
#include "Package/Package.hpp"
#include "Debug/Debug.hpp"
#include "Communication/Bluetooth.hpp"

/**
 * @brief
 * Initialises all the functions required to make
 * XFactor work. Should be the only function
 * present in void setup.
 *
 * @attention
 * (Claws_init), (BoardInit), (LEDS_Init), (Package_Init), (Alarm_Init)
 */
void XFactor_Init();