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
int Colour_GetClear(unsigned long hexValue);
{
    
}