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

  Package_StoreClaw();
  delay(1500);
}

void loop()
{
  /*ResetPositions();
  Execute_SearchForPackage();
  delay(500);
  Execute_ReturnHome();
  delay(500);*/

  Execute_CurrentFunction();
}
