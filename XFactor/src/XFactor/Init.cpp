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
#include "XFactor/Status.hpp"
#include "LibRobus.h"

#include "Actions/Actions.hpp"
#include "Communication/Bluetooth.hpp"
#include "LED/LED.hpp"
#include "Alarm/Alarm.hpp"
#include "Package/Package.hpp"


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

    if (LEDS_Init()){
        if (BT_Init()){
            if(Alarm_Init()){
                if(Package_Init()){
                    if(XFactor_SetNewStatus(XFactor_StatusEnum::WaitingForDelivery)){
                        if(SetNewExecutionFunction(FUNCTION_ID_WAIT_AFTER_SAFEBOX)){
                            // Function is successful.
                            return;
                        }
                    }
                }
            }
        }
        XFactor_SetNewStatus(XFactor_StatusEnum::Error);
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
    }
    // Cant continue initialisation.
}