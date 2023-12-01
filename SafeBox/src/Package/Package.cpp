/**
 * @file Package.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the functions that are used
 * to detect the status of the package bay of
 * SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Package/Package.hpp"

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
bool Package_Init()
{
    Debug_Start("Package_Init");
    Debug_Warning("Package", "Package_Init", "BYPASSED");
    Debug_End();
    return true;
}

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
bool Package_IsDeposited()
{
    return false;
}