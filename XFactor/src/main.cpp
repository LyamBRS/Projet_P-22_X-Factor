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
#include "Alarm/Alarm.hpp"
#include "LibRobus.h"


// TEMPORARY INCLUDES
#include "Movements/Vectors.hpp"

/// @brief Arduino's initialisation function.

void setup()
{
  XFactor_Init();
    //BoardInit();
    //Alarm_Init();
}



void loop()
{
  //Serial.print("MoveFromVector : " + String(MoveFromVector(STRAIGHT, 30.0f, true)));
  //Debug_Information("main", "loop", "MoveFromVector : " + String(MoveFromVector(STRAIGHT, 30.0f, true)));
  delay(2000);
  Execute_Alarm();
  //BacktraceSomeVectors(1);
  //Execute_Moving(50.0f);
  /*Debug_Information("main", "loop", "DeployClaw : " + String(Package_DeployClaw()));
  delay(2000);
  Debug_Information("main", "loop", "DeployClaw : " + String(Package_StoreClaw()));
  delay(2000);*/
  //Debug_Information("main", "loop", "PickUp : " + String(Package_PickUp()));
  
  //Debug_Information("main", "loop", "Alarm : " + String(Alarm_VerifyAccelerometer()));
  //delay(100);
  //delay(2000);
  //Claws_GetSwitchStatus();
  //Debug_Information("main", "loop", "Alarm : " + String(GROVE_GetColor()));
  //delay(100);
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
