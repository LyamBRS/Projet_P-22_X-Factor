/**
 * @file InfraRed.hpp
 * @author Élodie (you@domain.com)
 * @brief Header file used for generic low level functions that reads and utilizes infra red sensors
 * @version 0.1
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

/**
 * @brief Initialize an infra red sensor
 * 
 * @param pin 
 * the pin to initialize
 */
void IR_Innit(int pin);

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
bool IR_Read(int pin);