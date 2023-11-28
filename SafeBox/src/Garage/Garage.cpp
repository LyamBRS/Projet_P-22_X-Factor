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
    S3003_Init();
    return false;
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
    
    servo2.write(ANGLE_OPEN);
    servo3.write(ANGLE_OPEN);
    return false;
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
    servo2.write(ANGLE_CLOSED);
    servo3.write(ANGLE_CLOSED);
    return false;
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
    
    return false;
}