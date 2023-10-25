/**
 * @file Communication.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief 
 * File containing the header definitions of 
 * functions used in XFactor to handle the
 * Bluetooth communication with SafeBox.
 * @version 0.1
 * @date 2023-10-25
 * @copyright Copyright (c) 2023
 */

#pragma once

#pragma region [Command_Requests]

/**
 * @brief Asks SafeBox to change its package lid
 * status. Depending on the specified parameter,
 * the lid will either unlock and open or close
 * and lock.
 * 
 * @param wantedState
 * The wanted state of the lid. true is opened,
 * false is closed.
 * @return true: 
 * Successfully asked SafeBox to open its lid.
 * @return false: 
 * Failed to ask SafeBox to open its lid. 
 */
bool SafeBox_ChangeLidState(bool wantedState);

/**
 * @brief Asks SafeBox to change its garage door
 * status. Depending on the specified parameter,
 * the garage door will either unlock and open or
 * close and lock.
 *
 * @param wantedState
 * The wanted state of the garage door. true is
 * opened, false is closed.
 * @return true:
 * Successfully asked SafeBox to open the garage
 * door. 
 * @return false:
 * Failed to ask SafeBox to open the garage door.
 */
bool SafeBox_ChangeGarageState(bool wantedState);

/**
 * @brief Asks SafeBox to identify if a package
 * was recently deposited inside of it. The
 * returned parameter tells if a package was
 * deposited or not.
 * 
 * @return true:
 * A package was successfully deposited inside.
 * @return false:
 * No package were deposited since last time.
 */
bool SafeBox_CheckIfPackageDeposited();

/**
 * @brief Asks SafeBox to return its current
 * status. This is used when an alarm is
 * detected for example and allows both SafeBox
 * and XFactor to exhange their current status at
 * any time.
 * 
 * @attention
 * This function needs to be called at roughly
 * periodic intervals or whenever a significant
 * event happens. XFactor is the one to initiate
 * this handshake, not SafeBox.
 * 
 * @param xFactorStatus
 * The current status of XFactor.
 * @return int: The value of SafeBox's status.
 * If -1, no status were returned.
 */
int SafeBox_ExchangeStatus(int xFactorStatus);

#pragma endregion

#pragma region [Getters]
/**
 * @brief Asks SafeBox to give XFactor its
 * current package lid state.
 * @return true: The lid is opened
 * @return false: The lid is closed
 */
bool SafeBox_GetLidState();

/**
 * @brief Asks SafeBox to give XFactor its
 * current garage status.
 * @return true: The garage is opened
 * @return false: The garage is closed
 */
bool SafeBox_GetGarageState();

/**
 * @brief Asks SafeBox to tell XFactor how many
 * packages are currently deposited inside of it
 * @return int: How many packages are currently
 * deposited.
 */
int SafeBox_GetPackagesDeposited();

/**
 * @brief Asks SafeBox to tell XFactor the
 * current Doorbell status. This allowed it to
 * know if it should start the package retreival
 * or not.
 * @return true: The doorbell was activated.
 * @return false: The doorbell wasnt activated.
 */
bool SafeBox_GetDoorBellStatus();

/**
 * @brief Asks SafeBox to return its current
 * generic status which consists of a number
 * corresponding to an enum.
 * @return int: The value of SafeBox's status.
 */
int SafeBox_GetStatus();
#pragma endregion

#pragma region [Senders]
/**
 * @brief Sends XFactor's current status to
 * SafeBox.
 * 
 * @attention
 * SafeBox WILL return its own status shortly
 * after XFactor has sent his. If the message is
 * not read or cleared, it may fuck some shit up
 * later when Bluetooth communications are needed
 * 
 * @param xFactorStatus
 * The current status of XFactor.
 * @return true:
 * Successfully sent the status to SafeBox.
 * @return false:
 * Failed to send the status to SafeBox.
 */
bool SafeBox_SendStatus(int xFactorStatus);
#pragma endregion