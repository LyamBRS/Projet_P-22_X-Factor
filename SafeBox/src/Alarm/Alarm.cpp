/**
 * @file Alarm.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the functions used to handle
 * and detect alarms in the SafeBox program.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Alarm/Alarm.hpp"


/**
 * @brief
 * Function that initialises the Alarm and its
 * components such as the accelerometer installed
 * on SafeBox. Other components are initialised
 * on their own or inside other inits because
 * they are used by other things than the alarm.
 * @return true:
 * Successfully initialised the alarm.
 * @return false:
 * Failed to initialise the alarm.
 */
bool Alarm_Init()
{
    Debug_Start("Alarm_Init");
    //Debug_Warning("Alarm", "Alarm_Init", "BYPASSED");
    pinMode(ALARM_BUZZER_PIN, OUTPUT);
    Debug_End();
    return true;
}

/**
 * @brief
 * Sets the needed alarm state.
 *
 * @return true:
 * Successfully set alarm to specified mode
 * @return false:
 * Failed to set alarm to specified mode
 */
bool Alarm_SetState(bool wantedState)
{
    if(wantedState)
    {
        digitalWrite(ALARM_BUZZER_PIN, HIGH);
    }
    else
    {
        digitalWrite(ALARM_BUZZER_PIN, LOW);
    }

    return true;
}

/**
 * @brief
 * Verifies the accelerometer's values to see if
 * the alarm needs to be activated based off the
 * values returned.
 *
 * @return true:
 * An alarm needs to be activated.
 * @return false:
 * No alarm needs to be activated.
 */
bool Alarm_VerifyGyro()
{
    return false;
}

/**
 * @brief
 * Function that utilizes the Package status to
 * figure out if a package has been stolen inside
 * of SafeBox. If the package status is false,
 * this function is bypassed because there is no
 * package to steal.
 * @return true:
 * An alarm needs to be activated.
 * @return false:
 * No alarm needs to be activated.
 */
bool Alarm_VerifyPackage()
{
    return false;
}