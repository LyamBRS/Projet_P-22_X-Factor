/**
 * @file CommunicationTest.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the definition of functions
 * and defines used to linearly test
 * communications between SafeBox and XFactor
 * @version 0.1
 * @date 2023-11-16
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include "SafeBox/Status.hpp"
#include "SafeBox/Communication.hpp"
#include "XFactor/Status.hpp"

// - DEFINES - //

// - FUNCTIONS - //

void ContinuouslyAnswerXFactor();
/**
 * @brief 
 * Call this in main.cpp to test communications
 * between XFactor and SafeBox.
 */
void TestGoodCommunications();
bool TestOneStatus(int testNumber, XFactor_Status status);
void TestAllStatusCommunications();