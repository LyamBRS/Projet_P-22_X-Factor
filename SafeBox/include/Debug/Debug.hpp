/**
 * @file Debug.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing debug functions to call to
 * debug your code so they can easily be kept
 * in the program and removed for compilation
 * whenever necessary.
 * @version 0.5
 * @date 2023-11-14
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "Arduino.h"

// - DEFINES - //
/// @brief Switch between DEBUG_ENABLED and DEBUG_DISABLED to toggle on or off the debug prints
#define DEBUG_ENABLED
#define DEBUG_INFORMATION_ENABLED
#define DEBUG_WARNING_ENABLED
#define DEBUG_STACK_TRACE_ENABLED
/// @brief Which serial port is used for debugging the program.
#define DEBUG_SERIAL Serial
/// @brief Debug port speed.
#define DEBUG_BAUD_RATE 115200

#define MAX_INDENTATION_LEVEL 10


// - FUNCTIONS - //

/**
 * @brief Initialises the debug functions used
 * to debug prints in a serial port.
 *
 * @return true:
 * Successfully initialised the debug functions
 * @return false:
 * Failed to initialise the debug functions.
 */
bool Debug_Init();

/**
 * @brief Tells the debug program which function
 * is currently being debug. This will not
 * execute if Informations are disabled in the
 * program or @ref DEBUG_STACK_TRACE_ENABLED
 * is set to DEBUG_STACK_TRACE_DISABLED
 * @param nameOfTheFunction
 */
void Debug_Start(String nameOfTheFunction);

/**
 * @brief
 * Removes one indentation when a function returns
 * You must call this whenever a function ends
 * IF and only IF a Debug_Start was called at the
 * start of the function.
 */
void Debug_End();

/**
 * @brief 
 * Stops debugging functions entirely until
 * @ref Debug_Resume is called.
 */
void Debug_Stop();

/**
 * @brief 
 * Resumes debugging functions until
 * @ref Debug_Stop is called.
 */
void Debug_Resume();

/**
 * @brief 
 * Continuously prints the last error that was
 * printed. This works even if debug has stopped.
 */
void Debug_PrintLastError();

/**
 * @brief Makes the code shift right the further
 * it goes into functions. In other words, the
 * bigger the stack, the more indentation there
 * will be. YOU NEED TO CALL
 * Debug_Start and Debug_End in your program for
 * this to work.
 * @return String
 * The indentation level
 */
String GetIndentation();

/**
 * @brief Prints an error on the debug port that
 * can be used for trace back. Errors are things
 * that only occur if it really fucked up.
 *
 * @param fileName Name of the CPP file containing the error line.
 * @param functionName Name of the function in which the call is made
 * @param errorMessage Short, unique error message
 */
void Debug_Error(String fileName, String functionName, String errorMessage);

/**
 * @brief Prints a warning on the debug port that
 * can be used for trace back. Warning are things
 * that occurs often ish but isn't toooooo bad.
 *
 * @param fileName Name of the CPP file containing the error line.
 * @param functionName Name of the function in which the call is made
 * @param warningMessage Short, unique warning message
 */
void Debug_Warning(String fileName, String functionName, String warningMessage);

/**
 * @brief Prints an information on the debug
 * port that can be used for trace back.
 * Information are just generic things useful
 * for live debugging.
 *
 * @param fileName Name of the CPP file containing the error line.
 * @param functionName Name of the function in which the call is made
 * @param informationMessage Short, unique warning message
 */
void Debug_Information(String fileName, String functionName, String informationMessage);