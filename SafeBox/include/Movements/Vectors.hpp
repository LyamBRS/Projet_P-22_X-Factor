/**
 * @file Vectors.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains all the
 * functions used to handle the vector
 * buffers and savings for backtracking
 * to the box.
 * @version 0.1
 * @date 2023-10-23
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once

#pragma region -Vector Handling-
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
 * @return true 
 * The vector was successfully saved and
 * global variables were successfully
 * reset back to 0.
 * @return false
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
 * @return true 
 * The vectors were successfully reset.
 * @return false 
 * The vectors failed to be reset.
 */
bool ResetVectors();
#pragma endregion