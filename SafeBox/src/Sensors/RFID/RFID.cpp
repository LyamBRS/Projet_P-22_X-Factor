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
    pinMode(13, OUTPUT);

    // Initialise le Serial2 entre le module RFID et l'arduino
    Serial2.begin(9600);
    Serial.println("Test du ID-12 sur UART2 (RX2/Digital 17)");

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
  unsigned long long VALID_CARD_NUMBER = 842024496;

    if (RFID_GetCardNumber() == VALID_CARD_NUMBER) {
      //  Serial.print("");
      //  Serial.print(static_cast<unsigned long>(VALID_CARD_NUMBER));
        Serial.print("Bonne Carte");
        return true;
    } else {
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
    // si elle li une carte, elle ne doit pas lire
    if (isReadingRFID == true) return true;
    // elle ne li pas de carte, elle peut lire une carte. 
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
unsigned long long RFID_GetCardNumber() {
  unsigned long long int resultedID = 0;
  unsigned long long int temporaryBigNumber = 0;
  byte crecu, i, incoming = 0;
  byte id_tag[20];

  
  isReadingRFID = true;

  while (1) {
    if (Serial2.available()) {
      crecu = Serial2.read();
      switch (crecu) {
        case 0x02:
          // START OF TRANSMIT
          digitalWrite(13, HIGH);
          i = 0;
          incoming = 1;
          Serial.println("case 2");
          break;
        case 0x03:
          // END OF TRANSMIT
          digitalWrite(13, LOW);
          incoming = 0;
          Serial.println("case 3");

          for (int i = 0; i < 10; i++)
            Serial.println(id_tag[i]);

          Serial.println("");

          for (int i = 0; i < 8; i++) {
            temporaryBigNumber = id_tag[i];
            temporaryBigNumber = temporaryBigNumber << (8 * i);
            Serial.println(static_cast<unsigned long>(resultedID));
            resultedID = resultedID + temporaryBigNumber;
          }
         Serial.println(static_cast<unsigned long>(resultedID));

          isReadingRFID = false;
          return resultedID;

        default:
          if (incoming)
            id_tag[i++] = crecu;
          break;
      }
    }
  }
  return 0;
}