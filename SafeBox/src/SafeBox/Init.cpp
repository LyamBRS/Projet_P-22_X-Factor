/**
 * @file Init.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Contains the main function that initialises
 * SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "SafeBox/Init.hpp"

/**
 * @brief
 * Initialises all the functions required to make
 * SafeBox work. Should be the only function
 * present in void setup.
 *
 * @attention
 * (LEDS_Init), (Package_Init), (Alarm_Init), (Lid_Init), (Garage_Init)
 */
void SafeBox_Init()
{
    if (LEDS_Init()){
        if (BT_Init()){
            if(Alarm_Init()){
                if(Package_Init()){
                    if(Garage_Init()){
                        if(Lid_Init()){
                            if(SafeBox_SetNewStatus(SafeBox_StatusEnum::WaitingForXFactor)){
                                if(SetNewExecutionFunction(FUNCTION_ID_WAIT_AFTER_XFACTOR)){
                                    // Function is successful.
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
        SafeBox_SetNewStatus(SafeBox_StatusEnum::Error);
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
    }
    // Cant continue initialisation.
}