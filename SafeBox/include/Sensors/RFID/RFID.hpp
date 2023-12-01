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
#include <EEPROM.h>
#include "Debug/Debug.hpp"
#include "LED/LED.hpp"
#include "Alarm/Alarm.hpp"

// - DEFINES - //

/// @brief The number stored inside of the RFID card as a chain of characters. Must be within "".
#define RFID_VALID_CARD "4870485055685448534852481310"
/// @brief Which serial UART port is used to receive data from the RFID sensor.
#define RFID_SERIAL Serial2
/// @brief Which pin of the ARDUINO is connected to TIR allowing detection of cards to occur.
#define RFID_SENSOR_READING_PIN 12

#define RFID_NO_CARDS_READ "NO_CARDS_FOUND"
#define RFID_NO_EEPROM_CARD "NO_CARD_STORED"
#define RFID_SAVE_NEW_CARD_PIN 3

#define RFID_EEPROM_START_OF_CARD 254
#define RFID_EEPROM_END_OF_CARD 254

/**
 * @brief 
 * Returns wether the user wants to save a new
 * RFID card or not. This is true if the user
 * is actively pressing on the button to do so.
 * @return true:
 * The user wants to save a new RFID card.
 * @return false:
 * The user doesnt wanna save a new RFID card.
 */
bool RFID_WantsToSaveNewCard();

/**
 * @brief 
 * This function returns a card number that is
 * stored in the EEPROM of the Arduino.
 * If no card is stored, "NO_CARD_STORED" will
 * be returned.
 * @return String 
 */
String RFID_GetEEPROMCard();

/**
 * @brief
 * This function's purpose is to store it in
 * the Arduino's EEPROM to be read next time
 * the box boots up.
 * @return true:
 * Successfully stored the string in the EEPROM
 * @return false:
 * Failed to store the string in the EEPROM
 */
bool RFID_StoreCardInEEPROM(String cardToStore);

/**
 * @brief Function that handles the saving of
 * an RFID card in the EEPROM.
 * 
 * @return true:
 * Successfully stored the card in the EPPROM
 * @return false:
 * Failed to store the card in the EEPROM.
 */
bool RFID_HandleStoringNewCard(String cardToStore);

/**
 * @brief
 * Function that initialises what is required to
 * make the RFID module function on SafeBox.
 * @return true:
 * Successfully initialised the RFID of SafeBox.
 * @return false:
 * Failed to initialise the RFID sensor.
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


