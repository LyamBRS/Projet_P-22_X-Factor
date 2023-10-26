/**
 * @file Package.hpp
 * @author LyamBRS (ltam.brs@gmail.com)
 * @brief
 * File containing the header definitions of the
 * various functions that are used to locate,
 * identify, pick up, handle, and deposite a
 * package for XFactor. 
 * @version 0.1
 * @date 2023-10-26
 * @copyright Copyright (c) 2023
 */

#pragma once

#pragma region [DEFINES]
#define PACKAGE_CLAW_GRABBER_POSITION_TRANSPORT
#define PACKAGE_CLAW_HEIGHT_POSITION_TRANSPORT
#pragma endregion

#pragma region [FUNCTIONS]
/**
 * @brief 
 * Initialisation function that initialises the
 * Claw and other sensors used to detect a
 * package.
 * @return true:
 * Successfully initialised package functions. 
 * @return false:
 * Failed to initialise package functions.
 */
bool Package_Innit();

/**
 * @brief
 * Function that releases the package that is
 * currently inside of XFactor's claw system.
 * 
 * @return true:
 * A package was successfully released from the
 * claw system.
 * @return false:
 * Failed to release the package. Either because
 * there was no package inside the claw to start
 * with, or because the package is still detected
 * inside the claw after the function is executed
 * or because the claw is not deployed. 
 */
bool Package_Release();

/**
 * @brief
 * Function that automatically picks up a package
 * that is right where the claw is. This function
 * will deploy the claw if not already done.
 * 
 * @attention
 * The robot must already be facing the package
 * correctly before this function is executed.
 * 
 * @return true:
 * Successfully picked up a package
 * @return false:
 * Failed to pick up a package, try again.
 */
bool Package_PickUp();

/**
 * @brief Function that aligns XFactor's claw
 * with the position required to drop packages
 * inside of SafeBox. This function can be
 * executed even if there is no packages inside
 * of the claw.
 * 
 * @return true:
 * The claw is positioned how it needs to be to
 * drop packages inside SafeBox. 
 * @return false:
 * Failed to position the claw into the correct
 * position due to it not being deployed.
 */
bool Package_AlignWithSafeBox();

/**
 * @brief 
 * Function that stores the claw in a way that is
 * as small as possible so that its out of the
 * way when its not in use. This function only
 * calls @ref Claws_SetDeployement but could do
 * more if required.
 * 
 * @return true:
 * Successfully stored away XFactor's claw.
 * @return false:
 * Failed to store the claw because a package is
 * still detected inside of it.
 */
bool Package_StoreClaw();

/**
 * @brief 
 * Deploys XFactor's package recovery claw system
 * through @ref Claws_SetDeployement. This must
 * have been called prior to attempting to pick
 * up a package. 
 * @return true:
 * Successfully deployed the claw.
 * @return false:
 * Failed to deploy the claw.
 */
bool Package_DeployClaw();

/**
 * @brief
 * Arranges XFactor's claw into the best solution
 * for package transportation. This function must
 * be called before the robot starts to move with
 * a package inside of the claw. It ensures that
 * the package won't be in the way as much as it
 * would've if the function is not called.
 * 
 * @return true:
 * Successfully moved the claw into the
 * appropriated position to transport a package.
 * @return false:
 * Failed to move the claw into the wanted
 * position. Either because no package is found
 * inside the claw or because the claw is not
 * deployed.
 */
bool Package_Transport();

/**
 * @brief
 * Complex function that analyses XFactor's
 * related sensors to find potential packages
 * that may be around the robot as it moves. This
 * Function can also be used to verify that a
 * package is still inside of the claw.
 * 
 * Packages are identified using color sensors.
 * @return true:
 * A package was detected near the robot.
 * @return false:
 * No packages are detected anywhere near or
 * inside the robot.
 */
bool Package_Detected();

/**
 * @brief
 * Function that defines if XFactor should be
 * expecting to carry a package or not. For
 * example, if XFactor finds a package and picks
 * it up, this function should be called to
 * make the program know that XFactor should be
 * carrying a package. This way, if the package
 * is no longer detected, an alarm can be turned
 * on.
 * 
 * This also allows the alarm to not be turned on
 * if the package is supposed to be no longer
 * detected.
 * 
 * @param newPackageStatus
 * Should XFactor expect a package inside its
 * claw?
 * @return true:
 * The new package status was set successfully. 
 * @return false:
 * Failed to set a new package status.
 */
bool Package_SetStatus(bool newPackageStatus);

/**
 * @brief 
 * Function that simply returns the expected
 * package state.
 * 
 * @return true:
 * XFactor is supposed to be carrying a package
 * @return false:
 * XFactor shouldn't be carrying a package.
 */
bool Package_GetStatus();
#pragma endregion