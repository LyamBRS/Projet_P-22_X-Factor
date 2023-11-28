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
  RFID_Init();
  LEDS_Init();
  S3003_Init();
}

/// @brief Arduino's while(1) function.
void loop()
{
  //Execute_CurrentFunction();
  /*Garage_Open();
  delay(5000);
  Garage_Close();
  delay(5000);
  */

}