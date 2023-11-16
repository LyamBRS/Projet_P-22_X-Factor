/**
 * @file Communication.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the definitions of
 * functions used in SafeBox to handle the
 * Bluetooth communication with XFactor.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "SafeBox/Communication.hpp"

// #pragma region [Command_Requests]

/**
 * @brief
 * When this function is called, it should check
 * if SafeBox has received any Bluetooth
 * messages. If a message was received, it
 * will be executed and a reply will be sent to
 * XFactor.
 *
 * @attention
 * This is the only function you should need to
 * call to execute whatever functions was
 * received by XFactor. Only use the global
 * status of XFactor to determine wether you need
 * to change Execution Functions. This wont tell
 * you.
 * @return true:
 * Successfully handled a request.
 * @return false:
 * Failed to handle a request / There was no
 * requests.
 */
bool SafeBox_CheckAndExecuteMessage()
{
    // - VARIABLES - //
    String latestMessage = "";

    if(BT_MessagesAvailable() == 0)
    {
        // There is no messages in the reception buffer
        return false;
    }

    latestMessage = BT_GetLatestMessage();

    if(latestMessage == BT_ERROR_MESSAGE)
    {
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "Error message returned");
        return false;
    }

    if(latestMessage == BT_NO_MESSAGE)
    {
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "BT_MessagesAvailable failure");
        return false; 
    }

//#define COMMAND_LID_OPEN  "C_LID_O"
//#define COMMAND_LID_CLOSE "C_LID_C"
//#define COMMAND_LID_GET "C_LID_G"
//#define COMMAND_GARAGE_OPEN   "C_GAR_O"
//#define COMMAND_GARAGE_CLOSE  "C_GAR_C"
//#define COMMAND_GARAGE_GET  "C_GAR_G"
//#define COMMAND_DOORBELL_GET  "C_DRB_G"
//#define COMMAND_GET_PACKAGE_COUNT "C_PCK_G"
//#define COMMAND_CHECK_PACKAGE     "C_PCK_C"
//#define COMMAND_STATUS_EXCHANGE   "C_STE_"

    // - ANSWER CHECK - //
    if(latestMessage.endsWith(COMMAND_LID_OPEN))   
    {
        if(SafeBox_ChangeLidState(true)) return true;
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "Failed to execute ChangeLidState");
        return false;
    }

    if(latestMessage.endsWith(COMMAND_LID_CLOSE))   
    {
        if(SafeBox_ChangeLidState(false)) return true;
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "Failed to execute ChangeLidState");
        return false;
    }

    if(latestMessage.endsWith(COMMAND_GARAGE_OPEN))   
    {
        if(SafeBox_ChangeGarageState(true)) return true;
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "Failed to execute ChangeGarageState");
        return false;
    }

    if(latestMessage.endsWith(COMMAND_GARAGE_CLOSE))   
    {
        if(SafeBox_ChangeGarageState(false)) return true;
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "Failed to execute ChangeGarageState");
        return false;
    }

    if(latestMessage.endsWith(COMMAND_CHECK_PACKAGE))
    {
        if(SafeBox_ReplyToCheckIfPackageDeposited()) return true;
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "Failed ReplyToCheckIfPackageDeposited");
        return false;     
    }

    if(latestMessage.endsWith(COMMAND_GET_PACKAGE_COUNT))
    {
        if(SafeBox_ReturnDepositedPackages()) return true;
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "Failed ReturnDepositedPackages");
        return false;     
    }

    if(latestMessage.endsWith(COMMAND_STATUS_EXCHANGE))
    {
        if(SafeBox_ReplyStatus()) return true;
        Debug_Error("Communication", "SafeBox_CheckAndExecuteMessage", "Failed ReturnDepositedPackages");
        return false;     
    }
    return true;
}

/**
 * @brief Function that changes the lid state of
 * SafeBox depending on what XFactor wants.
 * Must firstly unlock it if its closed and
 * lock it after if it was open.
 *
 * @param wantedState
 * The wanted state of the lid. true is opened,
 * false is closed.
 * @return true:
 * Successfully asked SafeBox to open its lid.
 * @return false:
 * Failed to ask SafeBox to open its lid.
 */
bool SafeBox_ChangeLidState(bool wantedState)
{
    if(wantedState)
    {
        if(Lid_Open())
        {
            if(BT_SendString(ANSWER_LID_SUCCESS)) return true;
            Debug_Error("Communication", "SafeBox_ChangeLidState", "Failed RX lid open success");
            SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
            return false;
        }
        Debug_Error("Communication", "SafeBox_ChangeLidState", "Lid opening failure");

        if(BT_SendString(ANSWER_LID_FAILED)) return false;
        Debug_Error("Communication", "SafeBox_ChangeLidState", "Failed RX lid failure");
        SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
        return false;      
    }
    else
    {
        if(Lid_Close())
        {
            if(BT_SendString(ANSWER_LID_SUCCESS)) return true;
            Debug_Error("Communication", "SafeBox_ChangeLidState", "Failed RX lid close success");
            SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
            return false;
        }
        Debug_Error("Communication", "SafeBox_ChangeLidState", "Lid closing failure");

        if(BT_SendString(ANSWER_LID_FAILED)) return false;
        Debug_Error("Communication", "SafeBox_ChangeLidState", "Failed RX lid failure");
        SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
        return false;  
    }
    // Should not reach
    return false;
}

/**
 * @brief Makes SafeBox change its garage door
 * state depending on what XFactor wants.
 *
 * @param wantedState
 * The wanted state of the garage door. true is
 * opened, false is closed.
 * @return true:
 * Successfully opened the garage door.
 * @return false:
 * Failed to open the garage door.
 */
bool SafeBox_ChangeGarageState(bool wantedState)
{
    if(wantedState)
    {
        if(Garage_Open())
        {
            if(BT_SendString(ANSWER_GARAGE_SUCCESS)) return true;
            Debug_Error("Communication", "SafeBox_ChangeGarageState", "Failed RX garage open success");
            SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
            return false;
        }
        Debug_Error("Communication", "SafeBox_ChangeGarageState", "Garage opening failure");

        if(BT_SendString(ANSWER_GARAGE_FAILED)) return false;
        Debug_Error("Communication", "SafeBox_ChangeGarageState", "Failed RX garage failure");
        SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
        return false;      
    }
    else
    {
        if(Lid_Close())
        {
            if(BT_SendString(ANSWER_GARAGE_SUCCESS)) return true;
            Debug_Error("Communication", "SafeBox_ChangeGarageState", "Failed RX garage close success");
            SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
            return false;
        }
        Debug_Error("Communication", "SafeBox_ChangeGarageState", "Garage closing failure");

        if(BT_SendString(ANSWER_GARAGE_FAILED)) return false;
        Debug_Error("Communication", "SafeBox_ChangeGarageState", "Failed RX garage failure");
        SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
        return false;  
    }
    // Should not reach
    return false;
}

bool SafeBox_SaveReceivedXFactorStatus(String command)
{
    if(command == BT_NO_MESSAGE)
    {
        Debug_Error("Communication", "SafeBox_SaveReceivedXFactorStatus", "Empty message");
        return false;
    }

    if(command == BT_ERROR_MESSAGE)
    {
        Debug_Error("Communication", "SafeBox_SaveReceivedXFactorStatus", "Error message");
        return false;
    }

    if(command.endsWith("A"))   {XFactor_SetNewStatus(XFactor_Status::Alarm); return true;}
    if(command.endsWith("CRH")) {XFactor_SetNewStatus(XFactor_Status::CalculatingRouteHome); return true;}
    if(command.endsWith("CE"))  {XFactor_SetNewStatus(XFactor_Status::CommunicationError); return true;}
    if(command.endsWith("CDO")) {XFactor_SetNewStatus(XFactor_Status::ConfirmingDropOff); return true;}
    if(command.endsWith("DO")) {XFactor_SetNewStatus(XFactor_Status::DroppingOff); return true;}
    if(command.endsWith("ESB")) {XFactor_SetNewStatus(XFactor_Status::EnteringSafeBox); return true;}
    if(command.endsWith("E")) {XFactor_SetNewStatus(XFactor_Status::Error); return true;}
    if(command.endsWith("EAP")) {XFactor_SetNewStatus(XFactor_Status::ExaminatingAPackage); return true;}
    if(command.endsWith("LSB")) {XFactor_SetNewStatus(XFactor_Status::LeavingSafeBox); return true;}
    if(command.endsWith("M")) {XFactor_SetNewStatus(XFactor_Status::Maintenance); return true;}
    if(command.endsWith("NPF")) {XFactor_SetNewStatus(XFactor_Status::NoPackageFound); return true;}
    if(command.endsWith("O")) {XFactor_SetNewStatus(XFactor_Status::Off); return true;}
    if(command.endsWith("PDOF")) {XFactor_SetNewStatus(XFactor_Status::PackageDropOffFailed); return true;}
    if(command.endsWith("PEF")) {XFactor_SetNewStatus(XFactor_Status::PackageExaminationFailed); return true;}
    if(command.endsWith("PPUF")) {XFactor_SetNewStatus(XFactor_Status::PackagePickUpFailed); return true;}
    if(command.endsWith("PFDO")) {XFactor_SetNewStatus(XFactor_Status::PackageDropOffFailed); return true;}
    if(command.endsWith("PFTS")) {XFactor_SetNewStatus(XFactor_Status::PreparingForTheSearch); return true;}
    if(command.endsWith("RH")) {XFactor_SetNewStatus(XFactor_Status::ReturningHome); return true;}
    if(command.endsWith("SFAP")) {XFactor_SetNewStatus(XFactor_Status::SearchingForAPackage); return true;}
    if(command.endsWith("WFD")) {XFactor_SetNewStatus(XFactor_Status::WaitingForDelivery); return true;}

}

/**
 * @brief
 * Replies to XFactor's status exchange by
 * replying to it with the box's own status.
 * XFactor status's is the sole input parameter
 * of this function and it stores it as a global
 * variable somewhere.
 *
 * The function must return a boolean that
 * represents if the status was successfully
 * handled or not, and if the function managed to
 * reply to XFactor or not.
 * @return true:
 * Successfully stored the status and
 * @return false:
 * Failed to handle the status or exchange our own.
 */
bool SafeBox_ReplyStatus()
{
    // - VARIABLES - //
    String answer = ANSWER_STATUS_EXCHANGE;
    String statusEnding = "";
    SafeBox_Status currentStatus = SafeBox_GetStatus();

    // - Get the command ending.
    switch(currentStatus)
    {
        case(SafeBox_Status::CommunicationError):   statusEnding = "CE"     break;
        case(SafeBox_Status::Off):                  statusEnding = "O";     break;
        case(SafeBox_Status::WaitingForDelivery):   statusEnding = "WFD";   break;
        case(SafeBox_Status::WaitingForRetrieval):  statusEnding = "WFRI";  break;
        case(SafeBox_Status::WaitingForReturn):     statusEnding = "WFR";   break;
        case(SafeBox_Status::WaitingForXFactor):    statusEnding = "RFDO";  break;
        case(SafeBox_Status::ReadyForDropOff):      statusEnding = "RFDO";  break;
        case(SafeBox_Status::Unlocked):             statusEnding = "U";     break;
        case(SafeBox_Status::DroppingOff):          statusEnding = "DO";    break;
        case(SafeBox_Status::Maintenance):          statusEnding = "M";     break;
        case(SafeBox_Status::Error):                statusEnding = "E";     break;
        case(SafeBox_Status::Alarm):                statusEnding = "A";     break;

        default:
            Debug_Error("Communication", "SafeBox_ReplyStatus", "Unknown SafeBox status");
            return false;
    }

    // - Build command string and send it
    answer = answer.concat(statusEnding);
    if(BT_SendString(answer))
    {
        Debug_Error("Communication", "SafeBox_ReplyStatus", "Status TX failed");
        return false;
    }

    return true;
}

/**
 * @brief
 * Function that verifies if there is a new
 * package inside of the box. If yes, the proper
 * message is sent to XFactor as an answer,
 * otherwise, the false message is sent back to
 * XFactor.
 *
 * This function must take nothing as an input
 * parameter but must return a boolean that
 * specifies if it successfully managed to handle
 * the request or not.
 * @return true:
 * Successfully handled the request.
 * @return false:
 * Failed to handle the request.
 */
bool SafeBox_ReplyToCheckIfPackageDeposited()
{
    return false;
}

/**
 * @brief
 * Tells XFactor how many packages are
 * currently deposited inside of itself.
 * This sends on Bluetooth the amount of
 * packages currently inside of SafeBox.
 *
 * This function should return a Boolean that
 * corresponds to whether it successfully
 * handled the request or not.
 * @return true:
 * Successfully handled the request.
 * @return false:
 * Failed to handle the request.
 */
bool SafeBox_ReturnDepositedPackages()
{
    return false;
}

/**
 * @brief
 * Tell XFactor whether it recently detected
 * a doorbell or not. Doorbell value should
 * either be 0 or 1 and is reset by SafeBox only
 * when the package is deposited inside of it or
 * something happens, like a failure to get the
 * package.
 *
 * @return true:
 * Successfully handled the doorbell request.
 * @return false:
 * Failed to handle the doorbell request.
 */
bool SafeBox_GetDoorBellStatus()
{
    return false;
}
// #pragma endregion