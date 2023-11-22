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

// TEMPORARY INCLUDES
#include "Movements/Vectors.hpp"

/// @brief Arduino's initialisation function.
void setup()
{
  XFactor_Init();
}

/// @brief Arduino's while(1) function.
void loop()
{
  //MoveFromVector(STRAIGHT, 50.0f, true);
  //BacktraceSomeVectors(1);
  //Execute_Moving(50.0f);
  //Debug_Information("main", "loop", "DeployClaw : " + String(Package_DeployClaw()));
  //delay(2000);
  //Debug_Information("main", "loop", "DeployClaw : " + String(Package_StoreClaw()));
  //delay(2000);
  //Debug_Information("main", "loop", "PickUp : " + String(Package_PickUp()));

  //delay(2000);
  //Claws_GetSwitchStatus();
  Debug_Information("main", "loop", "PickUp : " + String(GROVE_GetColor()));
  delay(100);
  //S3003_SetPosition(CLAWS_PINS_GRABBER, 90.0f);
  //delay(2000);
  /*S3003_SetPosition(CLAWS_PINS_HEIGHT, 50.0f);
  Serial.println("FIRST : " + String("50"));
  delay(2000);
  Serial.println("FIRST : " + String("125"));
  S3003_SetPosition(CLAWS_PINS_HEIGHT, 125.0f);
  delay(2000);*/
  //Execute_CurrentFunction();
}