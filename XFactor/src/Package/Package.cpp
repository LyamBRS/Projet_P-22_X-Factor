/**
 * @file Package.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the various functions that are
 * used to locate, identify, pick up, handle, and
 * deposit a package for XFactor.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Package/Package.hpp"


bool package_setUp = false;
bool pickup = false;

unsigned short distanceDetected_cm;

/**
 * @brief
 * Initialisation function that initialises the
 * Claw and other sensors used to detect a
 * package.
 * @return true:
 * Successfully initialised package functions.
 * @return false:
 * Failed to initialise package functions.
 */
bool Package_Init()
{
    GP2D12_Init(FRONT_SENSOR_TRIG_PIN_NUMBER, FRONT_SENSOR_ECHO_PIN_NUMBER);
    GP2D12_Init(LEFT_SENSOR_TRIG_PIN_NUMBER, LEFT_SENSOR_ECHO_PIN_NUMBER);
    GP2D12_Init(RIGHT_SENSOR_TRIG_PIN_NUMBER, RIGHT_SENSOR_ECHO_PIN_NUMBER);
    
    if(GROVE_Init())
    {
        if(Claws_Init())
        {   
            Package_StoreClaw();
            //delay(1500);
            return true;
        }
        else
        {
            Debug_Error("Package", "Package_Init", "Failed to initialise Claws");
        }
    }
    else
    {
        Debug_Error("Package", "Package_Init", "Failed to initialise GROVE");
    }

    return false;
}

/**
 * @brief
 * Function that releases the package that is
 * currently inside of XFactor's claw system.
 *
 * @return true:
 * A package was successfully released from the
 * claw system.
 * @return false:
 * Failed to release the package. Either because
 * there was no package inside the claw to start
 * with, or because the package is still detected
 * inside the claw after the function is executed
 * or because the claw is not deployed.
 */

bool Package_Release()
{
    /*if (package_setUp == false && pickup == false){
        return false;
    }*/

    if (Claws_SetGrabbers(100) == true){
        Package_SetStatus(false);
        return true;
    }

    return false;
}

/**
 * @brief
 * Function that automatically picks up a package
 * that is right where the claw is. This function
 * will deploy the claw if not already done.
 *
 * @attention
 * The robot must already be facing the package
 * correctly before this function is executed.
 *
 * @return true:
 * Successfully picked up a package
 * @return false:
 * Failed to pick up a package, try again.
 */

bool Package_PickUp()
{
    Debug_Start("Package_PickUp");
    
    if(!Package_DeployClaw())
    {
        Debug_Error("Package", "Package_PickUp", "Failed to deploy the claw");
        Debug_End();
        return false;
    }

    for (int i = 0; i < 5; i++)
    {
        if (!pickup)
        {
            pickup = Claws_CloseUntilDetection();

            if (pickup)
            {
                if(!Claws_SetHeight(PACKAGE_CLAW_HEIGHT_POSITION_TRANSPORT))
                {
                    Debug_Error("Package", "Package_PickUp", "Failed to set height of the claw");
                    Debug_End();
                    return false;
                }
                Debug_Information("Package", "Package_PickUp", "Successfully picked up the package");
                Debug_End();
                Package_SetStatus(true);
                return true;
            }
        }
        delay(500); //MAY NEED TO BE REMOVED when we advance
    }

    
    if(!MoveFromVector(PICK_UP_PACKAGE_VECTOR))
    {
        Debug_Error("Package", "Package_PickUp", "Failed to move from vector");
        Debug_End();
        return false;
    }

    Debug_Error("Package", "Package_PickUp", "Failed to pick up the package");
    Debug_End();
    return false;
}

/**
 * @brief Function that aligns XFactor's claw
 * with the position required to drop packages
 * inside of SafeBox. This function can be
 * executed even if there is no packages inside
 * of the claw.
 *
 * @return true:
 * The claw is positioned how it needs to be to
 * drop packages inside SafeBox.
 * @return false:
 * Failed to position the claw into the correct
 * position due to it not being deployed.
 */

bool Package_AlignWithSafeBox()
{
    return MoveFromVector(ALIGN_WITH_SAFEBOX_VECTOR); // see if it works as intended
}

/**
 * @brief
 * Function that stores the claw in a way that is
 * as small as possible so that its out of the
 * way when its not in use. This function only
 * calls @ref Claws_SetDeployement but could do
 * more if required.
 *
 * @return true:
 * Successfully stored away XFactor's claw.
 * @return false:
 * Failed to store the claw because a package is
 * still detected inside of it.
 */

bool Package_StoreClaw()
{
    if (package_setUp == true){
        return false;
    }

    return Claws_SetDeployment(false);
}

/**
 * @brief
 * Deploys XFactor's package recovery claw system
 * through @ref Claws_SetDeployement. This must
 * have been called prior to attempting to pick
 * up a package.
 * @return true:
 * Successfully deployed the claw.
 * @return false:
 * Failed to deploy the claw.
 */

bool Package_DeployClaw()
{
    return Claws_SetDeployment(true);
}

/**
 * @brief
 * Arranges XFactor's claw into the best solution
 * for package transportation. This function must
 * be called before the robot starts to move with
 * a package inside of the claw. It ensures that
 * the package won't be in the way as much as it
 * would've if the function is not called.
 *
 * @return true:
 * Successfully moved the claw into the
 * appropriated position to transport a package.
 * @return false:
 * Failed to move the claw into the wanted
 * position. Either because no package is found
 * inside the claw or because the claw is not
 * deployed.
 */

bool Package_Transport()
{
    return Claws_SetHeight(PACKAGE_CLAW_HEIGHT_POSITION_TRANSPORT);
}

/**
 * @brief
 * Complex function that analyses XFactor's
 * color sensor to verify is the object examined
 * is indeed the package it needs to pick up.
 *
 * Packages are identified using color sensors.
 * @return true:
 * A package was detected near the robot.
 * @return false:
 * No packages are detected anywhere near or
 * inside the robot.
 */

bool Package_Confirmed()
{
    unsigned long currentColour = 0;

    currentColour = GROVE_GetColor();

    if (currentColour < 23) return true;
    /*if(Colour_Threshold(0x15000000, currentColour, 0xFFFFFFFF))
    {
        return true;
    }*/

    return false;
}

/**
 * @brief
 * Complex function that analyses XFactor's
 * distance sensors with the goal of locating
 * the package. It reads all of the distance
 * sensors and compares them to a threshold.
 * 
 * @param capteur
 * This is used to read from the right sensor 
 * so other functions know what to return 
 * depending on which sensor triggered.
 *
 * Packages are identified using color sensors.
 * @return PACKAGE_DETECTED:
 * A package was detected near the robot.
 * @return NOTHING_DETECTED:
 * No packages are detected anywhere near or
 * inside the robot.
 * @return BOX_DETECTED
 * SafeBox has been detected near the robot
 */
int Package_Detected(int capteur, float relativeRotation_rad)
{
    if (Claws_GetSwitchStatus())
    {
        return true;
    } 
    else
    {
        switch(capteur){
            case FRONT_SENSOR:
                distanceDetected_cm = GP2D12_Read(FRONT_SENSOR_TRIG_PIN_NUMBER, FRONT_SENSOR_ECHO_PIN_NUMBER);
                break;
            case LEFT_SENSOR:
                distanceDetected_cm = GP2D12_Read(LEFT_SENSOR_TRIG_PIN_NUMBER, LEFT_SENSOR_ECHO_PIN_NUMBER);
                break;
            case RIGHT_SENSOR:
                distanceDetected_cm = GP2D12_Read(RIGHT_SENSOR_TRIG_PIN_NUMBER, RIGHT_SENSOR_ECHO_PIN_NUMBER);
                break;
            default:
                return NOTHING_DETECTED;
                break;
        }
        //Debug_Information("Package.cpp", "Package_Detected", "Sensor : " + String(capteur));
        //Debug_Information("Package.cpp", "Package_Detected", "Distance : " + String(distanceDetected_cm));
        return distanceDetected_cm < DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM;

        //return Package_SafeBoxDetected(capteur, (float)distanceDetected_cm, relativeRotation_rad);
    }
}

/**
 * @brief
 * Getter for the detected distance
 * of the last distance detector
 * that measured the distance
 *
 * @return unsigned short :
 * distance in centimeters
 */
unsigned short Package_GetDetectedDistance()
{
    return distanceDetected_cm;
}

/**
 * @brief
 * Function that defines if XFactor should be
 * expecting to carry a package or not. For
 * example, if XFactor finds a package and picks
 * it up, this function should be called to
 * make the program know that XFactor should be
 * carrying a package. This way, if the package
 * is no longer detected, an alarm can be turned
 * on.
 *
 * This also allows the alarm to not be turned on
 * if the package is supposed to be no longer
 * detected.
 *
 * @param newPackageStatus
 * Should XFactor expect a package inside its
 * claw?
 * @return true:
 * The new package status was set successfully.
 * @return false:
 * Failed to set a new package status.
 */

bool Package_SetStatus(bool newPackageStatus)
{
    package_setUp = newPackageStatus;
    return true;

    /*if (Package_Confirmed() == true && Package_DeployClaw() == true && package_setUp == true){
        return true;
    }

    else if (!Package_Confirmed() == true && Package_StoreClaw() == true && package_setUp == false){
        return true;
    }
    package_setUp = false;
    return false;*/
}

/**
 * @brief
 * Function that simply returns the expected
 * package state.
 *
 * @return true:
 * XFactor is supposed to be carrying a package
 * @return false:
 * XFactor shouldn't be carrying a package.
 */

bool Package_GetStatus()
{
    return package_setUp;
}

/**
 * @brief
 * Return if the detected package is
 * in fact SafeBox or if the detected
 * object is out of the assigned area
 *
 * @return SAFEBOX_DETECTED:
 * Safebox is detected
 * @return PACKAGE_DETECTED:
 * What was detected is not SafeBox.
 * @return OUT_OF_BOUNDS_DETECTED:
 * What was detected is not within the demonstration area.
 */

int Package_SafeBoxDetected(int sensorId, float distanceDetected_cm, float relativeRotation_rad)
{
    MovementVector position = GetSavedPosition();
    position.rotation_rad += relativeRotation_rad;
    /*MovementVector position;
    position.distance_cm = 100.0f;
    position.rotation_rad = PI/4;

    relativeRotation_rad = -(PI / 4);
    distanceDetected_cm = 40.0f;
    sensorId = FRONT_SENSOR;*/

    switch (sensorId)
    {
        case FRONT_SENSOR:
            break;
        case LEFT_SENSOR:
            position.rotation_rad += TURN_90_LEFT;
            break;
        case RIGHT_SENSOR:
            position.rotation_rad += TURN_90_RIGHT;
            break;
        default:
            break;
    }

    float cosPosition = cos(position.rotation_rad);
    float sinPosition = sin(position.rotation_rad);

    float positionX = cosPosition * position.distance_cm;
    float positionY = sinPosition * position.distance_cm;

    float distanceDetectedX_cm = -cos(relativeRotation_rad) * distanceDetected_cm;
    float distanceDetectedY_cm = sin(relativeRotation_rad) * distanceDetected_cm;

    Debug_Information("Package", "Package_SafeBoxDetected", "PositionX : " + String(positionX));
    Debug_Information("Package", "Package_SafeBoxDetected", "PositionY : " + String(positionY));

    Debug_Information("Package", "Package_SafeBoxDetected", "Detection distance X : " + String(distanceDetectedX_cm));
    Debug_Information("Package", "Package_SafeBoxDetected", "Detection distance Y : " + String(distanceDetectedY_cm));

    if (positionY + distanceDetectedY_cm > DEMO_AREA_WIDTH_CM || positionY + distanceDetectedY_cm < 0)
    {
        Debug_Information("Package", "Package_SafeBoxDetected", "Out of bounds Y");
        LEDS_SetColor(LED_ID_STATUS_INDICATOR, 32, 32, 32); // Low white
        return OUT_OF_BOUNDS_DETECTED;
    }

    if (positionX + distanceDetectedX_cm > DEMO_AREA_LENGTH_CM || positionX + distanceDetectedX_cm < 0)
    {
        Debug_Information("Package", "Package_SafeBoxDetected", "Out of bounds X");
        LEDS_SetColor(LED_ID_STATUS_INDICATOR, 32, 32, 32); // Low white
        return OUT_OF_BOUNDS_DETECTED;
    }

    /*if (abs(positionY + distanceDetectedY_cm) < SAFEBOX_WIDTH_CM)
    {
        Debug_Information("Package", "Package_SafeBoxDetected", "SafeBox detected Y");
    }

    if (abs(positionX + distanceDetectedX_cm) < SAFEBOX_LENGTH_CM)
    {
        Debug_Information("Package", "Package_SafeBoxDetected", "SafeBox detected X");
    }*/

    if (abs(positionY + distanceDetectedY_cm) < SAFEBOX_WIDTH_CM && abs(positionX + distanceDetectedX_cm) < SAFEBOX_LENGTH_CM)
    {
        Debug_Information("Package", "Package_SafeBoxDetected", "SafeBox detected");
        LEDS_SetColor(LED_ID_STATUS_INDICATOR, 128, 32, 0); // Blue
        return SAFEBOX_DETECTED;
    }
    return PACKAGE_DETECTED;
}