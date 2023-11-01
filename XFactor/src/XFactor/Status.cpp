/**
 * @file Status.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the code definitions of
 * functions used to handle XFactor's status and
 * store as well as use the status.
 * @version 0.1
 * @date 2023-11-01
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "XFactor/Status.hpp"

#pragma region [FUNCTIONS]
/**
 * @brief
 * Function that sets the global variable which
 * holds the current status of XFactor to a new
 * desired value from @ref XFactor_StatusEnum
 * @param newStatus
 * A value within @ref XFactor_StatusEnum
 * @return true:
 * The status was successfully updated to the new
 * specified status.
 * @return false:
 * The specified status does not match available
 * status of @ref XFactor_StatusEnum
 */
bool XFactor_SetNewStatus(unsigned char newStatus)
{
    return false;
}

/**
 * @brief
 * Function that returns the current status of
 * XFactor as an unsigned char. You must check
 * that value with @ref XFactor_StatusEnum to
 * identify what the current status of XFactor is
 *
 * @return unsigned char
 * value from @ref XFactor_StatusEnum which
 * corresponds to XFactor's current status.
 */
unsigned char XFactor_GetStatus()
{
    return XFactorStatus;
}

#pragma endregion