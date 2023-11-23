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
    //BoardInit();
    //Alarm_Init();
    Package_StoreClaw();
    delay(1000); // needed for demo
}

void loop()
{
  //Serial.print("MoveFromVector : " + String(MoveFromVector(STRAIGHT, 30.0f, true)));
  int value = MoveFromVector(STRAIGHT, 70.0f, true);

  Debug_Information("main", "loop", "MoveFromVector : " + String(value));
  delay(2000);

  switch (value)
  {
    case PACKAGE_FOUND:
      Package_DeployClaw();
      Package_PickUp();
      for (;;)
      {
        // lazy boi
      }
    case ALARM_TRIGGERED:
      for (;;)
      {
        Execute_Alarm();
      }
  }
  
  //Execute_Alarm();
  //BacktraceSomeVectors(1);
  //Execute_Moving(50.0f);
  /*Debug_Information("main", "loop", "DeployClaw : " + String(Package_DeployClaw()));
  delay(2000);
  Debug_Information("main", "loop", "DeployClaw : " + String(Package_StoreClaw()));
  delay(2000);*/
  //Debug_Information("main", "loop", "PickUp : " + String(Package_PickUp()));
  /*if (Alarm_VerifyAccelerometer())
  {
    Debug_Information("main", "loop", "ALARM TRIGGERED");
  }*/
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
