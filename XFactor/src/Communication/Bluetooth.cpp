/**
 * @file Bluetooth.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief
 * Header file containing all the definitions of
 * Bluetooth functions used throughout the
 * SafeBox project. These functions allows the
 * robot to communicate with SafeBox and vise
 * versa.
 * @version 0.1
 * @date 2023-11-10
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Communication/Bluetooth.hpp"

// - GLOBAL LOCAL ACCESS - //
bool _messageReceived = false;

/**
 * @brief
 * This stupid function is the result of Arduino
 * wonderful String memory leaks! This function
 * handles a table of strings that contains the
 * messages received over Bluetooth that would
 * otherwise be global as its accessed by a lot
 * of Bluetooth functions.
 * @param newMessage
 * If you want to save a new message at a specified index, or concat a message.
 * @param bufferIndex
 * @param Action
 * 0: Read
 * 1: Write
 * 2: check if address is "E"mpty or "F"illed
 * @return String:
 * Always returns
 */
String MessageBuffer(String newMessage, unsigned char bufferIndex, int action)
{
    // Debug_Start("MessageBuffer");
    static String messageBuffer[BT_SIZE_OF_MESSAGE_BUFFER] = {};
    for(unsigned char messageIndex=0; messageIndex<BT_SIZE_OF_MESSAGE_BUFFER; messageIndex++)
    {
        messageBuffer[BT_SIZE_OF_MESSAGE_BUFFER].reserve(BT_MAX_MESSAGE_LENGTH);
    }

    if(bufferIndex >= BT_SIZE_OF_MESSAGE_BUFFER)
    {
        // Debug_Error("Bluetooth", "MessageBuffer", "Specified index is too large");
        // Debug_End();
        return BT_ERROR_MESSAGE;
    }

    if(action > 2 || action < 0)
    {
        // Debug_Error("Bluetooth", "MessageBuffer", "Specified action is wrong");
        // Debug_End();
        return BT_ERROR_MESSAGE;
    }

    // Is there a string at the specified index?
    if(action == 2)
    {
        // Debug_End();
        return (messageBuffer[bufferIndex].length()>0) ? "F" : "E";
    }

    //Read the buffer
    if(action == 0)
    {
        // Debug_End();
        return messageBuffer[bufferIndex];
    }

    //Write the buffer
    if(action == 1)
    {
        messageBuffer[bufferIndex] = newMessage;
        // Debug_End();
        return newMessage;
    }
    // Debug_Error("Bluetooth", "MessageBuffer", "Failed to execute specified action");
    // Debug_End();
    return BT_ERROR_MESSAGE;
}

/**
 * @brief
 * Wonderful function attempting to fix the bug
 * located in Init.cpp
 * @param newMessage
 * If "$$$" the message wont be overwrote.
 * @return String
 */
String CurrentMessage(String newMessage)
{
    String currentMessage = "";

    if(newMessage == "$$$")
    {
        return currentMessage;
    }

    currentMessage = newMessage;
    return "";
}

/**
 * @brief
 * ARDUINO function executed automatically
 * whenever Serial1 receives characters.
 * @attention
 * DO NOT CALL YOURSELF.
 */
BT_SERIAL_EVENT
{
    // - VARIABLES - //
    char receivedCharacter = 0;
    int messageBufferIndex = 0;
    static String _currentMessage = "";

    // Empty the internal buffer
    while(BT_SERIAL.available())
    {
        receivedCharacter = (char)BT_SERIAL.read();
        if(receivedCharacter >= 32 && receivedCharacter <= 126)
        {
            // Serial.print("RX: '");
            // Serial.print(receivedCharacter);
            // Serial.print("' before: \"");
            // Serial.print(_currentMessage);
            // Serial.print("\" after:\"");
            _currentMessage += receivedCharacter;
            // Serial.print(_currentMessage);
            // Serial.println("\"");
        }
        else
        {
            // END OF STRING
            if(receivedCharacter == '\n')
            {
                messageBufferIndex = BT_MessagesAvailable();

                if(messageBufferIndex > BT_SIZE_OF_MESSAGE_BUFFER-1)
                {
                    // Oh shit... Whos spamming? lmfao
                    Debug_Error("Bluetooth", "BT_SERIAL_EVENT", "BUFFER OVERFLOW. Message lost.");
                    _currentMessage = "";
                    _messageReceived = false;
                }
                else
                {
                    //receivedBTMessages[messageBufferIndex] = _currentMessage;
                    MessageBuffer(_currentMessage, messageBufferIndex, 1);
                    Debug_Information("Bluetooth", "BT_SERIAL_EVENT: new message: ", _currentMessage);
                    _currentMessage = "";
                    _messageReceived = true;
                }
            }
            else
            {
                if(receivedCharacter == '\r')
                {
                    // Expected. Disregarded
                }
                else
                {
                    Debug_Warning("Bluetooth", "BT_SERIAL_EVENT", "Unknown character");
                }
            }
        }

        /**
         * @brief
         * DO NOT TOUCH THIS DELAY.
         * Its the sole reason communication works.
         * Without it we go out of the interrupt, the
         * string of received characters will be lost
         * and so will your message.
         *
         * This allows more characters to be received while
         * the functions loops.
         */
        delay(10);
    }
}

/**
 * @brief Function that initialises Bluetooth on
 * an Arduino ATMEGA using an external UART
 * module. It initialises the used pins correctly
 * and makes XFactor ready for Bluetooth
 * communications.
 *
 * @attention
 * This must be called at the start of the
 * program inside of the setup function or
 * inside of a wider initialisation function.
 *
 * @return true:
 * Bluetooth was successfully initialised
 * @return false:
 * Bluetooth failed to be initialised
 */
bool BT_Init()
{
    BT_SERIAL.begin(BT_HC05_BAUDRATE);
    return true;

    // Reserves X amount of memory for the string to use and fill as data comes in
    // if(_currentMessage.reserve(BT_MAX_MESSAGE_LENGTH))
    // {
        // for(int index=0; index<BT_SIZE_OF_MESSAGE_BUFFER; index++)
        // {
            // if(receivedBTMessages[index].reserve(BT_MAX_MESSAGE_LENGTH))
            // {
                // SUCCESS
            // }
            // else
            // {
                // Debug_Error("Bluetooth", "BT_Init", "String buffer reserve failure");
                // return false;
            // }
        // }
        // return true;
    // }
    // else
    // {
        // Maybe the value of @ref BT_MAX_MESSAGE_LENGTH is too big.
        // Debug_Error("Bluetooth", "BT_Init", "String reserve failure");
        // return false;
    // }
}

/**
 * @brief Simple function that sends a string
 * through UART to the initialised Bluetooth
 * module to be sent to SafeBox.
 * @param message
 * A string containing the message that needs
 * to be sent to SafeBox. All messages must be
 * stored as DEFINES.
 * @return true:
 * The message was sent successfully.
 * @return false:
 * Failed to send the message.
 */
bool BT_SendString(String message)
{
    Debug_Start("BT_SendString");
    // - PRELIMINARY CHECKS - //
    if (message.length() > BT_MAX_MESSAGE_LENGTH)
    {
        Debug_Error("Bluetooth", "BT_SendString", "Message is too large.");
        Debug_End();
        return false;
    }

    // - FUNCTION EXECUTION - //
    Debug_Information("Bluetooth", "BT_SendString", message);
    unsigned int byteSent = BT_SERIAL.println(message);
    BT_SERIAL.flush();

    // - AFTERMATH CHECKS - //
    if (byteSent < message.length())
    {
        Debug_Error("Bluetooth", "BT_SendString", "Not enough bytes sent");
        Debug_End();
        return false;
    }
    Debug_End();
    return true;
}

/**
 * @brief Simple function that checks how many
 * messages are currently available for reading
 * and parsing inside of the UART buffer where
 * the Bluetooth device is connected.
 *
 * @return int: 0: no messages, >0: some messages
 */
int BT_MessagesAvailable()
{
    unsigned char messageCount = 0;

    for (unsigned char messageIndex=0; messageIndex<BT_SIZE_OF_MESSAGE_BUFFER; messageIndex++)
    {
        if(MessageBuffer("", messageIndex, 2) == String("F"))
        {
            messageCount++;
        }
        else
        {
            // Buffer is no longer full. No point in continuing to check other indexes.
            return messageCount;
        }
    }
    return messageCount;
}

/**
 * @brief Clears the buffer where messages are
 * stored. If any messages was not read before
 * this function is called, it will be forever
 * lost.
 *
 * @return true:
 * Successfully cleared the message buffer
 * @return false:
 * Failed to clear the message buffer.
 */
bool BT_ClearAllMessages()
{
    for(unsigned char messageIndex=0; messageIndex<BT_SIZE_OF_MESSAGE_BUFFER; messageIndex++)
    {
        MessageBuffer("", messageIndex, 1);
    }
    return true;
}

/**
 * @brief Will block the program for a specified
 * amount of milliseconds unless a Bluetooth
 * message is received during the specified time
 * reception window. If no messages is detected,
 * the function will return a fail.
 *
 * @bug
 * If the program runs for ~49 days, there is a
 * small window where, if this function is called
 * it will immediately timeout because the millis
 * value + the timeout in milliseconds will be
 * bigger than the max value of an unsigned long.
 * This is not fixed because it can't happen for
 * our project.
 *
 * @param millisecondsTimeOut
 * How long should the program wait for a message
 * @return true:
 * Successfully received a message during the
 * waiting window.
 * @return false:
 * Failed to receive a message / no message was
 * received.
 */
bool BT_WaitForAMessage(int millisecondsTimeOut)
{
    Debug_Start("BT_WaitForAMessage");
    // - VARIABLES - //
    unsigned long currentTime = millis();
    unsigned long timeOutTime = currentTime + millisecondsTimeOut;
    int messagesReceived = BT_MessagesAvailable();

    // - PRELIMINARY CHECKS - //
    if(messagesReceived > 0)
    {
        // Why wait for a message when there is already some waiting for you?
        Debug_End();
        return true;
    }

    if(timeOutTime < currentTime)
    {
        Debug_Error("Bluetooth", "BT_WaitForAMessage", "Time out millis overflow. (How tf did you manage that?)");
        Debug_End();
        return false;
    }

    while(currentTime < timeOutTime)
    {
        currentTime = millis();
        messagesReceived = BT_MessagesAvailable();
        if(messagesReceived > 0)
        {
            // Less go, we finally got some messages fr
            Debug_End();
            return true;
        }

        if(_messageReceived)
        {
            // FOUND MESSAGE LESS GO
            Debug_Information("Bluetooth", "BT_WaitForAMessage", "Flag raised");
            Debug_End();
            return true;
        }

        serialEvent1();
    }
    Debug_Warning("Bluetooth", "BT_WaitForAMessage", "Timedout");
    Debug_End();
    return false;
}

/**
 * @brief Returns a string which corresponds to
 * the oldest message stored in the reception
 * buffer of the device. If there is no messages
 * available, the string will be empty
 *
 * @attention
 * This function calls @ref BT_MessagesAvailable
 * and returns @ref BT_NO_MESSAGE if there is no
 * available message to return.
 *
 * @warning
 * THIS WILL AUTOMATICALLY CLEAR THE BUFFER
 * OF THE LATEST MESSAGE.
 *
 * @return string: The oldest message stored in
 * the reception buffer.
 */
String BT_GetLatestMessage()
{
    // - VARIABLES - //
    int availableMessages = 0;
    String oldestMessage = BT_NO_MESSAGE;

    // - PRELIMINARY CHECKS - //
    availableMessages = BT_MessagesAvailable();
    if(availableMessages == 0) return BT_NO_MESSAGE;

    // - FUNCTION EXECUTION - //
    oldestMessage = MessageBuffer("", 0, 0);

    // brings buffer forwards by one.
    if(BT_SIZE_OF_MESSAGE_BUFFER>1)
    {
        for(unsigned char messageIndex = 0; messageIndex<BT_SIZE_OF_MESSAGE_BUFFER-1; messageIndex++)
        {
            String message = MessageBuffer("", messageIndex+1, 0);
            MessageBuffer(message, messageIndex, 1);
        }
    }
    // Clear last message to avoid duplicates
    MessageBuffer("", BT_SIZE_OF_MESSAGE_BUFFER-1, 1);
    return oldestMessage;
}

/**
 * @brief Function that firstly clears the UART
 * buffer to empty it of any messages, then sends
 * a specified message to the UART Bluetooth
 * module. Once that is done, the function waits
 * for a message to be received to return it or
 * waits for the timeout to reach its specified
 * value.
 *
 * @attention
 * Its recommended to use this function rather
 * than the other BT functions as this one
 * handles everything there is to handle.
 *
 * @warning
 * This function automatically calls
 * @ref BT_BeforeCommsEvent
 * @ref BT_CommsSuccessEvent
 * @ref BT_CommsFailedEvent
 *
 * @param message
 * A string containing the message that needs
 * to be sent to SafeBox. All messages must be
 * stored as DEFINES.
 * @param millisecondsTimeOut
 * How long should the program wait for a message
 * @return string: The received message from
 * SafeBox.
 */
String BT_MessageExchange(String message, int millisecondsTimeOut)
{
    Debug_Start("BT_MessageExchange");
    // Firstly send the message.
    _messageReceived = false;
    if(!BT_SendString(message))
    {
        Debug_Error("Bluetooth", "BT_MessageExchange", "TX failure");
        Debug_End();
        return BT_ERROR_MESSAGE;
    }

    // WARNING: Could already be a message in the buffer. Be careful to check for that.
    if(BT_WaitForAMessage(millisecondsTimeOut))
    {
        Debug_End();
        return BT_GetLatestMessage();
    }
    else
    {
        Debug_Warning("Bluetooth", "BT_MessageExchange", "Failed to get a reply");
        Debug_End();
        return BT_ERROR_MESSAGE;
    }
}
