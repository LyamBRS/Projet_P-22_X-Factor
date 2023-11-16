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
    if(Debug_Init()){
        if (LEDS_Init()){
            if (BT_Init()){
                if(Alarm_Init() || true){
                    if(Package_Init() || true){
                        if(Garage_Init() || true){
                            if(Lid_Init() || true){
                                if(SafeBox_SetNewStatus(SafeBox_Status::WaitingForXFactor)){
                                    if(SetNewExecutionFunction(FUNCTION_ID_WAIT_AFTER_XFACTOR)){
                                        // Function is successful.
                                        Debug_Information("Init", "SafeBox_Init", "Successful initialisation");
                                        return;
                                    } else Debug_Error("Init", "SafeBox_Init", "SetNewExecutionFunction Failed");
                                } else Debug_Error("Init", "SafeBox_Init", "SafeBox_SetNewStatus Failed");
                            } else Debug_Error("Init", "SafeBox_Init", "Lid_Init Failed");
                        } else Debug_Error("Init", "SafeBox_Init", "Garage_Init Failed");
                    } else Debug_Error("Init", "SafeBox_Init", "Package_Init Failed");
                } else Debug_Error("Init", "SafeBox_Init", "Alarm_Init Failed");
            } else Debug_Error("Init", "SafeBox_Init", "BT_Init Failed");

            if(!SafeBox_SetNewStatus(SafeBox_Status::Error))
            {
                Debug_Error("Init", "SafeBox_Init", "SafeBox_SetNewStatus error Failed");
            }

            if(!SetNewExecutionFunction(FUNCTION_ID_ERROR))
            {
                Debug_Error("Init", "SafeBox_Init", "SetNewExecutionFunction error Failed");
            }
        }
    }
    // Cant continue initialisation.
}