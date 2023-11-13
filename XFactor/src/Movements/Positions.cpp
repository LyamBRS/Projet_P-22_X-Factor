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

RobotPosition robotPosition;

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
bool UpdateSavedRotation(float newRotation)
{
    return false;
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
bool UpdateSavedDistance(float distanceMade)
{
    return false;
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
    return false;
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
  // MAYBE VALIDATE WITH ACCELEROMETER TO REALIGN THAT BIG BOI ???
  int totalLeftPulses = robotPosition.positionXLeft_encoderPulses + robotPosition.positionYLeft_encoderPulses;
  int totalRightPulses = robotPosition.positionXRight_encoderPulses + robotPosition.positionYRight_encoderPulses;
  // use 3200 in #define when PID gotten there
  return (float)atan((double)(totalLeftPulses % 3200) / (double)(totalRightPulses % 3200)) * (180 / 3.14); // converting in degrees since atan works in radians
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
  float totalLeftPulses = robotPosition.positionXLeft_encoderPulses + robotPosition.positionYLeft_encoderPulses;
  return totalLeftPulses;
}