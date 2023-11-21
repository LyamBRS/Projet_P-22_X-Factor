/**
 * @file Alarm.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the functions used to handle
 * and detect alarms in the XFactor program.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Alarm/Alarm.hpp"
#include "LibRobus.h"

/**
 * @brief
 * Function that initialises the Alarm and its
 * components such as the accelerometer installed
 * on XFactor. Other components are initialised
 * on their own or inside other inits because
 * they are used by other things than the alarm.
 * @return true:
 * Successfully initialised the alarm.
 * @return false:
 * Failed to initialise the alarm.
 */
bool Alarm_Init()
{
    return Accelerometer_Init() && Package_Init();
}

/**
 * @brief
 * Verifies all the sensors of XFactor that can
 * be utilised to trigger an alarm.
 *
 * @return true:
 * An alarm needs to be triggered
 * @return false:
 * No alarm needs to be triggered
 */
bool Alarm_VerifySensors()
{
    return Alarm_VerifyAccel() || Alarm_VerifyPackage();
}

/**
 * @brief
 * Verifies the accelerometer's values to see if
 * the alarm needs to be activated based off the
 * values returned.
 * @return true:
 * Sensor linear acceleration has depased a certain threshold
 * @return false:
 * Sensor is not mouved in that time interval
 */
bool Alarm_VerifyAccel()
{
    unsigned nbReadings = 25;
    float deltaThresholdX = abs(AcceX_zero + (AcceX_zero * 0.05));  // Warning: make sur that Accelerometer_init() is called before this, 5% error rate is tolerated
    float deltaThresholdY = abs(AcceY_zero + (AcceY_zero * 0.05));  // Warning: make sur that Accelerometer_init() is called before this, 5% error rate is tolerated
    float deltaThresholdZ = abs(AcceZ_zero + (AcceZ_zero * 0.05)); // Warning: make sur that Accelerometer_init() is called before this, 5% error rate is tolerated

    Serial.print("using deltaThresholdX = ");
    Serial.println(deltaThresholdX);

    Serial.print("using deltaThresholdY = ");
    Serial.println(deltaThresholdY);

    Serial.print("using deltaThresholdZ = ");
    Serial.println(deltaThresholdZ);

    float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
    float avgX = 0.0f, avgY = 0.0f, avgZ = 0.0f;

    // Perform ACCELEROMETER_NB_CHECKING analyse
    for (size_t i = 0; i < ACCELEROMETER_NB_CHECKING; i++)
    {
        for (size_t j = 0; j < nbReadings; j++)
        {
            sumX += Accelerometer_GetX();
            sumY += Accelerometer_GetY();
            sumZ += Accelerometer_GetZ();
        }

        avgX = abs(sumX / nbReadings);
        avgY = abs(sumY / nbReadings);
        avgZ = abs(sumZ / nbReadings);

        Serial.print("AvgX: ");
        Serial.println(avgX);
        Serial.print("AvgY: ");
        Serial.println(avgY);
        Serial.print("AvgZ: ");
        Serial.println(avgZ);

        // Motion detection
        if (avgX > deltaThresholdX || avgY > deltaThresholdY)
        {
            if (avgZ > deltaThresholdZ)
            {
                Serial.println("Motion detected using delta threshold!");
                AX_BuzzerON();
                delay(200);
                AX_BuzzerOFF();
                return true;
            }
        }
        sumX = 0;
        sumY = 0;
        sumZ = 0;
    }

    return false;
}

/**
 * @brief
 * Function that utilises the Package status to
 * figure out if a package has been stolen inside
 * of XFactor. If the package status is false,
 * this function is bypassed because there is no
 * package to steal.
 * @return true:
 * An alarm needs to be activated.
 * @return false:
 * No alarm needs to be activated.
 */
bool Alarm_VerifyPackage()
{

    return !Package_GetStatus();
}

bool MoveStraightAndTest()
{
    for (size_t i = 0; i < 100; i++)
    {
        MOTOR_SetSpeed(LEFT, 0.3);
        MOTOR_SetSpeed(RIGHT, 0.3);
        Alarm_VerifyAccel();
    }

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}
