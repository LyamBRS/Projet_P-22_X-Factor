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

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
/**
 * @brief Initialises one GROVE sensor depending on given
 * pins.
 * 
 * @return true:
 * Successfully initialised the GROVE sensor.
 * @return false:
 * Failed to initialise the GROVE sensor.
 */
bool GROVE_Init()
{
    Debug_Start("GROVE_Init");
    if (tcs.begin()) {
        Debug_Information("GROVE", "GROVE_Init", "Sensor initialised");
        Debug_End();
        return true;
    } else {
        Debug_Error("GROVE", "GROVE_Init", "Failed to find a GROVE colour sensor.");
        Debug_End();
        return false;
    }
}

/**
 * @brief This function should use the specified colour
 * sensor pins and return a large number that represents
 * the hex value of the number. Numbers can be see as hex
 * and regular numbers automatically. It should combine
 * R,G,B into one big number.
 * 255,255,255 would be 255255255
 *
 * @return unsigned long of the hex colour returned by the sensor.
 */
unsigned long GROVE_GetColor()
{
    // - VARIABLES - //
    uint16_t clear, red, green, blue;
    //unsigned int colorHex;

    tcs.setInterrupt(false);      // turn on LED

    delay(60);  // takes 50ms to read

    tcs.getRawData(&red, &green, &blue, &clear);

    tcs.setInterrupt(true);  // turn off LED
    if (red>=255)
    {
        red = 255;
    }
    if (green>=255)
    {
        green = 255;
    }
    if (blue>=255)
    {
        blue = 255;
    }
    if (clear>=255)
    {
        clear = 255;
    }
   /* rouge = (int) (((float)red/1024)*255);
    vert =  (int) (((float)green/1024)*255);
    bleu =  (int) (((float)blue/1024)*255);
    clair = (int) (((float)clear/1024)*255);*/

    //Serial.print("C:\t"); Serial.print(clear);
    //Serial.print("\tR:\t"); Serial.print(red);
    //Serial.print("\tG:\t"); Serial.print(green);
    //Serial.print("\tB:\n"); Serial.println(blue);
//
    //Debug_Information("GROVE", "GROVE_GetColor", "Red : " + String(red));
    //Debug_Information("GROVE", "GROVE_GetColor", "Green : " + String(green));
    //Debug_Information("GROVE", "GROVE_GetColor", "Blue : " + String(blue));
    //Debug_Information("GROVE", "GROVE_GetColor", "Clear : " + String(clear));
    return Colour_GetHexFromRGBC(red, green, blue, clear);

}

