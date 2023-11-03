
/**
 * @file InfraRed.cpp
 * @author Élodie
 * @brief
 * File containing the functions used to make an
 * IR LED work.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Arduino.h"
#include "Sensors/Proximity/InfraRed.hpp"

/**
 * @brief Initialize an infra red sensor
 *
 * @param pin
 * the pin to initialize
 */
void IR_Innit(int pin){
    pinMode(pin, INPUT);
}

/**
 * @brief Reads the infra red sensor
 *
 * @param pin
 * the infra red's pin to read from
 * @return true
 * returned when something is detected
 * @return false
 * returned when nothing is detected
 */
bool IR_Read(int pin){
    return !digitalRead(pin);
}