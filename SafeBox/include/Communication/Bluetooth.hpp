/**
 * @file Bluetooth.hpp
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

#pragma once

// - INCLUDES - //
#include "Debug/Debug.hpp"
#include "Arduino.h"

// - DEFINES - //
/// @brief The baudrate of the Serial port which talks to the HC-05 module.
#define BT_HC05_BAUDRATE 19200
/// @brief Serial port used on an Arduino Mega. Where the HC-05 BT module will be connected.
#define BT_SERIAL Serial1
/// @brieg Serial event called by Arduino when a character is received. MUST BE LINKED WITH @ref BT_SERIAL
#define BT_SERIAL_EVENT void serialEvent1()
/// @brief How big in bytes can a message be until its discarded for being gibberish?
#define BT_MAX_MESSAGE_LENGTH 200
/// @brief How many strings can the message buffer receive before it overflows?
#define BT_SIZE_OF_MESSAGE_BUFFER 4
/// @brief Returned by BT_GetMessage functions to indicate that there was no message to get.
#define BT_NO_MESSAGE "%_NAN_%"
/// @brief Returned whenever an error occur in a function that returns and handles communications.
#define BT_ERROR_MESSAGE "%_ERROR_%"

#define BT_NEVER_RECEIVED_MESSAGE "%_ARDUINO_SUCKS_W_MEMORY%"

/**
 * @brief Function that initialises Bluetooth on
 * an Arduino ATMEGA using an external UART
 * module. It initialises the used pins correctly
 * and makes SafeBox ready for Bluetooth
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
bool BT_Init();

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
bool BT_SendString(String message);

/**
 * @brief Simple function that checks how many
 * messages are currently available for reading
 * and parsing inside of the UART buffer where
 * the Bluetooth device is connected.
 *
 * @return int: 0: no messages, >0: some messages
 */
int BT_MessagesAvailable();

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
bool BT_ClearAllMessages();

/**
 * @brief Will block the program for a specified
 * amount of milliseconds unless a Bluetooth
 * message is received during the specified time
 * reception window. If no messages is detected,
 * the function will return a fail.
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
bool BT_WaitForAMessage(int millisecondsTimeOut);

/**
 * @brief Returns a string which corresponds to
 * the oldest message stored in the reception
 * buffer of the device. If there is no messages
 * available, the string will be empty
 *
 * @attention
 * It is preferred that @ref BT_MessagesAvailable
 * is called before you call this function.
 *
 * @return string: The oldest message stored in
 * the reception buffer.
 */
String BT_GetLatestMessage();

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
String BT_MessageExchange(String message, int millisecondsTimeOut);

/**
 * @brief
 * This function used to be the function that
 * allowed interrupts to read from the serial
 * port. But as we know it, Arduino's string
 * classes as well as stack managaement sucks
 * ass. This function is called whenever we
 * perform a message exchange to make sure that
 * message was gathered correctly.
 * @return String:
 * "SWEET_FUCK_ALL": No message were found.
 */
String GetMessage(int millisecondsTimeOut);

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
String MessageBuffer(String newMessage, unsigned char bufferIndex, int action);

