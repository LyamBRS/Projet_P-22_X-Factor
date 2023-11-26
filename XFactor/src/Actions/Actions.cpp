/**
 * @file Actions.cpp
 * @author LyamBRS (lyam.brs@gmail.com)
 * @brief File which contains the action 
 * functions used throughout the program. These
 * action functions are functions that execute a
 * certain step of XFactor such as getting out of
 * the garage, finding a package, picking up a
 * package, returning home, depositing a package
 * etc.
 * @version 0.1
 * @date 2023-11-02
 * @copyright Copyright (c) 2023
 */

// - INCLUDE - //
#include "Actions/Actions.hpp"

/**
 * @brief
 * Global variable that is local to the action.c
 * file. This stores the current action's number
 * so that @ref Execute_CurrentFunction can be
 * executed properly without needing an input
 * parameter.
 *
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
 * should currently be executed by XFactor. To
 * specify that function, use
 * @ref SetNewExecutionFunction using defines
 * available in this header file.
*/
void Execute_CurrentFunction(){
    switch(currentFunctionID){

        case(FUNCTION_ID_ALARM):
            Execute_Alarm();
            break;

        case(FUNCTION_ID_AVOID_OBSTACLE):
            Execute_AvoidObstacle();
            break;

        case(FUNCTION_ID_CONFIRM_DROP_OFF):
            Execute_ConfirmDropOff();
            break;

        case(FUNCTION_ID_END_OF_PROGRAM):
            Execute_EndOfProgram();
            break;

        case(FUNCTION_ID_ERROR):
            Execute_Error();
            break;

        case(FUNCTION_ID_EXAMINE_FOUND_PACKAGE):
            Execute_ExamineFoundPackage();
            break;

        case(FUNCTION_ID_GETTING_OUT_OF_GARAGE):
            Execute_GettingOutOfGarage();
            break;

        case(FUNCTION_ID_PACKAGE_DROP_OFF):
            Execute_PackageDropOff();
            break;

        case(FUNCTION_ID_PICK_UP_PACKAGE):
            Execute_PickUpPackage();
            break;

        case(FUNCTION_ID_PREPARING_FOR_DROP_OFF):
            Execute_PreparingForDropOff();
            break;

        case(FUNCTION_ID_RETURN_HOME):
            Execute_ReturnHome();
            break;

        case(FUNCTION_ID_RETURN_INSIDE_GARAGE):
            Execute_ReturnInsideGarage();
            break;

        case(FUNCTION_ID_SEARCH_FOR_PACKAGE):
            Execute_SearchForPackage();
            break;

        case(FUNCTION_ID_SEARCH_PREPARATIONS):
            Execute_SearchPreparations();
            break;

        case(FUNCTION_ID_UNLOCKED):
            Execute_Unlocked();
            break;

        case(FUNCTION_ID_WAIT_AFTER_SAFEBOX):
            Execute_WaitAfterSafeBox();
            break;

        case(FUNCTION_ID_WAIT_FOR_DELIVERY):
            Execute_WaitForDelivery();
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
        case(FUNCTION_ID_AVOID_OBSTACLE):
        case(FUNCTION_ID_CONFIRM_DROP_OFF):
        case(FUNCTION_ID_END_OF_PROGRAM):
        case(FUNCTION_ID_ERROR):
        case(FUNCTION_ID_EXAMINE_FOUND_PACKAGE):
        case(FUNCTION_ID_GETTING_OUT_OF_GARAGE):
        case(FUNCTION_ID_PACKAGE_DROP_OFF):
        case(FUNCTION_ID_PICK_UP_PACKAGE):
        case(FUNCTION_ID_PREPARING_FOR_DROP_OFF):
        case(FUNCTION_ID_RETURN_HOME):
        case(FUNCTION_ID_RETURN_INSIDE_GARAGE):
        case(FUNCTION_ID_SEARCH_FOR_PACKAGE):
        case(FUNCTION_ID_SEARCH_PREPARATIONS):
        case(FUNCTION_ID_UNLOCKED):
        case(FUNCTION_ID_WAIT_AFTER_SAFEBOX):
        case(FUNCTION_ID_WAIT_FOR_DELIVERY):
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
void Execute_WaitAfterSafeBox()
{
  Debug_Start("Execute_WaitAfterSafeBox");
  XFactor_SetNewStatus(XFactor_Status::WaitingAfterSafeBox);
  LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_WAITING_FOR_COMMS);

  if (SafeBox_ExchangeStatus())
  {
    if (SafeBox_GetStatus() != SafeBox_Status::CommunicationError) // XFACTOR STATUS TO CONFIRM
    {
      Debug_Information("Actions", "Execute_WaitAfterSafeBox", "SafeBox detected");
      SetNewExecutionFunction(FUNCTION_ID_UNLOCKED);
      BT_ClearAllMessages();
    }
  }
  Debug_End();
}

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
void Execute_WaitForDelivery()
{
  Debug_Start("Execute_WaitForDelivery");
  XFactor_SetNewStatus(XFactor_Status::WaitingForDelivery);
  LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_WAITFORDELIVERY);

  SafeBox_ExchangeStatus();
  if (SafeBox_GetDoorBellStatus())
  {
    SetNewExecutionFunction(FUNCTION_ID_GETTING_OUT_OF_GARAGE);
  }

  int checkFunctionId = ExecutionUtils_StatusCheck(FUNCTION_ID_WAIT_FOR_DELIVERY);
  if (checkFunctionId != FUNCTION_ID_WAIT_FOR_DELIVERY)
  {
    Debug_Warning("Actions", "Execute_WaitForDelivery", "Changing execution function");
    SetNewExecutionFunction(checkFunctionId);
    Debug_End();
    return;
  }
  Debug_End();
}

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
void Execute_GettingOutOfGarage()
{
  XFactor_SetNewStatus(XFactor_Status::LeavingSafeBox);

  if (SafeBox_GetGarageState())
  {
    if (MoveFromVector(STRAIGHT, SAFEBOX_LENGTH_CM + ROBOT_WIDTH_CM, false, DONT_CHECK_SENSORS))
    {
      if (SafeBox_ExchangeStatus() && SafeBox_GetStatus() != SafeBox_Status::CommunicationError)
      {
        SetNewExecutionFunction(FUNCTION_ID_SEARCH_PREPARATIONS);
      }
    }
    else
    {
      SetNewExecutionFunction(FUNCTION_ID_ERROR);
    }
  }
  else
  {
    SafeBox_ChangeGarageState(true);
  }
}

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
void Execute_SearchPreparations()
{
  XFactor_SetNewStatus(XFactor_Status::PreparingForTheSearch);
  
  if (!SafeBox_GetGarageState())
  {
    if (MoveFromVector(STRAIGHT, ROBOT_WIDTH_CM, false, DONT_CHECK_SENSORS))
    {
      int checkFunctionId;
      ResetVectors();
    
      checkFunctionId = ExecutionUtils_CommunicationCheck(FUNCTION_ID_SEARCH_PREPARATIONS, MAX_COMMUNICATION_ATTEMPTS, true);

      if (checkFunctionId == FUNCTION_ID_ALARM || checkFunctionId == FUNCTION_ID_UNLOCKED)
      {
        SetNewExecutionFunction(checkFunctionId);
      }
      else
      {
        SetNewExecutionFunction(FUNCTION_ID_SEARCH_FOR_PACKAGE);
      }
    }
  }
  else
  {
    SafeBox_ChangeGarageState(false);
  }
}

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
void Execute_SearchForPackage()
{
  int checkFunctionId;

  XFactor_SetNewStatus(XFactor_Status::SearchingForAPackage);
  
  MovementVector searchPatternVectors[VECTOR_BUFFER_SIZE];
  int startAvailableVectors = GetAvailableVectors();

  // SEE WHEN NEGATIVE
  /*searchPatternVectors[0].distance_cm = DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM - (ROBOT_WIDTH_CM / 2);
  searchPatternVectors[0].rotation_rad = -PI / 2;

  searchPatternVectors[0].distance_cm = (DEMO_AREA_LENGTH_CM - SAFEBOX_LENGTH_CM) - (DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM - ROBOT_WIDTH_CM);
  searchPatternVectors[0].rotation_rad = PI / 2;*/

  float strafeDistance_cm = 0;
  int currentIndex = 0;
  int startAvailableVectors = GetAvailableVectors();

  // Initial search pattern vectors
  int totalStrafes = (int)((DEMO_AREA_LENGTH_CM / DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM) - 2); // - 2 is removing the edges
  int fullStrafes = (int)(((DEMO_AREA_LENGTH_CM - SAFEBOX_LENGTH_CM) / DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM) - 1); // - 1 is removing left edge

  /*int totalStrafes = (int)(DEMO_AREA_LENGTH_CM / SCANNABLE_AREA_WIDTH);
  int fullStrafes = (int)((DEMO_AREA_LENGTH_CM - SAFEBOX_LENGTH_CM) / SCANNABLE_AREA_WIDTH);*/

  MovementVector searchPatternVectors[VECTOR_BUFFER_SIZE];

  searchPatternVectors[0].rotation_rad = TURN_90_RIGHT;
  searchPatternVectors[0].distance_cm = DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM - (ROBOT_WIDTH_CM / 2);

  searchPatternVectors[1].rotation_rad = TURN_90_LEFT;
  searchPatternVectors[1].distance_cm = (DEMO_AREA_LENGTH_CM - SAFEBOX_LENGTH_CM) - (DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM - ROBOT_WIDTH_CM);

  searchPatternVectors[2].rotation_rad = TURN_90_RIGHT;
  searchPatternVectors[2].distance_cm = (DEMO_AREA_WIDTH_CM - ((DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM * 2) - ROBOT_WIDTH_CM));

  /*searchPatternVectors[0].rotation_rad = STRAIGHT;
  searchPatternVectors[0].distance_cm = DEMO_AREA_LENGTH_CM - SCANNABLE_AREA_WIDTH;

  searchPatternVectors[1].rotation_rad = TURN_90_RIGHT;
  searchPatternVectors[1].distance_cm = DEMO_AREA_WIDTH_CM - SCANNABLE_AREA_WIDTH;
  currentIndex++;*/

  // totalStrafes * 2 because 2 moves per strafe
  // currentIndex start value depending on amount of above start vectors
  for (currentIndex = 3; currentIndex < totalStrafes * 2; currentIndex += 4)
  {
    searchPatternVectors[currentIndex].rotation_rad = TURN_90_RIGHT;
    searchPatternVectors[currentIndex].distance_cm = DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM;

    if (currentIndex * 2 < fullStrafes)
    {
      strafeDistance_cm = DEMO_AREA_WIDTH_CM - (DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM * 2);
    }
    else
    {
      strafeDistance_cm = DEMO_AREA_WIDTH_CM - SAFEBOX_WIDTH_CM - (DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM * 2);
    }
      
    searchPatternVectors[currentIndex + 1].rotation_rad = TURN_90_RIGHT;
    searchPatternVectors[currentIndex + 1].distance_cm = strafeDistance_cm;

    searchPatternVectors[currentIndex + 2].rotation_rad = TURN_90_LEFT;
    searchPatternVectors[currentIndex + 2].distance_cm = DISTANCE_SENSOR_MAX_DETECTION_RANGE_CM;

    searchPatternVectors[currentIndex + 3].rotation_rad = TURN_90_LEFT;
    searchPatternVectors[currentIndex + 3].distance_cm = strafeDistance_cm;

    /*searchPatternVectors[currentIndex].rotation_rad = TURN_90_RIGHT;
    searchPatternVectors[currentIndex].distance_cm = SCANNABLE_AREA_WIDTH;

    strafeDistance_cm = currentIndex * 2 < fullStrafes ? DEMO_AREA_WIDTH_CM - (SCANNABLE_AREA_WIDTH * 2) : DEMO_AREA_WIDTH_CM - SAFEBOX_WIDTH_CM - (ROBOT_WIDTH_CM * 2);
      
    searchPatternVectors[currentIndex + 1].rotation_rad = TURN_90_RIGHT;
    searchPatternVectors[currentIndex + 1].distance_cm = strafeDistance_cm;

    searchPatternVectors[currentIndex + 2].rotation_rad = TURN_90_LEFT;
    searchPatternVectors[currentIndex + 2].distance_cm = SCANNABLE_AREA_WIDTH;

    searchPatternVectors[currentIndex + 3].rotation_rad = TURN_90_LEFT;
    searchPatternVectors[currentIndex + 3].distance_cm = strafeDistance_cm;*/
  }

  if ((currentIndex * 2) % 2 == 0) // PAIR OU IMPAIR lorsqu'il atteint l'extrême droite
  {
    searchPatternVectors[currentIndex].rotation_rad = TURN_180;
    searchPatternVectors[currentIndex].distance_cm = DEMO_AREA_WIDTH_CM - SAFEBOX_WIDTH_CM - ROBOT_LENGTH_CM;
    currentIndex++;
  }

  searchPatternVectors[currentIndex].rotation_rad = TURN_90_RIGHT;
  searchPatternVectors[currentIndex].distance_cm = SAFEBOX_LENGTH_CM + ROBOT_LENGTH_CM;
  currentIndex++;

  searchPatternVectors[currentIndex].rotation_rad = TURN_90_LEFT;
  searchPatternVectors[currentIndex].distance_cm = SAFEBOX_WIDTH_CM - ROBOT_WIDTH_CM;
  currentIndex++;

  searchPatternVectors[currentIndex].rotation_rad = TURN_90_LEFT;
  searchPatternVectors[currentIndex].distance_cm = 0.0f; // Turn on self
  currentIndex++;

  /*if ((currentIndex * 2) % 2 == 0) // PAIR OU IMPAIR lorsqu'il atteint l'extrême droite
  {
    searchPatternVectors[currentIndex].rotation_rad = TURN_180;
    searchPatternVectors[currentIndex].distance_cm = DEMO_AREA_WIDTH_CM - SAFEBOX_WIDTH_CM - (SCANNABLE_AREA_WIDTH * 2);
    currentIndex++;
  }

  searchPatternVectors[currentIndex].rotation_rad = TURN_90_RIGHT;
  searchPatternVectors[currentIndex].distance_cm = SAFEBOX_LENGTH_CM - SCANNABLE_AREA_WIDTH;
  currentIndex++;

  searchPatternVectors[currentIndex].rotation_rad = TURN_90_LEFT;
  searchPatternVectors[currentIndex].distance_cm = SAFEBOX_WIDTH_CM - SCANNABLE_AREA_WIDTH;
  currentIndex++;

  searchPatternVectors[currentIndex].rotation_rad = TURN_90_LEFT;
  searchPatternVectors[currentIndex].distance_cm = 0.0f; // Turn on self
  currentIndex++;*/

  if (currentIndex >= VECTOR_BUFFER_SIZE)
  {
    Debug_Error("Actions.cpp", "Execute_SearchForPackages", "There is not enough space in the vector buffer to try the search");
    SetNewExecutionFunction(FUNCTION_ID_ERROR);
    return;
  }

  for (int i = 0; i < VECTOR_BUFFER_SIZE; i++)
  {
    switch (MoveFromVector(searchPatternVectors[i].rotation_rad, searchPatternVectors[i].distance_cm, true, CHECK_SENSORS))
    {
      case MOVEMENT_COMPLETED:
        if (GetAvailableVectors() - startAvailableVectors == 5)
        {
          startAvailableVectors = GetAvailableVectors();
          checkFunctionId = ExecutionUtils_CommunicationCheck(FUNCTION_ID_SEARCH_FOR_PACKAGE, MAX_COMMUNICATION_ATTEMPTS, true);

          if (checkFunctionId == FUNCTION_ID_ALARM || checkFunctionId == FUNCTION_ID_ERROR)
          {
            SetNewExecutionFunction(checkFunctionId);
            return;
          }

          checkFunctionId = ExecutionUtils_StatusCheck(FUNCTION_ID_SEARCH_FOR_PACKAGE);

          if (checkFunctionId == FUNCTION_ID_UNLOCKED || checkFunctionId == FUNCTION_ID_ERROR)
          {
            SetNewExecutionFunction(checkFunctionId);
            return;
          }
        }
        break;
      case PACKAGE_FOUND:
        SetNewExecutionFunction(FUNCTION_ID_EXAMINE_FOUND_PACKAGE);
        return;
      case MOVEMENT_ERROR:
        SetNewExecutionFunction(FUNCTION_ID_ERROR);
        return;
    }
  }

  XFactor_SetNewStatus(XFactor_Status::NoPackageFound);
  SetNewExecutionFunction(FUNCTION_ID_RETURN_HOME);
}

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
void Execute_AvoidObstacle()
{
  // WILL NEED TO KNOW SOME THINGS BEFORE
}

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
void Execute_ExamineFoundPackage()
{
  int checkFunctionId;
  XFactor_SetNewStatus(XFactor_Status::ExaminatingAPackage);
  // ALIGN WITH POTENTIAL PACKAGE

  checkFunctionId = ExecutionUtils_CommunicationCheck(FUNCTION_ID_SEARCH_FOR_PACKAGE, MAX_COMMUNICATION_ATTEMPTS, true);

  if (checkFunctionId == FUNCTION_ID_ALARM || checkFunctionId == FUNCTION_ID_ERROR)
  {
    SetNewExecutionFunction(checkFunctionId);
    return;
  }

  if (Package_Detected())
  {
    //XFactor_SetNewStatus(XFactor_Status::); PUT GOOD STATUS
    SafeBox_ExchangeStatus();
    SetNewExecutionFunction(FUNCTION_ID_PICK_UP_PACKAGE);
  }
}

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
void Execute_PickUpPackage()
{
  int pickUpAttempt = 1;
  int checkFunctionId;

  XFactor_SetNewStatus(XFactor_Status::PickingUpAPackage);

  Package_PickUp();

  while(!Package_Detected())
  {
    if (pickUpAttempt >= MAX_PICKUP_ATTEMPTS)
    {
      // If we have time, undo the last vectors then redo ExamineFoundPackage
      SetNewExecutionFunction(FUNCTION_ID_ERROR);
      return;
    }
    pickUpAttempt++;

    checkFunctionId = ExecutionUtils_CommunicationCheck(FUNCTION_ID_SEARCH_FOR_PACKAGE, MAX_COMMUNICATION_ATTEMPTS, true);

    if (checkFunctionId == FUNCTION_ID_ALARM || checkFunctionId == FUNCTION_ID_ERROR || checkFunctionId == FUNCTION_ID_UNLOCKED)
    {
      SetNewExecutionFunction(checkFunctionId);
      return;
    }
    Package_PickUp();
  }
  SetNewExecutionFunction(FUNCTION_ID_RETURN_HOME);
}

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
void Execute_ReturnHome()
{
  int checkFunctionId;

  XFactor_SetNewStatus(XFactor_Status::ReturningHome);

  checkFunctionId = ExecutionUtils_CommunicationCheck(FUNCTION_ID_PREPARING_FOR_DROP_OFF, MAX_COMMUNICATION_ATTEMPTS, true);

  if (checkFunctionId == FUNCTION_ID_ALARM || checkFunctionId == FUNCTION_ID_ERROR || checkFunctionId == FUNCTION_ID_UNLOCKED)
  {
    SetNewExecutionFunction(checkFunctionId);
    return;
  }

  SetNewExecutionFunction(FUNCTION_ID_PREPARING_FOR_DROP_OFF);
}

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
void Execute_PreparingForDropOff()
{
  int checkFunctionId;

  XFactor_SetNewStatus(XFactor_Status::PreparingForDropOff);

  ResetVectors();
  ResetMovements();

  checkFunctionId = ExecutionUtils_CommunicationCheck(FUNCTION_ID_PREPARING_FOR_DROP_OFF, MAX_COMMUNICATION_ATTEMPTS, true);

  if (checkFunctionId == FUNCTION_ID_ALARM || checkFunctionId == FUNCTION_ID_ERROR)
  {
    SetNewExecutionFunction(checkFunctionId);
    return;
  }

  if (Package_AlignWithSafeBox())
  {
    SetNewExecutionFunction(FUNCTION_ID_PACKAGE_DROP_OFF);
  }
}

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
void Execute_PackageDropOff()
{
  // WILL NEED TO SEE WITH CHANGES TO SAFEBOX LAYOUT
  XFactor_SetNewStatus(XFactor_Status::DroppingOff);

  if (SafeBox_GetLidState())
  {
    if (Package_Release())
    {
      SafeBox_ChangeLidState(false);

      if (SafeBox_ExchangeStatus() && SafeBox_GetStatus() != SafeBox_Status::CommunicationError)
      {
        SetNewExecutionFunction(FUNCTION_ID_CONFIRM_DROP_OFF);
      }
    }
  }
  else
  {
    SafeBox_ChangeLidState(true);
  }
}

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
void Execute_ConfirmDropOff()
{
  int checkFunctionId;

  XFactor_SetNewStatus(XFactor_Status::ConfirmingDropOff);

  checkFunctionId = ExecutionUtils_CommunicationCheck(FUNCTION_ID_CONFIRM_DROP_OFF, MAX_COMMUNICATION_ATTEMPTS, true);

  if (checkFunctionId == FUNCTION_ID_ALARM || checkFunctionId == FUNCTION_ID_ERROR)
  {
    SetNewExecutionFunction(checkFunctionId);
    return;
  }

  if (SafeBox_CheckIfPackageDeposited())
  {
    // GO NEXT
  }
  else
  {
    SetNewExecutionFunction(FUNCTION_ID_ALARM);
  }
}

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
void Execute_Alarm()
{
  Debug_Information("Actions", "Execute_Alarm","Start");
  // - VARIABLES - //
    static bool mustBeOn = false;
    XFactor_SetNewStatus(XFactor_Status::Alarm);

    int checkFunctionId = ExecutionUtils_StatusCheck(FUNCTION_ID_ALARM);
    if (checkFunctionId != FUNCTION_ID_ALARM)
    {
      AX_BuzzerOFF();
      SetNewExecutionFunction(checkFunctionId);
      return;
    }


    //ExecutionUtils_HandleReceivedXFactorStatus();
    // - LED & BUZZER BLINK - //
    if(ExecutionUtils_LedBlinker(100))
    {
        mustBeOn = !mustBeOn;

        if(mustBeOn)
        {
            SafeBox_ExchangeStatus();
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ALARM);
            AX_BuzzerON();
        }
        else
        {
            LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_OFFLINE);
            AX_BuzzerOFF();
        }
    }
}

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
void Execute_Error()
{
  Debug_Error("Actions", "Execute_Error", "ERROR REACHED. DEBUG STOPPED");
  Debug_Stop();

  // - VARIABLES - //
  static bool mustBeOn = false; // everything is closed

  // - PROGRAM - //
  XFactor_SetNewStatus(XFactor_Status::Error);

  if(ExecutionUtils_LedBlinker(1000))
  {
      mustBeOn = !mustBeOn;

      if(mustBeOn)
      {
        LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_ALARM);
      }
      else
      {
        LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_OFFLINE);
      }
    }
  return;
}

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
void Execute_ReturnInsideGarage()
{
  int checkFunctionId;
  bool hasEnteredGarage = false;
  
  XFactor_SetNewStatus(XFactor_Status::EnteringSafeBox);

  checkFunctionId = ExecutionUtils_StatusCheck(FUNCTION_ID_RETURN_INSIDE_GARAGE);

  if (checkFunctionId == FUNCTION_ID_UNLOCKED || checkFunctionId == FUNCTION_ID_ERROR)
  {
    SetNewExecutionFunction(checkFunctionId);
    return;
  }

  if (SafeBox_GetGarageState() && !hasEnteredGarage)
  {
    // DRIVE INTO GARAGE
    hasEnteredGarage = true;
    return;
  }
  else
  {
    SafeBox_ChangeGarageState(true);
    return;
  }

  if (!SafeBox_GetGarageState())
  {
    // Go place package
    // Return to initial position
  }
  else
  {
    SafeBox_ChangeGarageState(false);
  }
}

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
void Execute_EndOfProgram()
{
  int checkFunctionId;

  checkFunctionId = ExecutionUtils_StatusCheck(FUNCTION_ID_END_OF_PROGRAM);

  if (checkFunctionId == FUNCTION_ID_UNLOCKED || checkFunctionId == FUNCTION_ID_ERROR)
  {
    SetNewExecutionFunction(checkFunctionId);
    return;
  }

  SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_DELIVERY);
}

/**
 * @brief
 * Action function that is executed whenever
 * the user unlocks SafeBox, which means that the
 * robot must stop, keep stuck there until it is
 * reset by the user
 */
void Execute_Unlocked()
{
  XFactor_SetNewStatus(XFactor_Status::Unlocked);

  LEDS_SetColor(LED_ID_STATUS_INDICATOR, LED_COLOR_DISARMED);
  Stop();

  int checkFunctionId;

  if (SafeBox_GetStatus() == SafeBox_Status::WaitingForDelivery)
  {
    SetNewExecutionFunction(FUNCTION_ID_WAIT_FOR_DELIVERY);
  }
  else
  {
    //Debug_Warning("Action.cpp","Execute_Unlocked","AAAAAAAAAAAAAA");
  }

  checkFunctionId = ExecutionUtils_StatusCheck(FUNCTION_ID_UNLOCKED);
  if (checkFunctionId != FUNCTION_ID_UNLOCKED)
  {
    Debug_Error("Actions", "Execute_Unlocked", "Changing execution function");
    SetNewExecutionFunction(checkFunctionId);
    return;
  }
}

//#pragma endregion