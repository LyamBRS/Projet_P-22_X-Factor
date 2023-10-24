#include "math.h"
#include "ROBUSAcceleration.hpp"


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
float ROBUS_GetSpeedFactorFromCurrentPosition(float distanceLeft, float totalDistance, float maximumSpeed)
{
  #pragma region -ERROR_CASES-
  if(distanceLeft > 1)
  {
    //Serial.println("Acceleration.cpp : l24 : ratio cannot be higher than 1.");
    return 0;
  }

  if(distanceLeft < 0)
  {
    //Serial.println("Acceleration.cpp : l30 : ratio cannot be lower than 0.");
    return 0;
  }
  #pragma endregion

  #pragma region -SPEED COMPENSATION-

  //Serial.println(totalDistance);

  // Adjusts the speeds according to the total distance we need to make
  if (totalDistance > MAX_SPEED_AT_THIS_DISTANCE)
  {
    maximumSpeed = 1;
  }
  else
  {
    float distanceRatio = totalDistance/MAX_SPEED_AT_THIS_DISTANCE;
    maximumSpeed = distanceRatio + START_SPEED_OFFSET;
  }

  /**
   * @brief allows the sin operator to
   * make a bell curve of the wanted speed
   * depending on the distance left to make
   * to reach the wanted destination.
   */
  float radians = distanceLeft * 3.14f;

  //float fixedSin = pow(sin(radians), 3);

  return (sin(radians) * maximumSpeed) + START_SPEED_OFFSET;

  #pragma endregion
}