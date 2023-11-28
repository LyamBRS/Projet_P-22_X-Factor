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

    delay(10);
    if(GP2D12_Read(GARAGE_TRIG_PIN, GARAGE_ECHO_PIN) == 0)
    {
        Debug_Error("Garage", "Garage_Init", "Distance sensor returned 0");
        Debug_End();
        return false;
    }

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
    Debug_Start("Garage_XFactorInside");
    unsigned short doorDistance = GP2D12_Read(GARAGE_TRIG_PIN, GARAGE_ECHO_PIN);
    Debug_Warning("Garage", "Garage_XFactorInside", String(doorDistance));
    if(doorDistance < GARAGE_DISTANCE_VALUE_CLOSED)
    {
        Debug_End();
        return true;
    }
    else
    {
        Debug_End();
        return false;       
    }

    //Debug_Warning("Garage", "Garage_XFactorInside", String(GP2D12_Read(GARAGE_TRIG_PIN, GARAGE_ECHO_PIN)));
    //Debug_End();
    //return false;
}