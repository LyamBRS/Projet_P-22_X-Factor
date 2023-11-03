/**
 * @file GROVE.hpp
 * @author Lyam BRS (lyam.brs@gmail.com)
 * @brief File used for the GROVE colour sensor.
 * If using another colour sensor, make a new file
 * with the same functions but with different front
 * name.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Sensors/Colour/GROVE.hpp"

/**
 * @brief Initialises one GROVE sensor depending on given
 * pins.
 * @param redPin Arduino pins for the red input
 * @param greenPin Arduino pins for the green input
 * @param bluePin Arduino pins for the blue input
 */
void GROVE_Innit(int redPin, int greenPin, int bluePin);

/**
 * @brief This function should use the specified colour
 * sensor pins and return a large number that represents
 * the hex value of the number. Numbers can be see as hex
 * and regular numbers automatically. It should combine
 * R,G,B into one big number.
 * 255,255,255 would be 255255255
 *
 * @param redPin Arduino pins for the red input
 * @param greenPin Arduino pins for the green input
 * @param bluePin Arduino pins for the blue input
 * @return unsigned int of the hex colour returned by the sensor.
 */
unsigned int GROVE_GetColor(int redPin, int greenPin, int bluePin);
