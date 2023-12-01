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
        Debug_Start("XFactor_Init");
        if(BT_Init()){
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
            MessageBuffer("", 0, 0);

            if (LEDS_Init()){
                LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_INITIALISING);
                delay(1000);

                unsigned char cycleCounter = 0;
                float currentPeak = 0.0f;
                float multiplierR = 0;
                float multiplierG = 0;
                float multiplierB = 0;
                while(!AX_IsLowBat())
                {
                    if(currentPeak > 3.14)
                    {
                        multiplierR = sin(currentPeak-3.14);
                    }
                    else
                    {
                        multiplierR = cos(currentPeak);
                    }
                    multiplierG = sin(currentPeak-1.57);
                    multiplierB = sin(currentPeak);

                    if(multiplierR<0) multiplierR=0;
                    if(multiplierG<0) multiplierG=0;
                    if(multiplierB<0) multiplierB=0;

                    currentPeak = currentPeak + 0.01;
                    if(currentPeak>4.71)
                    {
                        currentPeak = 0;
                        cycleCounter++;
                        if(cycleCounter > 5)
                        {
                            //break;
                        }
                    }

                    LEDS_SetColor(LED_ID_STATUS_INDICATOR, (float)(255.0f*multiplierR), (float)(255.0f*multiplierG), (float)(255.0f*multiplierB));
                    delay(1);
                }

                if(Alarm_Init()){
                    if(Package_Init()){
                        if(XFactor_SetNewStatus(XFactor_Status::WaitingForDelivery)){
                            if(SetNewExecutionFunction(FUNCTION_ID_WAIT_AFTER_SAFEBOX)){
                                Debug_Information("Init", "XFactor_Init", "Successful initialisation");
                                Debug_End();
                                return;
                            } else Debug_Error("Init", "XFactor_Init", "SetNewExecutionFunction Failed");
                        } else Debug_Error("Init", "XFactor_Init", "XFactor_SetNewStatus Failed");
                    } else Debug_Error("Init", "XFactor_Init", "Package_Init Failed");
                } else Debug_Error("Init", "XFactor_Init", "Alarm_Init Failed");
            } else Debug_Error("Init", "XFactor_Init", "LEDS_Init Failed");
        } else Debug_Error("Init", "XFactor_Init", "BT_Init Failed");

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
    SetNewExecutionFunction(FUNCTION_ID_ERROR);
    Debug_End();
}