/**
 * @file Claw.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File which contains the functions that
 * controls XFactor's package
 * recovery claw. These functions are then
 * used by regular Package functions.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Package/Claws.hpp"


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
bool Claws_Innit()
{
    return false;
}

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
bool Claws_GetSwitchStatus()
{
    return false;
}

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
bool Claws_SetGrabbers(unsigned char pourcent)
{
    return false;
}

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
bool Claws_SetHeight(unsigned char pourcent)
{
    return false;
}

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
bool Claws_SetDeployment(bool deployment)
{
    return false;
}

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
bool Claws_SqueezePackage()
{
    return false;
}

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
bool Claws_CloseUntilDetection()
{
    return false;
}