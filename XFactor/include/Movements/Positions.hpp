/**
 * @file Positions.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains the functions necessary to update
 * and use the current positions of the robot.
 * Current positions should be updated when the robot
 * finishes a movement and reset whenever a vector is
 * saved.
 * @version 0.1
 * @date 2023-10-23
 * @copyright Copyright (c) 2023
 */

#pragma once

#include "Movements/VectorDefines.hpp"
#include "Movements/Vectors.hpp"

/**
 * @brief Updates the total rotation of the robot
 * from a new rotation. This function needs to be
 * called AFTER the robot made a turn on itself.
 * The total radians will be updated and the
 * functions that saves vectors will use this
 * total rotation to keep track of the robot's
 * orientation relative to SafeBox.
 * @param newRotation
 * The amount of radians that the robot just
 * made.
 * @return true
 * The rotation was updated successfully.
 * @return false
 * The rotation failed to be updated.
 */
bool UpdateSavedRotation(float newRelativeRotation_rad);

/**
 * @brief Updates the robot's current distance
 * in centimeters. This distance is used when
 * a vector needs to be saved. This function
 * needs to be called AFTER the robot
 * successfully made the distance.
 * @param distanceMade
 * @return true:
 * The distance was updated successfully.
 * @return false:
 * The distance failed to be updated.
 */
bool UpdateSavedDistance(float distanceMade_cm);

/**
 * @brief Updates the robot's current position
 * based on the recently completed vector.
 * @param positionX_cm
 * @param positionY_cm
 * @return true:
 * The position was updated successfully.
 * @return false:
 * The position failed to be updated.
 */
bool UpdateSavedPosition();

/**
 * @brief Function that resets both global
 * variables that stores the robot's current
 * position. The distance and rotation will
 * be reset back to 0 when this is called.
 *
 * @attention
 * If vectors were not saved before this
 * function is called, it will be
 * impossible to retrieve the robot's
 * current position relative to the box.
 * @return true:
 * Positions was reset successfully.
 * @return false:
 * The position did not need to be reset.
 */
bool ResetPositions();

/**
 * @brief Returns the current value
 * of the global variable that
 * stores the robot's current
 * relative rotation.
 * @return float:
 * value of the saved rotation.
 */
float GetSavedRotation();

/**
 * @brief Returns the current value
 * of the global variable that
 * stores the robot's current
 * relative rotation.
 * @return float:
 * value of the saved rotation.
 */
float GetSavedDistance();

/**
 * @brief Returns the current value
 * of the global variable that
 * stores the robot's current
 * position.
 * @return RobotPosition:
 * Vector needed to go from start position
 * to the current position
 */
MovementVector GetSavedPosition();