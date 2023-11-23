/**
 * @file Claw.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File which contains the functions that
 * controls XFactor's package
 * recovery claw. These functions are then
 * used by regular Package functions.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Package/Claws.hpp"

/**
 * @brief
 * Global local variable allowing us to keep
 * track of the current deployment of the claw
 * throughout the multiple functions.
 */
bool deploymentStatus = CLAWS_STATUS_STORED;


/**
 * @brief
 * Initialises all the servo motors used in
 * XFactor's package recovery claw. This
 * function also sets their default positions
 * @return true:
 * Successfully initialised the claw
 * @return false:
 * Failed to initialise the claw.
 */
bool Claws_Init()
{
    Debug_Start("Claws_Init");
    pinMode(CLAWS_PINS_SWITCH, INPUT);
    S3003_Init(CLAWS_PINS_HEIGHT);
    S3003_Init(CLAWS_PINS_GRABBER);
    Debug_End();
    return true;
}

/**
 * @brief
 * Simple function that returns a boolean value
 * that corresponds to the claw's limit switch
 * value. This is used to detect if there is
 * something inside of XFactor's claw.
 * @return true:
 * Something is inside the claw.
 * @return false:
 * Nothing is inside the claw
 */
bool Claws_GetSwitchStatus()
{
    if(digitalRead(CLAWS_PINS_SWITCH)) return true;
    else return false;
}

/**
 * @brief
 * Function used to set the claw's grabber to a
 * wanted position in pourcents. The claw must be
 * deployed for this function to work.
 *
 * @param pourcent
 * How much should the grabbers be opened?
 * 0 means closed and 100 means completely
 * opened.
 * @return true:
 * Successfully moved the grabbers to the wanted
 * position.
 * @return false:
 * Failed to move the grabbers because the claw
 * is either not deployed or the specified value
 * is outside of the allowed range.
 */
bool Claws_SetGrabbers(unsigned char pourcent)
{
    float angle = (float)(pourcent*(CLAWS_GRABBERS_MAX-CLAWS_GRABBERS_MIN)/100);
    angle+=CLAWS_GRABBERS_MIN;

    if(angle<=CLAWS_GRABBERS_MIN && angle >=CLAWS_GRABBERS_MAX)
    {
        S3003_SetPosition(CLAWS_PINS_GRABBER, angle);
        return true;
    }
    else
    {
        Debug_Error("Claws", "Claws_SetGrabbers", "Angle outside boundaries");
        return false;
    }
}

/**
 * @brief
 * Function used to set the claw's height to a
 * wanted position in pourcents. The claw must be
 * deployed for this function to work.
 *
 * @param pourcent
 * How high should the claw be? 0 is as low as
 * possible and 100 is the highest.
 * @return true:
 * Successfully moved the height to the wanted
 * position.
 * @return false:
 * Failed to move the height because the claw
 * is either not deployed or the specified value
 * is outside of the allowed range.
 */
bool Claws_SetHeight(unsigned char pourcent)
{
    float angle = (float)(pourcent*(CLAWS_HEIGHT_MAX-CLAWS_HEIGHT_MIN)/100);
    angle+=CLAWS_HEIGHT_MIN;
    Debug_Information("","", String(pourcent));

    if(angle>=CLAWS_HEIGHT_MIN && angle<=CLAWS_HEIGHT_MAX)
    {
        S3003_SetPosition(CLAWS_PINS_HEIGHT, angle);
        return true;
    }
    else
    {
        Debug_Error("Claws", "Claws_SetHeight", "Angle outside boundaries");
        return false;
    }
}

/**
 * @brief
 * Function used to tell the claw to deploy or
 * store itself. While the claw doesn't
 * completely store itself, it will retract its
 * height and its grabbers to be out of the way
 * as much as possible. If you deploy the claw,
 * it will be in a position that is ready to pick
 * up packages.
 *
 * @attention
 * If a package is currently detected inside the
 * claw, deployment function will fail its
 * execution and return false.
 *
 * @param deployment
 * true means deployed and false means stored.
 * @return true:
 * Successfully deployed or stored the claw.
 * @return false:
 * Failed to deploy or store the claw.
 */
bool Claws_SetDeployment(bool deployment)
{
    if(!Claws_GetSwitchStatus())
    {
        if(deployment == CLAWS_STATUS_DEPLOYED)
        {
            deploymentStatus = CLAWS_STATUS_DEPLOYED;
            if(!Claws_SetGrabbers(CLAWS_GRABBERS_DEPLOYED))
            {
                Debug_Error("Claws", "Claws_SetDeployment", "Failed to set grabbers as CLAWS_GRABBERS_DEPLOYED");
                return false;
            }

            if(!Claws_SetHeight(CLAWS_HEIGHT_DEPLOYED))
            {
                Debug_Error("Claws", "Claws_SetDeployment", "Failed to set height as CLAWS_HEIGHT_DEPLOYED");
                return false;
            }
            return true;
        }
        else if (deployment == CLAWS_STATUS_STORED)
        {
            if(!Claws_SetHeight(CLAWS_HEIGHT_STORED))
            {
                Debug_Error("Claws", "Claws_SetDeployment", "Failed to set height as CLAWS_HEIGHT_STORED");
                return false;
            }

            if(!Claws_SetGrabbers(CLAWS_GRABBERS_STORED))
            {
                Debug_Error("Claws", "Claws_SetDeployment", "Failed to set grabbers as CLAWS_GRABBERS_STORED");
                return false;
            }
            deploymentStatus = CLAWS_STATUS_STORED;
            return true;
        }
        else
        {
            Debug_Error("Claws", "Claws_SetDeployment", "Improper deployment status.");
            return false;
        }
    }
    else
    {
        Debug_Error("Claws", "Claws_SetDeployment", "There is an object inside the claw.");
        return false;
    }
}

/**
 * @brief
 * This function slowly closes the claw a little
 * more than they currently are so that a certain
 * amount of force is applied to the package to
 * prevent it from easily slipping out of the
 * claw. The function will return wether or not
 * it was able to secure the package.
 *
 * @attention
 * The amount by which the claw closes to squeeze
 * the package is predefined inside of this
 * function.
 *
 * @return true:
 * Successfully squeezed the package.
 * @return false:
 * Failed to squeeze the package because there
 * either was never a package to start with, the
 * package was lost during squeezing or because
 * the claw is not deployed or not opened enough.
 */
bool Claws_SqueezePackage(int pourcentage)
{
    pourcentage -= CLAWS_SQUEEZE_DISTANCE;
    if(pourcentage < 0 || pourcentage > 100)
    {
        Debug_Error("Claws", "Claws_SqueezePackage", "Squeezing outside allowed range");
        return false;
    }
    else
    {
        if(!Claws_SetGrabbers(pourcentage))
        {
            Debug_Error("Claws", "Claws_SqueezePackage", "Failed to set grabbers at specified pourcent");
            return false;
        }
        return true;
    }
}

/**
 * @brief
 * Simple function that slowly closes XFactor's
 * grabbers until @ref Claws_GetSwitchStatus
 * detects a package. The speed at which the claw
 * closes its grabbers is set in a define.
 * @return true:
 * Successfully detected a package while closing
 * @return false:
 * Failed to detect a package while closing.
 */
bool Claws_CloseUntilDetection()
{
    Debug_Start("Claws_CloseUntilDetection");

    // - VARIABLES - //
    int percentage = 100;
    unsigned long previousInterval_ms = 0;

    // - GRADUAL CLOSING - //
    while(!Claws_GetSwitchStatus() && !(percentage <= 0))
    {
        if((millis()-previousInterval_ms) >= CLAW_CLOSE_INTERVAL_MS)
        {
            percentage-=CLAWS_CLOSING_SPEED;
            if(!Claws_SetGrabbers(percentage))
            {
                Debug_Error("Claws", "Claws_CloseUntilDetection", "Failed to set grabbers at specified pourcent.");
                Debug_End();
                return false;
            }
            previousInterval_ms = millis();
        }
        //return true;
    }

    //NECESSARY FOR DEBOUNCE ; WILL BE REPLACED IF WE HAVE TIME
    delay(5);

    if(Claws_GetSwitchStatus())
    {
        bool packageSqueezed = Claws_SqueezePackage(percentage);
        if(packageSqueezed)
        {
            Debug_End();
            return true;
        }
        else
        {
            if(!Claws_SetGrabbers(CLAWS_GRABBERS_DEPLOYED))
            {
                Debug_Error("Claws", "Claws_CloseUntilDetection", "Failed to set grabbers as CLAWS_GRABBERS_DEPLOYED");
                Debug_End();
                return false;
            }
            Debug_Error("Claws", "Claws_CloseUntilDetection", "Squeezed but lost the object.");
            Debug_End();
            return false;
        }
    }
    else if(percentage <= 0)
    {
        if(Claws_SetGrabbers(CLAWS_GRABBERS_DEPLOYED))
        {
            Debug_Error("Claws", "Claws_CloseUntilDetection", "Failed to set grabbers as CLAWS_GRABBERS_DEPLOYED");
            Debug_End();
            return false;
        }
        Debug_Warning("Claws", "Claws_CloseUntilDetection", "Did not grab anything even when completely closed.");
        Debug_End();
        return false;
    }
    else
    {
        Debug_Error("Claws", "Claws_CloseUntilDetection", "Fatal unexpected behaviour");
        Debug_End();
        return false;
    }
}