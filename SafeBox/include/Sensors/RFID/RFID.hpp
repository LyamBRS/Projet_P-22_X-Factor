/**
 * @file RFID.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the header definition of the
 * functions used to handle the RFID used on
 * SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDE - //
#include <Arduino.h>
#include "Debug/Debug.hpp"
#include "LED/LED.hpp"

// - DEFINES - //
#define RFID_VALID_CARD "4870485055685448534852481310"
#define RFID_SERIAL Serial2
#define RFID_SENSOR_READING_PIN 12

/**
 * @brief
 * Function that initialises what is required to
 * make the RFID module function on SafeBox.
 * @return true:
 * Successfully initialised the RFID of SafeBox.
 * @return false
 */
bool RFID_Init();

/**
 * @brief
 * Function that returns true or false depending
 * on if the user has his keycard on the RFID
 * module and that the card is valid.
 * @return int:
 * -1: Incorrect card read
 * 0: No card was read
 * 1: Correct card was detected
 */
int RFID_HandleCard();

/**
 * @brief
 * Function that assumes that the RFID
 * module can detect if a card is
 * currently laid on it.
 * @return true:
 * There is a card on the RFID reader.
 * @return false:
 * There is no card on the RFID reader.
 */
bool RFID_CheckIfCardIsThere();

/**
 * @brief
 * Function that reads the card that
 * is currently layed on the RFID
 * module. Return an int which
 * corresponds to the ID of the card
 * currently being read. If 0 is returned
 * then the module failed to read the
 * RFID tag / card.
 * @return unsigned long long:
 * The Card ID. If 0, there is no card.
 */
String RFID_GetCardNumber();


