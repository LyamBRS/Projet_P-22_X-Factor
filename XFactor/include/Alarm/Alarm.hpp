/**
 * @file Alarm.hpp
 * @author Mohamed
 * @brief
 * File containing the header definitions of
 * the functions used to handle and detect alarms
 * in the XFactor program.
 * @version 1.0
 * @date 2023-11-20
 * @copyright Copyright (c) 2023
 */

#pragma once
// - INCLUDES - //
#include "Sensors/Accelerometer/Accelerometer.hpp" //// Needs to be initialised in alarms initialise function, and used in accelerometer getters.
#include "Package/Package.hpp"

// #pragma region [DEFINES]

#define ACCELEROMETER_NB_CHECKING 1 // Number of accelerometer detection trials

#define THRESHOLD_OFFSET_X 0.25f
#define THRESHOLD_OFFSET_Y 0.25f
#define THRESHOLD_OFFSET_Z 0.20f

#define THRESHOLD_VERIFY_ALARM_COUNTER 5

#define ACCELEROMETER_BYPASS_PIN 48
// #pragma endregion

// #pragma region [FUNCTIONS]
/**
 * @brief
 * Function that initialises the Alarm and its
 * components such as the accelerometer installed
 * on XFactor. Other components are initialised
 * on their own or inside other inits because
 * they are used by other things than the alarm.
 * @return true:
 * Successfully initialised the alarm.
 * @return false:
 * Failed to initialise the alarm.
 */
bool Alarm_Init();

/**
 * @brief
 * Verifies all the sensors of XFactor that can
 * be utilised to trigger an alarm.
 *
 * @return true:
 * An alarm needs to be triggered
 * @return false:
 * No alarm needs to be triggered
 */
bool Alarm_VerifySensors();

/**
 * @brief
 * Resets the X and Y variation
 * thresholds to prevent the alarm
 * from setting off its own
 * movements
 */
void Alarm_ResetThresholds();

/**
 * @brief
 * Verifies the accelerometer's values to see if
 * the alarm needs to be activated based off the
 * values returned.
 *
 * @return true:
 * An alarm needs to be activated.
 * @return false:
 * No alarm needs to be activated.
 */
bool Alarm_VerifyAccelerometer();

/**
 * @brief
 * Function that utilises the Package status to
 * figure out if a package has been stolen inside
 * of XFactor. If the package status is false,
 * this function is bypassed because there is no
 * package to steal.
 * @return true:
 * An alarm needs to be activated.
 * @return false:
 * No alarm needs to be activated.
 */
bool Alarm_VerifyPackage();
bool MoveStraightAndTest();

// #pragma endregion
