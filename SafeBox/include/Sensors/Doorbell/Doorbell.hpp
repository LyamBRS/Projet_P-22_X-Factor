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

/**
 * @brief
 * Function that initialises the whistle detector
 * or doorbell reader used by SafeBox.
 *
 * @param doorbellPin
 * The pin that is used to read the whistle status
 * from.
 * @return true:
 * Successfully initialised the doorbell used by
 * SafeBox. (Whistle detector)
 * @return false:
 * Failed to initialised the doorbell used on
 * SafeBox.
 */
bool Doorbell_Init(int doorbellPin);

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