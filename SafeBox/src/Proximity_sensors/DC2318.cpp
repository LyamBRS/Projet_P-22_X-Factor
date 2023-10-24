/**
 * @file DC2318.cpp
 * @author Élodie
 * @brief 
 * Code file for functions associated with the DC2318 sensor
 * @version 0.1
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "Proximity_sensors/InfraRed.hpp"
#include "Proximity_sensors/DC2318.hpp"


/**
 * @brief Function that initialize a DC2318 sensor
 * 
 * @param leftPin 
 * the pin used for the left IR sensor
 * @param rightPin 
 * the pin used for the right IR sensor
 */
void DC2318_Innit(int leftPin, int rightPin){
    IR_Innit(leftPin);
    IR_Innit(rightPin);
}

/**
 * @brief Function that read a DC2318 sensor
 * 
 * @param leftPin 
 * the pin used for the left IR sensor
 * @param rightPin 
 * the pin used for the right IR sensor
 * @return unsigned char 
 * Value returned depending on the state of the IR sensor.
 * 0: no walls
 * 1: A wall on the right is detected
 * 2: A wall on the left is detected
 * 3: A wall straight ahead is detected
 * 4: An error occured with the sensor
 */
unsigned char DC2318_Read(int leftPin, int rightPin){
    bool leftState;
    bool rightState;
    leftState = IR_Read(leftPin);
    rightState = IR_Read(rightPin);

    if (leftState == 0 && rightState == 0) {
        return DC2318_NO_WALL;
    }
    if (leftState == 0 && rightState == 1) {
        return DC2318_LEFT_WALL;
    }
    if (leftState == 1 && rightState == 0) {
        return DC2318_RIGHT_WALL;
    }
    if (leftState == 1 && rightState == 1) {
        return DC2318_FRONT_WALL;
    }
    return DC2318_DETECTION_ERROR;
}
