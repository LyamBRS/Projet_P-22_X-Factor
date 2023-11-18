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

/// @brief Arduino's while(1) function.
void loop()
{
  Execute_CurrentFunction();
}