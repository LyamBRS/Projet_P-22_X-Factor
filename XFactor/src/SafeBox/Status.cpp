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
SafeBox_Status SafeBoxSavedStatus = SafeBox_Status::Off;

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
bool SafeBox_SetNewStatus(SafeBox_Status newStatus)
{
    switch(newStatus)
    {
        case(SafeBox_Status::CommunicationError):
        case(SafeBox_Status::Off):
        case(SafeBox_Status::WaitingForXFactor):
        case(SafeBox_Status::WaitingForDelivery):
        case(SafeBox_Status::WaitingForRetrieval):
        case(SafeBox_Status::WaitingForReturn):
        case(SafeBox_Status::ReadyForDropOff):
        case(SafeBox_Status::Unlocked):
        case(SafeBox_Status::DroppingOff):
        case(SafeBox_Status::Maintenance):
        case(SafeBox_Status::Error):
        case(SafeBox_Status::Alarm):
            // The status is valid.
            SafeBoxSavedStatus = newStatus;
            return true;

        default:
            // Invalid status
            SafeBoxSavedStatus = SafeBox_Status::Error;
            Debug_Information("Status", "SafeBox_SetNewStatus", "UNKNOWN SAFEBOX STATUS");
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
SafeBox_Status SafeBox_GetStatus()
{
    return SafeBoxSavedStatus;
}

//#pragma endregion