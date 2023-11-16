#include "Sensors/Accelerometer/Accelerometer.hpp"

void set_accelometer_scale(accelometer_scale scale)
{
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
    Wire.write(ACCEL_CONFIG);
    Wire.write(scale);
    Wire.endTransmission(true);
}

void set_gyro_scale(gyro_scale scale)
{
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
    Wire.write(GYRO_CONFIG);
    Wire.write(scale);
    Wire.endTransmission(true);
}

// // Offset calibration function
// // CAUSTION: place the IMU flat in order to get the proper values
void calculate_IMU_error(unsigned correctionCount)
{
    float AccX = 0.0f, AccY = 0.0f, AccZ = 0.0f;
    float GyroX = 0.0f, GyroY = 0.0f, GyroZ = 0.0f;
    float accAngleX = 0.0f, accAngleY = 0.0f;
    float gyroAngleX = 0.0f, gyroAngleY = 0.0f, gyroAngleZ = 0.0f;
    float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;
    float AccErrorX = 0.0f, AccErrorY = 0.0f;
    float GyroErrorX = 0.0f, GyroErrorY = 0.0f, GyroErrorZ = 0.0f;

    /*TODO: put the correct type here time_t */
    float elapsedTime = 0.0f, currentTime = 0.0f, previousTime = 0.0f;
    // Read accelerometer values correctionCount times
    for (size_t i = 0; i < correctionCount; i++)
    {
        Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
        Wire.write(ACCEL_XOUT_H);
        Wire.endTransmission(false);
        Wire.requestFrom(MPU6050_ADDRESS_AD0_LOW, DATA_SIZE, true);
        AccX = (Wire.read() << 8 | Wire.read()) / ACCELOMETER_SENSIVITY;
        AccY = (Wire.read() << 8 | Wire.read()) / ACCELOMETER_SENSIVITY;
        AccZ = (Wire.read() << 8 | Wire.read()) / ACCELOMETER_SENSIVITY;
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
        Wire.requestFrom(MPU6050_ADDRESS_AD0_LOW, DATA_SIZE, true);
        GyroX = (Wire.read() << 8 | Wire.read()) / GYROSCOPE_SENSIVITY;
        GyroY = (Wire.read() << 8 | Wire.read()) / GYROSCOPE_SENSIVITY;
        GyroZ = (Wire.read() << 8 | Wire.read()) / GYROSCOPE_SENSIVITY;
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
    Serial.print("AccErrorX: ");
    Serial.println(AccErrorX);
    Serial.print("AccErrorY: ");
    Serial.println(AccErrorY);
    Serial.print("GyroErrorX: ");
    Serial.println(GyroErrorX);
    Serial.print("GyroErrorY: ");
    Serial.println(GyroErrorY);
    Serial.print("GyroErrorZ: ");
    Serial.println(GyroErrorZ);
}

bool Accelerometer_Init()
{
    Serial.begin(BAUD_RATE);
    if (!Serial)
        return false;

    Wire.begin(); // Initialize comunication
    Wire.setClock(CLOCK_SPEED);
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW); // Start communication with MPU6050_ADDRESS_AD0_LOW=0x68
    Wire.write(PWR_MGMT_1);                          // Talk to the power management
    Wire.write(0x0);                                 // Make a reset
    Wire.endTransmission(true);                      // end the transmission

#ifdef SENSOR_CALIBRATE
    calculate_IMU_error(10000);
    delay(100);
#endif

    return true;
}

float getAccelometer_data(axes axe)
{
    // === Read acceleromter data === //
    Wire.beginTransmission(MPU6050_ADDRESS_AD0_LOW);
    Wire.write(ACCEL_XOUT_H); // Start with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDRESS_AD0_LOW, DATA_SIZE, true);

    // For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
    float AccX = (Wire.read() << 8 | Wire.read()) / ACCELOMETER_SENSIVITY; // X-axis value
    float AccY = (Wire.read() << 8 | Wire.read()) / ACCELOMETER_SENSIVITY; // Y-axis value
    float AccZ = (Wire.read() << 8 | Wire.read()) / ACCELOMETER_SENSIVITY; // Z-axis value

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

float Accelerometer_GetX()
{
    return getAccelometer_data(x_axis);
}

float Accelerometer_GetY()
{
    return getAccelometer_data(y_axis);
}

float Accelerometer_GetZ()
{
    return getAccelometer_data(z_axis);
}

float Accelerometer_GetCompass()
{
    return 0.0f;
}