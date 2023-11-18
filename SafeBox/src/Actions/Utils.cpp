
/**
 * @file Utils.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing definitions of Utils functions
 * used in SafeBox.
 * @version 0.1
 * @date 2023-11-18
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Actions/Utils.hpp"

/**
 * @brief
 * This function allows you to create a simple
 * blinking statement in your Execution functions
 * Especially useful for the Alarm and Error.
 * @param blinkingPeriodMS
 * How long should the timer wait before it needs
 * to return the end of period so that you can
 * make your LED blink?
 * @return true:
 * The timer has reached the end. You must make
 * the LED blink.
 * @return false:
 * The timer is still counting.
 */
bool ExecutionUtils_LedBlinker(unsigned long blinkingPeriodMS)
{
    // - VARIABLES - //
    static unsigned long currentMS = millis();
    static unsigned long previousMS = millis();

    currentMS = millis();

    if((currentMS - previousMS) > blinkingPeriodMS)
    {
        previousMS = millis();
        return true;
    }
    return false;
}