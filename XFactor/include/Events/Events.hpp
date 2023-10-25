/**
 * @file Events.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief This file contains the definition
 * of event functions. Event functions are
 * called whenever their associated events
 * happen during execution of XFactor's code.
 * In the event functions, things such as
 * checking for alarms and packages must be
 * done.
 * @version 0.1
 * @date 2023-10-24
 * @copyright Copyright (c) 2023
 */

#pragma once

#pragma region [Movement_Related]

/**
 * @brief Periodically executed when the
 * robot is moving in a straight line.
 * Depending on the robot's current mode 
 * this function will be behaving
 * differently. This function's goal is 
 * to tell if the robot should still be
 * moving and to change its mode if
 * something happens.
 * 
 * @param executionRatio
 * Ratio from 0.0f to 1.0f that tells
 * you how close to being finished the
 * event is. 0 mean the event just
 * started, <1 means the event is in
 * progress and 1 means the event just 
 * finished.
 * @param direction
 * Value from 0 to 1. 0 means
 * the robot is moving forwards. 1 means
 * the robot is moving backwards.
 * @return true:
 * The robot can continue to move
 * @return false:
 * The robot needs to stop.
 */
bool MovingEvent(float executionRatio, unsigned char direction);

/**
 * @brief Periodically executed when the 
 * robot is turning on itself. Depending 
 * on the robot's current mode, this 
 * function will be behaving differently. 
 * This function's goal is to tell if the 
 * robot should still be turning and to 
 * change its mode if something happens.
 * 
 * @param executionRatio
 * Ratio from 0.0f to 1.0f that tells
 * you how close to being finished the
 * event is. 0 mean the event just 
 * started, <1 means the event is in 
 * progress and 1 means the event just 
 * finished.
 * @param direction
 * Value from 0 to 1. 0 means the robot
 * is turning left. 1 means the robot is 
 * turning right.
 * @return true:
 * The robot can continue to turn
 * @return false:
 * The robot needs to stop.
 */
bool TurningEvent(float executionRatio, unsigned char direction);

#pragma endregion

#pragma region [Communication_Related]

/**
 * @brief Event called once before any
 * communication is executed. This is called at
 * the very start of @ref BT_MessageExchange.
 * This function is used to change LED statuses
 * and do other things that might need to be done
 * before a communication can take place.
 * 
 * @param messageToBeSent
 * Message that will be sent to the UART module
 * after this function finishes.
 * @return true:
 * The message can be sent to the UART module.
 * @return false:
 * Do not send the message to the UART module.
 */
bool BT_BeforeCommunicationEvent(string messageToBeSent);

/**
 * @brief Event called once after a message is
 * sent to the UART Bluetooth module. This is
 * called after @ref BT_SendString executes and
 * sent a message to the Bluetooth module.
 * 
 * @param messageSent
 * The message that was sent to the Bluetooth
 * module.
 * @return true:
 * Event handled successfully
 * @return false:
 * Something went wrong during the event.
 */
bool BT_MessageSentEvent(string messageSent);

/**
 * @brief Event periodically called at an interval
 * of milliseconds while the program waits for a
 * new message to be received and stored inside
 * of the UART buffer. This function is called
 * periodically at a consistant interval while the
 * program waits. This function can be used to
 * change the LED colours dynamically or do other
 * things while the program waits to receive a
 * message.
 * 
 * @param executionRatio
 * Ratio from 0.0f to 1.0f that tells
 * you how close to being finished the
 * event is. 0 mean the event just
 * started, <1 means the event is in
 * progress and 1 means the event just 
 * finished.
 * @return true:
 * The event was handled successfully.
 * @return false:
 * The event failed to be handled.
 */
bool BT_WaitingForMessageEvent(float executionRatio);

/**
 * @brief Event called once after a message
 * exchange between XFactor and SafeBox was
 * successful. Allows the robot to change LED
 * status or perform other actions depending on
 * the received messages or the message that was
 * sent.
 * 
 * @param messageSent
 * A string corresponding to the message that was
 * sent to SafeBox.
 * @param messageReceived
 * A string corresponding to the message that was
 * received from SafeBox as an answer to the
 * message that was sent. 
 * @return true:
 * The event was handled successfully.
 * @return false:
 * The event failed to be handled successfully.
 */
bool BT_MessageExchangeSuccessEvent(string messageSent, string messageReceived);

/**
 * @brief Event that is called once when a
 * message is received by XFactor. This allows
 * the program to change LED color or perform
 * other necessary actions depending on the
 * received message from SafeBox
 * 
 * @param receivedMessage
 * A string corresponding to the message that was
 * received from SafeBox as an answer to the
 * message that was sent. 
 * @return true:
 * The event was handled successfully.
 * @return false:
 * The event failed to be handled.
 */
bool BT_MessageReceivedEvent(string receivedMessage);

/**
 * @brief Event called once whenever the BT
 * communication timesout and no message was
 * received during the allocated time.
 * 
 * @param timeoutDuration
 * Value corresponding to the amount of
 * milliseconds that was allocated for the
 * message to be received.
 * @return true:
 * The event was handled successfully
 * @return false:
 * The event failed to be handled.
 */
bool BT_TimeoutEvent(int timeoutDuration);

#pragma endregion