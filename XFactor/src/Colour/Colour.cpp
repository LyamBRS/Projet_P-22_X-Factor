/**
 * @file Colour.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief 
 * Contains the functions necessary to handle
 * colours utils functions.
 * @version 0.1
 * @date 2023-11-16
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Colour/Colour.hpp"
// - DEFINES - //

// - FUNCTIONS - //
/**
 * @brief 
 * This function takes an RGBC value and
 * transforms it into an HEX value so it can be
 * read in one single value with BitShifts.
 * 0,255,0,255 will thus be 0x00FF00FF
 * @param red 
 * Red value ranging from 0 to 255
 * @param green
 * green value ranging from 0 to 255
 * @param blue 
 * blue value ranging from 0 to 255
 * @param clear 
 * clear value ranging from 0 to 255
 * @return unsigned long:
 * hex value of 0xRRGGBBCC
 */
unsigned long Colour_GetHexFromRGBC(int red, int green, int blue, int clear)
{
    unsigned long resultedColor = 0;
    unsigned long temporaryLargeValue = 0;

    //lowest value is the clear value, it thus does not need to be shifted.
    resultedColor = resultedColor + (unsigned long)(clear);

    //third value is the blue value; it needs to be shifted by 8 bits, We thus need to use a new type to hold

    temporaryLargeValue = (unsigned long)(blue);
    temporaryLargeValue = temporaryLargeValue << 8;
    resultedColor = resultedColor + temporaryLargeValue;

    //second value is the green value, it needs to be shifted by 16 bits.
    temporaryLargeValue = (unsigned long)(green);
    temporaryLargeValue = temporaryLargeValue << 16;
    resultedColor = resultedColor + temporaryLargeValue;

    //first value is the red value; it needs to be shifted by 24 bits.

    temporaryLargeValue = (unsigned long)(red);
    temporaryLargeValue = temporaryLargeValue << 24;
    resultedColor = resultedColor + temporaryLargeValue;

    return resultedColor;
}

/**
 * @brief 
 * Checks if an HEX value is within a range.
 * So it would check if 0x00AA00FF is within
 * 0x00BB00FF and 0x00CC00FF for example.
 * @param lowValue
 * The lowest RGB value in the threshold
 * @param currentValue
 * The value to check if within threshold
 * @param maxValue 
 * The maximum value of the threshold
 * @return true:
 * Is within the threshold
 * @return false:
 * Isnt within the threshold
 */
bool Colour_Threshold(unsigned long lowValue, unsigned long currentValue, unsigned long maxValue)
{
    //int red = Colour_GetRed(currentValue);
    //if (red <= 21)
    //{
    //    return true;
    //}
    //return false;
    // - VARIABLES - //
    int lowRed      = Colour_GetRed(lowValue);
    int lowGreen    = Colour_GetGreen(lowValue);
    int lowBlue     = Colour_GetBlue(lowValue);
    int lowClear    = Colour_GetClear(lowValue);

    int currentRed      = Colour_GetRed(currentValue);
    int currentGreen    = Colour_GetGreen(currentValue);
    int currentBlue     = Colour_GetBlue(currentValue);
    int currentClear    = Colour_GetClear(currentValue);

    int maxRed      = Colour_GetRed(maxValue);
    int maxGreen    = Colour_GetGreen(maxValue);
    int maxBlue     = Colour_GetBlue(maxValue);
    int maxClear    = Colour_GetClear(maxValue);

    // - Check thresholds - //
    if(!(lowRed <= currentRed && maxRed >= currentRed))
    {
        // The value is not within the red threshold.
        Debug_Error("-", "-", "Red does not match: " + String(lowRed) + " " + String(currentRed) + " " + String(maxRed));
        return false;
    }

    if(!(lowGreen <= currentGreen && maxGreen >= currentGreen))
    {
        // The value is not within the green threshold.
        Debug_Error("-", "-", "Green does not match: " + String(lowGreen) + " " + String(currentGreen) + " " + String(maxGreen));
        return false;
    }

    if(!(lowBlue <= currentBlue && maxBlue >= currentBlue))
    {
        // The value is not within the blue threshold.
        Debug_Error("-", "-", "blue does not match by: " + String(currentBlue - lowBlue));
        return false;
    }

    if(!(lowClear <= currentClear && maxClear >= currentClear))
    {
        // The value is not within the clear threshold.
        Debug_Error("-", "-", "clear does not match by: " + String(currentClear - lowClear));
        return false;
    }

    // All RGB values are within the threshold
    return true;
}

/**
 * @brief 
 * Returns a value from 0 to 255 from an HEX
 * RGBC value that corresponds to the red value
 * inside the HEX value.
 * @param hexValue 
 * @return int
 * value from 0 to 255
 */
int Colour_GetRed(unsigned long hexValue)
{
    // - VARIABLES - //
    int result = 0;

    result = (int)(hexValue >> 24);
    result = 0xFF & result;
    return result; 
}

/**
 * @brief 
 * Returns a value from 0 to 255 from an HEX
 * RGBC value that corresponds to the green value
 * inside the HEX value.
 * @param hexValue 
 * @return int
 * value from 0 to 255
 */
int Colour_GetGreen(unsigned long hexValue)
{
    // - VARIABLES - //
    int result = 0;

    hexValue = hexValue >> 16;
    hexValue = hexValue & 0xFF;
    result = (int)hexValue;
    return result; 
}

/**
 * @brief 
 * Returns a value from 0 to 255 from an HEX
 * RGBC value that corresponds to the blue value
 * inside the HEX value.
 * @param hexValue 
 * @return int
 * value from 0 to 255
 */
int Colour_GetBlue(unsigned long hexValue)
{
    // - VARIABLES - //
    int result = 0;

    hexValue = hexValue >> 8;
    hexValue = hexValue & 0xFF;
    result = (int)hexValue;
    return result; 
}

/**
 * @brief 
 * Returns a value from 0 to 255 from an HEX
 * RGBC value that corresponds to the clear value
 * inside the HEX value.
 * @param hexValue 
 * @return int
 * value from 0 to 255
 */
int Colour_GetClear(unsigned long hexValue)
{
    // - VARIABLES - //
    int result = 0;

    hexValue = hexValue & 0xFF;
    result = (int)hexValue;
    return result; 
}