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


/**
 * @brief
 * Initialises all the functions required to make
 * XFactor work. Should be the only function
 * present in void setup.
 *
 * @attention
 * (Claws_innit), (BoardInnit), (LEDS_Innit), (Package_Innit), (Alarm_Innit)
 */
void XFactor_Innit()
{
    bool initialisationError = false;

    BoardInit();

    if (LEDS_Innit()){
        if (BT_Innit()){
            if(Alarm_Innit()){
                XFactor_SetNewStatus(XFactor_StatusEnum::WaitingForDelivery);
                SetNewExecutionFunction()
            }
        }
    }
}