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
 * @param servoMotorPin
 * Represents the pin on the arduino where the
 * PWM output is located.
 * @param wantedPosition
 * How much PWM should there be on the motor.
 */
void S3003_SetPosition(int servoMotorPin, float wantedPosition)
{
    SERVO_SetAngle(servoMotorPin, wantedPosition);
}
