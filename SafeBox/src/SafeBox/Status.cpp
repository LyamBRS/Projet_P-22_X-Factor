/**
 * @file Status.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the functions used to handle
 * SafeBox's status and store as well as use the
 * status.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "SafeBox/Status.hpp"

/**
 * @brief
 * Local global variable that holds the status
 * of SafeBox so that getter and setter functions
 * can be used in the program.
 */
SafeBox_Status SafeBoxSavedStatus = SafeBox_Status::Off;

//#pragma region [FUNCTIONS]
/**
 * @brief
 * Function that sets the global variable which
 * holds the current status of SafeBox to a new
 * desired value from @ref SafeBox_StatusEnum
 * @param newStatus
 * A value within @ref SafeBox_StatusEnum
 * @return true:
 * The status was successfully updated to the new
 * specified status.
 * @return false:
 * The specified status does not match available
 * status of @ref SafeBox_StatusEnum
 */
bool SafeBox_SetNewStatus(SafeBox_Status newStatus)
{
    static SafeBox_Status oldStatus = SafeBox_Status::Off;

    Debug_Start("SafeBox_SetNewStatus");
    switch(newStatus)
    {
        case(SafeBox_Status::CommunicationError):
        case(SafeBox_Status::Off):
        case(SafeBox_Status::WaitingForDelivery):
        case(SafeBox_Status::WaitingForRetrieval):
        case(SafeBox_Status::WaitingForReturn):
        case(SafeBox_Status::WaitingForXFactor):
        case(SafeBox_Status::ReadyForDropOff):
        case(SafeBox_Status::Unlocked):
        case(SafeBox_Status::DroppingOff):
        case(SafeBox_Status::Maintenance):
        case(SafeBox_Status::Error):
        case(SafeBox_Status::Alarm):
            // The status is valid.
            SafeBoxSavedStatus = newStatus;

            if(oldStatus != newStatus)
            {
                oldStatus = newStatus;
                SafeBox_SaveStatusInEEPROM();
            }

            Debug_End();
            return true;

        default:
            // Invalid status
            SafeBoxSavedStatus = SafeBox_Status::Error;
            Debug_Error("Status", "SafeBox_SetNewStatus", "Unknown status. Error returned");
            Debug_End();
            return false;
    }
    // SHOULD NEVER REACH HERE
    return false;
}

/**
 * @brief
 * Function that returns the current status of
 * SafeBox as an unsigned char. You must check
 * that value with @ref SafeBox_StatusEnum to
 * identify what the current status of SafeBox is
 *
 * @return unsigned char
 * value from @ref SafeBox_StatusEnum which
 * corresponds to SafeBox's current status.
 */
SafeBox_Status SafeBox_GetStatus()
{
    return SafeBoxSavedStatus;
}

/**
 * @brief 
 * Converts the enum to an int.
 * Used in the EEPROM functions
 * @param status 
 * @return uint8_t 
 */
uint8_t SafeBox_StatusToInt(SafeBox_Status status)
{
    switch(status)
    {
        case(SafeBox_Status::CommunicationError): return 253;
        case(SafeBox_Status::Off):                return 0;
        case(SafeBox_Status::WaitingForDelivery): return 1;
        case(SafeBox_Status::WaitingForRetrieval): return 2;
        case(SafeBox_Status::WaitingForReturn): return 3;
        case(SafeBox_Status::WaitingForXFactor): return 254;
        case(SafeBox_Status::ReadyForDropOff): return 4;
        case(SafeBox_Status::Unlocked): return 5;
        case(SafeBox_Status::DroppingOff): return 10;
        case(SafeBox_Status::Maintenance): return 50;
        case(SafeBox_Status::Error): return 51;
        case(SafeBox_Status::Alarm): return 255;

        default:
            return 0;
    }
    // SHOULD NEVER REACH HERE
    return 0;
}

/**
 * @brief 
 * Converts the int to a status.
 * Used in the EEPROM functions
 * @param status 
 * @return SafeBox_Status 
 */
SafeBox_Status SafeBox_IntToStatus(uint8_t status)
{
    switch(status)
    {
        case(253):  return SafeBox_Status::CommunicationError;
        case(0):    return SafeBox_Status::Off;
        case(1):    return SafeBox_Status::WaitingForDelivery;
        case(2):    return SafeBox_Status::WaitingForRetrieval;
        case(3):    return SafeBox_Status::WaitingForReturn;
        case(254):  return SafeBox_Status::WaitingForXFactor;
        case(4):    return SafeBox_Status::ReadyForDropOff;
        case(5):    return SafeBox_Status::Unlocked;
        case(10):   return SafeBox_Status::DroppingOff;
        case(50):   return SafeBox_Status::Maintenance;
        case(51):   return SafeBox_Status::Error;
        case(255):  return SafeBox_Status::Alarm;

        default:
            return SafeBox_Status::Off;
    }
    // SHOULD NEVER REACH HERE
    return SafeBox_Status::Off;
}

/**
 * @brief 
 * Saves the status of SafeBox in the EEPROM.
 * Automatically uses the current status of
 * SafeBox so no input parameter is needed.
 * @return true:
 * Successfully stored the status in the eeprom
 * @return false:
 * Failed to store the status in the eeprom.
 */
bool SafeBox_SaveStatusInEEPROM()
{
    Debug_Start("SafeBox_SaveStatusInEEPROM");
    Debug_Information("Status", "SafeBox_SaveStatusInEEPROM", String(SafeBox_StatusToInt(SafeBoxSavedStatus)));
    
    EEPROM.write(EEPROM.length()-70, SafeBox_StatusToInt(SafeBoxSavedStatus));
    delay(1);

    if(SafeBoxSavedStatus != SafeBox_GetEEPROMStatus())
    {
        Debug_Error("Status", "SafeBox_SaveStatusInEEPROM", "Saved value in EEPROM didnt match wanted value");
    }
    Debug_End();
    return true;
}

SafeBox_Status SafeBox_GetEEPROMStatus()
{
    int status = EEPROM.read(EEPROM.length()-70);
    return SafeBox_IntToStatus(status);
}

/**
 * @brief Function that returns true if the alarm
 * needs to be activated depending on the status
 * that is found in the Arduino's EEPROM.
 * 
 * @return true:
 * Should start the alarm
 * @return false:
 * Should not start the alarm.
 */
bool SafeBox_EEPROMStatusShouldStartAlarm()
{
    Debug_Start("SafeBox_EEPROMStatusShouldStartAlarm");
    SafeBox_Status status = SafeBox_GetEEPROMStatus();

    switch(status)
    {
        case(SafeBox_Status::CommunicationError):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is CommunicationError");
        Debug_End();
        return false;

        case(SafeBox_Status::Off):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is Off");
        Debug_End();           
        return false;

        case(SafeBox_Status::WaitingForDelivery):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is WaitingForDelivery");
        Debug_End();
        return true;

        case(SafeBox_Status::WaitingForRetrieval):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is WaitingForRetrieval");
        Debug_End();
        return true;

        case(SafeBox_Status::WaitingForReturn):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is WaitingForReturn");
        Debug_End();
        return true;

        case(SafeBox_Status::WaitingForXFactor):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is WaitingForXFactor");
        Debug_End();
        return false;

        case(SafeBox_Status::ReadyForDropOff):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is ReadyForDropOff");
        Debug_End();
        return true;

        case(SafeBox_Status::Unlocked):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is Unlocked");
        Debug_End();
        return false;

        case(SafeBox_Status::DroppingOff):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is DroppingOff");
        Debug_End();
        return true;

        case(SafeBox_Status::Maintenance):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is Maintenance");
        Debug_End();
        return false;

        case(SafeBox_Status::Error):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is Error");
        Debug_End();
        return false;

        case(SafeBox_Status::Alarm):
        Debug_Information("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is Alarm");
        Debug_End();
        return true;

        default:
            Debug_Error("Status", "SafeBox_EEPROMStatusShouldStartAlarm", "EEPROM status is UNKNOWN");
            Debug_End();
            return false;
    }
}
//#pragma endregion