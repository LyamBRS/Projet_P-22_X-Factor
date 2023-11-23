/**
 * @file Actions.hpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief File which contains the header
 * definition of action functions used
 * throughout the program. These action functions
 * are functions that execute a certain step of
 * XFactor such as getting out of the garage,
 * finding a package, picking up a package,
 * returning home, depositing a package etc.
 * @version 0.1
 * @date 2023-10-25
 * @copyright Copyright (c) 2023
 */

#pragma once

// - INCLUDES - //
#include <LibRobus.h>
#include "Alarm/Alarm.hpp"
#include "LED/LED.hpp"
#include "Movements/Movements.hpp"
#include "Package/Package.hpp"
#include "Actions/Utils.hpp"

//#pragma region [OTHER] // WILL BE CHANGED WHEN MORE OF EM SHOW UP

#define MAX_COMMUNICATION_ATTEMPTS 5
#define MAX_PICKUP_ATTEMPTS 5

//#pragma endregion

//#pragma region DISTANCES // WILL BE MIGRATED TO MOVEMENTS

#define STRAIGHT 0.0f
#define TURN_90_RIGHT TURN_90 * TURN_RIGHT
#define TURN_90_LEFT TURN_90 * TURN_LEFT

#define SAFEBOX_LENGTH_CM 50.0f // PUT REAL DIMENSIONS HERE
#define SAFEBOX_WIDTH_CM 50.0f // PUT REAL DIMENSIONS HERE

#define DEMO_AREA_WIDTH_CM 200.0f
#define DEMO_AREA_LENGTH_CM 200.0f

#define SCANNABLE_AREA_WIDTH 25.0f

#define ROBOT_WIDTH_CM 25.0f
#define ROBOT_LENGTH_CM 25.0f

//#pragma endregion

//#pragma region [ACTION_HANDLERS]
/**
 * @brief Function periodically called in void
 * loop. This function is a massive switch case
 * that checks which execution / action function
 * should currently be executed by XFactor. To
 * specify that function, use
 * @ref SetNewExecutionFunction using defines
 * available in this header file.
 */
void Execute_CurrentFunction();

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
bool SetNewExecutionFunction(unsigned char functionID);

/**
 * @brief
 * Function that returns the value of the current
 * action/execution function that is being
 * executed in @ref Execute_CurrentFunction.
 * @return unsigned char:
 * function ID
 */
unsigned char GetCurrentExecutionFunction();
//#pragma endregion

//#pragma region [ACTION_FUNCTIONS]
/**
 * @brief
 * Action function executed once at the start of
 * the program. This function's purpose is to
 * make XFactor wait until SafeBox is online and
 * is answering Bluetooth commands.
 *
 * This function simply sends a status exchange
 * to SafeBox each second and waits until a time
 * out for its reply. LED status should clearly
 * indicate that XFactor is awaiting SafeBox to
 * begin its program.
 *
 * XFactor will not do anything until SafeBox
 * begins to reply to Bluetooth commands.
 */
void Execute_WaitAfterSafeBox();

/**
 * @brief
 * Action function that makes XFactor wait for
 * SafeBox to tell it that a package is received
 * and that it must go fetch it. LED status must
 * clearly indicate that XFactor is waiting for
 * SafeBox to give it the go.
 *
 * Communication requests must be done each
 * second to avoid spamming SafeBox with data.
 *
 * @attention
 * XFactor should remain stuck in this action
 * function until SafeBox gives it the go.
 * After which, XFactor should go to
 * @ref Execute_GettingOutOfGarage
 *
 * @warning
 * Depending on SafeBox's status, XFactor will
 * either be armed or disarmed.
 */
void Execute_WaitForDelivery();

/**
 * @brief
 * Action function that makes XFactor leave
 * SafeBox's garage. Firstly, XFactor must ask
 * SafeBox to open the garage. Once SafeBox
 * confirms that the garage is open, XFactor must
 * drive out of it. This action function should
 * update XFactor's status to match with what its
 * doing. Once XFactor is outside of the garage,
 * they need to exchange status. Only after this
 * can XFactor starts
 * @ref Execute_SearchPreparations
 *
 * @attention
 * If XFactor cannot establish a communication
 * with SafeBox after it has driven outside of it
 * XFactor must drive back inside of SafeBox and
 * return to @ref Execute_WaitAfterSafeBox
 *
 * @warning
 * From there on out, XFactor needs to be armed
 */
void Execute_GettingOutOfGarage();

/**
 * @brief
 * This action function must first ask SafeBox
 * to close its garage before resetting the
 * vector buffer as well as movements and
 * positions before moving the robot to the
 * starting point of the package search pattern,
 * wherever that is. After which, XFactor needs
 * to change its status and do another status
 * exchange with SafeBox.
 *
 * If everything goes well,
 * @ref Execute_SearchForPackage should be called
 *
 * @attention
 * If XFactor cannot establish a communication
 * with SafeBox after its ready to begin searching
 * XFactor must try again for 5 attempts after
 * which it needs to enter @ref Execute_Alarm
 *
 * @warning
 * From there on out, XFactor needs to be armed
 */
void Execute_SearchPreparations();

/**
 * @brief This function makes XFactor move in
 * zig-zag search pattern throughout the searching
 * zone until a package or obstacle is detected.
 * Movements are done through the use of Vectors
 * which are saved in memory so that XFactor can
 * return home later.
 * Each 5 vectors, XFactor must perform a status
 * exchange with SafeBox to know if both can
 * still communicate and talk to each other.
 *
 * @attention
 * If XFactor cannot establish a communication
 * with SafeBox, XFactor must try again for 5
 * attempts after which it needs to enter
 * @ref Execute_Alarm.
 *
 * @warning
 * If an obstacle is detected that is not a
 * potential package,
 * @ref Execute_AvoidObstacle must be called.
 * Otherwise, @ref Execute_ExamineFoundPackage
 * must be called instead. If the vector buffer
 * becomes full, @ref Execute_NoPackageFound must
 * be called.
 */
void Execute_SearchForPackage();

/**
 * @brief
 * Action function that allows XFactor to go
 * around a detected obstacle. If the detected
 * obstacle is too long and XFactor cannot go
 * around it, it must resume its search pattern.
 * All the movements in this function must be
 * done through vectors and saved in the vector
 * buffer. Once the obstacle is avoided,
 * @ref Execute_SearchForPackage must be
 * directly called without going through
 * @ref Execute_SearchPreparations
 *
 * @attention
 * If the vector buffer becomes full, XFactor
 * must tell SafeBox about it through a status
 * exchange and XFactor must begin its return
 * home process.
 */
void Execute_AvoidObstacle();

/**
 * @brief
 * Action function that makes XFactor position
 * itself with a potential package through vector
 * based movements before analyzing it with its
 * colour sensor. If it is indeed a package,
 * XFactor must notify SafeBox about it and start
 * @ref Execute_PickUpPackage If it is not
 * a package, XFactor must backtrace all the
 * vectors that it just did and remove them from
 * the vector table.
 *
 * @attention
 * If the vector buffer becomes full, XFactor
 * must tell SafeBox about it through a status
 * exchange and XFactor must begin its return
 * home process. If communication cannot be
 * established after 5 attempts, XFactor must
 * enter alarm mode and continue to try to talk
 * with SafeBox.
 */
void Execute_ExamineFoundPackage();

/**
 * @brief
 * Action function that makes XFactor pick up a
 * found package. This action function must
 * change XFactor's status and perform a status
 * exchange with SafeBox to tell it that it has
 * found a package and its about to pick it up.
 *
 * If XFactor fails to pick up the package after
 * 5 attempts, @ref Execute_Error must be called
 * but the alarm must not be activated unless
 * XFactor is tempered with.
 *
* @attention
 * If the vector buffer becomes full, XFactor
 * must tell SafeBox about it through a status
 * exchange and XFactor must begin its return
 * home process. If communication cannot be
 * established after 5 attempts, XFactor must
 * enter alarm mode and continue to try to talk
 * with SafeBox.
 */
void Execute_PickUpPackage();

/**
 * @brief
 * Action function that firstly changes XFactor's
 * status to indicate that its about to return
 * home and then perform a status exchange with
 * SafeBox. Once that is done, XFactor must
 * use the vector table to calculate a vector
 * from its current position to the starting
 * position while avoiding all the obstacles that
 * it has found on its way to the package. This
 * is extremely difficult.
 *
 * @attention
 * If XFactor is not able to calculate a return
 * vector, XFactor must simply redo all the saved
 * vectors in reverse until its back to the start
 *
 * @warning
 * A status exchange must be performed each 5
 * vectors to ensure that communication with
 * SafeBox is still possible.
 */
void Execute_ReturnHome();

/**
 * @brief
 * Action function performed when XFactor has
 * returned to its starting point and is ready
 * to drop off the package that its currently
 * holding. This function must clear all the
 * vectors in the vector table as well as
 * movements and perform a status exchange
 * with SafeBox to ensure that communication is
 * possible before drop off is attempted.
 *
 * Once that is done, XFactor must position
 * itself to be ready to quickly drop off the
 * package inside of SafeBox.
 *
 * @attention
 * XFactor is allowed any amount of attempts
 * to establish the communication with SafeBox
 * However, it must only try to talk with SafeBox
 * each second.
 */
void Execute_PreparingForDropOff();

/**
 * @brief
 * Action function that firstly asks SafeBox to
 * open its delivery lid. Once SafeBox confirms
 * that the lid is open, XFactor must immediately
 * drop the package inside SafeBox and then ask
 * SafeBox to close the lid. Once that is done,
 * this function will update XFactor's status
 * and perform a status exchange. After that
 * status exchange is performed,
 * @ref Execute_ConfirmDropOff must be called.
 *
 * @attention
 * SafeBox will immediately close the lid once
 * it detects that a package was dropped off.
 * Its accelerometer alarm MUST be disabled when
 * the package drops to avoid triggering it.
 * XFactor asking SafeBox to close the lid is
 * simply a safety precaution.
 */
void Execute_PackageDropOff();

/**
 * @brief
 * Action function that executes a communication
 * with SafeBox to ensure it has received the
 * package that XFactor just dropped inside of it
 * not too long ago. Depending on what SafeBox
 * answers, an alarm will or will not be turned
 * on. If SafeBox has not detected a new package
 * within itself, it would mean that someone has
 * stolen it.
 *
 * @attention
 * XFactor is allowed to try to communicate with
 * SafeBox as many time as it wants. However,
 * after 5 attempts, it must indicate a potential
 * communication error.
 */
void Execute_ConfirmDropOff();

/**
 * @brief
 * Action function that blocks until SafeBox
 * sends a reset command to XFactor. In this
 * function, XFactor must make its LEDs flash in
 * the alarm pattern as well as perform loud
 * activities of any sorts. This function must
 * call @ref AlarmEvent at the very start before
 * looping and constantly checking if SafeBox has
 * sent the reset command.
 *
 * @attention
 * XFactor's status must be changed to the alarm
 * status and status exchanges must be executed
 * each second until SafeBox returns a reset
 * status after which XFactor can stop the alarm
 * and wait for SafeBox to no longer say that its
 * status is reset. After which,
 * @ref Execute_WaitForDelivery must be called.
 */
void Execute_Alarm();

/**
 * @brief
 * Action function that blocks until SafeBox's
 * status is updated to reset. In this function,
 * XFactor's status is updated to error and its
 * indicators must flash in the associated error
 * pattern. XFactor must also periodically
 * perform status exchanges as well as use Serial
 * to print the error code so it can be debugged
 * by a computer if need be.
 *
 * @attention
 * XFactor's status must be changed to the error
 * status and status exchanges must be executed
 * each second until SafeBox returns a reset
 * status after which XFactor can stop blocking
 * and wait for SafeBox to no longer say that its
 * status is reset. After which,
 * @ref Execute_WaitForDelivery must be called.
 */
void Execute_Error();

/**
 * @brief
 * Action function that firstly updates XFactor's
 * status to the corresponding one before
 * performing a status exchange with SafeBox.
 * After that is done, it must move XFactor to be
 * in front of SafeBox's garage before performing
 * a command request to ask SafeBox to open the
 * garage. After that is done, it must drive
 * XFactor inside the garage before asking
 * SafeBox to close the garage door. Once that
 * is done, XFactor must perform a status
 * exchange with SafeBox.
 *
 * @attention
 * Once XFactor is back inside the garage,
 * SafeBox is ready to wait for another doorbell
 * or can enter its finished state and wait for
 * the user to come retrieve its packages
 */
void Execute_ReturnInsideGarage();

/**
 * @brief
 * Action function that is executed whenever
 * SafeBox's status indicate that the box is
 * no longer waiting for packages and is
 * disarmed. This is executed whenever XFactor
 * has successfully completed a delivery and
 * can reset itself back to the start of the
 * program.
 */
void Execute_EndOfProgram();

/**
 * @brief
 * Action function that is executed whenever
 * the user unlocks SafeBox, which means that the
 * robot must stop, keep stuck there until it is
 * reset by the user
 */
void Execute_Unlocked();
//#pragma endregion