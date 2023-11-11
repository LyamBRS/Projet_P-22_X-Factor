/**
 * @file Status.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the functions used to handle
 * SafeBox's status and store as well as use the
 * status.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "SafeBox/Status.hpp"

/**
 * @brief
 * Local global variable that holds the status
 * of SafeBox so that getter and setter functions
 * can be used in the program.
 */
unsigned char SafeBoxSavedStatus = SafeBox_StatusEnum::Off;

//#pragma region [FUNCTIONS]
/**
 * @brief
 * Function that sets the global variable which
 * holds the current status of SafeBox to a new
 * desired value from @ref SafeBox_StatusEnum
 * @param newStatus
 * A value within @ref SafeBox_StatusEnum
 * @return true:
 * The status was successfully updated to the new
 * specified status.
 * @return false:
 * The specified status does not match available
 * status of @ref SafeBox_StatusEnum
 */
bool SafeBox_SetNewStatus(unsigned char newStatus)
{
    switch(newStatus)
    {
        case(SafeBox_StatusEnum::CommunicationError):
        case(SafeBox_StatusEnum::Off):
        case(SafeBox_StatusEnum::WaitingForDelivery):
        case(SafeBox_StatusEnum::WaitingForRetrieval):
        case(SafeBox_StatusEnum::WaitingForReturn):
        case(SafeBox_StatusEnum::ReadyForDropOff):
        case(SafeBox_StatusEnum::Unlocked):
        case(SafeBox_StatusEnum::DroppingOff):
        case(SafeBox_StatusEnum::Maintenance):
        case(SafeBox_StatusEnum::Error):
        case(SafeBox_StatusEnum::Alarm):
            // The status is valid.
            SafeBoxSavedStatus = newStatus;
            return true;

        default:
            // Invalid status
            SafeBoxSavedStatus = SafeBox_StatusEnum::Error;
            return false;
    }
    // SHOULD NEVER REACH HERE
    return false;
}

/**
 * @brief
 * Function that returns the current status of
 * SafeBox as an unsigned char. You must check
 * that value with @ref SafeBox_StatusEnum to
 * identify what the current status of SafeBox is
 *
 * @return unsigned char
 * value from @ref SafeBox_StatusEnum which
 * corresponds to SafeBox's current status.
 */
unsigned char SafeBox_GetStatus()
{
    return SafeBoxSavedStatus;
}

//#pragma endregion