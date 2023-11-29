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

/**
 * @brief
 * Local global variable used to store XFactor's
 * current status.
 */
XFactor_Status CurrentXFactorStatus = XFactor_Status::Off;

//#pragma region [FUNCTIONS]
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
bool XFactor_SetNewStatus(XFactor_Status newStatus)
{
    switch(newStatus)
    {
        case(XFactor_Status::Alarm):
        case(XFactor_Status::CalculatingRouteHome):
        case(XFactor_Status::CommunicationError):
        case(XFactor_Status::ConfirmingDropOff):
        case(XFactor_Status::DroppingOff):
        case(XFactor_Status::EnteringSafeBox):
        case(XFactor_Status::Error):
        case(XFactor_Status::ExaminatingAPackage):
        case(XFactor_Status::LeavingSafeBox):
        case(XFactor_Status::Maintenance):
        case(XFactor_Status::NoPackageFound):
        case(XFactor_Status::Off):
        case(XFactor_Status::PackageDropOffFailed):
        case(XFactor_Status::PackageExaminationFailed):
        case(XFactor_Status::PackagePickUpFailed):
        case(XFactor_Status::PreparingForDropOff):
        case(XFactor_Status::PreparingForTheSearch):
        case(XFactor_Status::ReturningHome):
        case(XFactor_Status::SearchingForAPackage):
        case(XFactor_Status::WaitingForDelivery):
        case(XFactor_Status::WaitingAfterSafeBox):
        case(XFactor_Status::Unlocked):
            CurrentXFactorStatus = newStatus;
            return true;

        default:
            CurrentXFactorStatus = XFactor_Status::CommunicationError;
            return false;
    }

    //Should not reach here.
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
XFactor_Status XFactor_GetStatus()
{
    return CurrentXFactorStatus;
}

//#pragma endregion