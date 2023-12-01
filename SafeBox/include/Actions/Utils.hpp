/**
 * @file Utils.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing definitions of Utils functions
 * used in SafeBox.
 * @version 0.1
 * @date 2023-11-18
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "Sensors/Doorbell/Doorbell.hpp"
#include "LED/LED.hpp"
#include "SafeBox/Communication.hpp"
#include "SafeBox/Status.hpp"
#include "Sensors/RFID/RFID.hpp"
#include "math.h"
#include "Alarm/Alarm.hpp"

// - DEFINES - //
#define EXECUTIONUTILS_CLOCKS_TILL_GARAGE_ALARM 125

// - FUNCTIONS - //

/**
 * @brief
 * This function ensures that the door is read as
 * closed. If its not the case after X amount of
 * attempts, the alarm needs to be executed
 * because the door is NOT supposed to be open.
 * @return true:
 * We good, no alarms mate
 * @return false:
 * Euuh... Why is the door not closed? 
 */
bool ExecutionUtils_CheckIfGarageIsClosed();

/**
 * @brief
 * This function allows you to create a simple
 * blinking statement in your Execution functions
 * Especially useful for the Alarm and Error.
 * @param blinkingPeriodMS
 * How long should the timer wait before it needs
 * to return the end of period so that you can
 * make your LED blink?
 * @return true:
 * The timer has reached the end. You must make
 * the LED blink.
 * @return false:
 * The timer is still counting.
 */
bool ExecutionUtils_LedBlinker(unsigned long blinkingPeriodMS);

/**
 * @brief
 * This function's sole purpose is to handle the
 * RFID card reader in each execution function
 * where SafeBox can be unlocked when its armed.
 */
bool ExecutionUtils_HandleArmedUnlocking();

/**
 * @brief
 * Checks the latest status received from XFactor
 * and executes different actions based on its
 * status, such as entering alarm mode or
 * error mode.
 */
void ExecutionUtils_HandleReceivedXFactorStatus();