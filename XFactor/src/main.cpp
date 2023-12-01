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
  /*bool hasPackage = false;
  for (;;)
  {
    switch (MoveFromVector(TURN_90_RIGHT, 100.0f, false, CHECK_SENSORS, true, false, 0.4f))
    {
      case ALARM_TRIGGERED:
        for (;;)
        {
          LEDS_SetColor(LED_ID_STATUS_INDICATOR, 0, 64, 64);
          Stop();
        }
        break;
      case OBJECT_LOCATED_FRONT:
        if (!hasPackage)
        {
          MoveFromVector(GO_TO_DETECTED_OBJECT_FRONT_VECTOR);
          if (Package_PickUp())
          {
            hasPackage = true;
          }
        }
      default:
        break;
    }
  }*/

  //MoveFromVector(TURN_180, 100.0f, true, false, false, false, 0.4f);
  //delay(2000);
  //GetReturnVector();
  //delay(10000);
  //Debug_Information("main", "loop", "Bumper : " + String(Claws_GetSwitchStatus()));
  //delay(100);
  //MoveFromVector(TURN_90_LEFT, SAFEBOX_LENGTH_CM + ROBOT_LENGTH_CM - 10.0f, false, DONT_CHECK_SENSORS, true, false, 0.4f);
  //MoveFromVector(TURN_90_LEFT, 0.0f, false, DONT_CHECK_SENSORS, false, false, 0.2f);
  //delay(5000);
  //Execute_PickUpPackage();
  //GetReturnVector();
  //delay(10000);

  //GROVE_GetColor();
  //delay(250);
  /*for (int i = 0; i < 1000; i++)
  {
    MoveFromVector(TURN_180 * i, 50.0f, true, false, false, false, 0.4f);
    delay(2000);
  }*/
  
  ResetPositions();
  Execute_SearchForPackage();
  delay(500);
  Execute_ReturnHome();
  delay(500);

  /*Debug_Information("main", "loop", "Distance Front : " + String(GP2D12_Read(FRONT_SENSOR_TRIG_PIN_NUMBER, FRONT_SENSOR_ECHO_PIN_NUMBER)));
  Debug_Information("main", "loop", "Distance Left : " + String(GP2D12_Read(LEFT_SENSOR_TRIG_PIN_NUMBER, LEFT_SENSOR_ECHO_PIN_NUMBER)));
  Debug_Information("main", "loop", "Distance Right : " + String(GP2D12_Read(RIGHT_SENSOR_TRIG_PIN_NUMBER, RIGHT_SENSOR_ECHO_PIN_NUMBER)));
  delay(1000);*/

  //Execute_CurrentFunction();
  
  /*LEDS_SetColor(0, 0,0,255);
  if (ROBUS_IsBumper(3)){
     LEDS_SetColor(0, 0,255,0);
     MoveFromVector(0, 200, false, false);
     //MoveStraight(1000);
  }
  LEDS_SetColor(0, 255,0,0);*/
}
