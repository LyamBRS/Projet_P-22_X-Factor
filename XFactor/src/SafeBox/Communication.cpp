/**
 * @file Communication.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * File containing the header definitions of
 * functions used in XFactor to handle the
 * Bluetooth communication with SafeBox.
 * @version 0.1
 * @date 2023-10-25
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "SafeBox/Communication.hpp"

//#pragma region [Command_Requests]

/**
 * @brief Asks SafeBox to change its package lid
 * status. Depending on the specified parameter,
 * the lid will either unlock and open or close
 * and lock.
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
    // - VARIABLES - //
    String command = wantedState ? COMMAND_LID_OPEN : COMMAND_LID_CLOSE; //Ternary operators go brrr
    String answer = "";

    answer = BT_MessageExchange(command, COMMS_TIMEOUT_MS);
    if(answer != ANSWER_LID_SUCCESS)
    {
        if(answer == ANSWER_LID_FAILED)
        {
            Debug_Error("Communication", "SafeBox_ChangeLidState", "Received lid failure");
            return false;
        }

        if(answer == ANSWER_LID_OPEN)
        {
            Debug_Error("Communication", "SafeBox_ChangeLidState", "Received lid open");
            return false;
        }

        if(answer == ANSWER_LID_CLOSED)
        {
            Debug_Error("Communication", "SafeBox_ChangeLidState", "Received lid closed");
            return false;
        }

        Debug_Error("Communication", "SafeBox_ChangeLidState", "Received unknown answer:");
        Debug_Error("Communication", "SafeBox_ChangeLidState", answer);
        return false;
    }

    return true;
}

/**
 * @brief Asks SafeBox to change its garage door
 * status. Depending on the specified parameter,
 * the garage door will either unlock and open or
 * close and lock.
 *
 * @param wantedState
 * The wanted state of the garage door. true is
 * opened, false is closed.
 * @return true:
 * Successfully asked SafeBox to open the garage
 * door.
 * @return false:
 * Failed to ask SafeBox to open the garage door.
 */
bool SafeBox_ChangeGarageState(bool wantedState)
{
    // - VARIABLES - //
    String command = wantedState ? COMMAND_GARAGE_OPEN : COMMAND_GARAGE_CLOSE; //Ternary operators go brrr
    String answer = "";

    answer = BT_MessageExchange(command, COMMS_TIMEOUT_MS);
    if(answer != ANSWER_GARAGE_SUCCESS)
    {
        if(answer == ANSWER_GARAGE_FAILED)
        {
            Debug_Error("Communication", "SafeBox_ChangeGarageState", "Received garage failure");
            return false;
        }

        if(answer == ANSWER_GARAGE_OPEN)
        {
            Debug_Error("Communication", "SafeBox_ChangeGarageState", "Received garage open");
            return false;
        }

        if(answer == ANSWER_GARAGE_CLOSED)
        {
            Debug_Error("Communication", "SafeBox_ChangeGarageState", "Received garage closed");
            return false;
        }

        Debug_Error("Communication", "SafeBox_ChangeGarageState", "Received unknown answer:");
        Debug_Error("Communication", "SafeBox_ChangeGarageState", answer);
        return false;
    }

    return true;
}

/**
 * @brief Asks SafeBox to identify if a package
 * was recently deposited inside of it. The
 * returned parameter tells if a package was
 * deposited or not.
 *
 * @return true:
 * A package was successfully deposited inside.
 * @return false:
 * No package were deposited since last time.
 */
bool SafeBox_CheckIfPackageDeposited()
{
    // - VARIABLES - //
    String answer = "";

    answer = BT_MessageExchange(COMMAND_CHECK_PACKAGE, COMMS_TIMEOUT_MS);
    if(answer == ANSWER_PACKAGE_CHECK_SUCCESS) return true;
    if(answer == ANSWER_PACKAGE_CHECK_FAILED) return false;

    Debug_Error("Communication", "SafeBox_CheckIfPackageDeposited", "Unknown answer:");
    Debug_Error("Communication", "SafeBox_CheckIfPackageDeposited", answer);
    return false;
}

/**
 * @brief Asks SafeBox to return its current
 * status. This is used when an alarm is
 * detected for example and allows both SafeBox
 * and XFactor to exchange their current status at
 * any time. Use Status functions to set and
 * compare their status. This one is only used to
 * exchange the status.
 *
 * @attention
 * This function needs to be called at roughly
 * periodic intervals or whenever a significant
 * event happens. XFactor is the one to initiate
 * this handshake, not SafeBox.
 *
 * @return true:
 * Successfully exchanged the status.
 * @return false:
 * Failed to exchange the status.
 */
bool SafeBox_ExchangeStatus()
{
    // - VARIABLES - //
    String command = COMMAND_STATUS_EXCHANGE;
    String statusEnding = "     ";
    String answer = "     ";
    XFactor_Status currentStatus = XFactor_GetStatus();

    // - Get the command ending.
    switch(currentStatus)
    {
        case(XFactor_Status::Alarm):                    statusEnding = "A";     break;
        case(XFactor_Status::CalculatingRouteHome):     statusEnding = "CRH";   break;
        case(XFactor_Status::CommunicationError):       statusEnding = "CE";    break;
        case(XFactor_Status::ConfirmingDropOff):        statusEnding = "CDO";   break;
        case(XFactor_Status::DroppingOff):              statusEnding = "DO";    break;
        case(XFactor_Status::EnteringSafeBox):          statusEnding = "ESB";   break;
        case(XFactor_Status::Error):                    statusEnding = "E";     break;
        case(XFactor_Status::ExaminatingAPackage):      statusEnding = "EAP";   break;
        case(XFactor_Status::LeavingSafeBox):           statusEnding = "LSB";   break;
        case(XFactor_Status::Maintenance):              statusEnding = "M";     break;
        case(XFactor_Status::NoPackageFound):           statusEnding = "NPF";   break;
        case(XFactor_Status::Off):                      statusEnding = "O";     break;
        case(XFactor_Status::PackageDropOffFailed):     statusEnding = "PDOF";  break;
        case(XFactor_Status::PackageExaminationFailed): statusEnding = "PEF";   break;
        case(XFactor_Status::PackagePickUpFailed):      statusEnding = "PPUF";  break;
        case(XFactor_Status::PreparingForDropOff):      statusEnding = "PFDO";  break;
        case(XFactor_Status::PreparingForTheSearch):    statusEnding = "PFTS";  break;
        case(XFactor_Status::ReturningHome):            statusEnding = "RH";    break;
        case(XFactor_Status::SearchingForAPackage):     statusEnding = "SFAP";  break;
        case(XFactor_Status::WaitingForDelivery):       statusEnding = "WFD";   break;
        case(XFactor_Status::WaitingAfterSafeBox):      statusEnding = "WASB";  break;

        default:
            Debug_Error("Communication", "SafeBox_ExchangeStatus", "Unknown XFactor status");
            Debug_Error("Communication", "SafeBox_ExchangeStatus", String((int)currentStatus));
            return false;
    }

    // - Build command string and send it
    command.concat(statusEnding);
    answer = BT_MessageExchange(command, COMMS_TIMEOUT_MS);
    BT_ClearAllMessages();

    if (answer == BT_ERROR_MESSAGE)
    {
        Debug_Error("Communication", "SafeBox_ExchangeStatus", "BT_MessageExchange Failed");
        BT_ClearAllMessages();
        return false;
    }

    // - ANSWER CHECK - //
    if(answer.endsWith("CE"))   {SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);  BT_ClearAllMessages(); return true;}
    if(answer.endsWith("O"))    {SafeBox_SetNewStatus(SafeBox_Status::Off);                 BT_ClearAllMessages(); return true;}
    if(answer.endsWith("WFX"))  {SafeBox_SetNewStatus(SafeBox_Status::WaitingForXFactor);   BT_ClearAllMessages(); return true;}
    if(answer.endsWith("WFD"))  {SafeBox_SetNewStatus(SafeBox_Status::WaitingForDelivery);  BT_ClearAllMessages(); return true;}
    if(answer.endsWith("WFRI")) {SafeBox_SetNewStatus(SafeBox_Status::WaitingForRetrieval); BT_ClearAllMessages(); return true;}
    if(answer.endsWith("WFR"))  {SafeBox_SetNewStatus(SafeBox_Status::WaitingForReturn);    BT_ClearAllMessages(); return true;}
    if(answer.endsWith("RFDO")) {SafeBox_SetNewStatus(SafeBox_Status::ReadyForDropOff);     BT_ClearAllMessages(); return true;}
    if(answer.endsWith("U"))    {SafeBox_SetNewStatus(SafeBox_Status::Unlocked);            BT_ClearAllMessages(); return true;}
    if(answer.endsWith("DO"))   {SafeBox_SetNewStatus(SafeBox_Status::DroppingOff);         BT_ClearAllMessages(); return true;}
    if(answer.endsWith("M"))    {SafeBox_SetNewStatus(SafeBox_Status::Maintenance);         BT_ClearAllMessages(); return true;}
    if(answer.endsWith("E"))    {SafeBox_SetNewStatus(SafeBox_Status::Error);               BT_ClearAllMessages(); return true;}
    if(answer.endsWith("A"))    {SafeBox_SetNewStatus(SafeBox_Status::Alarm);               BT_ClearAllMessages(); return true;}

    Debug_Error("Communication", "SafeBox_ExchangeStatus", "Received answer did not match:");
    Debug_Error("Communication", "SafeBox_ExchangeStatus", answer);

    BT_ClearAllMessages();
    return false;
}

//#pragma endregion

//#pragma region [Getters]

/**
 * @brief Asks SafeBox to give XFactor its
 * current package lid state.
 * @return true: The lid is opened
 * @return false: The lid is closed
 */
bool SafeBox_GetLidState()
{
    // - VARIABLES - //
    String answer = "";

    answer = BT_MessageExchange(COMMAND_LID_GET, COMMS_TIMEOUT_MS);
    if(answer == ANSWER_LID_OPEN) return true;
    if(answer == ANSWER_LID_CLOSED) return false;

    Debug_Error("Communication", "SafeBox_GetLidState", "Unknown answer:");
    Debug_Error("Communication", "SafeBox_GetLidState", answer);
    return false;
}

/**
 * @brief Asks SafeBox to give XFactor its
 * current garage status.
 * @return true: The garage is opened
 * @return false: The garage is closed
 */
bool SafeBox_GetGarageState()
{
    // - VARIABLES - //
    String answer = "";

    answer = BT_MessageExchange(COMMAND_GARAGE_GET, COMMS_TIMEOUT_MS);
    if(answer == ANSWER_GARAGE_OPEN) return true;
    if(answer == ANSWER_GARAGE_CLOSED) return false;

    Debug_Error("Communication", "SafeBox_GetGarageState", "Unknown answer:");
    Debug_Error("Communication", "SafeBox_GetGarageState", answer);
    return false;
}

/**
 * @brief Asks SafeBox to tell XFactor how many
 * packages are currently deposited inside of it
 * @return int: How many packages are currently
 * deposited.
 */
int SafeBox_GetPackagesDeposited()
{
    // Cant be asked, won't use it.
    // But yk, its there if somehow
    // we end up using it.
    return -1;
}

/**
 * @brief Asks SafeBox to tell XFactor the
 * current Doorbell status. This allowed it to
 * know if it should start the package retrieval
 * or not.
 * @return true: The doorbell was activated.
 * @return false: The doorbell wasn't activated.
 */
bool SafeBox_GetDoorBellStatus()
{
    // - VARIABLES - //
    String answer = "";

    answer = BT_MessageExchange(COMMAND_DOORBELL_GET, COMMS_TIMEOUT_MS);
    if(answer == ANSWER_DOORBELL_RANG) return true;
    if(answer == ANSWER_DOORBELL_NOT_RANG) return false;

    Debug_Error("Communication", "SafeBox_GetDoorBellStatus", "Unknown answer:");
    Debug_Error("Communication", "SafeBox_GetDoorBellStatus", answer);
    return false;
}

//#pragma endregion