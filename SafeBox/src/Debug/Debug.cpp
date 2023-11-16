/**
 * @file Debug.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing debug functions to call to
 * debug your code so they can easily be kept
 * in the program and removed for compilation
 * whenever necessary.
 * @version 0.1
 * @date 2023-11-14
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Debug/Debug.hpp"

/**
 * @brief Initialises the debug functions used
 * to debug prints in a serial port.
 *
 * @return true:
 * Successfully initialised the debug functions
 * @return false:
 * Failed to initialise the debug functions.
 */
bool Debug_Init()
{
    #ifdef DEBUG_ENABLED
        DEBUG_SERIAL.begin(DEBUG_BAUD_RATE);
    #endif
    return true;
}

/**
 * @brief Prints an error on the debug port that
 * can be used for trace back. Errors are things
 * that only occur if it really fucked up.
 *
 * @param fileName Name of the CPP file containing the error line.
 * @param functionName Name of the function in which the call is made
 * @param errorMessage Short, unique error message
 */
void Debug_Error(String fileName, String functionName, String errorMessage)
{
    #ifdef DEBUG_ENABLED
        DEBUG_SERIAL.print("[ERROR]: ");
        DEBUG_SERIAL.print(fileName);
        DEBUG_SERIAL.print(": ");
        DEBUG_SERIAL.print(functionName);
        DEBUG_SERIAL.print(": ");
        DEBUG_SERIAL.print(errorMessage);
        DEBUG_SERIAL.print("\n\r");
        DEBUG_SERIAL.flush();
    #endif
}

/**
 * @brief Prints a warning on the debug port that
 * can be used for trace back. Warning are things
 * that occurs often ish but isn't toooooo bad.
 *
 * @param fileName Name of the CPP file containing the error line.
 * @param functionName Name of the function in which the call is made
 * @param warningMessage Short, unique warning message
 */
void Debug_Warning(String fileName, String functionName, String warningMessage)
{
    #ifdef DEBUG_ENABLED
        #ifdef DEBUG_WARNING_ENABLED
            DEBUG_SERIAL.print("[WARNS]: ");
            DEBUG_SERIAL.print(fileName);
            DEBUG_SERIAL.print(": ");
            DEBUG_SERIAL.print(functionName);
            DEBUG_SERIAL.print(": ");
            DEBUG_SERIAL.print(warningMessage);
            DEBUG_SERIAL.print("\n\r");
            DEBUG_SERIAL.flush();
        #endif
    #endif
}

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
void Debug_Information(String fileName, String functionName, String informationMessage)
{
    #ifdef DEBUG_ENABLED
        #ifdef DEBUG_INFORMATION_ENABLED
            DEBUG_SERIAL.print("[INFOS]: ");
            DEBUG_SERIAL.print(fileName);
            DEBUG_SERIAL.print(": ");
            DEBUG_SERIAL.print(functionName);
            DEBUG_SERIAL.print(": ");
            DEBUG_SERIAL.print(informationMessage);
            DEBUG_SERIAL.print("\n\r");
            DEBUG_SERIAL.flush();
        #endif
    #endif
}