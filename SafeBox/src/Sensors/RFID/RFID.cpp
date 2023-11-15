/**
 * @file RFID.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the functions used to handle
 * the RFID used on SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Sensors/RFID/RFID.hpp"
#include <Arduino.h>

bool isReadingRFID = false;

/**
 * @brief
 * Function that initialises what is required to
 * make the RFID module function on SafeBox.
 * @param RFIDPin
 * The pin used for the RFID module. May need to
 * be changed in the future depending on how the
 * sensor functions.
 * @return true:
 * Successfully initialised the RFID of SafeBox.
 * @return false
 */
bool RFID_Init(int RFIDPin)
{
   // Set la Del de l'Arduino
    // pinMode(13, OUTPUT);

    // Initialise le Serial2 entre le module RFID et l'arduino
    RFID_SERIAL.begin(9600);

    return true;
}


/**
 * @brief
 * Function that returns true or false depending
 * on if the user has his keycard on the RFID
 * module and that the card is valid.
 * @return true:
 * A card has been successfully read.
 * @return false:
 * There is no card / the card does not have the
 * correct number.
 */
bool RFID_HandleCard()
{
  const String VALID_CARD_NUMBER(RFID_VALID_CARD);

    if (RFID_GetCardNumber().compareTo(VALID_CARD_NUMBER) == 0) {
        Debug_Information("RFID","RFID_HandleCard","Valid card");
        return true;
    } else {
        Debug_Warning("RFID","RFID_HandleCard","Mismatched card");
        Serial.print("Mauvaise Carte");
        return false;
    }
}

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
bool RFID_CheckIfCardIsThere()
{
    // Si elle lu la bonne carte, plus besoin de lire
    if (RFID_HandleCard()) return true;
    // elle n'a pas lu la bonne carte
    return false;
}

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
String RFID_GetCardNumber() {
  byte crecu, incoming = 0;
  String id_tag;

  isReadingRFID = true;

  while (1) {
    if (RFID_SERIAL.available()) {
      crecu = RFID_SERIAL.read();
      switch (crecu) {
        case 0x02:
          // START OF TRANSMIT
          digitalWrite(13, HIGH);
          incoming = 1;
          break;

        case 0x03:
          // END OF TRANSMIT
          incoming = 0;

          for (int i = 0; i < 10; i++) isReadingRFID = false;
          return id_tag;

        default:
          if (incoming)
          {
            id_tag.concat(crecu);
          }
          break;
      }
    }
  }
  return "";
}