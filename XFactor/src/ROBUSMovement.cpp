#include "ROBUSMovement.hpp"
#include "math.h"
#define constanteRatio 100

void ROBUSMovement_stop()
{
  MOTOR_SetSpeed(LEFT_MOTOR, 0);
  MOTOR_SetSpeed(RIGHT_MOTOR, 0);
}

// direction : direction which to go (FOWARD or BACKWARD)
// speed_pct : speed as a percentage
// distance_cm : distance in centimeters
void ROBUSMovement_moveStraight(float direction, float speed_pct, float distance_cm)
{
  ENCODER_Reset(LEFT_ENCODER);
  ENCODER_Reset(RIGHT_ENCODER);

  float rotations= distance_cm / CIRCUMFERENCE;

  /**
   * @brief Keeps track of how close we are to reaching the wanted
   * distance. This value will range from 0 to 1.
   */
  float distanceRatio = 0;

  float currentRotations = 0;
  float wantedRotations = rotations*FULL_ROTATIONS_PULSES;

  MOTOR_SetSpeed(LEFT_MOTOR, speed_pct / 100);
  MOTOR_SetSpeed(RIGHT_MOTOR, speed_pct / 100);  

  while (currentRotations <= wantedRotations)
  {
    currentRotations = (float)ENCODER_Read(LEFT_ENCODER);
    distanceRatio = currentRotations / wantedRotations;

    ROBUSMovement_adjustDirection(speed_pct, 50, distanceRatio, wantedRotations);
  }

  ROBUSMovement_stop();
}

void ROBUSMovement_adjustDirection(float speed_pct, int delay_ms, float distanceRatio, float totalDistance)

{

  static int maxOffset = 0;

  int leftEncoderCount= ENCODER_Read(LEFT_ENCODER);

  int rightEncoderCount= ENCODER_Read(RIGHT_ENCODER);

 

  float speedRatio = speed_pct/100;

  float speedFactor = ROBUS_GetSpeedFactorFromCurrentPosition(distanceRatio, totalDistance, speedRatio);

 

  float offset = leftEncoderCount - rightEncoderCount;

  float offsetRatio = offset/CONSTANTE_RATIO;

  //offsetRatio= 1-offsetRatio;

 /* if (abs(offset) > maxOffset)

  {

    maxOffset = abs(offset);

    Serial.println(maxOffset);

  }*/
 

  Serial.println(offsetRatio);

 

  MOTOR_SetSpeed(RIGHT_MOTOR, offsetRatio+speedFactor);

  MOTOR_SetSpeed(LEFT_MOTOR, -offsetRatio+speedFactor);

  /*

  if (abs(leftEncoderCount) > abs(rightEncoderCount))

  {

    MOTOR_SetSpeed(LEFT_MOTOR, speedFactor*0.197);

  }

  else if (abs(leftEncoderCount) < abs(rightEncoderCount))

  {

    MOTOR_SetSpeed(LEFT_MOTOR, speedFactor*0.2015);

  }

  */

 

}



float ROBUSMovement_turnOnSelf_math(float distance_between_wheels, float wheel_diameter, int degrees)
{
  uint32_t arc = distance_between_wheels * degrees * 3200;
  arc = arc / (360*wheel_diameter);
  return arc;
}

float PID(float Kc, float Ti, float set_point, float process_variable, int nbr_cycle, float start_time, float current_time, float Ubias, float* erreur_integrale)
{
  
  float Ki = Kc/Ti;

  float temps_tourner = current_time - start_time;

  float delta_T = (temps_tourner/nbr_cycle)/ 1000; // temps/cycle = delta_T en ms ; /1000 pour secondes

  int32_t erreur = set_point-process_variable;

  Serial.println(erreur);
  *erreur_integrale += (erreur * delta_T);

  float speedLeft = Ubias + Kc*erreur + Ki*(*erreur_integrale);

  return speedLeft;

}

// direction : direction which to go (LEFT or RIGHT) (-1 or 1)
// speed_pct : speed as a percentage, main speed divided by 2
// turn_degrees : turn in degrees
void ROBUSMovement_turnOnSelf(float direction, float speed_pct, int turn_degrees)
{
  Serial.println("Début tourner");
  
  float Kc = 0.0069427506;
  float Ti = 200;  //Baisser pour 20

  ////// ROBOT A ///////
  //////////////////////
  //à modifier selon tests
  float Kd_gauche = 0.93; 
  float Kd_droite = 0.925; 
  float distance_between_wheels = 18.7;
  //////////////////////


  ////// ROBOT B ///////
  //////////////////////
  //à modifier selon tests
  /*float Kd_gauche = 0.933; //0.92323
  float Kd_droite = 0.937; //1.002 //1.077777
  float distance_between_wheels = 18.6;*/

  float Kd = 1; 
  if (direction == 1){
    Kd = Kd_droite;
  }
  else{
    Kd = Kd_gauche;
  }
  float degrees = (float)turn_degrees * Kd;
  float wheel_size = 7.5;

  int wanted_movement = ROBUSMovement_turnOnSelf_math(distance_between_wheels, wheel_size, degrees);

  int cycle = 1;
  float erreur_integrale = 0;
  
  //Serial.print("wanted_movement : ");
  //Serial.println(wanted_movement);
  
  //Reset
  ENCODER_Reset(LEFT_ENCODER);
  ENCODER_Reset(RIGHT_ENCODER);
  erreur_integrale = 0;
  uint32_t total_movement = 0;

  MOTOR_SetSpeed(RIGHT_MOTOR, (-1*direction*speed_pct));
  MOTOR_SetSpeed(LEFT_MOTOR, (direction*speed_pct));

  //Timing
  //Serial.println("Before millis");
  float temps_debut = millis();
  //Serial.println("After millis");

  float temps_derniere_intervalle = 0;
  float intervalle = 50;

  //Serial.print("temps_debut: ");
  //Serial.println(temps_debut);

  int tourne = 1;

  //Serial.println("Début while");

  while(tourne == 1){
    
    //Serial.println("Dans le while");

    uint32_t right = abs(ENCODER_Read(RIGHT_ENCODER));
    uint32_t left  = abs(ENCODER_Read(LEFT_ENCODER));
    ENCODER_Reset(LEFT_ENCODER);
    ENCODER_Reset(RIGHT_ENCODER);
    //uint32_t right = abs(ENCODER_ReadReset(RIGHT_ENCODER));
    //uint32_t left  = abs(ENCODER_ReadReset(LEFT_ENCODER));
    total_movement += right;
    
    //Serial.print("total_movement: ");
    //Serial.println(total_movement);
    
    float temps_actuel = millis();
    if ((temps_actuel - temps_derniere_intervalle) > intervalle){
      float nouvelle_vitesse = direction * PID(Kc, Ti, right, left, cycle, temps_debut, temps_actuel, speed_pct, &erreur_integrale);
      if (!(nouvelle_vitesse < -0.4) && !(nouvelle_vitesse > 0.4)){
        MOTOR_SetSpeed(LEFT_MOTOR, nouvelle_vitesse);
      }
      else {
        Serial.println("Error : too fast >:(");
      }
      temps_derniere_intervalle = 0;

      Serial.print("nouvelle vitesse: ");
      Serial.println(nouvelle_vitesse);
    } 

    if (total_movement >= wanted_movement){
      ROBUSMovement_stop();
      tourne = 0;
      //Serial.println("Fin while");
      //if (wanted_movement > right){

      //}
    }
  }
}