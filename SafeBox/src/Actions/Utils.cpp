
/**
 * @file Utils.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing definitions of Utils functions
 * used in SafeBox.
 * @version 0.1
 * @date 2023-11-18
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Actions/Utils.hpp"
#include "Actions/Actions.hpp"

/**
 * @brief
 * This function ensures that the door is read as
 * closed. If its not the case after X amount of
 * attempts, the alarm needs to be executed
 * because the door is NOT supposed to be open.
 * @return true:
 * We good, no alarms mate
 * @return false:
 * Euuh... Why is the door not closed? 
 */
bool ExecutionUtils_CheckIfGarageIsClosed()
{
    static int checkingCounter = 0;

    if(!Garage_GetSupposedWantedStatus())
    {
        if(!Garage_IsClosed())
        {
            checkingCounter++;

            if(checkingCounter > EXECUTIONUTILS_CLOCKS_TILL_GARAGE_ALARM)
            {
                checkingCounter = 0;
                Debug_Warning("Utils", "ExecutionUtils_CheckIfGarageIsClosed", "Door has not been closed for long enough");
                return false;
            }
        }
    }
    else
    {
        //Debug_Warning("Utils", "ExecutionUtils_CheckIfGarageIsClosed", "Bypassing door checks");
        checkingCounter = 0;
    }
    return true;
}

/**
 * @brief
 * This function allows you to create a simple
 * blinking statement in your Execution functions
 * Especially useful for the Alarm and Error.
 * @param blinkingPeriodMS
 * How long should the timer wait before it needs
 * to return the end of period so that you can
 * make your LED blink?
 * @return true:
 * The timer has reached the end. You must make
 * the LED blink.
 * @return false:
 * The timer is still counting.
 */
bool ExecutionUtils_LedBlinker(unsigned long blinkingPeriodMS)
{
    // - VARIABLES - //
    static unsigned long currentMS = millis();
    static unsigned long previousMS = millis();

    currentMS = millis();

    if((currentMS - previousMS) > blinkingPeriodMS)
    {
        previousMS = millis();
        return true;
    }
    return false;
}

/**
 * @brief
 * This function's sole purpose is to handle the
 * RFID card reader in each execution function
 * where SafeBox can be unlocked when its armed.
 * 
 * true: Keep going
 * False: stop. New function set.
 */
bool ExecutionUtils_HandleArmedUnlocking()
{
    int result = RFID_HandleCard();

    if(result == 1)
    {
        Debug_Information("Utils", "ExecutionUtils_HandleArmedUnlocking", "Valid card detected");
        if(!SetNewExecutionFunction(FUNCTION_ID_UNLOCKED))
        {
            Debug_Error("Utils", "ExecutionUtils_HandleArmedUnlocking", "Failed to set UNLOCKED execution function");
            SetNewExecutionFunction(FUNCTION_ID_ALARM);
            return false;
        }
        return false;
    }
    if(result == -1)
    {
        Debug_Error("Utils", "ExecutionUtils_HandleArmedUnlocking", "Incorrect card.");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return false;
    }
    return true;
}

/**
 * @brief
 * Checks the latest status received from XFactor
 * and executes different actions based on its
 * status, such as entering alarm mode or
 * error mode.
 */
void ExecutionUtils_HandleReceivedXFactorStatus()
{
    // - VARIABLES - //
    unsigned char currentFunction = GetCurrentExecutionFunction();
    XFactor_Status currentXFactorStatus = XFactor_GetStatus();

    // - BASIC GLOBAL STATUS - //
    if(currentXFactorStatus == XFactor_Status::Alarm)
    {
        //Debug_Warning("Utils", "ExecutionUtils_HandleReceivedXFactorStatus", "XFactor has an ongoing alarm.");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;
    }

    if(currentXFactorStatus == XFactor_Status::Error)
    {
        Debug_Error("Utils", "ExecutionUtils_HandleReceivedXFactorStatus", "XFactor has an ongoing error.");
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
        return;
    }

    if(currentXFactorStatus == XFactor_Status::Unlocked)
    {
        //SetNewExecutionFunction(FUNCTION_ID_UNLOCKED);
        return;
    }

    //if(currentXFactorStatus == XFactor_Status::Off)
    //{
    //    Debug_Error("Utils", "ExecutionUtils_HandleReceivedXFactorStatus", "XFactor should never be seen as Off. Ongoing error.");
    //    SetNewExecutionFunction(FUNCTION_ID_ERROR);
    //    return;
    //}

    // - PROGRAM - //
    switch(currentFunction){

        case(FUNCTION_ID_ALARM):

            if(currentXFactorStatus == XFactor_Status::Alarm) break; // Good, it should be in alarm status as well.

            if(currentXFactorStatus == XFactor_Status::CommunicationError)
            {
                Debug_Error("Utils", "ExecutionUtils_HandleReceivedXFactorStatus", "XFactor communication error");
                break;
            }

            //Debug_Error("Utils", "ExecutionUtils_HandleReceivedXFactorStatus", "XFactor status doesnt match expected status");
            break;

        case(FUNCTION_ID_ERROR):
            // We dont care about whatever status XFactor is in. We have bigger problems.
            break;

        case(FUNCTION_ID_WAIT_AFTER_XFACTOR):
            switch(currentXFactorStatus)
            {
                case(XFactor_Status::Off):
                    break;

                case(XFactor_Status::CalculatingRouteHome):
                    Debug_Error("Utils", "800", "XFactor should'nt be calculating route home when SafeBox is waiting after it. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::CommunicationError):
                    // Simply not finding XFactor
                    break;

                case(XFactor_Status::ConfirmingDropOff):
                    Debug_Error("Utils", "800", "XFactor should'nt be confirming drop off when SafeBox is waiting after it. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::DroppingOff):
                    Debug_Error("Utils", "800", "XFactor should'nt be dropping off when SafeBox is waiting after it. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::EnteringSafeBox):
                    Debug_Error("Utils", "800", "XFactor should'nt be entering SafeBox when its waiting after it. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::ExaminatingAPackage):
                    Debug_Error("Utils", "800", "XFactor should'nt be examinating package when SafeBox when its waiting after it. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::LeavingSafeBox):
                    Debug_Error("Utils", "800", "XFactor should'nt be leaving SafeBox when its waiting after it. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::Maintenance):
                    Debug_Error("Utils", "800", "XFactor should'nt be in maintenance. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;

                case(XFactor_Status::NoPackageFound):
                    Debug_Error("Utils", "800", "XFactor should'nt be in NoPackageFound. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::PackageDropOffFailed):
                    Debug_Error("Utils", "800", "XFactor should'nt be in PackageDropOffFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::PackageExaminationFailed):
                    Debug_Error("Utils", "800", "XFactor should'nt be in PackageExaminationFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PackagePickUpFailed):
                    Debug_Error("Utils", "800", "XFactor should'nt be in PackagePickUpFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PreparingForDropOff):
                    Debug_Error("Utils", "800", "XFactor should'nt be in PreparingForDropOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::PreparingForTheSearch):
                    Debug_Error("Utils", "800", "XFactor should'nt be in PreparingForTheSearch. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::ReturningHome):
                    Debug_Error("Utils", "800", "XFactor should'nt be in ReturningHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::SearchingForAPackage):
                    Debug_Error("Utils", "800", "XFactor should'nt be in SearchingForAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::WaitingForDelivery):
                    Debug_Error("Utils", "800", "XFactor should'nt be in WaitingForDelivery. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_DELIVERY);
                    break;

                case(XFactor_Status::WaitingAfterSafeBox):
                    //Debug_Information("Utils", "100", "Good status received");
                    SetNewExecutionFunction(FUNCTION_ID_UNLOCKED);
                    break;

                default:
                    Debug_Error("Utils", "900", "INCORRECT XFACTOR STATUS.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;
            }
            break;

        case(FUNCTION_ID_START_OF_DELIVERY):
            switch(currentXFactorStatus)
            {
                case(XFactor_Status::CalculatingRouteHome):
                    Debug_Error("Utils", "801", "XFactor should'nt be CalculatingRouteHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::CommunicationError):
                    // Simply not finding XFactor
                    break;

                case(XFactor_Status::ConfirmingDropOff):
                    Debug_Error("Utils", "801", "XFactor should'nt be ConfirmingDropOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::DroppingOff):
                    Debug_Error("Utils", "801", "XFactor should'nt DroppingOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::EnteringSafeBox):
                    Debug_Error("Utils", "801", "XFactor should'nt be EnteringSafeBox. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::ExaminatingAPackage):
                    Debug_Error("Utils", "801", "XFactor should'nt be ExaminatingAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::LeavingSafeBox):
                    Debug_Error("Utils", "801", "XFactor should'nt be LeavingSafeBox. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::Maintenance):
                    Debug_Error("Utils", "801", "XFactor should'nt be in maintenance. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;

                case(XFactor_Status::NoPackageFound):
                    Debug_Error("Utils", "801", "XFactor should'nt be in NoPackageFound. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::PackageDropOffFailed):
                    Debug_Error("Utils", "801", "XFactor should'nt be in PackageDropOffFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::PackageExaminationFailed):
                    Debug_Error("Utils", "801", "XFactor should'nt be in PackageExaminationFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PackagePickUpFailed):
                    Debug_Error("Utils", "801", "XFactor should'nt be in PackagePickUpFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PreparingForDropOff):
                    Debug_Error("Utils", "801", "XFactor should'nt be in PreparingForDropOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::PreparingForTheSearch):
                    Debug_Information("Utils", "101", "Correct Status. Changing execution function");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::ReturningHome):
                    Debug_Error("Utils", "801", "XFactor should'nt be in ReturningHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::SearchingForAPackage):
                    Debug_Error("Utils", "801", "XFactor should'nt be in SearchingForAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::WaitingForDelivery):
                    Debug_Error("Utils", "801", "XFactor should'nt be in WaitingForDelivery anymore.");
                    break;

                case(XFactor_Status::WaitingAfterSafeBox):
                    //Debug_Information("Utils", "101", "Good status received");
                    SetNewExecutionFunction(FUNCTION_ID_UNLOCKED);
                    break;

                default:
                    Debug_Error("Utils", "901", "INCORRECT XFACTOR STATUS.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;
            }
            break;

        case(FUNCTION_ID_WAIT_FOR_DELIVERY):
            switch(currentXFactorStatus)
            {
                case(XFactor_Status::CalculatingRouteHome):
                    Debug_Error("Utils", "802", "XFactor should'nt be CalculatingRouteHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::CommunicationError):
                    // Simply not finding XFactor
                    break;

                case(XFactor_Status::ConfirmingDropOff):
                    Debug_Error("Utils", "802", "XFactor should'nt be ConfirmingDropOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::DroppingOff):
                    Debug_Error("Utils", "802", "XFactor should'nt DroppingOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::EnteringSafeBox):
                    Debug_Error("Utils", "802", "XFactor should'nt be EnteringSafeBox. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::ExaminatingAPackage):
                    Debug_Error("Utils", "802", "XFactor should'nt be ExaminatingAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::LeavingSafeBox):
                    Debug_Error("Utils", "802", "XFactor should'nt be LeavingSafeBox. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::Maintenance):
                    Debug_Error("Utils", "802", "XFactor should'nt be in maintenance. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;

                case(XFactor_Status::NoPackageFound):
                    Debug_Error("Utils", "802", "XFactor should'nt be in NoPackageFound. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::PackageDropOffFailed):
                    Debug_Error("Utils", "802", "XFactor should'nt be in PackageDropOffFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::PackageExaminationFailed):
                    Debug_Error("Utils", "802", "XFactor should'nt be in PackageExaminationFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PackagePickUpFailed):
                    Debug_Error("Utils", "802", "XFactor should'nt be in PackagePickUpFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PreparingForDropOff):
                    Debug_Error("Utils", "802", "XFactor should'nt be in PreparingForDropOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::PreparingForTheSearch):
                    Debug_Information("Utils", "802", "Correct Status. Changing execution function");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::ReturningHome):
                    Debug_Error("Utils", "802", "XFactor should'nt be in ReturningHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::SearchingForAPackage):
                    Debug_Error("Utils", "802", "XFactor should'nt be in SearchingForAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::WaitingForDelivery):
                    //Debug_Error("Utils", "102", "Good status");
                    break;

                case(XFactor_Status::WaitingAfterSafeBox):
                    //Debug_Error("Utils", "702", "XFactor should'nt be in WaitingAfterSafeBox. Correcting.");
                    break;

                default:
                    Debug_Error("Utils", "902", "INCORRECT XFACTOR STATUS.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;
            }
            break;

        case(FUNCTION_ID_UNLOCKED):
            switch(currentXFactorStatus)
            {
                case(XFactor_Status::CalculatingRouteHome):
                    Debug_Error("Utils", "803", "XFactor should'nt be CalculatingRouteHome. Correcting.");
                    break;

                case(XFactor_Status::CommunicationError):
                    // Simply not finding XFactor
                    break;

                case(XFactor_Status::ConfirmingDropOff):
                    Debug_Error("Utils", "803", "XFactor should'nt be ConfirmingDropOff. Correcting.");
                    break;

                case(XFactor_Status::DroppingOff):
                    Debug_Error("Utils", "803", "XFactor should'nt DroppingOff. Correcting.");
                    break;

                case(XFactor_Status::EnteringSafeBox):
                    Debug_Error("Utils", "803", "XFactor should'nt be EnteringSafeBox. Correcting.");
                    break;

                case(XFactor_Status::ExaminatingAPackage):
                    Debug_Error("Utils", "803", "XFactor should'nt be ExaminatingAPackage. Correcting.");
                    break;

                case(XFactor_Status::LeavingSafeBox):
                    Debug_Error("Utils", "803", "XFactor should'nt be LeavingSafeBox. Correcting.");
                    break;

                case(XFactor_Status::Maintenance):
                    Debug_Error("Utils", "803", "XFactor should'nt be in maintenance. Correcting.");
                    break;

                case(XFactor_Status::NoPackageFound):
                    Debug_Error("Utils", "803", "XFactor should'nt be in NoPackageFound. Correcting.");
                    break;

                case(XFactor_Status::PackageDropOffFailed):
                    Debug_Error("Utils", "803", "XFactor should'nt be in PackageDropOffFailed. Correcting.");
                    break;

                case(XFactor_Status::PackageExaminationFailed):
                    Debug_Error("Utils", "803", "XFactor should'nt be in PackageExaminationFailed. Correcting.");
                    break;

                case(XFactor_Status::PackagePickUpFailed):
                    Debug_Error("Utils", "803", "XFactor should'nt be in PackagePickUpFailed. Correcting.");
                    break;

                case(XFactor_Status::PreparingForDropOff):
                    Debug_Error("Utils", "803", "XFactor should'nt be in PreparingForDropOff. Correcting.");
                    break;

                case(XFactor_Status::PreparingForTheSearch):
                    Debug_Information("Utils", "803", "Correct Status. Changing execution function");
                    break;

                case(XFactor_Status::ReturningHome):
                    Debug_Error("Utils", "803", "XFactor should'nt be in ReturningHome. Correcting.");
                    break;

                case(XFactor_Status::SearchingForAPackage):
                    Debug_Error("Utils", "803", "XFactor should'nt be in SearchingForAPackage. Correcting.");
                    break;

                case(XFactor_Status::WaitingForDelivery):
                    Debug_Error("Utils", "803", "Good status");
                    break;

                case(XFactor_Status::WaitingAfterSafeBox):
                    //Debug_Error("Utils", "703", "XFactor should'nt be in WaitingAfterSafeBox. Correcting.");
                    break;

                default:
                    Debug_Error("Utils", "903", "INCORRECT XFACTOR STATUS.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;
            }
            break;

        case(FUNCTION_ID_DROP_OFF):
            switch(currentXFactorStatus)
            {
                case(XFactor_Status::CalculatingRouteHome):
                    Debug_Error("Utils", "804", "XFactor should'nt be CalculatingRouteHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::CommunicationError):
                    // Simply not finding XFactor
                    break;

                case(XFactor_Status::ConfirmingDropOff):
                    //Debug_Information("Utils", "104", "Good Status");
                    // SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::DroppingOff):
                    //Debug_Information("Utils", "104", "Good Status");
                    // SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::EnteringSafeBox):
                    Debug_Error("Utils", "804", "XFactor should'nt be EnteringSafeBox. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::ExaminatingAPackage):
                    Debug_Error("Utils", "804", "XFactor should'nt be ExaminatingAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::LeavingSafeBox):
                    Debug_Error("Utils", "804", "XFactor should'nt be LeavingSafeBox. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::Maintenance):
                    Debug_Error("Utils", "804", "XFactor should'nt be in maintenance. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;

                case(XFactor_Status::NoPackageFound):
                    Debug_Error("Utils", "804", "XFactor should'nt be in NoPackageFound. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::PackageDropOffFailed):
                    Debug_Error("Utils", "704", "XFactor failed to drop off the package");
                    SetNewExecutionFunction(FUNCTION_ID_ALARM);
                    break;

                case(XFactor_Status::PackageExaminationFailed):
                    Debug_Error("Utils", "804", "XFactor should'nt be in PackageExaminationFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PackagePickUpFailed):
                    Debug_Error("Utils", "804", "XFactor should'nt be in PackagePickUpFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PreparingForDropOff):
                    //Debug_Information("Utils", "104", "Good Status");
                    // SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::PreparingForTheSearch):
                    Debug_Information("Utils", "804", "Correct Status. Changing execution function");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::ReturningHome):
                    Debug_Error("Utils", "804", "XFactor should'nt be in ReturningHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::SearchingForAPackage):
                    Debug_Error("Utils", "804", "XFactor should'nt be in SearchingForAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::WaitingForDelivery):
                    Debug_Error("Utils", "704", "XFactor should'nt be in WaitingForDelivery. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_DELIVERY);
                    break;

                case(XFactor_Status::WaitingAfterSafeBox):
                    //Debug_Error("Utils", "704", "XFactor should'nt be in WaitingAfterSafeBox.");
                    break;

                default:
                    Debug_Error("Utils", "904", "INCORRECT XFACTOR STATUS.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;
            }
            break;

        case(FUNCTION_ID_WAIT_FOR_RETRIEVAL):
            switch(currentXFactorStatus)
            {
                case(XFactor_Status::CalculatingRouteHome):
                    Debug_Error("Utils", "805", "XFactor should'nt be CalculatingRouteHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::CommunicationError):
                    // Simply not finding XFactor
                    break;

                case(XFactor_Status::ConfirmingDropOff):
                    Debug_Error("Utils", "805", "XFactor should'nt be CalculatingRouteHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::DroppingOff):
                    Debug_Error("Utils", "805", "XFactor should'nt be DroppingOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::EnteringSafeBox):
                    Debug_Error("Utils", "805", "XFactor should'nt be EnteringSafeBox. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::ExaminatingAPackage):
                    //Debug_Information("Utils", "105", "Good status");
                    break;

                case(XFactor_Status::LeavingSafeBox):
                    //Debug_Information("Utils", "105", "Good status");
                    break;

                case(XFactor_Status::Maintenance):
                    Debug_Error("Utils", "805", "XFactor should'nt be in maintenance. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;

                case(XFactor_Status::NoPackageFound):
                    Debug_Error("Utils", "805", "XFactor should'nt be in NoPackageFound. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::PackageDropOffFailed):
                    Debug_Error("Utils", "805", "XFactor should'nt be PackageDropOffFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::PackageExaminationFailed):
                    //Debug_Information("Utils", "105", "Good status");
                    break;

                case(XFactor_Status::PackagePickUpFailed):
                    //Debug_Information("Utils", "105", "Good status");
                    break;

                case(XFactor_Status::PreparingForDropOff):
                    Debug_Error("Utils", "805", "XFactor should'nt be PreparingForDropOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::PreparingForTheSearch):
                    //Debug_Information("Utils", "105", "Good status");
                    break;

                case(XFactor_Status::ReturningHome):
                    Debug_Error("Utils", "805", "XFactor should'nt be in ReturningHome. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETURN);
                    break;

                case(XFactor_Status::SearchingForAPackage):
                    //Debug_Information("Utils", "105", "Good status");
                    break;

                case(XFactor_Status::WaitingForDelivery):
                    Debug_Error("Utils", "705", "XFactor should'nt be in WaitingForDelivery. Correcting.");
                    // SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::WaitingAfterSafeBox):
                    //Debug_Error("Utils", "705", "XFactor should'nt be in WaitingAfterSafeBox.");
                    break;

                default:
                    Debug_Error("Utils", "905", "INCORRECT XFACTOR STATUS.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;
            }
            break;

        case(FUNCTION_ID_WAIT_FOR_RETURN):
            switch(currentXFactorStatus)
            {
                case(XFactor_Status::CalculatingRouteHome):
                    //Debug_Information("Utils", "106", "Good Status");
                    break;

                case(XFactor_Status::CommunicationError):
                    // Simply not finding XFactor
                    break;

                case(XFactor_Status::ConfirmingDropOff):
                    Debug_Error("Utils", "806", "XFactor should'nt be ConfirmingDropOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::DroppingOff):
                    Debug_Error("Utils", "806", "XFactor should'nt be DroppingOff. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::EnteringSafeBox):
                    //Debug_Information("Utils", "106", "Good Status");
                    break;

                case(XFactor_Status::ExaminatingAPackage):
                    Debug_Error("Utils", "806", "XFactor should'nt be ExaminatingAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::LeavingSafeBox):
                    Debug_Error("Utils", "806", "XFactor should'nt be LeavingSafeBox. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::Maintenance):
                    Debug_Error("Utils", "806", "XFactor should'nt be in maintenance. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;

                case(XFactor_Status::NoPackageFound):
                    //Debug_Information("Utils", "106", "Good Status");
                    break;

                case(XFactor_Status::PackageDropOffFailed):
                    Debug_Error("Utils", "706", "XFactor failed to drop off the package");
                    SetNewExecutionFunction(FUNCTION_ID_ALARM);
                    break;

                case(XFactor_Status::PackageExaminationFailed):
                    Debug_Error("Utils", "806", "XFactor should'nt be in PackageExaminationFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PackagePickUpFailed):
                    Debug_Error("Utils", "806", "XFactor should'nt be in PackagePickUpFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::PreparingForDropOff):
                    Debug_Error("Utils", "806", "XFactor should'nt be in PackagePickUpFailed. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_DROP_OFF);
                    break;

                case(XFactor_Status::PreparingForTheSearch):
                    Debug_Error("Utils", "806", "XFactor should'nt be in PreparingForTheSearch. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::ReturningHome):
                    //Debug_Information("Utils", "106", "Good Status");
                    break;

                case(XFactor_Status::SearchingForAPackage):
                    Debug_Error("Utils", "806", "XFactor should'nt be in SearchingForAPackage. Correcting.");
                    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::WaitingForDelivery):
                    Debug_Error("Utils", "706", "XFactor should'nt be in WaitingForDelivery. Correcting.");
                    // SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
                    break;

                case(XFactor_Status::WaitingAfterSafeBox):
                    //Debug_Error("Utils", "706", "XFactor should'nt be in WaitingAfterSafeBox.");
                    break;

                default:
                    Debug_Error("Utils", "906", "INCORRECT XFACTOR STATUS.");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                    break;
            }
            break;

        case(FUNCTION_ID_END_OF_PROGRAM):
            break;

        default:
            Debug_Error("Utils", "1000", "UNKNOWN FUNCTION ID");
            SetNewExecutionFunction(FUNCTION_ID_ERROR);
            return;
    }
    //delay(1000);
}