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

/// @brief Arduino's initialisation function.
void setup()
{
  XFactor_Init();
}

void loop()
{
  Debug_Information("main", "loop", "Distance Front : " + String(GP2D12_Read(FRONT_SENSOR_TRIG_PIN_NUMBER, FRONT_SENSOR_ECHO_PIN_NUMBER)));
  Debug_Information("main", "loop", "Distance Left : " + String(GP2D12_Read(LEFT_SENSOR_TRIG_PIN_NUMBER, LEFT_SENSOR_ECHO_PIN_NUMBER)));
  Debug_Information("main", "loop", "Distance Right : " + String(GP2D12_Read(RIGHT_SENSOR_TRIG_PIN_NUMBER, RIGHT_SENSOR_ECHO_PIN_NUMBER)));
  //Execute_CurrentFunction();
}
