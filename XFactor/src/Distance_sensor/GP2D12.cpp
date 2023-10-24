/**
 * @file GP2D12.cpp
 * @author William ()
 * @brief Source file for GP2D12 distance sensor
 * @version 0.1
 * @date 2023-10-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Distance_sensor/GP2D12.hpp"

/**
 * @brief Sets base values
 * 
 * @param pinNumber 
 * pin to set (between 0 and 3) (J9-J12)
 */
void GP2D12_Init(int pinNumber)
{
    //s'il y a des choses à initialiser comme des variables globales pour le capteur, faire ici
}

/**
 * @brief Reads the IR sensor at specified pin
 * 
 * @param pinNumber 
 * pin to read (between 0 and 3)
 * @return float
 * distance to object (in centimeters)
 */
float GP2D12_Read(int pinNumber)
{
    return 0.0f;
    // pinNumber devrait être un #define et on veut simplement caller la fonction de LibRobus (voir wiki librobus/documentation matérielle)
    // voir https://github.com/UdeS-GRO/LibRobus/wiki/Documentation-mat%C3%A9rielle#capteur-de-distance-gp2d12-et-gp2y0a21yk0f
}