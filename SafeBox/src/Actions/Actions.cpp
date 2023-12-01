/**
 * @file Actions.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief File which contains the functions used
 * throughout the program. These action functions
 * are functions that execute a certain step of
 * SafeBox such as waiting after XFactor, being
 * unlocked, waiting for a doorbell and so on.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDES - //
#include "Actions/Actions.hpp"

/**
 * @brief
 * Global variable that is local to the action.c
 * file. This stores the current action's number
 * so that @ref Execute_CurrentFunction can be
 * executed properly without needing an input
 * parameter.
 * @attention
 * DO NOT USE THIS INSIDE EXECUTION FUNCTIONS.
 * THIS SHOULD ONLY BE USED IN ACTION HANDLERS.
 */
static unsigned char currentFunctionID = 0;

//#pragma region [ACTION_HANDLERS]
/**
 * @brief Function periodically called in void
 * loop. This function is a massive switch case
 * that checks which execution / action function
 * should currently be executed by SafeBox. To
 * specify that function, use
 * @ref SetNewExecutionFunction using defines
 * available in this header file.
 */
void Execute_CurrentFunction()
{
    switch(currentFunctionID){

        case(FUNCTION_ID_ALARM):
            Execute_Alarm();
            break;

        case(FUNCTION_ID_SAVE_NEW_CARD):
            Execute_SaveNewEEPROMCard();
            break;

        case(FUNCTION_ID_ERROR):
            Execute_Error();
            break;

        case(FUNCTION_ID_WAIT_AFTER_XFACTOR):
            Execute_WaitAfterXFactor();
            break;

        case(FUNCTION_ID_START_OF_DELIVERY):
            Execute_StartOfDelivery();
            break;

        case(FUNCTION_ID_WAIT_FOR_DELIVERY):
            Execute_WaitForDelivery();
            break;

        case(FUNCTION_ID_UNLOCKED):
            Execute_Unlocked();
            break;

        case(FUNCTION_ID_DROP_OFF):
            Execute_DropOff();
            break;

        case(FUNCTION_ID_WAIT_FOR_RETRIEVAL):
            Execute_WaitForRetrieval();
            break;

        case(FUNCTION_ID_WAIT_FOR_RETURN):
            Execute_WaitForReturn();
            break;

        case(FUNCTION_ID_END_OF_PROGRAM):
            Execute_EndOfProgram();
            break;

        default:
            // The current function ID is not recognized.
            // Error is executed by default.
            Execute_Error();
            return;
    }
    // Successful execution //
}

/**
 * @brief
 * Function that specifies a new function ID
 * so that @ref Execute_CurrentFunction can
 * execute that function instead of the current
 * one that is executed. This is used to change
 * execution functions.
 *
 * @warning
 * DO NOT call execution functions outside of
 * @ref Execute_CurrentFunction. Programming in
 * that manner WILL cause stack overflows
 * because functions won't ever return to
 * previous ones.
 *
 * @param functionID
 * The ID of the function to execute. IDs are
 * available as defines in this document.
 * @return true:
 * Successfully changed the execution function
 * to the new ID.
 * @return false:
 * Failed to change the execution function to the
 * new ID. May be out of range.
 */
bool SetNewExecutionFunction(unsigned char functionID)
{
    switch(currentFunctionID){

        case(FUNCTION_ID_ALARM):
        case(FUNCTION_ID_END_OF_PROGRAM):
        case(FUNCTION_ID_ERROR):
        case(FUNCTION_ID_START_OF_DELIVERY):
        case(FUNCTION_ID_WAIT_FOR_DELIVERY):
        case(FUNCTION_ID_UNLOCKED):
        case(FUNCTION_ID_DROP_OFF):
        case(FUNCTION_ID_WAIT_AFTER_XFACTOR):
        case(FUNCTION_ID_WAIT_FOR_RETRIEVAL):
        case(FUNCTION_ID_WAIT_FOR_RETURN):
        case(FUNCTION_ID_SAVE_NEW_CARD):
            // The specified function is indeed a valid function ID.
            currentFunctionID = functionID;
            return true;

        default:
            // The current function ID is not recognized.
            // The function ID will therefor not be set.
            return false;
    }
}

/**
 * @brief
 * Function that returns the value of the current
 * action/execution function that is being
 * executed in @ref Execute_CurrentFunction.
 * @return unsigned char:
 * function ID
 */
unsigned char GetCurrentExecutionFunction()
{
    return currentFunctionID;
}
//#pragma endregion

//#pragma region [ACTION_FUNCTIONS]

/**
 * @brief
 * Attemps to save a new card in the EEPROM
 * until it fails or succeed. If it fails, it
 * will return to the unlocked state.
 */
void Execute_SaveNewEEPROMCard()
{
    // - VARIABLES - //
    static bool flipLed = false;
    String readCard = "";

    SafeBox_CheckAndExecuteMessage();

    // - LED HANDLING - //
    if(ExecutionUtils_LedBlinker(1000))
    {
        flipLed = !flipLed;
        if(flipLed)
        {
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, 32,16,64);
        }
        else
        {
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, 32,64,16);
        }
    }

    readCard = RFID_GetCardNumber();
    if(readCard.compareTo(RFID_NO_CARDS_READ) != 0)
    {
        Debug_Information("Actions", "Execute_WaitAfterXFactor", "A card was found");
        if(!RFID_StoreCardInEEPROM(readCard))
        {
            Debug_Error("Actions", "Execute_WaitAfterXFactor", "Failed to use EEPROM");
            SetNewExecutionFunction(FUNCTION_ID_ERROR);
            return;
        }
        else
        {
            /// @brief Too lazy to make a function for this. Especially cuz this is surplus and wasnt planned in the project.
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, 255,32,32);
            Alarm_SetState(true);
            delay(40);
            Alarm_SetState(false);
            delay(40);
            Alarm_SetState(true);
            delay(40);
            Alarm_SetState(false);
            delay(40);
            Alarm_SetState(true);
            delay(40);
            Alarm_SetState(false);
            delay(40);
            Alarm_SetState(true);
            delay(40);
            Alarm_SetState(false);
            SafeBox_SetNewStatus(SafeBox_Status::CommunicationError);
            SetNewExecutionFunction(FUNCTION_ID_UNLOCKED);
        }
    }
}


/**
 * @brief
 * Action function executed once at the
 * start of the program. This function's
 * purpose is to make SafeBox wait
 * until XFactor is online and is sending
 * Bluetooth commands.
 *
 * This function simply waits for a
 * status exchange from XFactor each
 * second. LED status should clearly
 * indicate that SafeBox is awaiting
 * XFactor to begin its program.
 *
 * SafeBox will not do anything until
 * XFactor begins to send to Bluetooth commands.
 * It simply call the function that allows to
 * reply to Bluetooth commands and set
 * @ref Execute_Unlocked as the next execution
 * command.
 */
void Execute_WaitAfterXFactor()
{
    Debug_Start("Execute_WaitAfterXFactor");
    static float currentRadian = 0;
    float currentRatio = 0;
    SafeBox_SetNewStatus(SafeBox_Status::WaitingForXFactor);

    currentRadian = currentRadian + 0.1f;
    if(currentRadian>3.14)
    {
        currentRadian = 0;
    }
    currentRatio = sin(currentRadian);
    Debug_Information("Actions", "Execute_WaitAfterXFactor", "Handling LED");
    if(!LEDS_SetColor(LED_ID_STATUS_INDICATOR, (unsigned char)(32.0f*currentRatio), (unsigned char)(32.0f*currentRatio), 0))
    {
        Debug_Error("Actions", "Execute_WaitAfterXFactor", "Failed to set WS2812");
        Debug_End();
        return;
    }

    Debug_Information("Actions", "Execute_WaitAfterXFactor", "Checking if user wants to save a new card");
    if(RFID_WantsToSaveNewCard())
    {
        Debug_Information("Actions", "Execute_WaitAfterXFactor", "User wants to save a new card");
        SetNewExecutionFunction(FUNCTION_ID_SAVE_NEW_CARD);
        Debug_End();
        return; 
    }

    Debug_Information("Actions", "Execute_WaitAfterXFactor", "Checking if messages needs to be executed");
    if(SafeBox_CheckAndExecuteMessage())
    {
        Debug_Information("Actions", "Execute_WaitAfterXFactor", "XFactor detected");
        SetNewExecutionFunction(FUNCTION_ID_UNLOCKED);
        BT_ClearAllMessages();
        Debug_End();
        return;
    }

    // - Allows the user to bypass the waiting for XFactor
    Debug_Information("Actions", "Execute_WaitAfterXFactor", "Checking waiting bypass");
    if(Doorbell_GetState() && (RFID_HandleCard() == 1))
    {
        Debug_Information("Actions", "Execute_WaitAfterXFactor", "Going to unlocked");
        if(!SetNewExecutionFunction(FUNCTION_ID_UNLOCKED))
        {
            Debug_Error("Actions", "Execute_WaitAfterXFactor", "Failed to set new execution function");
            SetNewExecutionFunction(FUNCTION_ID_ERROR);
        }
        Debug_End();
        return;
    }

    ExecutionUtils_HandleReceivedXFactorStatus();
    Debug_End();
}

/**
 * @brief
 * Action function that makes SafeBox wait after
 * the doorbell to tell XFactor to go retrieve a
 * package. If the box gets unlocked during this
 * time, the execution function should be set to
 * @ref Execute_Unlocked.
 *
 * If a doorbell is detected, the next execution
 * function should be
 * @ref Execute_WaitForDelivery
 */
void Execute_WaitForDelivery()
{
    // CHECKS IF ITS THE FIRST EXECUTION
    if(SafeBox_GetStatus() != SafeBox_Status::WaitingForDelivery)
    {
        Debug_Information("Actions", "Execute_WaitForDelivery", "Start of wait for delivery");
        SafeBox_SetNewStatus(SafeBox_Status::WaitingForDelivery);
    }

    if(!Lid_IsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "LID IS NO LONGER CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;       
    }

    if(!SafeBox_SetNewStatus(SafeBox_Status::WaitingForDelivery))
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "Failed to set status");
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
        return;
    }

    if(!ExecutionUtils_HandleArmedUnlocking())
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "New execution function set");
        return;
    }

    if(!Garage_Close())
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "Failed to close garage door");
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
        return;
    }

    if(!Lid_Lock())
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "Failed to lock lid door");
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
        return;
    }

    if(RFID_WantsToSaveNewCard())
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "Someone wants to save a new card");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return; 
    }

    LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_WAITFORDELIVERY);
    SafeBox_CheckAndExecuteMessage();

    if(!ExecutionUtils_CheckIfGarageIsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "GARAGE IS NOT CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;
    }

    ExecutionUtils_HandleReceivedXFactorStatus();
    if (Doorbell_GetState())
    {
        LEDS_SetColor(LED_ID_STATUS_INDICATOR,LED_COLOR_ARMED);
        SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
        return;
    }
}

/**
 * @brief
 * This execution functions needs to reply to
 * XFactor that a doorbell was heard and that it
 * needs to start executing its package
 * retrieval. Once XFactor exchanges a status
 * that no longer says that its waiting for the
 * doorbell, the next execution function should
 * be executed: @ref Execute_WaitForRetrieval
 */
void Execute_StartOfDelivery()
{
    if(SafeBox_GetStatus() != SafeBox_Status::WaitingForRetrieval)
    {
        XFactor_SetNewStatus(XFactor_Status::WaitingForDelivery);
    }

    if(!Lid_IsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "LID IS NO LONGER CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;       
    }

    if(RFID_WantsToSaveNewCard())
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "Someone wants to save a new card");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return; 
    }

    if(!ExecutionUtils_CheckIfGarageIsClosed())
    {
        Debug_Warning("Actions", "Execute_StartOfDelivery", "GARAGE IS NOT CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;
    }

    SafeBox_SetNewStatus(SafeBox_Status::WaitingForRetrieval);
    LEDS_SetColor(LED_ID_STATUS_INDICATOR,LED_COLOR_ARMED);
    SafeBox_CheckAndExecuteMessage();
    ExecutionUtils_HandleArmedUnlocking();
    ExecutionUtils_HandleReceivedXFactorStatus();
    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_RETRIEVAL);
}

/**
 * @brief
 * Execution function that is executed when
 * SafeBox awaits XFactor to find a package. Its
 * sole goal is to reply to XFactor's status
 * exchanges as they come and be ready to unlock
 * if it has to. If XFactor says that it has a
 * package, the next execution function should
 * simply be @ref Execute_DropOff.
 *
 * Should continuously try to execute received
 * commands from XFactor and engage different
 * modes if status are such as alarm or error.
 */
void Execute_WaitForRetrieval()
{
    SafeBox_SetNewStatus(SafeBox_Status::WaitingForRetrieval);
    LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ARMED);

    if(RFID_WantsToSaveNewCard())
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "Someone wants to save a new card");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return; 
    }

    if(!Lid_IsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "LID IS NO LONGER CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;       
    }

    if(!ExecutionUtils_CheckIfGarageIsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForRetrieval", "GARAGE IS NOT CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;
    }

    SafeBox_CheckAndExecuteMessage();
    ExecutionUtils_HandleReceivedXFactorStatus();
    ExecutionUtils_HandleArmedUnlocking();
}

/**
 * @brief
 * Execution function that is executed whenever
 * XFactor is in the process of returning to the
 * garage. This is only called whenever XFactor
 * either fails to find a package or XFactor is
 * returning inside the garage after it has found a
 * package and dropped it off.
 *
 * Once XFactor confirms that it has entered the
 * garage through status exchanges, SafeBox
 * should go back to waiting after a doorbell.
 */
void Execute_WaitForReturn()
{
    SafeBox_SetNewStatus(SafeBox_Status::WaitingForReturn);
    LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ARMED);

    if(RFID_WantsToSaveNewCard())
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "Someone wants to save a new card");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return; 
    }

    if(!Lid_IsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "LID IS NO LONGER CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;       
    }

    if(!ExecutionUtils_CheckIfGarageIsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "GARAGE IS NOT CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;
    }

    SafeBox_CheckAndExecuteMessage();
    ExecutionUtils_HandleReceivedXFactorStatus();
    ExecutionUtils_HandleArmedUnlocking();
}

/**
 * @brief
 * Execution function that is executed whenever
 * XFactor approaches the drop off event. This
 * allows SafeBox to check what XFactor is
 * sending and be ready to quickly open the lid
 * and close it. This event is also happening
 * when there needs to be a confirmation of the
 * drop-off inside the box.
 */
void Execute_DropOff()
{
    SafeBox_SetNewStatus(SafeBox_Status::DroppingOff);
    LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ARMED);

    if(RFID_WantsToSaveNewCard())
    {
        Debug_Error("Actions", "Execute_WaitForDelivery", "Someone wants to save a new card");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return; 
    }

    if(!Lid_IsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "LID IS NO LONGER CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;       
    }

    if(!ExecutionUtils_CheckIfGarageIsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "GARAGE IS NOT CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;
    }

    SafeBox_CheckAndExecuteMessage();
    ExecutionUtils_HandleReceivedXFactorStatus();
    ExecutionUtils_HandleArmedUnlocking();
}

/**
 * @brief
 * Action function executed whenever the box is
 * in its unlocked mode which allows the user to
 * freely handle it as well as XFactor. This
 * function must reply to XFactor communication
 * with a status that specifies that it is
 * unlocked and that therefor XFactor should stop
 * and return to SafeBox.
 *
 * Must continously check if the box is being
 * locked or not, then it should go in
 * @ref Execute_WaitForDelivery
 */
void Execute_Unlocked()
{
    if(SafeBox_GetStatus() != SafeBox_Status::Unlocked)
    {
        XFactor_SetNewStatus(XFactor_Status::Unlocked);
    }

    if(!Garage_Open())
    {
        Debug_Error("Actions", "Execute_Unlocked", "Failed to close garage door");
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
        return;
    }

    if(RFID_WantsToSaveNewCard())
    {
        Debug_Information("Actions", "Execute_Unlocked", "User wants to save a new card");
        SetNewExecutionFunction(FUNCTION_ID_SAVE_NEW_CARD);
        return; 
    }

    if(!Lid_Unlock())
    {
        Debug_Error("Actions", "Execute_Unlocked", "Failed to unlock lid door");
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
        return;
    }

    LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
    SafeBox_SetNewStatus(SafeBox_Status::Unlocked);
    SafeBox_CheckAndExecuteMessage();

    if(!ExecutionUtils_CheckIfGarageIsClosed())
    {
        Debug_Warning("Actions", "Execute_WaitForDelivery", "GARAGE IS NOT CLOSED");
        SetNewExecutionFunction(FUNCTION_ID_ALARM);
        return;
    }

    if(RFID_HandleCard() == 1)
    {
        if(!Lid_IsClosed())
        {
            Debug_Information("Actions", "Execute_Unlocked", "Lid is not completely closed");
            /// @brief Too lazy to make a function for this. Especially cuz this is surplus and wasnt planned in the project.
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, 255,16,16);
            Alarm_SetState(true);
            delay(40);
            Alarm_SetState(false);
            delay(40);
            Alarm_SetState(true);
            delay(40);
            Alarm_SetState(false);
            delay(40);
            Alarm_SetState(true);
            delay(40);
            Alarm_SetState(false);
            delay(40);
            Alarm_SetState(true);
            delay(40);
            Alarm_SetState(false);
            return;
        }

        Debug_Information("Actions", "Execute_Unlocked", "Going to Wait for delivery");
        if(!SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_DELIVERY))
        {
            Debug_Error("Actions", "Execute_Unlocked", "Failed to set new execution function");
            SetNewExecutionFunction(FUNCTION_ID_ERROR);
        }
    }
}

/**
 * @brief
 * Action function that blocks and does not sets
 * a new execution function until the box gets
 * unlocked through RFID. If that happens, the
 * alarm should be stopped and the status of the
 * box should be changed to RESET.
 *
 * SafeBox will only reset to the
 * @ref Execution_Unlocked execution function
 * when XFactor says that its status is no longer
 * alarm.
 */
void Execute_Alarm()
{
    // - VARIABLES - //
    static bool mustBeOn = false;

    SafeBox_SetNewStatus(SafeBox_Status::Alarm);
    SafeBox_CheckAndExecuteMessage();
    ExecutionUtils_HandleReceivedXFactorStatus();
    // - LED & BUZZER BLINK - //
    if(ExecutionUtils_LedBlinker(100))
    {
        mustBeOn = !mustBeOn;

        if(mustBeOn)
        {
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ALARM);
            Alarm_SetState(true);
        }
        else
        {
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_OFFLINE);
            Alarm_SetState(false);

            // - RFID DISARM ALARM CHECKS - //
            if (RFID_HandleCard() == 1)
            {
                LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_OFFLINE);
                if(!SetNewExecutionFunction(FUNCTION_ID_UNLOCKED))
                {
                    Debug_Error("Actions", "Execute_Alarm", "Failed to set new execution function");
                    SetNewExecutionFunction(FUNCTION_ID_ERROR);
                }
                Alarm_SetState(false);
            }
        }
    }
}

/**
 * @brief
 * Action function that blocks until SafeBox is
 * reset. It should display an error blinking
 * pattern using the LEDs functions.
 *
 * Indicates that a fatal programming error
 * occurred during use.
 */
void Execute_Error()
{
    Debug_Warning("Actions", "Execute_Error", "ERROR REACHED. DEBUG STOPPED");
    Debug_Stop();

    // - VARIABLES - //
    static bool mustBeOn = false; // everything is closed

    // - PROGRAM - //
    //SafeBox_SetNewStatus(SafeBox_Status::Error);
    ExecutionUtils_HandleReceivedXFactorStatus();
    SafeBox_CheckAndExecuteMessage();

    // - LED BLINK - //
    if(ExecutionUtils_LedBlinker(1000))
    {
        mustBeOn = !mustBeOn;

        if(mustBeOn)
        {
            Debug_PrintLastError();
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ERROR);
        }
        else
        {
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_OFFLINE);
        }
    }
}

/**
 * @brief
 * Executed at the very end of the program once
 * XFactor has returned inside of the garage. The
 * box should remain locked until it is unlocked
 * since it is now containing a package.
 */
void Execute_EndOfProgram()
{
    // WE SHOULD NOT REACH THIS FUNCTION//
    ExecutionUtils_HandleReceivedXFactorStatus();
}

//#pragma endregion