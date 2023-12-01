/**
 * @file Doorbell.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the functions used to handle
 * the doorbell reading done by SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Sensors/Doorbell/Doorbell.hpp"

/**
 * @brief
 * Function that initialises the whistle detector
 * or doorbell reader used by SafeBox.
 *
 * @return true:
 * Successfully initialised the doorbell used by
 * SafeBox. (Whistle detector)
 * @return false:
 * Failed to initialised the doorbell used on
 * SafeBox.
 */
bool Doorbell_Init()
{
    Debug_Start("Doorbell_Init");
    pinMode(DOORBELL_AMBIENT_NOISE_PIN, INPUT);
    pinMode(DOORBELL_WHISLE_NOISE_PIN, INPUT);
    pinMode(DOORBELL_SWITCH_BYPASS_PIN, INPUT);
    Debug_End();
    return true;
}

/**
 * @brief
 * Function that returns if there is a doorbell
 * that is currently being detected or not.
 * @return true:
 * Doorbell is being ran live.
 * @return false:
 * No doorbell detected.
 */
bool Doorbell_GetState()
{
    Debug_Start("Doorbell_GetState");
    bool doorbellDetected = true;
    int ambiantNoisesLevel = analogRead(DOORBELL_AMBIENT_NOISE_PIN);
    int whistleNoisesLevel = analogRead(DOORBELL_WHISLE_NOISE_PIN);
    
    doorbellDetected = (ambiantNoisesLevel < whistleNoisesLevel);
    if(doorbellDetected)
    {
        Debug_Information("Doorbell", "Doorbell_GetState", "DETECTED");
        Debug_End();
        return true;
    }

    if(digitalRead(DOORBELL_SWITCH_BYPASS_PIN))
    {
       Debug_Information("Doorbell", "Doorbell_GetState", "BYPASS_DETECTED");
        Debug_End();
        return true;      
    }

    Debug_End();
    return false;
}