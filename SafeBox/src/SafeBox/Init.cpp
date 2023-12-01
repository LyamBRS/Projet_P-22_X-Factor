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
    /**
     * @brief
     * DO NOT TOUCH
     * @bug
     * If this isn't there, the static String
     * inside of this function will be created
     * later in the program. While you may think
     * that its not an issue, turns out that this
     * somehow started to clash with the WS2812
     * class. It would cause the program to
     * literally STOP after 2 execution. Void
     * loop would literally no longer be executed
     *
     * This was either fixed by removing 2 lines
     * of "pixels" inside WS2812 functions we
     * made (pixels.show()) OR by removing the
     * "static" from the strings in this function
     *
     * Obviously, we need the strings to be static
     * How else would we keep track of the buffer
     * of received messages? A global variable?
     * yeah, that fucked up too and caused the
     * Strings to clash with other Serial ports
     * buffers somehow.
     *
     * So this fix allows the code to continue
     * executing normally... go figure bruh.
     */
    //MessageBuffer("", 0, 0);

    if(Debug_Init()){
        Debug_Start("SafeBox_Init");
        if (BT_Init()){
            if (LEDS_Init()){
                LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_INITIALISING);
                delay(1000);
                if(Alarm_Init()){
                    if(Package_Init()){
                        if(Garage_Init()){
                            if(Lid_Init()){
                                if(Doorbell_Init()){
                                    if(RFID_Init()){

                                        if(SafeBox_EEPROMStatusShouldStartAlarm())
                                        {
                                            Debug_Information("Init", "SafeBox_Init", "Alarm started due to EEPROM values");
                                            SetNewExecutionFunction(FUNCTION_ID_ALARM);
                                            Debug_End();
                                            return;
                                        }

                                        if(SafeBox_SetNewStatus(SafeBox_Status::WaitingForXFactor)){
                                            if(SetNewExecutionFunction(FUNCTION_ID_WAIT_AFTER_XFACTOR)){
                                                // Function is successful.
                                                Debug_Information("Init", "SafeBox_Init", "Successful initialisation");
                                                Debug_End();
                                                return;
                                            } else Debug_Error("Init", "SafeBox_Init", "SetNewExecutionFunction Failed");
                                        } else Debug_Error("Init", "SafeBox_Init", "SafeBox_SetNewStatus Failed");
                                    } else Debug_Error("Init", "SafeBox_Init", "RFID_Init Failed");
                                } else Debug_Error("Init", "SafeBox_Init", "Doorbell_Init Failed");
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
    Debug_End();
}