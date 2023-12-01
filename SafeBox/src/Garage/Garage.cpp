/**
 * @file Garage.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing functions used to handle
 * SafeBox's garage.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Garage/Garage.hpp"

/**
 * @brief 
 * Global local variable used to keep track of
 * what the garage is supposed to be.
 * This is useful to bypass the IsClosed and the
 * potential alarm triggers if the door stays
 * open.
 */
volatile bool supposedGarageStatus = false;

/**
 * @brief
 * Initialises the garage of SafeBox.
 * Initialises the servo motors and sensors used
 * to control the garage of SafeBox.
 * @return true:
 * Successfully initialised all the sensors and
 * motors
 * @return false
 */
bool Garage_Init()
{
    Debug_Start("Garage_Init");
    GP2D12_Init(GARAGE_TRIG_PIN, GARAGE_ECHO_PIN);
    S3003_Init();

    // This delay is there to ensure that Arduino
    // has enough time to configure the pins used
    // This should take micro seconds, but its
    // good enough
    delay(10);
    if(GP2D12_Read(GARAGE_TRIG_PIN, GARAGE_ECHO_PIN) == 0)
    {
        Debug_Error("Garage", "Garage_Init", "Distance sensor returned 0");
        Debug_End();
        return false;
    }
    pinMode(GARAGE_IS_CLOSED_DEBUG_PIN, OUTPUT);
    Debug_End();
    return true;
}

/**
 * @brief
 * Function that attempts to open the garage of
 * SafeBox.
 * @return true:
 * Successfully opened the garage.
 * @return false:
 * Failed to open the garage.
 */
bool Garage_Open()
{
    Debug_Start("Garage_Open");
    servo2.write(ANGLE_OPEN);
    servo3.write(ANGLE_OPEN);
    if(!supposedGarageStatus)
    {
        Debug_Information("Garage", "Garage_Open", "Garage should no longer check to see if its closed");
    }
    supposedGarageStatus = true;
    Debug_End();
    return true;
}

/**
 * @brief
 * Function that attempts to close the garage of
 * SafeBox.
 * @return true:
 * Successfully closed the garage.
 * @return false:
 * Failed to close the garage.
 */
bool Garage_Close()
{
    Debug_Start("Garage_Close");
    servo2.write(ANGLE_CLOSED);
    servo3.write(ANGLE_CLOSED);

    if(supposedGarageStatus)
    {
        Debug_Information("Garage", "Garage_Close", "Garage should now check to see if its closed");
    }
    supposedGarageStatus = false;
    Debug_End();
    return true;
}

/**
 * @brief
 * Function that analyses the distance sensor
 * that is inside SafeBox's garage and tells the
 * program if XFactor is inside.
 *
 * @return true:
 * XFactor is detected inside the garage.
 * @return false:
 * XFactor is not inside of the garage.
 */
bool Garage_XFactorInside()
{
    Debug_Start("Garage_XFactorInside");
    Debug_Warning("Garage", "Garage_XFactorInside", "BYPASSED");
    Debug_End();
    return false;
}

/**
 * @brief 
 * Returns wether the garage SHOULD be closed
 * or not.
 * @return true:
 * Should be opened.
 * @return false:
 * Should be closed.
 */
bool Garage_GetSupposedWantedStatus()
{
    return supposedGarageStatus;
}

/**
 * @brief
 * Function that analyses the distance sensor
 * that is inside SafeBox's garage and tells the
 * program if XFactor is inside.
 *
 * @return true:
 * The distance sensor sees the door as closed.
 * @return false:
 * XFactor is either inside or the door is closed.
 */
bool Garage_IsClosed()
{
    Debug_Start("Garage_IsClosed");
    unsigned short doorDistance = GP2D12_Read(GARAGE_TRIG_PIN, GARAGE_ECHO_PIN);
    Debug_Warning("Garage", "Garage_IsClosed", String(doorDistance));
    if(doorDistance < GARAGE_DISTANCE_VALUE_CLOSED)
    {
        digitalWrite(GARAGE_IS_CLOSED_DEBUG_PIN, HIGH);
        Debug_End();
        return true;
    }
    else
    {
        digitalWrite(GARAGE_IS_CLOSED_DEBUG_PIN, LOW);
        Debug_End();
        return false;       
    }
}

/**
 * @brief Enables the debug light.
 * Is on if its closed and off
 * if its open.
 */
void Garage_ShowDebugLight()
{
    unsigned short doorDistance = GP2D12_Read(GARAGE_TRIG_PIN, GARAGE_ECHO_PIN);
    if(doorDistance < GARAGE_DISTANCE_VALUE_CLOSED)
    {
        digitalWrite(GARAGE_IS_CLOSED_DEBUG_PIN, HIGH);
    }
    else
    {
        digitalWrite(GARAGE_IS_CLOSED_DEBUG_PIN, LOW);   
    }
}