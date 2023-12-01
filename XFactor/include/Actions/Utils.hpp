/**
 * @file Utils.hpp
 * @author WSimard47536
 * @brief File which contains the header
 * definition of utilities functions for actions
 * throughout the program.
 * @version 0.1
 * @date 2023-11-14
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDE - //
#include "LED/LED.hpp"
#include "SafeBox/Communication.hpp"
#include "XFactor/Status.hpp"
#include "Movements/Movements.hpp"

//#pragma region [FUNCTION_IDS]

#define FUNCTION_ID_WAIT_AFTER_SAFEBOX 0
#define FUNCTION_ID_WAIT_FOR_DELIVERY 1
#define FUNCTION_ID_GETTING_OUT_OF_GARAGE 2
#define FUNCTION_ID_SEARCH_PREPARATIONS 3
#define FUNCTION_ID_SEARCH_FOR_PACKAGE 4
#define FUNCTION_ID_AVOID_OBSTACLE 5
#define FUNCTION_ID_EXAMINE_FOUND_PACKAGE 6
#define FUNCTION_ID_PICK_UP_PACKAGE 7
#define FUNCTION_ID_RETURN_HOME 8
#define FUNCTION_ID_PREPARING_FOR_DROP_OFF 9
#define FUNCTION_ID_PACKAGE_DROP_OFF 10
#define FUNCTION_ID_CONFIRM_DROP_OFF 11
#define FUNCTION_ID_ALARM 12
#define FUNCTION_ID_ERROR 13
#define FUNCTION_ID_RETURN_INSIDE_GARAGE 14
#define FUNCTION_ID_END_OF_PROGRAM 15
#define FUNCTION_ID_UNLOCKED 16
#define FUNCTION_ID_CALIBRATE_COLOUR 17

//#pragma endregion

/**
 * @brief 
 * Function which blocks the program until a
 * successful status exchange occurs with
 * SafeBox. This function will execute until
 * the status received isnt CommunicationError
 */
void ExecutionUtils_ForceAStatusExchange();

/**
 * @brief
 * This function handles the first time that a
 * state gets executed. Its goal is to set the
 * status of XFactor to the corresponding,
 * specified status, and erase the status that
 * is currently saved for SafeBox. This prevents
 * the program from automatically changing the
 * execution function back to an unwanted
 * execution function automatically.
 * 
 * @param wantedStatus
 * The status that XFactor should hold while in
 * this current state.
 */
void ExecutionUtils_HandleFirstExecution(XFactor_Status wantedStatus);

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
int ExecutionUtils_StatusCheck(int currentExecutionFunctionId);

/**
 * @brief Function that checks
 * if XFactor can communicate with
 * SafeBox and if it doesn't return
 * the communication error status
 * @param attempts
 * The number of times communication is tried before 
 * it gives up
 * @return bool:
 * If it has succeeded to get a good communication
 * in the specified amount of attempts
 */
bool ExecutionUtils_CommunicationCheck(int attempts);

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
int ExecutionUtils_CommunicationCheck(int currentExecutionFunctionId, int attempts, bool isArmed);

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
bool ExecutionUtils_LedBlinker(unsigned long blinkingPeriodMS);

/**
 * @brief Function that checks
 * if something unexpected happened in
 * movement and returns status of the
 * event that happened
 * @param currentExecutionFunctionId
 * The id of the current execution function.
 * @return int:
 * Value of the new execution function id to execute, 
 * currentExecutionFunctionId if no changes
 */
int ExecutionUtils_ComputeMovementResults(int currentExecutionFunctionId, int movementStatus);