/**
 * @file Accelerometer.cpp
 * @author Mohamed El Hadi
 * @brief
 * File containing functions
 * used to make a basic accelerometer work.
 * @version 1
 * @date 2023-11-16
 * @copyright Copyright (c) 2023
 */
#include "Sensors/Accelerometer/Accelerometer.hpp"

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
bool Accelerometer_Init()
{
    Wire.begin(); // Initialize communication
    Wire.setClock(MPU6050_CLOCK_SPEED);
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW); // Start communication with MPU6050_ADDRESS_AD0_LOW=0x68
    Wire.write(PWR_MGMT_1);                          // Talk to the power management
    Wire.write(0x0);                                 // Make a reset
    Wire.endTransmission(true);                      // end the transmission

#ifdef SENSOR_CALIBRATE
    Accelerometer_CalculateIMUError(10000);
    delay(100);
#endif

    return true;
}

/**
 * @brief
 * Function that returns the value of an axis
 * This is a backend function, do not use outside
 * of Accelerometer functions.
 * @param axe
 * Which axe to read from.
 * @return float
 */
float Accelerometer_GetData(axes axe)
{
    // === Read acceleromter data === //
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
    Wire.write(ACCELEROMETER_XOUT_H); // Start with register 0x3B (ACCELEROMETER_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDRESS_AD0_LOW, MPU6050_DATA_SIZE, true);

    // For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
    float AccX = (Wire.read() << 8 | Wire.read()) / ACCELEROMETER_SENSITIVITY; // X-axis value
    float AccY = (Wire.read() << 8 | Wire.read()) / ACCELEROMETER_SENSITIVITY; // Y-axis value
    float AccZ = (Wire.read() << 8 | Wire.read()) / ACCELEROMETER_SENSITIVITY; // Z-axis value

    switch (axe)
    {
    case x_axis:
        return AccX;
    case y_axis:
        return AccY;
    case z_axis:
        return AccZ;
    default:
        return 0.0f; // pseudo-error code
    }
}

/**
 * @brief
 * Function that returns the X value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetX()
{
    return Accelerometer_GetData(x_axis);
}

/**
 * @brief
 * Function that returns the Y value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetY()
{
    return Accelerometer_GetData(y_axis);
}

/**
 * @brief
 * Function that returns the Y value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetZ()
{
    return Accelerometer_GetData(z_axis);
}

/**
 * @brief
 * Function that returns the Compass value of the
 * accelerometer using functions from an arduino
 * library. The value is raw and not converted.
 * @return float:
 * If 0 is returned, the function failed to
 * execute properly.
 */
float Accelerometer_GetCompass()
{
    return 0.0f;
}






/**
 * @brief
 * Sets the axis scales of the accelerometer.
 * DO NOT USE OUTSIDE OF ACCELEROMETER.CPP
 * @param scale
 */
void Accelerometer_SetScale(accelerometer_scale scale)
{
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
    Wire.write(ACCELEROMETER_CONFIG);
    Wire.write(scale);
    Wire.endTransmission(true);
}

/**
 * @brief
 * Sets the axis scales of the gyro.
 * DO NOT USE OUTSIDE OF ACCELEROMETER.CPP
 * @param scale
 */
void Accelerometer_SetGyroScale(gyro_scale scale)
{
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
    Wire.write(GYRO_CONFIG);
    Wire.write(scale);
    Wire.endTransmission(true);
}

// // Offset calibration function
// // CAUSTION: place the IMU flat in order to get the proper values
void Accelerometer_CalculateIMUError(unsigned correctionCount)
{
    float AccX = 0.0f, AccY = 0.0f, AccZ = 0.0f;
    float GyroX = 0.0f, GyroY = 0.0f, GyroZ = 0.0f;
    // float accAngleX = 0.0f, accAngleY = 0.0f;
    // float gyroAngleX = 0.0f, gyroAngleY = 0.0f, gyroAngleZ = 0.0f;
    // float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;
    float AccErrorX = 0.0f, AccErrorY = 0.0f;
    float GyroErrorX = 0.0f, GyroErrorY = 0.0f, GyroErrorZ = 0.0f;

    /*TODO: put the correct type here time_t */
    // float elapsedTime = 0.0f, currentTime = 0.0f, previousTime = 0.0f;
    // Read accelerometer values correctionCount times
    for (size_t i = 0; i < correctionCount; i++)
    {
        Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
        Wire.write(ACCELEROMETER_XOUT_H);
        Wire.endTransmission(false);
        Wire.requestFrom(MPU6050_ADDRESS_AD0_LOW, MPU6050_DATA_SIZE, true);
        AccX = (Wire.read() << 8 | Wire.read()) / ACCELEROMETER_SENSITIVITY;
        AccY = (Wire.read() << 8 | Wire.read()) / ACCELEROMETER_SENSITIVITY;
        AccZ = (Wire.read() << 8 | Wire.read()) / ACCELEROMETER_SENSITIVITY;
        // Sum all readings
        AccErrorX += ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI)); // TODO: add the reference
        AccErrorY += ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    }

    // Divide the sum by correctionCount to get the error value
    AccErrorX /= correctionCount;
    AccErrorY /= correctionCount;

    // Read gyro values correctionCount times
    for (size_t j = 0; j < correctionCount; j++)
    {
        Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
        Wire.write(GYRO_XOUT_H);
        Wire.endTransmission(false);
        Wire.requestFrom(MPU6050_ADDRESS_AD0_LOW, MPU6050_DATA_SIZE, true);
        GyroX = (Wire.read() << 8 | Wire.read()) / GYROSCOPE_SENSITIVITY;
        GyroY = (Wire.read() << 8 | Wire.read()) / GYROSCOPE_SENSITIVITY;
        GyroZ = (Wire.read() << 8 | Wire.read()) / GYROSCOPE_SENSITIVITY;
        // Sum all readings
        GyroErrorX += GyroX;
        GyroErrorY += GyroY;
        GyroErrorZ += GyroZ;
    }

    // Divide the sum by correctionCount to get the error value
    GyroErrorX /= correctionCount;
    GyroErrorY /= correctionCount;
    GyroErrorZ /= correctionCount;

    // Print the error values on the Serial Monitor
    //Serial.print("AccErrorX: ");
    //Serial.println(AccErrorX);
    //Serial.print("AccErrorY: ");
    //Serial.println(AccErrorY);
    //Serial.print("GyroErrorX: ");
    //Serial.println(GyroErrorX);
    //Serial.print("GyroErrorY: ");
    //Serial.println(GyroErrorY);
    //Serial.print("GyroErrorZ: ");
    //Serial.println(GyroErrorZ);
}
