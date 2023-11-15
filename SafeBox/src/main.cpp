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
#include "Sensors/RFID/RFID.hpp"

/// @brief Arduino's initialisation function.
void setup()
{
 // SafeBox_Init();
  // Set la Del de l'Arduino
    pinMode(13, OUTPUT);

    // Initialise le Serial2 entre le module RFID et l'arduino
    Serial2.begin(9600);
    Serial.begin(9600);
    Serial.println("Test du ID-12 sur UART2 (RX2/Digital 17)");
}

/// @brief Arduino's while(1) function.
void loop()
{
  RFID_HandleCard();
  
    RFID_CheckIfCardIsThere();
  //Execute_CurrentFunction();
}