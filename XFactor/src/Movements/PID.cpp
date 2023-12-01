/**
 * @file PID.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File that contains the definition of PID
 * functions. The PID needs to be reset before
 * each long term use.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Movements/PID.hpp"

// - VARIABLES - //
float previousValue = 0.0f;
float errorSum = 0.0f;

/**
 * @brief Basic PID function that returns an
 * adjusted value over time based on a wanted
 * value. P,I,D numbers are used to tune the
 * responsiveness of the PID and varies
 * depending on its applications.
 *
 * For example, to tune the difference
 * between 2 motors that should be the same,
 * you would first get the difference between
 * the 2, knowing you want it to be 0, and
 * then call he function as following:
 * speedCorrection = PID(p, i, d, difference, 0);
 *
 * @param proportional
 * the P value of the PID. The increase of the
 * number the further it is from wanted value.
 * @param integral
 * the I value of the PID. In other words, the
 * small corrections done over time if there's
 * oscillation.
 * @param derivative
 * The D value of the PID. When the value
 * begins to reach the wanted value, this
 * will de accelerate the PID.
 * @param currentValue
 * The current value that was just read.
 * Its the value that needs to be corrected
 * @param wantedValue
 * The value we want to reach using this
 * PID.
 * @param startingValue
 * The value at which the PID will start.
 * This is to avoid overshoots and
 * overreacting if the PID does not start
 * at 0.
 * @return float
 */
float PID(float proportional, float integral, float derivative, float currentValue, float wantedValue, float startingValue)
{
    float error = wantedValue-currentValue;
    float derivativeError = currentValue-(previousValue);
    errorSum += error;

    float PID = startingValue + proportional*error + integral*(errorSum) + derivative*(derivativeError);

    previousValue = currentValue;

    //if (PID == ovf) return 0.0f;
    //Debug_Information("PID", "PID", "Speed : " + String(PID));
    if (PID > SPEED_MAX + 0.05f) PID = SPEED_MAX + 0.05f;
    if (PID < -SPEED_MAX - 0.05f) PID = -SPEED_MAX - 0.05f;
    return PID;
}

/**
 * @brief Resets @ref PID
 * function. Must be called before you
 * use the @ref PID function. this
 * allows it to reset its previous
 * variables and sum of errors.
 * @return true:
 * Successfully reset the PID.
 * @return false:
 * Failed to reset the PID.
 */
bool ResetPID()
{
    previousValue = 0.0f;
    errorSum = 0.0f;
    
    if (previousValue == 0 && errorSum == 0) return true;
    else return false;
}