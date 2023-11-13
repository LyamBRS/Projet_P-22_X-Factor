/**
 * @file Communication.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the header definitions of
 * functions used in SafeBox to handle the
 * Bluetooth communication with XFactor.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "XFactor/Status.hpp"           //// Used to store the status of SafeBox and get the enumeration of its possible values
#include "Communication/Bluetooth.hpp"  //// Used to communicate information and receive information from SafeBox

// #pragma region [Command_Requests]

/**
 * @brief
 * When this function is called, it should check
 * if SafeBox has received any Bluetooth
 * messages. If a message was received, it
 * will be executed and a reply will be sent to
 * XFactor.
 *
 * @attention
 * This is the only function you should need to
 * call to execute whatever functions was
 * received by XFactor. Only use the global
 * status of XFactor to determine wether you need
 * to change Execution Functions. This wont tell
 * you.
 * @return true:
 * Successfully handled a request.
 * @return false:
 * Failed to handle a request / There was no
 * requests.
 */
bool SafeBox_CheckAndExecuteMessage();

/**
 * @brief Function that changes the lid state of
 * SafeBox depending on what XFactor wants.
 * Must firstly unlock it if its closed and
 * lock it after if it was open.
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
 * @brief Makes SafeBox change its garage door
 * state depending on what XFactor wants.
 *
 * @param wantedState
 * The wanted state of the garage door. true is
 * opened, false is closed.
 * @return true:
 * Successfully opened the garage door.
 * @return false:
 * Failed to open the garage door.
 */
bool SafeBox_ChangeGarageState(bool wantedState);

/**
 * @brief
 * Makes SafeBox change its package lid state
 * depending on what XFactor wants.
 * @param wantedLidState
 * @return true:
 * Successfully changed the lid state.
 * @return false:
 * Failed to change the lid state.
 */
bool SafeBox_ChangeLidState(bool wantedLidState);

/**
 * @brief
 * Replies to XFactor's status exchange by
 * replying to it with the box's own status.
 * XFactor status's is the sole input parameter
 * of this function and it stores it as a global
 * variable somewhere.
 *
 * The function must return a boolean that
 * represents if the status was successfully
 * handled or not, and if the function managed to
 * reply to XFactor or not.
 * @return true:
 * Successfully stored the status and
 * @return false:
 * Failed to handle the status or exchange our own.
 */
bool SafeBox_ReplyStatus();

/**
 * @brief
 * Function that verifies if there is a new
 * package inside of the box. If yes, the proper
 * message is sent to XFactor as an answer,
 * otherwise, the false message is sent back to
 * XFactor.
 *
 * This function must take nothing as an input
 * parameter but must return a boolean that
 * specifies if it successfully managed to handle
 * the request or not.
 * @return true:
 * Successfully handled the request.
 * @return false:
 * Failed to handle the request.
 */
bool SafeBox_ReplyToCheckIfPackageDeposited();

/**
 * @brief
 * Tells XFactor how many packages are
 * currently deposited inside of itself.
 * This sends on Bluetooth the amount of
 * packages currently inside of SafeBox.
 *
 * This function should return a Boolean that
 * corresponds to whether it successfully
 * handled the request or not.
 * @return true:
 * Successfully handled the request.
 * @return false:
 * Failed to handle the request.
 */
bool SafeBox_ReturnDepositedPackages();

/**
 * @brief
 * Tell XFactor whether it recently detected
 * a doorbell or not. Doorbell value should
 * either be 0 or 1 and is reset by SafeBox only
 * when the package is deposited inside of it or
 * something happens, like a failure to get the
 * package.
 *
 * @return true:
 * Successfully handled the doorbell request.
 * @return false:
 * Failed to handle the doorbell request.
 */
bool SafeBox_GetDoorBellStatus();
// #pragma endregion