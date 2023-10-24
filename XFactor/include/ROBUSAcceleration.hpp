#pragma once

#include <LibRobus.h>

/**
 * @brief 
 * Define that sets at which speed the robot starts.
 * This cannot be 0 because otherwise the robot wont ever move.
 */
#define START_SPEED_OFFSET 0.05

/**
 * @brief define that defines at which rotation
 * count should the ratio of speed reach the maximum
 * speed of 1. By default, is set to 10000 encoder turns
 * or 1 meters in distance.
 */
#define MAX_SPEED_AT_THIS_DISTANCE 10000

/**
 * @brief This function's use is to calculate the wanted speed
 * factor depending on how much distance is left to move to
 * reach its wanted destination. This allows the robot to perform
 * an acceleration when starting its movement and a deacceleration
 * when its nearing the end of its distance.
 * 
 * This is important to make the robot go faster than instant
 * acceleration that would otherwise cause slip and potential
 * drifting.
 * @param distanceLeft 
 * a ratio from 0 to 1 that indicates how close the robot is
 * to reaching its destination.
 * @param totalDistance
 * this parameter is used to tune the acceleration depending on the
 * total distance there is to do. If the distance is too short,
 * the acceleration would be instant and if its too long, it would
 * simply be super slow. This should theorically base the speed itself.
 * @param maximumSpeed 
 * a ratio from 0 to 1 that indicates how fast the wheel should get.
 * @return float 
 */
float ROBUS_GetSpeedFactorFromCurrentPosition(float distanceLeft, float totalDistance, float maximumSpeed);