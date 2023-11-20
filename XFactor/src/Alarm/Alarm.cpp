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

    // TODO: find the optimal triplet of {frequency interval - number of readings - threshold} to detect the moving --> HARD AF !
    // TODO: maybe use different thresholds for each axes ?
    unsigned interval = 500; // Interval for frequency analysis in milliseconds
    unsigned nbReadings = 1000;
    float deltaThreshold = 0.02; // Adjust the delta threshold as needed
    unsigned long currentMillis = 0;
    unsigned long previousMillis = 0;
    // TODO: WARNING TOO MUCH ERROR ACCULUMATION IN THIS FLOAT OPERATING SUM
    float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
    float avgX = 0.0f, avgY = 0.0f;
    float prevXAccel = 0.0f;
    float prevYAccel = 0.0f;
    float deltaX = 0.0f;
    float deltaY = 0.0f;

    // Z axes data, uncomment if neccessary
    // float avgZ = 0.0f;
    // float prevZAccel = 0.0f;
    // float deltaZ = 0.0f;
    for (size_t j = 0; j < ACCELEROMETER_NB_CHECKING; ++j)
    {
        currentMillis = millis();
        if (currentMillis - previousMillis >= interval)
        {
            // Perform frequency analysis
            for (size_t i = 0; i < nbReadings; ++i)
            {
                sumX += Accelerometer_GetX();
                sumY += Accelerometer_GetY();
                // sumZ += Accelerometer_GetZ();
            }

            avgX = sumX / nbReadings;
            avgY = sumY / nbReadings;
            // avgZ = sumZ / nbReadings;

            deltaX = abs(avgX - prevXAccel);
            deltaY = abs(avgY - prevYAccel);
            // deltaZ = abs(avgZ - prevZAccel);

            Serial.print("AvgX: ");
            Serial.println(avgX);
            Serial.print("AvgY: ");
            Serial.println(avgY);
            // Serial.print("AvgZ: ");
            // Serial.println(avgZ);

            Serial.print("deltaX: ");
            Serial.println(deltaX);
            Serial.print("deltaY: ");
            Serial.println(deltaY);
            // Serial.print("AvgZ: ");
            // Serial.println(avgZ);

            // Motion detected
            if (deltaX > deltaThreshold || deltaY > deltaThreshold) // || deltaZ > deltaThreshold
            {
                Serial.println("Motion detected using delta threshold!");
                return true;
            }

            // Update previous values for delta threshold
            prevXAccel = avgX;
            prevYAccel = avgY;
            // prevZAccel = avgZ;

            // Reset the interval timer
            previousMillis = currentMillis;
        }
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