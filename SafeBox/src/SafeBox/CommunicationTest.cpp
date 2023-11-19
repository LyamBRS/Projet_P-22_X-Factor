/**
 * @file CommunicationTest.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing functions
 * used to linearly test
 * communications between SafeBox and XFactor
 * @version 0.1
 * @date 2023-11-16
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "SafeBox/CommunicationTest.hpp"

// - DEFINES - //

// - FUNCTIONS - //

void ContinuouslyAnswerXFactor()
{
    SafeBox_CheckAndExecuteMessage();
}

/**
 * @brief
 * Call this in main.cpp to test communications
 * between XFactor and SafeBox.
 */
void TestGoodCommunications()
{

}

bool TestOneStatus(int testNumber, XFactor_Status status)
{
    return false;
}

void TestAllStatusCommunications()
{
}