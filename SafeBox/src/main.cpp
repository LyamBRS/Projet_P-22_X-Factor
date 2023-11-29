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

/// @brief Arduino's initialisation function.
void setup()
{
  SafeBox_Init();
}

/// @brief Arduino's while(1) function.
void loop()
{
  //Execute_CurrentFunction();

  Garage_Open();
  LEDS_SetColor(0,0,255,0);
  delay(1000);
  Garage_Close();
  LEDS_SetColor(0,255,0,0);
  delay(1000);
}