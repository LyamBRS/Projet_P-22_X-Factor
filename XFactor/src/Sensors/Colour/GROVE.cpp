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



byte gammatable[256];


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
/**
 * @brief Initialises one GROVE sensor depending on given
 * pins.
 * @param redPin Arduino pins for the red input
 * @param greenPin Arduino pins for the green input
 * @param bluePin Arduino pins for the blue input
 */
void GROVE_Init()
{
    Serial.begin(9600);
    Serial.println("Color View Test!");

    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }
}

/*void ColorDetector_detectColor() {
    uint16_t clear, red, green, blue;
    unsigned long clair=0;
    unsigned long rouge=0;
    unsigned long vert=0;
    unsigned long bleu=0;

    tcs.setInterrupt(false);      // turn on LED

    delay(60);  // takes 50ms to read

    tcs.getRawData(&red, &green, &blue, &clear);

    tcs.setInterrupt(true);  // turn off LED

    Serial.print("C1:\t"); Serial.print(clear);
    Serial.print("\tR1:\t"); Serial.print(red);
    Serial.print("\tG1:\t"); Serial.print(green);
    Serial.print("\tB1:\t"); Serial.println(blue);

    rouge = (unsigned long) (((float)red/1024)*255);
    vert = (unsigned long) (((float)green/1024)*255);
    bleu = (unsigned long)(((float)blue/1024)*255);
    clair = (unsigned long)(((float)clear/1024)*255);

    Serial.print("C:\t"); Serial.print(clear);
    Serial.print("\tR:\t"); Serial.print(red);
    Serial.print("\tG:\t"); Serial.print(green);
    Serial.print("\tB:\t"); Serial.println(blue);
}*/

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
unsigned int GROVE_GetColor()
{
    uint16_t clear, red, green, blue;
    //unsigned int colorHex;
    unsigned long clair=0;
    unsigned long rouge=0;
    unsigned long vert=0;
    unsigned long bleu=0;

    tcs.setInterrupt(false);      // turn on LED

    delay(60);  // takes 50ms to read

    tcs.getRawData(&red, &green, &blue, &clear);

    tcs.setInterrupt(true);  // turn off LED

    rouge = (unsigned long) (((float)red/1024)*255);
    vert = (unsigned long) (((float)green/1024)*255);
    bleu = (unsigned long)(((float)blue/1024)*255);
    clair = (unsigned long)(((float)clear/1024)*255);

    Serial.print("C:\t"); Serial.print(clair);
    Serial.print("\tR:\t"); Serial.print(rouge);
    Serial.print("\tG:\t"); Serial.print(vert);
    Serial.print("\tB:\n"); Serial.println(bleu);

    unsigned long resultedColor = 0;
    unsigned long temporaryLargeValue = 0;

    //lowest value is the clear value, it thus does not need to be shifted.
    resultedColor = resultedColor + clair;
    Serial.print("convertedColorClear:\t"); Serial.print(resultedColor);
    //third value is the blue value; it needs to be shifted by 8 bits, We thus need to use a new type to hold

    temporaryLargeValue = bleu;
    temporaryLargeValue = temporaryLargeValue << 8;
    resultedColor = resultedColor + temporaryLargeValue;
    Serial.print("\nconvertedColorClear+Blue\t"); Serial.print(resultedColor);

    //second value is the green value, it needs to be shifted by 16 bits.
    temporaryLargeValue = vert;
    temporaryLargeValue = temporaryLargeValue << 16;
    resultedColor = resultedColor + temporaryLargeValue;
    Serial.print("\nconvertedColorClear+Blue+Green:\t"); Serial.print(resultedColor);

    //first value is the red value; it needs to be shifted by 24 bits.

    temporaryLargeValue = rouge;
    temporaryLargeValue = temporaryLargeValue << 24;
    resultedColor = resultedColor + temporaryLargeValue;
    Serial.print("\nconvertedColorClear+RGB:\t"); Serial.print(resultedColor);

    return 0;

}
/*void getRGB(float *r, float *g, float *b) {
  uint16_t red, green, blue, clear;
  tcs.getRawData(&red, &green, &blue, &clear);
  uint32_t sum = clear;

  // Avoid divide by zero errors ... if clear = 0 return black
  if (clear == 0) {
    *r = *g = *b = 0;
    return;
  }

  *r = (float)red / sum * 255.0;
  *g = (float)green / sum * 255.0;
  *b = (float)blue / sum * 255.0;
}*/

