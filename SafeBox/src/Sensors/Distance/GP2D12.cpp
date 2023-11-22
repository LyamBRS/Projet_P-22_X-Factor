/**
 * @file GP2D12.hpp
 * @author Trevis ()
 * @brief File for GP2D12 distance sensor
 * @version 0.1
 * @date 2023-10-19
 * @copyright Copyright (c) 2023
 */
#include "Sensors/Distance/GP2D12.hpp"

<<<<<<< HEAD
#include "Sensors/Distance/GP2D12.hpp"

// const int trigPin = 9;
// const int echoPin = 10;

// float duration, distance;

// void setup() {
  // pinMode(trigPin, OUTPUT);
  // pinMode(echoPin, INPUT);
  // Serial.begin(9600);
// }

// void loop() {
  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);

  // duration = pulseIn(echoPin, HIGH);
  // distance = (duration*.0343)/2;
  // Serial.print("Distance: ");
  // Serial.println(distance);
  // delay(100);
// }
=======
/**
 * @brief Sets base values
 *
 * @param trigPin, echoPin
 * pin to set trigger pin & echo pin numbers
 */
void GP2D12_Init(int trigPin, int echoPin){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //Serial.begin(9600);
}

/**
 * @brief Reads the IR sensor at specified pin
 * @param trigPin, echoPin
 * pin to read (between 0 and 3)
 * @return unsigned short
 * raw data (16 bits)
 */
unsigned short GP2D12_Read(int trigPin, int echoPin){

  // - VARIABLES - //
  long duration = 0;
  unsigned short cm = 0;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  cm = (duration*.0343)/2;
  //Serial.print("Distance: ");
  //Serial.println(cm);

  return cm;
}
>>>>>>> e9c540f33225ec30cfc4a7f8aed5568c826e1f3a
