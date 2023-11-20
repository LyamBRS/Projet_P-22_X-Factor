/**
 * @file main.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Arduino's main file.
 * @version 0.1
 * @date 2023-11-01
 * @copyright Copyright (c) 2023
 */

// - INCLUDES -//
#include "Actions/Actions.hpp"
#include "XFactor/Init.hpp"

// TODO: find the optimal triplet of {frequency interval - number of readings - threshold} to detect the moving --> HARD AF !
const long interval = 500; // Interval for frequency analysis in milliseconds
unsigned nbReadings = 1000;
float deltaThreshold = 0.02; // Adjust the delta threshold as needed
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
// TODO: WARNING ERROR ACCULUMATION IN A FLOAT OPERATING SUM
float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
float avgX = 0.0f, avgY = 0.0f, avgZ = 0.0f;
float prevXAccel = 0.0f;
float prevYAccel = 0.0f;
float prevZAccel = 0.0f;
float deltaX = 0.0f;
float deltaY = 0.0f;
float deltaZ = 0.0f;

void setup()
{
    Serial.begin(9600);
}

void loop()
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
        avgZ = sumZ / nbReadings;

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

        if (deltaX > deltaThreshold || deltaY > deltaThreshold) //|| deltaZ > deltaThreshold)
        {
            // Motion detected
            Serial.println("Motion detected using delta threshold!");
        }
        else
        {
            // No motion
            // Serial.println("No motion using delta threshold");
        }
        // Update previous values for delta threshold
        prevXAccel = avgX;
        prevYAccel = avgY;
        // prevZAccel = avgZ;
        // Reset the interval timer
        previousMillis = currentMillis;
    }
    sumX = 0;
    sumY = 0;
    sumZ = 0;
}

