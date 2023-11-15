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
#include "Actions/Actions.hpp"

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
    default:
      return currentExecutionFunctionId;
    }
  }

  return FUNCTION_ID_ERROR;
}
