/**
 * @file main.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Arduino's main file.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES -//
//#include "Actions/Actions.hpp"
#include "SafeBox/Init.hpp"

/// @brief Arduino's initialisation function.
void setup()
{
  //SafeBox_Init();
  LEDS_Init();
  Debug_Init();
  RFID_Init(0);
}

/// @brief Arduino's while(1) function.
void loop()
{
  //Debug_Information("-","-","-");
  if(RFID_HandleCard())
  {
    LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
  }
  else
  {
    LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ALARM);   
  }
  //Execute_CurrentFunction();
}