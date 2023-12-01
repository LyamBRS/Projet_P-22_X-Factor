/**
 * @file Positions.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains the functions
 * necessary to update and use the current
 * positions of the robot. Current positions
 * should be updated when the robot finishes a
 * movement and reset whenever a vector is saved.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Movements/Positions.hpp"

RobotPosition position;

// start variables (no init)
float positionX_cm = DEMO_AREA_LENGTH_CM - (ROBOT_LENGTH_CM + SAFEBOX_LENGTH_CM);
float positionY_cm = 0.0f;
float absoluteRotation_rad = PI;

float currentRelativeRotation_rad = 0;
float currentDistance_cm = 0;
//MovementVector robotPosition; // Vector representing start to current point.

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
 * @return true:
 * The rotation was updated successfully.
 * @return false:
 * The rotation failed to be updated.
 */
bool UpdateSavedRotation(float newRelativeRotation_rad)
{
  absoluteRotation_rad -= newRelativeRotation_rad;
  currentRelativeRotation_rad = newRelativeRotation_rad;
  return true;
}

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
bool UpdateSavedDistance(float distanceMade_cm)
{
  currentDistance_cm = distanceMade_cm;
  return true;
}

/**
 * @brief Updates the robot's current position
 * based on the recently completed vector.
 * @param positionX_cm
 * @param positionY_cm
 * @return true:
 * The position was updated successfully.
 * @return false:
 * The position failed to be updated.
 * @attention
 * This needs to be called AFTER the two other position updates functions.
 */
bool UpdateSavedPosition()
{
  Debug_Information("Positions", "UpdateSavecPosition", "Position X PRE : " + String(position.positionX_cm));
  Debug_Information("Positions", "UpdateSavecPosition", "Position Y PRE : " + String(position.positionY_cm));
  Debug_Information("Positions", "UpdateSavecPosition", "Rotation PRE : " + String(position.rotation_rad));

  position.positionX_cm += cos(absoluteRotation_rad) * currentDistance_cm;
  position.positionY_cm += sin(absoluteRotation_rad) * currentDistance_cm;
  position.rotation_rad += currentRelativeRotation_rad;

  Debug_Information("Positions", "UpdateSavecPosition", "Position X POST : " + String(position.positionX_cm));
  Debug_Information("Positions", "UpdateSavecPosition", "Position Y POST : " + String(position.positionY_cm));
  Debug_Information("Positions", "UpdateSavecPosition", "Rotation POST : " + String(position.rotation_rad));
  //robotPosition = GetOppositeVector(GetReturnVector());
  return true;
}

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
bool ResetPositions()
{
  currentRelativeRotation_rad = 0;
  currentDistance_cm = 0;

  position.positionX_cm = DEMO_AREA_LENGTH_CM - (ROBOT_LENGTH_CM + SAFEBOX_LENGTH_CM);
  position.positionY_cm = ROBOT_WIDTH_CM / 2;
  position.rotation_rad = PI;
  //robotPosition.distance_cm = 0.0f;
  return true;
}

/**
 * @brief Returns the current value
 * of the global variable that
 * stores the robot's current
 * relative rotation.
 * @return float
 * value of the saved rotation.
 */
float GetSavedRotation()
{
  return currentRelativeRotation_rad;
}

/**
 * @brief Returns the current value
 * of the global variable that
 * stores the robot's current
 * relative rotation.
 * @return float
 * value of the saved rotation.
 */
float GetSavedDistance()
{
  return currentDistance_cm;
}

/**
 * @brief Returns the current value
 * of the global variable that
 * stores the robot's current
 * position.
 * @return RobotPosition:
 * Vector needed to go from start position
 * to the current position
 */
RobotPosition GetSavedPosition()
{
  return position;
}