/**
 * @file Status.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief File which contains SafeBox's status
 * which is used during comms with it.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "SafeBox/Status.hpp"

/**
 * @brief Global variable used to store the last
 * status that was retrieved from SafeBox.
 * Declared as extern in @ref Status.hpp but
 * initialised in @ref SafeBox_Init.
 */
extern int SafeBoxStatus;