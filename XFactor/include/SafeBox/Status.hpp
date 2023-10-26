/**
 * @file Status.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief File which contains the definitions of
 * SafeBox's status which is used during comms
 * with it.
 * @version 0.1
 * @date 2023-10-25
 * @copyright Copyright (c) 2023
 */

#pragma once

#pragma region [Enumerations]
/**
 * @brief Enumeration of all the possible status
 * that SafeBox can have. Each status represents
 * a mode or activity that SafeBox is currently
 * ongoing. These modes are used for XFactor to
 * take decisions and communicate properly.
 * 
 * @attention
 * Usage: variable = SafeBox_StatusEnum::Off;
 * @warning
 * Usage: SafeBoxStatus = SafeBox_StatusEnum::Alarm;
 */
enum SafeBox_StatusEnum {

    /// @brief Default status when the program fails to retreive SafeBox's status.
    CommunicationError = -1,

    /// @brief SafeBox is off
    Off = 0,

    /// @brief Safebox is waiting for the user to input package number and other informations. Its disarmed.
    WaitingAfterConfiguration = 1,

    /// @brief Safebox is currently being configurated by its user. Its disarmed.
    BeingConfigurated = 2,

    /// @brief Safebox is armed and waiting for a doorbell to start the proceedure
    WaitingForDoorbell = 3,

    /// @brief Safebox identified that a package was delivered. XFactor needs to retreive it. Its armed.
    PackageDelivered = 4,

    /// @brief Safebox is waiting for XFactor to deposite its package inside of it
    WaitingForDeposite = 5,

    /// @brief Safebox is waitin for XFactor to return inside of it.
    WaitingForXFactor = 6,

    /// @brief Safebox is in maintenance mode
    Maintenance = 50,
    
    /// @brief SafeBox encountered a critical error
    Error = 51,

    /// @brief SafeBox has sounded the alarm and won't stop until the user disarms it.
    Alarm = 255
};

/**
 * @brief Global variable used to store the last
 * status that was retreived from SafeBox.
 * Declared as extern in @ref Status.hpp but
 * initialised in @ref SafeBox_Innit. 
 */
extern int SafeBoxStatus;
#pragma endregion

#pragma region [Functions]
/*
bool SafeBox_ExecuteReceivedStatus(int receivedSafeBoxStatus);
*/
/**
 * @brief 
 * 
 * @param newSafeBoxStatus 
 * @return true 
 * @return false 
 */
/*
bool SafeBox_CompareNewStatusWithOld(int newSafeBoxStatus);
*/
#pragma endregion