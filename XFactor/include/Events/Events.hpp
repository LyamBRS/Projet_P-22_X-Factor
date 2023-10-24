/**
 * @file Events.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains the definition
 * of event functions. Event functions are
 * called whenever their associated events
 * happen during execution of XFactor's code.
 * In the event functions, things such as
 * checking for alarms and packages must be
 * done.
 * @version 0.1
 * @date 2023-10-24
 * @copyright Copyright (c) 2023
 */

#pragma once

#pragma region [Movement_Related]

/**
 * @brief Periodically executed when the
 * robot is moving in a straight line.
 * Depending on the robot's current mode 
 * this function will be behaving
 * differently. This function's goal is 
 * to tell if the robot should still be
 * moving and to change its mode if
 * something happens.
 * 
 * @param executionRatio
 * Ratio from 0.0f to 1.0f that tells
 * you how close to being finished the
 * event is. 0 mean the event just
 * started, <1 means the event is in
 * progress and 1 means the event just 
 * finished.
 * @param direction
 * Value from 0 to 1. 0 means
 * the robot is moving forwards. 1 means
 * the robot is moving backwards.
 * @return true:
 * The robot can continue to move
 * @return false:
 * The robot needs to stop.
 */
bool MovingEvent(float executionRatio, unsigned char direction);

/**
 * @brief Periodically executed when the 
 * robot is turning on itself. Depending 
 * on the robot's current mode, this 
 * function will be behaving differently. 
 * This function's goal is to tell if the 
 * robot should still be turning and to 
 * change its mode if something happens.
 * 
 * @param executionRatio
 * Ratio from 0.0f to 1.0f that tells
 * you how close to being finished the
 * event is. 0 mean the event just 
 * started, <1 means the event is in 
 * progress and 1 means the event just 
 * finished.
 * @param direction
 * Value from 0 to 1. 0 means the robot
 * is turning left. 1 means the robot is 
 * turning right.
 * @return true:
 * The robot can continue to turn
 * @return false:
 * The robot needs to stop.
 */
bool TurningEvent(float executionRatio, unsigned char direction);

#pragma endregion