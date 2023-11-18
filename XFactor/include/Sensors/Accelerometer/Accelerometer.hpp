/**
 * @file Accelerometer.hpp
 * @author Mohamed El Hadi
 * @brief
 * File containing header definition of functions
 * used to make a basic accelerometer work.
 * @version 1
 * @date 2023-11-16
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include <Wire.h>
#include <Arduino.h>

// - DEFINES - //
//#define BAUD_RATE 9600
#define MPU6050_CLOCK_SPEED 400000           // 400kHz I2C clock
#define MPU6050_ADDRESS_AD0_LOW 0x68 // MPU6050 low I2c address
#define ACCELEROMETER_XOUT_H 0x3B            // Accelerometer first high register
#define GYRO_XOUT_H 0x43             // Gyroscope first high register
#define ACCELEROMETER_CONFIG 0x1C            // Accelerometer configuration register
#define GYRO_CONFIG 0x1B             // Gyroscope configuration register
#define PWR_MGMT_1 0X6B              // Power management 1 register

// TODO: make this as a function of the scale, use AFS_SEL registers
#define ACCELEROMETER_SENSITIVITY 16384.0f //  in deg/s for a scale of +- 2g, see the data sheet
#define GYROSCOPE_SENSITIVITY 131.0f     //  in g for scale of +- 250 deg/s, see the data sheet
#define MPU6050_DATA_SIZE 6                    // Read 6 values in total, each axis value is stored in 2 registers

// uncomment this to calibrate the sensor
// #define SENSOR_CALIBRATE 1

/**
 * @brief enum containing all the possible scales
 * that the accelerometer can have. Used to send
 * on Wire (i2c/spi)
 *
 * @warning
 * DO NOT USE OUTSIDE OF ACCELEROMETER.CPP
 */
typedef enum
{
    TWO_G = 0x00, // +- 2g full scale range
    FOUR_G = 0x01,
    EIGHT_G = 0x10,
    SIXTEEN_G = 0x11
} accelerometer_scale;

/**
 * @brief enum containing all the possible scales
 * that the gyro can have. Used to send
 * on Wire (i2c/spi)
 *
 * @warning
 * DO NOT USE OUTSIDE OF ACCELEROMETER.CPP
 */
typedef enum
{
    TWO_FIFTY_DEG_PER_SEC = 0x00, // +- 250 deg/sec full scale
    FIVE_HUNDRED_DEG_PER_SEC = 0x1,
    THOUSAND_DEG_PER_SEC = 0x10,
    TWO_THOUSAND_DEG_PER_SEC = 0x11
} gyro_scale;

/**
 * @brief enum containing all the axes of the
 * accelerometer. Used on Wire (i2c/spi)
 *
 * @warning
 * DO NOT USE OUTSIDE OF ACCELEROMETER.CPP
 */
typedef enum
{
    x_axis = 0,
    y_axis,
    z_axis
} axes;

/**
 * @brief
 * Initialises an accelerometer to be used on
 * XFactor. The name of the accelerometer will
 * define how these functions are called.
 * @return true:
 * Successfully initialised the accelerometer
 * using the specifed pins.
 * @return false:
 * Failed to initialise the accelerometer.
 */
bool Accelerometer_Init();

/**
 * @brief
 * Function that returns the X value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetX();

/**
 * @brief
 * Function that returns the Y value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * The pin of the accelerometer that was
 * initialised
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetY();

/**
 * @brief
 * Function that returns the Z value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * The pin of the accelerometer that was
 * initialised
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetZ();

/**
 * @brief
 * Function that returns the compass value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * The pin of the accelerometer that was
 * initialised
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetCompass();

// Offset calibration function
// CAUTION: place the IMU flat in order to get the proper values
void Accelerometer_CalculateIMUError(unsigned correctionCount);

/**
 * @brief
 * Sets the axis scales of the accelerometer.
 * DO NOT USE OUTSIDE OF ACCELEROMETER.CPP
 *
 * Scales are defined in the data sheet
 * @param scale
 */
void Accelerometer_SetScale(accelerometer_scale scale);

/**
 * @brief
 * Sets the axis scales of the gyro.
 * DO NOT USE OUTSIDE OF ACCELEROMETER.CPP
 *
 * Scales are defined in the data sheet
 * @param scale
 */
void Accelerometer_SetGyro(gyro_scale scale);
