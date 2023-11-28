/**
 * @file Utils.hpp
 * @author WSimard47536
 * @brief File which contains the definitions
 * of utilities functions for actions
 * throughout the program.
 * @version 0.1
 * @date 2023-11-14
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Actions/Utils.hpp"

/**
 * @brief Function that checks
 * for some common cases of failure
 * that returns a new function id based on
 * the specific case encountered.
 * @param currentExecutionFunctionId
 * The id of the current execution function.
 * @return int:
 * Value of the new execution function id to execute, 
 * currentExecutionFunctionId if no changes
 */
int ExecutionUtils_StatusCheck(int currentExecutionFunctionId)
{
  if (SafeBox_ExchangeStatus())
  {
    switch (SafeBox_GetStatus())
    {
      case SafeBox_Status::Unlocked:
        return FUNCTION_ID_UNLOCKED;
      case SafeBox_Status::WaitingForDelivery:
        return FUNCTION_ID_WAIT_FOR_DELIVERY;
      case SafeBox_Status::Alarm:
        Debug_Error("Utils", "ExecutionUtils_StatusCheck", "SAFEBOX IS IN ALARM");
        return FUNCTION_ID_ALARM;
      case SafeBox_Status::Error:
        Debug_Error("Utils", "ExecutionUtils_StatusCheck", "SAFEBOX IS IN ERROR");
        return FUNCTION_ID_ERROR;
      default:
        return currentExecutionFunctionId;
    }
  }
  else
  {
    Debug_Error("Utils","ExecutionUtils_StatusCheck", "Status exchange failed");
  }
  return currentExecutionFunctionId;
}

/**
 * @brief Function that checks
 * if communication has been severed with
 * SafeBox, in which case XFactor goes into Alarm status,
 * as it may mean that the robot has been taken away
 * @param currentExecutionFunctionId
 * The id of the current execution function.
 * @param attempts
 * The number of times communication is tried before the
 * it gives up
 * @param isArmed
 * If it gives up, determines whether or not it
 * will start the alarm
 * @return int:
 * Value of the new execution function id to execute, 
 * currentExecutionFunctionId if no changes
 */
int ExecutionUtils_CommunicationCheck(int currentExecutionFunctionId, int attempts, bool isArmed)
{
  for (int currentAttempt = 0; currentAttempt < attempts; currentAttempt ++)
  {
    if (SafeBox_ExchangeStatus())
    {
      if (SafeBox_GetStatus() != SafeBox_Status::CommunicationError)
      {
        return currentExecutionFunctionId;
      }
    }
    else
    {
      return currentExecutionFunctionId;
      //return FUNCTION_ID_ERROR;
    }
  }

  if (isArmed)
  {
    return FUNCTION_ID_ALARM;
  }
  else
  {
    return FUNCTION_ID_ERROR;
  }
}


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
