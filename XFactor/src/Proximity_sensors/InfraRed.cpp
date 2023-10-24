#include "Arduino.h"
#include "Proximity_sensors/InfraRed.hpp"

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