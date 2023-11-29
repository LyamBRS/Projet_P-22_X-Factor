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

// - INCLUDES - //
#include "Debug/Debug.hpp"
#include "SafeBox/Status.hpp"           //// Used to store the status of SafeBox and get the enumeration of its possible values
#include "XFactor/Status.hpp"           //// Used to get the enumeration of XFactor status possible values
#include "Communication/Bluetooth.hpp"  //// Used to communicate information and receive information from SafeBox

// - DEFINES - //
#define COMMS_TIMEOUT_MS 2000

#define COMMAND_LID_OPEN  "C_LID_O"
#define COMMAND_LID_CLOSE "C_LID_C"
#define COMMAND_LID_GET "C_LID_G"
#define COMMAND_GARAGE_OPEN   "C_GAR_O"
#define COMMAND_GARAGE_CLOSE  "C_GAR_C"
#define COMMAND_GARAGE_GET  "C_GAR_G"
#define COMMAND_DOORBELL_GET  "C_DRB_G"
#define COMMAND_GET_PACKAGE_COUNT "C_PCK_G"
#define COMMAND_CHECK_PACKAGE     "C_PCK_C"
#define COMMAND_STATUS_EXCHANGE   "C_STE_"

#define ANSWER_LID_OPEN      "A_LID_O"
#define ANSWER_LID_CLOSED    "A_LID_C"
#define ANSWER_LID_SUCCESS   "A_LID_S"
#define ANSWER_LID_FAILED    "A_LID_S"

#define ANSWER_GARAGE_OPEN      "A_GAR_O"
#define ANSWER_GARAGE_CLOSED    "A_GAR_C"
#define ANSWER_GARAGE_SUCCESS   "A_GAR_S"
#define ANSWER_GARAGE_FAILED    "A_GAR_S"

#define ANSWER_DOORBELL_RANG     "A_DRB_R"
#define ANSWER_DOORBELL_NOT_RANG "A_DRB_N"

#define ANSWER_PACKAGE_COUNT         "A_PCK_C"
#define ANSWER_PACKAGE_CHECK_SUCCESS "A_PCK_S"
#define ANSWER_PACKAGE_CHECK_FAILED  "A_PCK_F"

#define ANSWER_STATUS_EXCHANGE       "A_STE_"

// #pragma region [Command_Requests]

/**
 * @brief 
 * Resets the global parameters
 * that are used in communication
 * @param answer 
 * @return true:
 * The parameters have been reset
 * @return false:
 * A problem has occurred.
 */
bool ResetSavedParameters();

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
 * and XFactor to exchange their current status at
 * any time. Use Status functions to set and
 * compare their status. This one is only used to
 * exchange the status.
 *
 * @attention
 * This function needs to be called at roughly
 * periodic intervals or whenever a significant
 * event happens. XFactor is the one to initiate
 * this handshake, not SafeBox.
 *
 * @return true:
 * Successfully exchanged the status.
 * @return false:
 * Failed to exchange the status.
 */
bool SafeBox_ExchangeStatus();

// #pragma endregion

// #pragma region [Getters]
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
 * know if it should start the package retrieval
 * or not.
 * @return true: The doorbell was activated.
 * @return false: The doorbell wasn't activated.
 */
bool SafeBox_GetDoorBellStatus();

// #pragma endregion