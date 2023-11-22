/**
 * @file main.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Arduino's main file.
 * @version 0.1
 * @date 2023-11-01
 * @copyright Copyright (c) 2023
 */

// - INCLUDES -//
#include "Actions/Actions.hpp"
#include "XFactor/Init.hpp"
#include "Alarm/Alarm.hpp"
#include "LibRobus.h"

void setup()
{
    BoardInit();
    Alarm_Init();
}



void loop()
{
    MoveStraightAndTest();
}

