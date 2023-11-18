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

/**
 * @brief
 * Call this in main.cpp to test communications
 * between XFactor and SafeBox.
 */
void TestGoodCommunications()
{
    Debug_Start("TestGoodCommunications");
    // - VARIABLES - //
    static unsigned char currentTest = 0;

    // LEDS_SetColor(0, 64, 32, 0);
    // delay(100);

    switch(currentTest)
    {
        case(0):
            Debug_Information("TEST", "SafeBox_ChangeLidState", "Testing Lid open");
            if(SafeBox_ChangeLidState(true))
            {
                // delay(100);
                // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
                Debug_Information("TEST", "SafeBox_ChangeLidState", "OPEN SUCCESS");
                currentTest++;
                Debug_End();
                return;
            }
            Debug_Error("TEST", "SafeBox_ChangeLidState", "OPEN FAILED");
            break;

        case(1):
            if(SafeBox_ChangeLidState(false))
            {
                // delay(100);
                // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
                Debug_Information("TEST", "SafeBox_ChangeLidState", "CLOSE SUCCESS");
                currentTest++;
                Debug_End();
                return;
            }
            Debug_Error("TEST", "SafeBox_ChangeLidState", "CLOSE FAILED");
            break;

        case(2):
            if(SafeBox_ChangeGarageState(true))
            {
                // delay(100);
                // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
                Debug_Information("TEST", "SafeBox_ChangeGarageState", "OPEN SUCCESS");
                currentTest++;
                Debug_End();
                return;
            }
            Debug_Error("TEST", "SafeBox_ChangeGarageState", "OPEN FAILED");
            break;

        case(3):
            if(SafeBox_ChangeGarageState(false))
            {
                // delay(100);
                // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
                Debug_Information("TEST", "SafeBox_ChangeGarageState", "CLOSE SUCCESS");
                currentTest++;
                Debug_End();
                return;
            }
            Debug_Error("TEST", "SafeBox_ChangeGarageState", "CLOSE FAILED");
            break;

        case(4):
            if(SafeBox_GetDoorBellStatus())
            {
                // delay(100);
                // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
                Debug_Information("TEST", "SafeBox_GetDoorBellStatus", "SUCCESS");
                currentTest++;
                Debug_End();
                return;
            }
            Debug_Error("TEST", "SafeBox_ChangeGarageState", "DOORBELL FAILED");
            break;

        case(5):
            if(SafeBox_CheckIfPackageDeposited())
            {
                // delay(100);
                // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
                Debug_Information("TEST", "SafeBox_CheckIfPackageDeposited", "SUCCESS");
                currentTest++;
                Debug_End();
                return;
            }
            Debug_Error("TEST", "SafeBox_CheckIfPackageDeposited", "PACKAGE CHECK FAILED");
            break;

        case(6):
            XFactor_SetNewStatus(XFactor_Status::WaitingForDelivery);
            if(SafeBox_ExchangeStatus())
            {
                // delay(100);
                // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
                Debug_Information("TEST", "SafeBox_ExchangeStatus", "SUCCESS");
                currentTest++;
                Debug_End();
                return;
            }
            Debug_Error("TEST", "SafeBox_ExchangeStatus", "STATUS EXCHANGE FAILED");
            break;
    }

    if(currentTest == 7)
    {
        // delay(100);
        // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
        Debug_Information("TEST", "RESULT", "SUCCESS");
        Debug_End();
        delay(5000);
        return;
    }
    // delay(100);
    // LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ALARM);
    delay(5000);
    Debug_End();
}

bool TestOneStatus(int testNumber, XFactor_Status status)
{
    XFactor_SetNewStatus(status);
    if(SafeBox_ExchangeStatus())
    {
        Debug_Information("TEST", "SafeBox_ExchangeStatus", "SUCCESS");
        return true;
    }
    Debug_Error("TEST", "SafeBox_ExchangeStatus", "CLOSE FAILED");
    Debug_Error("TEST", "FAILED", String(testNumber));
    return false;
}

void TestAllStatusCommunications()
{
    // - VARIABLES - //
    static unsigned char currentTest = 0;

    switch(currentTest)
    {
        case(0):
            if(TestOneStatus(0, XFactor_Status::Alarm)) currentTest++;
            return;

        case(1):
            if(TestOneStatus(1, XFactor_Status::CalculatingRouteHome)) currentTest++;
            return;

        case(2):
            if(TestOneStatus(2, XFactor_Status::CommunicationError)) currentTest++;
            return;

        case(3):
            if(TestOneStatus(3, XFactor_Status::ConfirmingDropOff)) currentTest++;
            return;

        case(4):
            if(TestOneStatus(4, XFactor_Status::DroppingOff)) currentTest++;
            return;

        case(5):
            if(TestOneStatus(4, XFactor_Status::EnteringSafeBox)) currentTest++;
            return;

        case(6):
            if(TestOneStatus(6, XFactor_Status::Error)) currentTest++;
            return;

        case(7):
            if(TestOneStatus(7, XFactor_Status::ExaminatingAPackage)) currentTest++;
            return;

        case(8):
            if(TestOneStatus(8, XFactor_Status::LeavingSafeBox)) currentTest++;
            return;

        case(9):
            if(TestOneStatus(9, XFactor_Status::Maintenance)) currentTest++;
            return;

        case(10):
            if(TestOneStatus(10, XFactor_Status::NoPackageFound)) currentTest++;
            return;

        case(11):
            if(TestOneStatus(11, XFactor_Status::Off)) currentTest++;
            return;

        case(12):
            if(TestOneStatus(12, XFactor_Status::PackageDropOffFailed)) currentTest++;
            return;

        case(13):
            if(TestOneStatus(13, XFactor_Status::PackageExaminationFailed)) currentTest++;
            return;

        case(14):
            if(TestOneStatus(14, XFactor_Status::PackagePickUpFailed)) currentTest++;
            return;

        case(15):
            if(TestOneStatus(15, XFactor_Status::PickingUpAPackage)) currentTest++;
            return;

        case(16):
            if(TestOneStatus(16, XFactor_Status::PreparingForDropOff)) currentTest++;
            return;

        case(17):
            if(TestOneStatus(17, XFactor_Status::PreparingForTheSearch)) currentTest++;
            return;

        case(18):
            if(TestOneStatus(18, XFactor_Status::ReturningHome)) currentTest++;
            return;

        case(19):
            if(TestOneStatus(19, XFactor_Status::SearchingForAPackage)) currentTest++;
            return;

        case(20):
            if(TestOneStatus(20, XFactor_Status::WaitingAfterSafeBox)) currentTest++;
            return;

        case(21):
            if(TestOneStatus(21, XFactor_Status::WaitingForDelivery)) currentTest++;
            return;
    }

    if(currentTest == 22)
    {
        Debug_Information("TEST", "RESULT", "SUCCESS");
        currentTest = 23;
    }

    delay(1000);
}