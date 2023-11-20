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
MovementVector emptyMovementVector;

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
    emptyMovementVector.distance_cm = 0.0f;
    emptyMovementVector.rotation_rad = 0.0f;
    ResetVectors();
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
    for (int vectorBufferIndex = 0; vectorBufferIndex < VECTOR_BUFFER_SIZE; vectorBufferIndex++)
    {
        if (!(vectorBuffer[vectorBufferIndex].distance_cm == 0.0f && vectorBuffer[vectorBufferIndex].rotation_rad == 0.0f))
        {
            return VECTOR_BUFFER_SIZE - vectorBufferIndex;
        }
    }
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
    for (int vectorBufferIndex = 0; vectorBufferIndex < VECTOR_BUFFER_SIZE; vectorBufferIndex++)
    {
        if (vectorBuffer[vectorBufferIndex].distance_cm == 0.0f && vectorBuffer[vectorBufferIndex].rotation_rad == 0.0f)
        {
            vectorBuffer[vectorBufferIndex].rotation_rad = GetSavedRotation();
            vectorBuffer[vectorBufferIndex].distance_cm = GetSavedDistance();
            return true;
        }
    }
    Debug_Error("Vectors.cpp", "SaveNewVector", "Cannot save; Vector buffer is already full");
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
    for (int vectorBufferIndex = 0; vectorBufferIndex < VECTOR_BUFFER_SIZE; vectorBufferIndex++)
    {
        vectorBuffer[vectorBufferIndex] = emptyMovementVector;
    }
    return true;
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
    for (int vectorBufferIndex = 0; vectorBufferIndex < VECTOR_BUFFER_SIZE; vectorBufferIndex++)
    {
        if ((vectorBuffer[vectorBufferIndex].distance_cm == 0.0f && vectorBuffer[vectorBufferIndex].rotation_rad == 0.0f) || vectorBufferIndex == VECTOR_BUFFER_SIZE)
        {
            if (vectorBufferIndex > 0)
            {
                UpdateSavedRotation(vectorBuffer[vectorBufferIndex - 1].rotation_rad);
                UpdateSavedDistance(vectorBuffer[vectorBufferIndex - 1].distance_cm);

                vectorBuffer[vectorBufferIndex].rotation_rad = emptyMovementVector.rotation_rad;
                vectorBuffer[vectorBufferIndex].distance_cm = emptyMovementVector.distance_cm;

                return true;
            }
            else
            {
                Debug_Error("Vectors.cpp", "RemoveLastVector", "Cannot remove last vector; the buffer is empty.");
                return false;
            }
        }
    }
    Debug_Error("Vectors.cpp", "RemoveLastVector", "VECTOR_BUFFER_SIZE is equal to 0.");
    return false;
}