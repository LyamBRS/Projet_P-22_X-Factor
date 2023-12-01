/**
 * @file Lid.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Contains the functions used to handle the
 * lid of SafeBox.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Lid/Lid.hpp"

/**
 * @brief
 * Function that initialises the servo motors and
 * other sensors that is used to make a the lid
 * of SafeBox function.
 * @return true:
 * Successfully initialised the lid of SafeBox.
 * @return false:
 * Failed to initialise the lid of SafeBox.
 */
bool Lid_Init()
{
    Debug_Start("Lid_Init");
    Debug_End();
    return true;
}

/**
 * @brief
 * This function attempts to open the package
 * drop off lid of SafeBox.
 *
 * @return true:
 * Successfully opened the lid.
 * @return false:
 * Failed to unlock or open the lid.
 */
bool Lid_Open()
{
    Debug_Start("Lid_Open");
    Debug_Warning("Lid", "Lid_Open", "BYPASSED");
    Debug_End();
    return false;
}

/**
 * @brief
 * This function attempts to close the package
 * drop off lid of SafeBox.
 *
 * @return true:
 * Successfully closed the lid.
 * @return false:
 * Failed to lock or close the lid.
 */
bool Lid_Close()
{
    Debug_Start("Lid_Close");
    Debug_Warning("Lid", "Lid_Close", "BYPASSED");
    Debug_End();
    return false;
}

/**
 * @brief
 * This function attempts to lock the package
 * drop off lid of SafeBox.
 *
 * @return true:
 * Successfully locked the lid.
 * @return false:
 * Failed to lock the lid.
 */
bool Lid_Unlock()
{
    Debug_Start("Lid_Unlock");
    servo1.attach(SERVO_RFID_PIN); 
    servo1.write(PACKAGE_DOOR_UNLOCKED);
    Debug_End();
    return true;
}

/**
 * @brief
 * This function attempts to unlock the package
 * drop off lid of SafeBox.
 *
 * @return true:
 * Successfully unlock the lid.
 * @return false:
 * Failed to unlock the lid.
 */
bool Lid_Lock()
{
    Debug_Start("Lid_Lock");
    servo1.attach(SERVO_RFID_PIN); 
    servo1.write(PACKAGE_DOOR_LOCKED);
    Debug_End();
    return true;
}

/**
 * @brief 
 * Returns true if the lid is really closed or
 * not.
 * @return true:
 * It is locked.
 * @return false:
 * It isnt locked.
 */
bool Lid_IsClosed()
{
    return digitalRead(LID_CLOSED_SWITCH_PIN);
}