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

    unsigned nbReadings = 50;
    float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
    float avgX = 0.0f, avgY = 0.0f, avgZ = 0.0f;

    // Do a first scan for initial values
    // This was moved form the Accelerometer_init() fucntion because robos mouvement influnces the offsets
    for (size_t i = 0; i < nbReadings; i++)
    {
        sumX += Accelerometer_GetX();
        sumY += Accelerometer_GetY();
        sumZ += Accelerometer_GetZ();
    }

    AcceX_zero = (sumX / nbReadings);
    AcceY_zero = (sumY / nbReadings);
    AcceZ_zero = (sumZ / nbReadings);

    Serial.print("Using AcceX_zero:\t");
    Serial.println(AcceX_zero);

    Serial.print("Using AcceY_zero:\t");
    Serial.println(AcceY_zero);

    Serial.print("Using AcceZ_zero:\t");
    Serial.println(AcceZ_zero);

    float deltaThresholdX = abs(AcceX_zero + (AcceX_zero * 0.1)); // Warning: make sur that Accelerometer_init() is called before this, 1% error rate is tolerated
    float deltaThresholdY = abs(AcceY_zero + (AcceY_zero * 0.1)); // Warning: make sur that Accelerometer_init() is called before this, 10% error rate is tolerated
    float deltaThresholdZ = abs(AcceZ_zero + (AcceZ_zero * 0.05)); // Warning: make sur that Accelerometer_init() is called before this, 5% error rate is tolerated

    Serial.print("using deltaThresholdX:\t ");
    Serial.println(deltaThresholdX);

    Serial.print("using deltaThresholdY:\t");
    Serial.println(deltaThresholdY);

    Serial.print("using deltaThresholdZ:\t");
    Serial.println(deltaThresholdZ);

    // Do ACCELEROMETER_NB_CHECKING scans
    sumX = 0, sumY = 0, sumZ = 0;
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

        Serial.print("AvgX:\t");
        Serial.println(avgX);
        Serial.print("AvgY:\t");
        Serial.println(avgY);
        Serial.print("AvgZ:\t");
        Serial.println(avgZ);

        // Motion detection thresholds
        if (avgX > deltaThresholdX || avgY > deltaThresholdY)
        {
            if (avgZ > deltaThresholdZ)
            {
                Serial.println("Motion detected using delta threshold!");
                for (size_t i = 0; i < 3; i++)
                {
                    AX_BuzzerON();
                    delay(200);
                    AX_BuzzerOFF();
                }
                return true;
            }
        }
        sumX = 0, sumY = 0, sumZ = 0;
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
    for (size_t i = 0; i < 40; i++)
    {
        MOTOR_SetSpeed(LEFT, 0.2);
        MOTOR_SetSpeed(RIGHT, 0.2);
        Alarm_VerifyAccel();
    }

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}
