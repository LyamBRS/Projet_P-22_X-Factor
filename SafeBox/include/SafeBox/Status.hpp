/**
 * @file Status.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the header definitions of
 * functions used to handle SafeBox's status and
 * store as well as use the status.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

#pragma once

//#pragma region [ENUM]

/**
 * @brief Enumeration of all the possible status
 * that SafeBox can have. Each status represents
 * a mode or activity that SafeBox is currently
 * doing. These modes are used for SafeBox to
 * take decisions and communicate properly.
 *
 * @attention
 * Usage: variable = SafeBox_StatusEnum::Off;
 * @warning
 * Usage: SafeBoxStatus = SafeBox_StatusEnum::Alarm;
 */
enum class SafeBox_Status {

    /// @brief Default status when the program fails to communicate properly.
    CommunicationError = 253,

    /// @brief SafeBox is off. Default status.
    Off = 0,

    /// @brief Status used when SafeBox awaits the doorbell to be read to give it the go
    WaitingForDelivery = 1,

    /// @brief Status used when SafeBox is waiting for XFactor to retrieve a package.
    WaitingForRetrieval = 2,

    /// @brief Status used when SafeBox is waiting for XFactor to return to SafeBox
    WaitingForReturn = 3,

    /// @brief Status used when SafeBox is ready to handle a drop off by XFactor
    ReadyForDropOff = 4,

    /// @brief Status used when SafeBox is unlocked
    Unlocked = 5,

    /// @brief Status used when SafeBox is actively dropping the package inside SafeBox
    DroppingOff = 10,

    /// @brief Safebox is in maintenance mode
    Maintenance = 50,

    /// @brief SafeBox encountered a critical error
    Error = 51,

    /// @brief Safebox is waiting for XFactor to be online.
    WaitingForXFactor = 254,

    /// @brief SafeBox has sounded the alarm and won't stop until the user disarms it.
    Alarm = 255
};

//#pragma endregion

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
bool SafeBox_SetNewStatus(SafeBox_Status newStatus);

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
SafeBox_Status SafeBox_GetStatus();

//#pragma endregion