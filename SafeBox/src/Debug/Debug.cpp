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

// - LOCAL GLOBAL - //
volatile int _indentationLevel = 0;

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
 * @brief Tells the debug program which function
 * is currently being debug. This will not
 * execute if Informations are disabled in the
 * program or @ref DEBUG_STACK_TRACE_ENABLED
 * is set to DEBUG_STACK_TRACE_DISABLED
 * @param nameOfTheFunction
 */
void Debug_Start(String nameOfTheFunction)
{
    #ifdef DEBUG_INFORMATION_ENABLED
        #ifdef DEBUG_STACK_TRACE_ENABLED
            DEBUG_SERIAL.print("[     ]: ");
            DEBUG_SERIAL.print(GetIndentation());
            DEBUG_SERIAL.print("[");
            DEBUG_SERIAL.print(nameOfTheFunction);
            DEBUG_SERIAL.println("]:");
            DEBUG_SERIAL.flush();
            if(_indentationLevel < MAX_INDENTATION_LEVEL) _indentationLevel++;
        #endif
    #endif
}

/**
 * @brief
 * Removes one indentation when a function returns
 */
void Debug_End()
{
    #ifdef DEBUG_INFORMATION_ENABLED
        #ifdef DEBUG_STACK_TRACE_ENABLED
            if(_indentationLevel > 0) _indentationLevel--;
            DEBUG_SERIAL.print("[     ]: ");
            DEBUG_SERIAL.print(GetIndentation());
            DEBUG_SERIAL.println("[END]");
            DEBUG_SERIAL.flush();
        #endif
    #endif
}

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
String GetIndentation()
{
    #ifdef DEBUG_INFORMATION_ENABLED
        #ifdef DEBUG_STACK_TRACE_ENABLED
            String indentation = "";

            if(_indentationLevel == 0) return "";

            for(int indentationLevel=0; indentationLevel<_indentationLevel; indentationLevel++)
            {
                indentation += "|\t";
            }
            return indentation;
        #endif
    #endif
    return "";
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
        #ifdef DEBUG_STACK_TRACE_ENABLED
            DEBUG_SERIAL.print(GetIndentation());
        #elif
            DEBUG_SERIAL.print(fileName);
            DEBUG_SERIAL.print(": ");
            DEBUG_SERIAL.print(functionName);
            DEBUG_SERIAL.print(": ");
        #endif

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
            #ifdef DEBUG_STACK_TRACE_ENABLED
                DEBUG_SERIAL.print(GetIndentation());
            #elif
                DEBUG_SERIAL.print(fileName);
                DEBUG_SERIAL.print(": ");
                DEBUG_SERIAL.print(functionName);
                DEBUG_SERIAL.print(": ");
            #endif

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
            #ifdef DEBUG_STACK_TRACE_ENABLED
                DEBUG_SERIAL.print(GetIndentation());
            #elif
                DEBUG_SERIAL.print(fileName);
                DEBUG_SERIAL.print(": ");
                DEBUG_SERIAL.print(functionName);
                DEBUG_SERIAL.print(": ");
            #endif

            DEBUG_SERIAL.print(informationMessage);
            DEBUG_SERIAL.print("\n\r");
            DEBUG_SERIAL.flush();
        #endif
    #endif
}