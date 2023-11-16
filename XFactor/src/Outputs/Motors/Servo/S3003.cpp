/**
 * @file ServoMotors.cpp
 * @author Lyam BRS (lyam.brs@gmail.com)
 * @brief File used for servo motors S3003.
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */

// - INCLUDES - //
#include "Outputs/Motors/Servo/S3003.hpp"

/**
 * @brief Initialisation function of a single
 * servo motor based on a given arduino pin
 * number.
 * @param servoMotorPin
 * Arduino pin number
 */
void S3003_Init(int servoMotorPin)
{
    SERVO_Enable(servoMotorPin);
}

/**
 * @brief Simple function that must put the right
 * PWM value at the specified pin.
 * This function should transform a value from 0 to 100
 * into the right PWM number for the servo to reach that
 * position.
 * @param WantedPosition
 */
void S3003_SetPosition(int servoMotorPin, float wantedPosition)
{
    const float WANTED_POSITION_MAX_VALUE = 100;
    float angle = wantedPosition * MAX_SERVO_ANGLE / WANTED_POSITION_MAX_VALUE;
    
    SERVO_SetAngle(servoMotorPin, angle);
}
