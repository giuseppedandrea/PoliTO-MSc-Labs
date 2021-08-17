/******************************************************************************
 * @file:    game.h
 * @brief:   Game configuration and prototypes of functions included in the
 *           game.c file
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

/**
  \brief  Game Configuration
 */

// BUTTONs LandTiger HW Mapping
// Remember: INT0 -> 0; KEY1 -> 1; KEY2 -> 2
#define BUTTON_START 0                      // HW mapping of the START button
#define BUTTON_ROTATE 1                     // HW mapping of the ROTATE button
#define BUTTON_RUN 2                        // HW mapping of the RUN button

// LEDs LandTiger HW Mapping
// Remember: LD11 -> 0; LD10 -> 1; ...; LD4 -> 7
#define LED_DIR_W 0                         // HW mapping of the DIRECTION LED: West
#define LED_DIR_S 1                         // HW mapping of the DIRECTION LED: South
#define LED_DIR_E 2                         // HW mapping of the DIRECTION LED: East
#define LED_DIR_N 3                         // HW mapping of the DIRECTION LED: North
#define LED_RUNNING 5                       // HW mapping of the RUNNING LED

// Robot starting position
#define START_X 7                           // Robot starting position on the X axis
#define START_Y 7                           // Robot starting position on the Y axis

// Robot starting direction
// Remember: 0 -> North; 1 -> East; 2 -> South; 3 -> West
#define START_DIR 1                         // Robot starting direction

// Robot speed
#define SPEED_X 1                           // Robot speed on X axis [slots per second]
#define SPEED_Y 1                           // Robot speed on Y axis [slots per second]

// Robot dimensions
#define DIM_X 1                             // Robot dimension on X axis [slots]
#define DIM_Y 1                             // Robot dimension on Y axis [slots]

// Labyrinth dimensions
#define MAX_X 15                            // Labyrinth dimension on X axis [slots]
#define MAX_Y 13                            // Labyrinth dimension on Y axis [slots]

// The DIRECTION LED currently active blinks with the following specifications according to the distance from the obstacle/wall:
// * Greater than RANGE_FAR_SUP slots: no blink
// * Between RANGE_FAR_SUP (included) and RANGE_FAR_INF (included) slots: blink at FREQ_DIRECTION_RANGE_FAR
// * Between RANGE_NEAR_SUP (included) and RANGE_NEAR_INF (included) slot: blink at FREQ_DIRECTION_RANGE_NEAR
// * 0 slots: blink at FREQ_DIRECTION_BLOCK
#define RANGE_FAR_SUP 5                     // The supremum of the far range
#define RANGE_FAR_INF 3                     // The infimum of the far range
#define RANGE_NEAR_SUP 2                    // The supremum of the near range
#define RANGE_NEAR_INF 1                    // The infimum of the near range

// Blink frequencies
// Remember: the blink frequency must be intended with duty cycle of 50% (half period low and half period high)
#define FREQ_RUNNING_MOVE 1                 // Blink frequency of the RUNNING LED when the robot is moving
#define FREQ_RUNNING_BLOCK 5                // Blink frequency of the RUNNING LED when the robot is blocked and the RUN button is pressed
#define FREQ_DIRECTION_RANGE_FAR 2          // Blink frequency of the DIRECTION LED when distance from the obstacle/wall is between RANGE_FAR_SUP (included) and RANGE_FAR_INF (included) slots
#define FREQ_DIRECTION_RANGE_NEAR 4         // Blink frequency of the DIRECTION LED when distance from the obstacle/wall is between RANGE_NEAR_SUP (included) and RANGE_NEAR_INF (included) slots
#define FREQ_DIRECTION_BLOCK 8              // Blink frequency of the DIRECTION LED when the robot is right in front of the obstacle/wall
#define FREQ_WIN 1                          // Blink frequency of all LEDs when the player reaches one of the exits

/**
  \brief  game.c: Functions to handle the game
 */
void GAME_Init(void);
void GAME_Start(void);
void GAME_RobotRotate(void);
void GAME_RobotMove(void);
void GAME_RobotStop(void);

/**
  \brief  game.c: Getters
 */
uint8_t GAME_GetDirectionLED(void);
uint8_t GAME_GetRunningLED(void);

#endif
