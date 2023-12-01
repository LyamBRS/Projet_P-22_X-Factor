/**
 * @file Lid.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Contains the header definitions of functions
 * used to handle the lid of SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "Outputs/Motors/Servo/S3003.hpp"
#include "Debug/Debug.hpp"
#define PACKAGE_DOOR_LOCKED 0
#define PACKAGE_DOOR_UNLOCKED 95

#define LID_CLOSED_SWITCH_PIN 4
/**
 * @brief
 * Function that initialises the servo motors and
 * other sensors that is used to make a the lid
 * of SafeBox function.
 * @return true:
 * Successfully initialised the lid of SafeBox.
 * @return false:
 * Failed to initialise the lid of SafeBox.
 */
bool Lid_Init();

/**
 * @brief
 * This function attempts to open the package
 * drop off lid of SafeBox.
 *
 * @return true:
 * Successfully opened the lid.
 * @return false:
 * Failed to unlock or open the lid.
 */
bool Lid_Open();

/**
 * @brief
 * This function attempts to close the package
 * drop off lid of SafeBox.
 *
 * @return true:
 * Successfully closed the lid.
 * @return false:
 * Failed to lock or close the lid.
 */
bool Lid_Close();

/**
 * @brief
 * This function attempts to lock the package
 * drop off lid of SafeBox.
 *
 * @return true:
 * Successfully locked the lid.
 * @return false:
 * Failed to lock the lid.
 */
bool Lid_Unlock();

/**
 * @brief
 * This function attempts to unlock the package
 * drop off lid of SafeBox.
 *
 * @return true:
 * Successfully unlock the lid.
 * @return false:
 * Failed to unlock the lid.
 */
bool Lid_Lock();

/**
 * @brief 
 * Returns true if the lid is really closed or
 * not.
 * @return true:
 * It is locked.
 * @return false:
 * It isnt locked.
 */
bool Lid_IsClosed();