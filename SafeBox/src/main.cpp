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
  Execute_CurrentFunction();

  /*
  Garage_Close();
  for(int i=0; i<1000; i++)
  {
    Garage_IsClosed(); 
    delay(1);
  }
  Garage_Open();
  for(int i=0; i<1000; i++)
  {
    Garage_IsClosed();
    delay(1);
  }
  */
}