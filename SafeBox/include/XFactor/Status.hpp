/**
 * @file Status.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the header definitions of
 * functions used to handle XFactor's status and
 * store as well as use the status.
 * @version 0.1
 * @date 2023-10-26
 * @copyright Copyright (c) 2023
 */

#pragma once
#include "Debug/Debug.hpp"

//#pragma region [ENUM]

/**
 * @brief Enumeration of all the possible status
 * that XFactor can have. Each status represents
 * a mode or activity that XFactor is currently
 * doing. These modes are used for SafeBox to
 * take decisions and communicate properly.
 *
 * @attention
 * Usage: variable = XFactor_StatusEnum::Off;
 * @warning
 * Usage: XFactorStatus = XFactor_StatusEnum::Alarm;
 */
enum class XFactor_Status {

    /// @brief Default status when the program fails to communicate properly.
    CommunicationError = 253,

    /// @brief XFactor is off. Default status.
    Off = 0,

    /// @brief Status used when XFactor awaits for SafeBox to give it the go
    WaitingForDelivery = 1,

    /// @brief Status used when XFactor is leaving the garage
    LeavingSafeBox = 2,

    /// @brief Status used when XFactor is preparing to search for the package
    PreparingForTheSearch = 3,

    /// @brief Status used when XFactor is actively searching for a package
    SearchingForAPackage = 4,

    /// @brief Status used when XFactor is examinating a potential package
    ExaminatingAPackage = 5,

    /// @brief Status used when XFactor is actively picking up or trying to pick up a package
    PickingUpAPackage = 6,

    /// @brief Status used when XFactor is trying to calculate a way home
    CalculatingRouteHome = 7,

    /// @brief Status used when XFactor is actively returning home
    ReturningHome = 8,

    /// @brief Status used when XFactor is preparing to drop off a package inside of SafeBox
    PreparingForDropOff = 9,

    /// @brief Status used when XFactor is actively dropping the package inside SafeBox
    DroppingOff = 10,

    /// @brief Status used when XFactor is attempting to confirm that the package was dropped off
    ConfirmingDropOff = 11,

    /// @brief Status used when XFactor is trying to enter the garage
    EnteringSafeBox = 12,

    /// @brief Status used when XFactor fails to examine if a potential package is a package or not.
    PackageExaminationFailed = 13,

    /// @brief Status used when XFactor cannot pick up an identified package after X attempts
    PackagePickUpFailed = 14,

    /// @brief Status used when XFactor fails to drop off the package inside SafeBox.
    PackageDropOffFailed = 15,

    /// @brief Status used when XFactor failed to find a package that was delivered
    NoPackageFound = 16,

    /// @brief Status used when XFactor awaits for SafeBox to reply to its communications
    WaitingAfterSafeBox = 17,

    Unlocked = 18,

    /// @brief Safebox is in maintenance mode
    Maintenance = 50,

    /// @brief SafeBox encountered a critical error
    Error = 51,

    /// @brief SafeBox has sounded the alarm and won't stop until the user disarms it.
    Alarm = 255
};

//#pragma endregion

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
bool XFactor_SetNewStatus(XFactor_Status newStatus);

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
XFactor_Status XFactor_GetStatus();

//#pragma endregion