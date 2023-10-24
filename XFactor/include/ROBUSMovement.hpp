#pragma once
#include <LibRobus.h>
#include "ROBUSAcceleration.hpp"
#define CONSTANTE_RATIO 925

#define FORWARD 1.0f
#define BACKWARD -1.0f

//#define LEFT_TURN -1.0f
//#define RIGHT_TURN 1.0f

#define LEFT_TURN 1.0f
#define RIGHT_TURN -1.0f

#define FULL_ROTATIONS_PULSES 3200 // 
#define DISTANCE_BT_WHEEL 0.196f // rayon entre les deux roues en metre 
#define DIAMETER_WHEEL 0.076f // diametre de la roue (m)

#define MAGIC_lEFT 0.93
#define MAGIC_RIGHT 0.94

//0.9597

#define LARGE_CIRCUMFERENCE_ROBOT (3.1416*DISTANCE_BT_WHEEL*3)
#define SMALL_CIRCUMFERENCE_ROBOT (3.1416*DISTANCE_BT_WHEEL)

#define CIRCUMFERENCE_WHEEL (3.1416*DIAMETER_WHEEL)


#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1
#define LEFT_ENCODER 0
#define RIGHT_ENCODER 1

#define DIAMETER 7.60f
#define CIRCUMFERENCE 23.87f
#define FULL_ROTATIONS_PULSES 3200.00f

void ROBUSMovement_adjustDirection(float speed_pct, int delay_ms, float distanceRatio, float totalDistance);
void ROBUSMovement_moveStraight(float direction, float speed_pct, float distance_cm);
void ROBUSMovement_turn(float direction, float speed_pct, int turnRadius_degreesPerWheelCycle, int turnDistance_degrees);

//float PID_test(float Ubias, float proportional_const, float integral_const, float derivative_const, float set_point, float process_variable, float time, float* previousDerivativeError, float* errorSum);
//float PID_calibration_test(float Ubias, float* Ku, float set_point, float process_variable);

//void ROBUSMovement_test(float direction, float speed_pct, int distance, int degrees);

//float ROBUSMovement_straight_math(float distance, float wheel);
float ROBUSMovement_turnOnSelf_math(float distance, float wheel, int degrees);

void ROBUSMovement_straight(float direction, float speed_pct, int distance);
void ROBUSMovement_turnOnSelf(float direction, float speed_pct, int turnDistance_degrees);
float PID(float Kc, float Ti, float set_point, float process_variable, int nbr_cycle, float start_time, float current_time, float Ubias, float* erreur_integrale);




void ROBUSMovement_stop();


//hpp de l acceleration


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