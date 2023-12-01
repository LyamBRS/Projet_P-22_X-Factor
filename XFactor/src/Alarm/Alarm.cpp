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

float deltaThresholdX;
float deltaThresholdY;
float deltaThresholdZ;

float valueX, valueY, valueZ;
float avgX, avgY, avgZ;
int counter;

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
    if (!Accelerometer_Init())
    {
        Debug_Error("Alarm", "Alarm_Init", "Failed to initialize accelerometer");
        return false;
    }

    pinMode(ACCELEROMETER_BYPASS_PIN, INPUT);

    counter = 0;
    
    avgX = 0.0f;
    avgY = 0.0f;
    avgZ = 0.0f;

    valueX = 0.0f;
    valueY = 0.0f;
    valueZ = 0.0f;

    Alarm_ResetThresholds();
    
    return true;
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
    return Alarm_VerifyAccelerometer() || Alarm_VerifyPackage();
}

/**
 * @brief
 * Resets the X and Y variation
 * thresholds to prevent the alarm
 * from setting off its own
 * movements
 */
void Alarm_ResetThresholds()
{
    unsigned nbReadings = 10;
    float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;

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

    deltaThresholdX = abs(AcceX_zero + (AcceX_zero * 0.20) + THRESHOLD_OFFSET_X); // Warning: make sur that Accelerometer_init() is called before this, 1% error rate is tolerated
    deltaThresholdY = abs(AcceY_zero + (AcceY_zero * 0.20) + THRESHOLD_OFFSET_Y); // Warning: make sur that Accelerometer_init() is called before this, 10% error rate is tolerated
    deltaThresholdZ = abs(AcceZ_zero + (AcceZ_zero * 0.25) + THRESHOLD_OFFSET_Z); // Warning: make sur that Accelerometer_init() is called before this, 5% error rate is tolerated
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
bool Alarm_VerifyAccelerometer()
{
    static int alarmTriggerCounter = 0;

    valueX += Accelerometer_GetX();
    valueY += Accelerometer_GetY();
    valueZ += Accelerometer_GetZ();

    if (digitalRead(ACCELEROMETER_BYPASS_PIN))
    {
        return false;
    }

    if (counter >= THRESHOLD_VERIFY_ALARM_COUNTER)
    {
        avgX = abs(valueX / counter);
        avgY = abs(valueY / counter);
        avgZ = abs(valueZ / counter);

        valueX = 0.0f;
        valueY = 0.0f;
        valueZ = 0.0f;

        counter = 0;
        if (avgX > deltaThresholdX || avgY > deltaThresholdY || avgX < -deltaThresholdX || avgY < -deltaThresholdY)// || avgZ > deltaThresholdZ)
        {
            alarmTriggerCounter++;
            if (alarmTriggerCounter > 7)
            {
                return true;
            }
            return false;
        }
        alarmTriggerCounter = 0;
    }

    counter++;
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
    if (Package_GetStatus())
    {
        return !Claws_GetSwitchStatus();
    }
    return false;
}
