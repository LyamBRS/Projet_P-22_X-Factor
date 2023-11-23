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
#include "Actions/Actions.hpp"
#include "SafeBox/Init.hpp"
//#include "Outputs/LEDs/WS2812/WS2812.hpp"

/// @brief Arduino's initialisation function.
void setup()
{
  //SafeBox_Init();
  RFID_Init(0);
  LEDS_Init();
  S3003_Init();
}

/// @brief Arduino's while(1) function.
void loop()
{
  //Execute_CurrentFunction();
  if(RFID_HandleCard())
  {
    LEDS_SetColor(0,0,255,0);
  }
  else
  {
    LEDS_SetColor(0,255,0,0);   
  }
}