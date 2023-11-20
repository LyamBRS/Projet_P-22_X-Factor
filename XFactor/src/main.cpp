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
#include "Sensors/Colour/GROVE.hpp"

// TEMPORARY INCLUDES
#include "Movements/Vectors.hpp"

/// @brief Arduino's initialisation function.
void setup()
{
  //XFactor_Init();
  Vectors_Init();
  Serial.begin(9600);
}

/// @brief Arduino's while(1) function.
void loop()
{
  MovementVector testReturnVector = GetReturnVector();
  Serial.print("Distance : ");
  Serial.println(testReturnVector.distance_cm);
  Serial.print("Rotation : ");
  Serial.println(testReturnVector.rotation_rad);
  delay(1000);
  //Execute_CurrentFunction();
}