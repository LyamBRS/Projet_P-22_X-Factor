/**
 * @file Vectors.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains all the
 * functions used to handle the vector
 * buffers and savings for backtracking
 * to the box.
 * @version 0.1
 * @date 2023-10-23
 * @copyright Copyright (c) 2023
 */
#pragma once

#include "Movements/Positions.hpp"
#include "Movements/VectorDefines.hpp"
#include "Movements/Distances.hpp"
#include "Sensors/Distance/GP2D12.hpp"
#include "Debug/Debug.hpp"
#include "math.h"

#define VECTOR_BUFFER_SIZE 32 // MAY NEED TO CHANGE

// #pragma region -Vector Handling-

/**
 * @brief Fills up
 * the vector buffer with
 * "empty" values
 * @return bool
 * true if it has worked, false if
 * a problem has occurred
 */
bool Vectors_Init();

/**
 * @brief Returns how many
 * vectors can still be saved.
 * In other words, it tells you
 * how much space is left in
 * the vector buffer.
 * @return int:
 * How many vectors are left.
 * If 0, the robot can no longer
 * keep track of where it will be
 * after it moved.
 */
int GetAvailableVectors();

/**
 * @brief Function that uses global variables
 * to save the current rotation and distance
 * as a new vector in the vector buffer.
 *
 * If the robot's current rotation remains
 * the same as the previous vector, the
 * previous vector will be updated instead
 * of saving a new one.
 * @return true:
 * The vector was successfully saved and
 * global variables were successfully
 * reset back to 0.
 * @return false:
 * Something went wrong and the function
 * could not save the global variables in
 * the vector buffer.
 */
bool SaveNewVector();

/**
 * @brief Resets the vector buffer back
 * to default values (0). Also resets
 * global variables that keep tracks
 * of the available vectors.
 * @return true:
 * The vectors were successfully reset.
 * @return false:
 * The vectors failed to be reset.
 */
bool ResetVectors();

/**
 * @brief Resets the last vector stored in the
 * vector buffers back to default values (0).
 * Also reduces global variables that keep tracks
 * of the available vectors left.
 * @return true:
 * The vector was successfully removed.
 * @return false:
 * The vector failed to be removed.
 */
bool RemoveLastVector();

/**
 * @brief Calculates and return the 
 * vector needed to go back to the garage door.
 * If needed for further uses, we might add
 * a parameter that contains the reference
 * point (the 0,0)
 * @return MovementVector:
 * The calculated vector, equal to empty if 
 * a problem has occurred.
 */
MovementVector GetReturnVector();

/**
 * @brief Calculates and return the 
 * opposite vector to the last non
 * empty vector in the vector buffer.
 * @return MovementVector:
 * The calculated vector, equal to empty (0.0f, 0.0f) if 
 * a problem has occurred.
 */
MovementVector GetLastOppositeVector();

/**
 * @brief Calculates and return the 
 * opposite vector to the 
 * provided MovementVector
 * @return MovementVector:
 * The calculated vector, should
 * not get errors in this
 */
MovementVector GetOppositeVector(MovementVector movementVector);
// #pragma endregion