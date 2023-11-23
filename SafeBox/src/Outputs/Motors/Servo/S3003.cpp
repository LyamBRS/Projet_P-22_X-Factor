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

Servo servo1;
Servo servo2;
Servo servo3; 

/**
 * @brief Initialisation function of a single
 * servo motor based on a given arduino pin
 * number.
 * @param servoMotorPin
 * Arduino pin number
 */
void S3003_Init()
{
    servo1.attach(SERVO_RFID_PIN); 
    servo1.write(95);
    servo2.attach(SERVO_PORTE_HAUT_PIN ); 
    servo2.write(85);
    servo3.attach(SERVO_PORTE_BAS_PIN ); 
    servo3.write(85);
}

/**
 * @brief Simple function that must put the right
 * PWM value at the specified pin.
 * This function should transform a value from 0 to 100
 * into the right PWM number for the servo to reach that
 * position.
 * @param WantedPosition
 */
void S3003_SetPosition(int angle)
{

    servo1.write(angle);
    servo2.write(angle);
    servo3.write(angle);
}
