/**
 * @file Claw.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File which contains the header definitions
 * of functions that controls XFactor's package
 * recovery claw. These functions are then
 * used by regular Package functions.
 * @version 0.1
 * @date 2023-10-26
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "Outputs/Motors/Servo/S3003.hpp" //// Used to make the claw move. 2 servo motors should be used.
#include "Debug/Debug.hpp"

// #pragma region [DEFINES]
#define CLAWS_SQUEEZE_DISTANCE 10
#define CLAWS_CLOSING_SPEED 1

#define CLAWS_GRABBERS_MAX 15
#define CLAWS_GRABBERS_MIN 90

#define CLAWS_HEIGHT_MIN 95
#define CLAWS_HEIGHT_MAX 180

#define CLAWS_PINS_SWITCH A15
#define CLAWS_PINS_GRABBER 1
#define CLAWS_PINS_HEIGHT 0

#define CLAWCLOSE_INTERVAL_MS 30

#define CLAWS_STATUS_DEPLOYED 1
#define CLAWS_STATUS_STORED 0

// TEMPORATY DEFINES TO MAKE THAT SHIT WORK, remove when they're not used in the code anymore

#define CLAWS_HEIGHT_DEPLOYED 0
#define CLAWS_HEIGHT_STORED 100

#define CLAWS_GRABBERS_DEPLOYED 100
#define CLAWS_GRABBERS_STORED 100

#define CLAW_CLOSE_INTERVAL_MS 10

// #pragma endregion

// #pragma region [FUNCTIONS]

/**
 * @brief
 * Initialises all the servo motors used in
 * XFactor's package recovery claw. This
 * function also sets their default positions
 * @return true:
 * Successfully initialised the claw
 * @return false:
 * Failed to initialise the claw.
 */
bool Claws_Init();

/**
 * @brief
 * Simple function that returns a boolean value
 * that corresponds to the claw's limit switch
 * value. This is used to detect if there is
 * something inside of XFactor's claw.
 * @return true:
 * Something is inside the claw.
 * @return false:
 * Nothing is inside the claw
 */
bool Claws_GetSwitchStatus();

/**
 * @brief
 * Function used to set the claw's grabber to a
 * wanted position in pourcents. The claw must be
 * deployed for this function to work.
 *
 * @param pourcent
 * How much should the grabbers be opened?
 * 0 means closed and 100 means completely
 * opened.
 * @return true:
 * Successfully moved the grabbers to the wanted
 * position.
 * @return false:
 * Failed to move the grabbers because the claw
 * is either not deployed or the specified value
 * is outside of the allowed range.
 */
bool Claws_SetGrabbers(unsigned char pourcent);

/**
 * @brief
 * Function used to set the claw's height to a
 * wanted position in pourcents. The claw must be
 * deployed for this function to work.
 *
 * @param pourcent
 * How high should the claw be? 0 is as low as
 * possible and 100 is the highest.
 * @return true:
 * Successfully moved the height to the wanted
 * position.
 * @return false:
 * Failed to move the height because the claw
 * is either not deployed or the specified value
 * is outside of the allowed range.
 */
bool Claws_SetHeight(unsigned char pourcent);

/**
 * @brief
 * Function used to tell the claw to deploy or
 * store itself. While the claw doesn't
 * completely store itself, it will retract its
 * height and its grabbers to be out of the way
 * as much as possible. If you deploy the claw,
 * it will be in a position that is ready to pick
 * up packages.
 *
 * @attention
 * If a package is currently detected inside the
 * claw, deployment function will fail its
 * execution and return false.
 *
 * @param deployment
 * true means deployed and false means stored.
 * @return true:
 * Successfully deployed or stored the claw.
 * @return false:
 * Failed to deploy or store the claw.
 */
bool Claws_SetDeployment(bool deployment);

/**
 * @brief
 * This function slowly closes the claw a little
 * more than they currently are so that a certain
 * amount of force is applied to the package to
 * prevent it from easily slipping out of the
 * claw. The function will return wether or not
 * it was able to secure the package.
 *
 * @attention
 * The amount by which the claw closes to squeeze
 * the package is predefined inside of this
 * function.
 *
 * @return true:
 * Successfully squeezed the package.
 * @return false:
 * Failed to squeeze the package because there
 * either was never a package to start with, the
 * package was lost during squeezing or because
 * the claw is not deployed or not opened enough.
 */
bool Claws_SqueezePackage();

/**
 * @brief
 * Simple function that slowly closes XFactor's
 * grabbers until @ref Claws_GetSwitchStatus
 * detects a package. The speed at which the claw
 * closes its grabbers is set in a define.
 * @return true:
 * Successfully detected a package while closing
 * @return false:
 * Failed to detect a package while closing.
 */
bool Claws_CloseUntilDetection();

// #pragma endregion