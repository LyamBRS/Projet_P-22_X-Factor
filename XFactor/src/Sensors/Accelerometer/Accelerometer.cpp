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
    // TODO: check error codes here
    Serial.begin(MPU6050_BAUD_RATE);
    Wire.begin(); // Initialize communication
    Wire.setClock(MPU6050_CLOCK_SPEED);
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW); // Start communication with MPU6050_ADDRESS_AD0_LOW=0x68
    Wire.write(PWR_MGMT_1);                          // Talk to the power management
    Wire.write(0x0);                                 // Make a reset
    uint8_t error =  Wire.endTransmission(true);     // end the transmission
    if (error == 0)
        return true;
    return false;
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
    float AcceX = Accelerometer_GetData(x_axis);

// TODO: this one is not so abvious, as the calibration should be written directly to the EEPROM for efficient use, mais peut etre apres ?
#ifdef SENSOR_CALIBRATE
    Accelerometer_calibration calibration;
    Accelerometer_linearCalibration(&calibration, 5000, x_axis);
    float AcceXcalibrated = AcceX - ((calibration.slope * AcceX) + calibration.yIntercept);
    return AcceXcalibrated;
#endif
    return AcceX;
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
    float AcceY = Accelerometer_GetData(y_axis);

#ifdef SENSOR_CALIBRATE
    Accelerometer_calibration calibration;
    Accelerometer_linearCalibration(&calibration, 5000, y_axis);
    float AcceYcalibrated = AcceY - ((calibration.slope * AcceY) + calibration.yIntercept);
    return AcceYcalibrated;
#endif
    return AcceY;
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
    float AcceZ = Accelerometer_GetData(z_axis);

#ifdef SENSOR_CALIBRATE
    Accelerometer_calibration calibration;
    Accelerometer_linearCalibration(&calibration, 5000, z_axis);
    float AcceZcalibrated = AcceZ - ((calibration.slope * AcceZ) + calibration.yIntercept);
    return AcceZcalibrated;
#endif

    return AcceZ;
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
 * Sets the sensor scales of the accelerometer.
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
 * Sets the sensor scales of the gyro.
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

// TODO: debug this function
void Accelerometer_linearCalibration(Accelerometer_calibration *calibration, unsigned calibrationTime, axes axe)
{
    if (!calibration || calibrationTime <= 0)
        return;

    unsigned nbPoints = 0, xSum = 0, ySum = 0, xSquareSum = 0, xTimesYSum = 0, offset = 0;
    unsigned now = 0, endCalibration = 0;

    Serial.println("Orient the sensor upwards against gravity - Click Enter When Ready");
    while (Serial.available() && Serial.read())
        ; // empty buffer
    while (!Serial.available())
        ; // wait for data
    while (Serial.available() && Serial.read())
        ; // empty buffer again

    Serial.print("Beginning to Calibrate Part 1 (Acceleration = 1g) for ");
    Serial.print(calibrationTime * 0.001);
    Serial.println(" seconds");
    now = millis();
    endCalibration = now + calibrationTime;

    while (now < endCalibration)
    {
        nbPoints += 1;
        offset = Accelerometer_GetData(axe) - 1;
        xSum += 1;
        ySum += offset;
        xSquareSum += 1;
        xTimesYSum += offset;
        now = millis();
    }

    Serial.println("Orient the sensor downwards against gravity - Click Enter When Ready");
    while (Serial.available() && Serial.read())
        ; // empty buffer
    while (!Serial.available())
        ; // wait for data
    while (Serial.available() && Serial.read())
        ; // empty buffer again

    // Gravity should be -1g in this scenerio
    Serial.print("Beginning to Calibrate Part 2 (Acceleration = -1g) for ");
    Serial.print(calibrationTime * 0.001);
    Serial.println(" seconds");
    now = millis();
    endCalibration = now + calibrationTime;
    while (now < endCalibration)
    {
        nbPoints += 1;
        offset = Accelerometer_GetData(axe) + 1;
        xSum += -1;
        ySum += offset;
        xSquareSum += 1;
        xTimesYSum += -offset;
        now = millis();
    }

    Serial.println("Orient the sensor perpendicular against gravity - Click Enter When Ready");
    while (Serial.available() && Serial.read())
        ; // empty buffer
    while (!Serial.available())
        ; // wait for data
    while (Serial.available() && Serial.read())
        ; // empty buffer again
    Serial.print("Beginning to Calibrate Part 3 (Acceleration = 0g) for ");
    Serial.print(calibrationTime * 0.001);
    Serial.print("seconds");
    now = millis();
    endCalibration = now + calibrationTime;
    //     Just showing the zero for consistency purposes
    while (now < endCalibration)
    {
        nbPoints += 1;
        offset = Accelerometer_GetData(axe) + 0;
        xSum += 0;
        ySum += offset;
        xSquareSum += 0;
        xTimesYSum += (0 * offset);
        now = millis();
    }

    // linear regression parameters formula
    calibration->slope = ((nbPoints * xTimesYSum) - (xSum * ySum)) / ((nbPoints * xSquareSum) - pow(xSum, 2));
    calibration->yIntercept = (ySum - (calibration->slope * xSum)) / nbPoints;
}


