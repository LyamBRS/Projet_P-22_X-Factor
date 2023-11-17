/**
 * @file Initialisation.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Contains the main function that initialises
 * XFactor.
 * @version 0.1
 * @date 2023-11-01
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "XFactor/Init.hpp"

/**
 * @brief
 * Initialises all the functions required to make
 * XFactor work. Should be the only function
 * present in void setup.
 *
 * @attention
 * (Claws_init), (BoardInit), (LEDS_Init), (Package_Init), (Alarm_Init)
 */
void XFactor_Init()
{
    BoardInit();

    if (Debug_Init()){
        if (LEDS_Init()){
            if(BT_Init()){
                if(Alarm_Init() || true){
                    if(Package_Init() || true){
                        if(XFactor_SetNewStatus(XFactor_Status::WaitingForDelivery)){
                            if(SetNewExecutionFunction(FUNCTION_ID_WAIT_AFTER_SAFEBOX)){
                                Debug_Information("Init", "XFactor_Init", "Successful initialisation");
                                return;
                            } else Debug_Error("Init", "XFactor_Init", "SetNewExecutionFunction Failed");
                        } else Debug_Error("Init", "XFactor_Init", "XFactor_SetNewStatus Failed");
                    } else Debug_Error("Init", "XFactor_Init", "BT_Init Failed");
                } else Debug_Error("Init", "XFactor_Init", "Package_Init Failed");
            } else Debug_Error("Init", "XFactor_Init", "Alarm_Init Failed");
        } else Debug_Error("Init", "XFactor_Init", "LEDS_Init Failed");

        if(!XFactor_SetNewStatus(XFactor_Status::Error))
        {
            Debug_Error("Init", "XFactor_Init", "XFactor_SetNewStatus Error Failed");
        }

        if(!SetNewExecutionFunction(FUNCTION_ID_ERROR))
        {
            Debug_Error("Init", "XFactor_Init", "SetNewExecutionFunction Error Failed");
        }
    }
    // Cant continue initialisation.
}