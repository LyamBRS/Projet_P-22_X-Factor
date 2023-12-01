/**
 * @file Package.hpp
 * @author LyamBRS (ltam.brs@gmail.com)
 * @brief
 * File containing the header definitions of the
 * various functions that are used to detect the
 * status of the package bay of SafeBox
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "Debug/Debug.hpp"

/// @brief How much game should there be between low and high threshold
#define PACKAGE_COLOUR_THRESHOLD 10

/**
 * @brief
 * Initialisation function that initialises the
 * sensors used to detect a package inside of
 * SafeBox.
 * @return true:
 * Successfully initialised package functions.
 * @return false:
 * Failed to initialise package functions.
 */
bool Package_Init();

/**
 * @brief Function that returns a boolean value
 * that indicates if there currently is a package
 * inside of SafeBox.
 *
 * @return true:
 * There is a package inside SafeBox
 * @return false:
 * There is no package inside of SafeBox.
 */
bool Package_IsDeposited();