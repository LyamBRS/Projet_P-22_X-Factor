/**
 * @file GP2D12.hpp
 * @author William ()
 * @brief Header file for GP2D12 distance sensor
 * @version 0.1
 * @date 2023-10-19
 * @copyright Copyright (c) 2023
 */

#pragma once

#include "Arduino.h"

#define FRONT_SENSOR_TRIG_PIN_NUMBER 46
#define FRONT_SENSOR_ECHO_PIN_NUMBER 47

#define LEFT_SENSOR_TRIG_PIN_NUMBER 10
#define LEFT_SENSOR_ECHO_PIN_NUMBER 11

#define RIGHT_SENSOR_TRIG_PIN_NUMBER 44
#define RIGHT_SENSOR_ECHO_PIN_NUMBER 45

/**
 * @brief Sets base values
 *
 * @param trigPin, echoPin
 * pin to set trigger pin & echo pin numbers
 */
void GP2D12_Init(int trigPin, int echoPin);


/**
 * @brief Reads the IR sensor at specified pin
 * @param trigPin, echoPin
 * pin to read (between 0 and 3)
 * @return unsigned short
 * raw data (16 bits)
 */
unsigned short GP2D12_Read(int trigPin, int echoPin);