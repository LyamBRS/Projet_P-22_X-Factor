/**
 * @file Vectors.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains all the
 * functions used to handle the vector
 * buffers and savings for backtracking
 * to the box.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Movements/Vectors.hpp"

MovementVector vectorBuffer[VECTOR_BUFFER_SIZE];

/**
 * @brief Fills up
 * the vector buffer with
 * "empty" values
 * @return bool
 * true if it has worked, false if
 * a problem has occurred
 */
bool Vectors_Init()
{
    MovementVector emptyMovementVector;
    emptyMovementVector.distance_cm = 0.0f;
    emptyMovementVector.rotation_rad = 0.0f;

    for (int i = 0; i < VECTOR_BUFFER_SIZE; i++)
    {
        vectorBuffer[i] = emptyMovementVector;
    }
    return true;
}

/**
 * @brief Returns how many
 * vectors can still be saved.
 * In other words, it tells you
 * how much space is left in
 * the vector buffer.
 * @return int
 * How many vectors are left.
 * If 0, the robot can no longer
 * keep track of where it will be
 * after it moved.
 */
int GetAvailableVectors()
{
    return 0;
}

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
bool SaveNewVector()
{
    return false;
}

/**
 * @brief Resets the vector buffer back
 * to default values (0). Also resets
 * global variables that keep tracks
 * of the available vectors.
 * @return true
 * The vectors were successfully reset.
 * @return false
 * The vectors failed to be reset.
 */
bool ResetVectors()
{
    return false;
}

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
bool RemoveLastVector()
{
    return false;
}