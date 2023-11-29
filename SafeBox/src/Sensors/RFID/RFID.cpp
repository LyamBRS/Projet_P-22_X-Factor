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
bool RFID_Init()
{
   // Set la Del de l'Arduino
    pinMode(RFID_SENSOR_READING_PIN, INPUT);

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
int RFID_HandleCard()
{
  // - VARIABLES - //
  //static unsigned short previousMillis = millis();
  //unsigned short currentMillis = millis();

  //Debug_Information("RFID", "RFID_HandleCard", "Tir pin: " + String(digitalRead(RFID_SENSOR_READING_PIN)));
  //previousMillis = currentMillis;

  const String VALID_CARD_NUMBER(RFID_VALID_CARD);
  String receivedCard = "NO_CARDS_FOUND";

  // - Bypassed if there is no cards on the sensor
  if(!RFID_CheckIfCardIsThere())
  {
    return 0;
  }
  
  Debug_Start("RFID_HandleCard");
  receivedCard = RFID_GetCardNumber();

    if (receivedCard.compareTo(RFID_VALID_CARD) == 0) {
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
        Debug_Warning("RFID","RFID_HandleCard",RFID_VALID_CARD);
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
 * @return unsigned long long:
 * The Card ID. If 0, there is no card.
 */
String RFID_GetCardNumber() {
  Debug_Start("RFID_GetCardNumber");
  byte crecu = ' ';
  bool incoming = 0;
  bool thereWasACard = false;
  String id_tag = "NO_CARDS_FOUND";

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
          Debug_Information("RFID","RFID_GetCardNumber","Start of reading");
          // START OF TRANSMIT
          incoming = 1;
          break;

        case 0x03:
          Debug_Information("RFID","RFID_GetCardNumber","END OF READING");
          Debug_Information("RFID","RFID_GetCardNumber",id_tag);
          // END OF TRANSMIT
          incoming = 0;

          for (int i = 0; i < 10; i++) isReadingRFID = false;
          Debug_End();
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