/**
 * @file Doorbell.hpp
 * @author your name (you@domain.com)
 * @brief
 * File containing the header definitions of
 * functions used to handle the doorbell reading
 * done by SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

#pragma once

#include <Arduino.h>
#include "Debug/Debug.hpp"

/// @brief Arduino analog pin that reads the ambiant noises around the detector
#define DOORBELL_AMBIENT_NOISE_PIN A0
/// @brief Arduino analog pin that reads the whistle noises around the detector
#define DOORBELL_WHISLE_NOISE_PIN A1
/// @brief The pin that is read to force a doorbell status in case it cannot wor.
#define DOORBELL_SWITCH_BYPASS_PIN 6

/**
 * @brief
 * Function that initialises the whistle detector
 * or doorbell reader used by SafeBox.
 *
 * @return true:
 * Successfully initialised the doorbell used by
 * SafeBox. (Whistle detector)
 * @return false:
 * Failed to initialised the doorbell used on
 * SafeBox.
 */
bool Doorbell_Init();

/**
 * @brief
 * Function that returns if there is a doorbell
 * that is currently being detected or not.
 * @return true:
 * Doorbell is being ran live.
 * @return false:
 * No doorbell detected.
 */
bool Doorbell_GetState();