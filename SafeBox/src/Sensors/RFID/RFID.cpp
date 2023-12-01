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
#include <EEPROM.h>
bool isReadingRFID = false;

//String eepromCard = "";

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
bool RFID_Init()
{
  Debug_Start("RFID_Init");
   // Set la Del de l'Arduino
    pinMode(RFID_SENSOR_READING_PIN, INPUT);
    pinMode(RFID_SAVE_NEW_CARD_PIN, INPUT);

    // Initialise le Serial2 entre le module RFID et l'arduino
    RFID_SERIAL.begin(9600);

    // Check if there is an EEPROM card
    if(RFID_GetEEPROMCard().compareTo(RFID_NO_EEPROM_CARD) == 0)
    {
      Debug_Warning("RFID", "RFID_Init", "No card stored in the EEPROM");
    }
    else
    {
      Debug_Information("RFID", "RFID_Init", "Card stored in the EEPROM");   
    }
    Debug_End();
    return true;
}

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
bool RFID_WantsToSaveNewCard()
{
  return digitalRead(RFID_SAVE_NEW_CARD_PIN);
}

/**
 * @brief 
 * This function returns a card number that is
 * stored in the EEPROM of the Arduino.
 * If no card is stored, "NO_CARD_STORED" will
 * be returned.
 * @return String 
 */
String RFID_GetEEPROMCard()
{
  Debug_Start("RFID_GetEEPROMCard");
  // - VARIABLES - //
  int currentEEPROMIndex = EEPROM.length()-1;
  uint8_t savedEEPROMValue = 0;
  String extractedCard = "";

  // - Identify if the last EEPROM value corresponds to a saved card value.
  savedEEPROMValue = EEPROM.read(currentEEPROMIndex);
  if(savedEEPROMValue != RFID_EEPROM_START_OF_CARD)
  {
    Debug_Error("RFID", "RFID_GetEEPROMCard", "Last value was not RFID_EEPROM_START_OF_CARD");
    Debug_End();
    return "EEPROM_READ_ERROR";
  }

  // - Extract each letter individually
  for(int characterIndex=0; characterIndex<69; characterIndex++)
  {
    currentEEPROMIndex--;
    savedEEPROMValue = EEPROM.read(currentEEPROMIndex);
    Debug_Information("RFID", "RFID_GetEEPROMCard", "Reading: " + String(savedEEPROMValue) + " at index " + String(currentEEPROMIndex));

    if(savedEEPROMValue == RFID_EEPROM_END_OF_CARD)
    {
      Debug_Information("RFID", "RFID_GetEEPROMCard", "End of the card found. Card number is: " + String(extractedCard));
      Debug_End();
      return extractedCard;
    }

    extractedCard = extractedCard + (char)(savedEEPROMValue);
  }

  Debug_Error("RFID", "RFID_GetEEPROMCard", "Failed to find a valid card");
  Debug_End();
  return RFID_NO_EEPROM_CARD;
}

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
bool RFID_StoreCardInEEPROM(String cardToStore)
{
  Debug_Start("RFID_StoreCardInEEPROM");
  Debug_Information("RFID", "RFID_StoreCardInEEPROM", "Trying to store "+cardToStore);

  // - VARIABLES - //
  Debug_Information("RFID", "RFID_StoreCardInEEPROM", "Getting variables");
  int currentEEPROMIndex = EEPROM.length()-1;
  int lengthOfCardToStore = cardToStore.length();

  // - To identify later if a card is stored
  Debug_Information("RFID", "RFID_StoreCardInEEPROM", "First writes");
  EEPROM.write(currentEEPROMIndex, RFID_EEPROM_START_OF_CARD);

  // - Store each letter individually
  for(int characterIndex=0; characterIndex<lengthOfCardToStore; characterIndex++)
  {
    currentEEPROMIndex = currentEEPROMIndex - 1;
    Debug_Information("RFID", "RFID_StoreCardInEEPROM", "Storing: " + String(cardToStore.charAt(characterIndex)) + " at index " + String(currentEEPROMIndex));
    EEPROM.write(currentEEPROMIndex, (uint8_t)(cardToStore.charAt(characterIndex)));
  }

  // - Write the end of the card.
  EEPROM.write(currentEEPROMIndex-1, RFID_EEPROM_END_OF_CARD);
  Debug_Information("RFID", "RFID_StoreCardInEEPROM", "Success");
  Debug_End();
  return true;
}

/**
 * @brief Function that handles the saving of
 * an RFID card in the EEPROM.
 * 
 * @return true:
 * Successfully stored the card in the EPPROM
 * @return false:
 * Failed to store the card in the EEPROM.
 */
bool RFID_HandleStoringNewCard(String cardToStore)
{
  Debug_Start("RFID_HandleStoringNewCard");
  String savedCard = "";

  if(!RFID_StoreCardInEEPROM(cardToStore))
  {
    Debug_Error("RFID", "RFID_HandleStoringNewCard", "Failed to store card");
    Debug_End();
    return false;
  }

  savedCard = RFID_GetEEPROMCard();

  if(savedCard.compareTo(cardToStore) != 0)
  {
    Debug_Error("RFID", "RFID_HandleStoringNewCard", "Stored card does not match card to store.");
    Debug_Warning("RFID", "RFID_HandleStoringNewCard", "Wanted card: " + cardToStore);
    Debug_Warning("RFID", "RFID_HandleStoringNewCard", "Stored card: " + savedCard);
    Debug_End();
    return false;
  }
  Debug_End();
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
int RFID_HandleCard()
{
  // - VARIABLES - //
  String receivedCard = "NO_CARDS_FOUND";

  // - Bypassed if there is no cards on the sensor
  if(!RFID_CheckIfCardIsThere())
  {
    return 0;
  }
  
  Debug_Start("RFID_HandleCard");

  String expectedCard = RFID_GetEEPROMCard();
  if(expectedCard.compareTo(RFID_NO_EEPROM_CARD) == 0)
  {
    LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_NEW_CARD_SAVED);
    Alarm_SetState(true);
    delay(40);
    Alarm_SetState(false);
    delay(40);
    Alarm_SetState(true);
    delay(40);
    Alarm_SetState(false);
    delay(40);
    Alarm_SetState(false);
    delay(40);
    Alarm_SetState(true);
    delay(40);
    Alarm_SetState(false);
    return 0;
  }

  receivedCard = RFID_GetCardNumber();

    if (receivedCard.compareTo(expectedCard) == 0) {
        Debug_Information("RFID","RFID_HandleCard","Valid card");

        Alarm_SetState(true);
        delay(40);
        Alarm_SetState(false);
        delay(40);
        Alarm_SetState(true);
        delay(40);
        Alarm_SetState(false);

        Debug_End();
        return 1;
    } else {

        if(receivedCard == "NO_CARDS_FOUND")
        {
          Debug_Warning("RFID","RFID_HandleCard","No card was detected");
          Debug_End();
          return 0;
        }
        LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ALARM);
        Alarm_SetState(true);
        delay(250);
        LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_OFFLINE);
        Alarm_SetState(false);   

        Debug_Warning("RFID","RFID_HandleCard","Expected:");
        Debug_Warning("RFID","RFID_HandleCard",expectedCard);
        Debug_Warning("RFID","RFID_HandleCard","Got");
        Debug_Warning("RFID","RFID_HandleCard",receivedCard);
        Debug_End();
        return -1;
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
  if(digitalRead(RFID_SENSOR_READING_PIN))
  {
    return true;
  }
  else
  {
    return false;  
  }
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
 * @return String:
 * The Card ID. If "NO_CARDS_FOUND", there is no card.
 */
String RFID_GetCardNumber() {
  Debug_Start("RFID_GetCardNumber");

  // - VARIABLES - //
  String id_tag = "NO_CARDS_FOUND";
  byte crecu = ' ';
  bool incoming = 0;
  bool thereWasACard = false;
  int currentCharacter = 0;

  isReadingRFID = true;

  while (RFID_CheckIfCardIsThere()) {
    if(!thereWasACard)
    {
      id_tag = "";
      LEDS_SetColor(LED_ID_STATUS_INDICATOR,LED_COLOR_COMMUNICATING);
      LEDS_SetColor(LED_ID_STATUS_INDICATOR,LED_COLOR_COMMUNICATING);
      thereWasACard = true;
    }
    if (RFID_SERIAL.available()) {
      currentCharacter++;
      crecu = RFID_SERIAL.read();

      //String receivedCharacter = String(crecu);
      //String currentChar = String(currentCharacter);
      //currentChar.concat(receivedCharacter);

      //Debug_Information("RFID","RFID_GetCardNumber",currentChar);

      switch (crecu) {
        case 0x02:
          Debug_Information("RFID","RFID_GetCardNumber","Start of RFID reading");
          // START OF TRANSMIT
          incoming = 1;
          break;

        case 0x03:
          Debug_Information("RFID","RFID_GetCardNumber","END OF RFID READING");
          Debug_Information("RFID","RFID_GetCardNumber",id_tag);
          // END OF TRANSMIT
          incoming = 0;

          for (int i = 0; i < 10; i++) isReadingRFID = false;
          break;

        default:
          if (incoming)
          {
            //Debug_Information("RFID","RFID_GetCardNumber",String(crecu));
            id_tag.concat(crecu);
          }
          break;
      }
    }
  }
  Debug_End();
  return id_tag;
}