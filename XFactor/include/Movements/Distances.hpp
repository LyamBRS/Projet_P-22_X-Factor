/**
 * @file Movements.hpp
 * @author Williiam ()
 * @brief Contains defines for
 * distances used by Movement, Package
 * and actions
 * @version 0.1
 * @date 2023-11-26
 * @copyright Copyright (c) 2023
 */
#pragma once

#define STRAIGHT 0.0f
#define TURN_90_RIGHT TURN_90 * TURN_RIGHT
#define TURN_90_LEFT TURN_90 * TURN_LEFT

#define SAFEBOX_LENGTH_CM 65.0f
#define SAFEBOX_WIDTH_CM 80.0f

#define DEMO_AREA_WIDTH_CM 200.0f
#define DEMO_AREA_LENGTH_CM 200.0f

#define SCANNABLE_AREA_WIDTH 25.0f

#define ROBOT_WIDTH_CM 28.0f
#define ROBOT_LENGTH_CM 52.0f

#define DISTANCE_SENSOR_DIFF_BETWEEN_SIDE_AND_FRONT_CM 10.0f //roughly

//#pragma region [moveVectors]

#define GETTING_OUT_OF_GARAGE_VECTOR STRAIGHT, SAFEBOX_LENGTH_CM + ROBOT_LENGTH_CM - (ROBOT_WIDTH_CM / 4), false, DONT_CHECK_SENSORS, true, false, 0.4f
#define GETTING_BACK_INTO_GARAGE_VECTOR TURN_90_LEFT, SAFEBOX_LENGTH_CM + ROBOT_LENGTH_CM - 20.0f, false, DONT_CHECK_SENSORS, true, false, 0.4f

#define BACKTRACE_VECTOR backtraceVector.rotation_rad, backtraceVector.distance_cm, false, DONT_CHECK_SENSORS, true, false, SPEED_MAX

#define SEARCH_PATTERN_VECTOR searchPatternVectors[i].rotation_rad, searchPatternVectors[i].distance_cm, true, CHECK_SENSORS, true, false, 0.4f
#define GO_TO_DETECTED_OBJECT_FRONT_VECTOR STRAIGHT, Package_GetDetectedDistance() + (DISTANCE_SENSOR_DIFF_BETWEEN_SIDE_AND_FRONT_CM / 2), true, DONT_CHECK_SENSORS, true, false, 0.2f
#define GO_TO_DETECTED_OBJECT_LEFT_VECTOR TURN_90_LEFT - (PI / 90), Package_GetDetectedDistance(), true, DONT_CHECK_SENSORS, true, false, 0.2f
#define GO_TO_DETECTED_OBJECT_RIGHT_VECTOR TURN_90_RIGHT + (PI / 90), Package_GetDetectedDistance(), true, DONT_CHECK_SENSORS, true, false,0.2f

#define EXAMINE_PACKAGE_LEFT_SWIPE_VECTOR TURN_90_RIGHT, 0.0f, true, true, true, false, 0.2f
#define EXAMINE_PACKAGE_WHOLE_SWIPE_VECTOR TURN_180, 0.0f, true, true, true, false, 0.2f
#define EXAMINE_PACKAGE_CLOSEUP_VECTOR STRAIGHT, Package_GetDetectedDistance(), true, false, true, true, 0.2f

#define RETURN_HOME_VECTOR returnVector.rotation_rad, returnVector.distance_cm, true, false, true, false, 0.4f

#define PICK_UP_PACKAGE_VECTOR STRAIGHT, PACKAGE_BACK_MOVEMENT, true, DONT_CHECK_SENSORS, false, false, 0.2f
#define ALIGN_WITH_SAFEBOX_VECTOR PI/2, 0, false, DONT_CHECK_SENSORS, true, false, 0.2f
//#pragma endregion