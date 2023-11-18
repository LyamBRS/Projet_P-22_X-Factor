/**
 * @file ServoMotors.hpp
 * @author Lyam BRS (lyam.brs@gmail.com)
 * @brief Header file used for servo motors colour sensor.
 * @version 0.1
 * @date 2023-10-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include "LibRobus.h"

#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

#define SERVO_VERTICAL_AXIS 0
#define SERVO_CLAW 1

#define SERVO_VERTICAL_AXIS_MIN_ANGLE 90
#define SERVO_VERTICAL_AXIS_MAX_ANGLE (SERVO_VERTICAL_AXIS_MIN_ANGLE+45)

#define SERVO_CLAW_MIN_ANGLE 0
#define SERVO_CLAW_MAX_ANGLE 180

/**
 * @brief Initialisation function of a single
 * servo motor based on a given arduino pin
 * number.
 * @param servoMotorPin 
 * Arduino pin number
 */
void S3003_Init(int servoMotorPin);

/**
 * @brief Simple function that must put the right
 * PWM value at the specified pin.
 * This function should transform a value from 0 to 100
 * into the right PWM number for the servo to reach that
 * position.
 * @param WantedPosition 
 */
void S3003_SetPosition(int servoMotorPin, float WantedPosition);

// I suggest making more functions. You could make one that makes the big number from the smaller number for example.
// The functions you make should start with GROVE_. variables as inputs should be lower camelCase.
