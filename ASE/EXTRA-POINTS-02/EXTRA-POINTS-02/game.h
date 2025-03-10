/******************************************************************************
 * @file:    game.h
 * @brief:   Game configuration and prototypes of functions included in the
 *           game.c file
 * @version: V1.00
 * @date:    2021-01-20
 ******************************************************************************/

#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

/**
  \brief  Game Configuration
 */

// Game starting mode
// Remember: 0x00 -> MOVE; 0xFF -> EXPLORE
#define GAME_START_MODE 0x00                // Game starting mode

// Robot starting position
#define ROBOT_START_X 7                     // Robot starting position on the X axis
#define ROBOT_START_Y 7                     // Robot starting position on the Y axis

// Robot starting direction
// Remember: 0 -> North; 1 -> East; 2 -> South; 3 -> West
#define ROBOT_START_DIR 1                   // Robot starting direction

// Robot speed
#define ROBOT_SPEED_X 1                     // Robot speed on X axis [slots per second]
#define ROBOT_SPEED_Y 1                     // Robot speed on Y axis [slots per second]

// Robot dimensions
#define ROBOT_DIM_X 1                       // Robot dimension on X axis [slots]
#define ROBOT_DIM_Y 1                       // Robot dimension on Y axis [slots]

// Robot proximity sensor
#define ROBOT_MAX_RANGE 5                   // Range in which the robot detects an obstacle [slots]

// Labyrinth dimensions
#define LABYRINTH_MAX_X 15                  // Labyrinth dimension on X axis [slots]
#define LABYRINTH_MAX_Y 13                  // Labyrinth dimension on Y axis [slots]

/**
  \brief  GUI Configuration
 */

#define GUI_CHAR_WIDTH 8
#define GUI_CHAR_HEIGHT 16

#define GUI_BG_COLOR                        Black

#define GUI_LABYRINTH_WIDTH                 210
#define GUI_LABYRINTH_HEIGHT                182
#define GUI_LABYRINTH_X0                    15
#define GUI_LABYRINTH_Y0                    (16 + GUI_CHAR_HEIGHT + 16)
#define GUI_LABYRINTH_BG_COLOR              White
#define GUI_LABYRINTH_SLOT_WIDTH            (GUI_LABYRINTH_WIDTH / LABYRINTH_MAX_X)
#define GUI_LABYRINTH_SLOT_HEIGHT           (GUI_LABYRINTH_HEIGHT / LABYRINTH_MAX_Y)

#define GUI_RESTART_BUTTON_WIDTH            90
#define GUI_RESTART_BUTTON_HEIGHT           30
#define GUI_RESTART_BUTTON_X0               15
#define GUI_RESTART_BUTTON_Y0               (GUI_LABYRINTH_Y0 + GUI_LABYRINTH_HEIGHT + 30)
#define GUI_RESTART_BUTTON_BG_COLOR         White

#define GUI_CLEAR_BUTTON_WIDTH              90
#define GUI_CLEAR_BUTTON_HEIGHT             30
#define GUI_CLEAR_BUTTON_X0                 (15 + GUI_RESTART_BUTTON_WIDTH + 30)
#define GUI_CLEAR_BUTTON_Y0                 (GUI_LABYRINTH_Y0 + GUI_LABYRINTH_HEIGHT + 30)
#define GUI_CLEAR_BUTTON_BG_COLOR           White

#define GUI_TITLE_TEXT                      "Blind Labyrinth"
#define GUI_TITLE_TEXT_WIDTH                ((sizeof(GUI_TITLE_TEXT) - 1) * GUI_CHAR_WIDTH)
#define GUI_TITLE_TEXT_HEIGHT               GUI_CHAR_HEIGHT
#define GUI_TITLE_TEXT_X0                   ((MAX_X - GUI_TITLE_TEXT_WIDTH) / 2)
#define GUI_TITLE_TEXT_Y0                   16
#define GUI_TITLE_TEXT_COLOR                White
#define GUI_TITLE_TEXT_BG_COLOR             GUI_BG_COLOR

#define GUI_RESTART_TEXT                    "Restart"
#define GUI_RESTART_TEXT_WIDTH              ((sizeof(GUI_RESTART_TEXT) - 1) * GUI_CHAR_WIDTH)
#define GUI_RESTART_TEXT_HEIGHT             GUI_CHAR_HEIGHT
#define GUI_RESTART_TEXT_X0                 (GUI_RESTART_BUTTON_X0 + ((GUI_RESTART_BUTTON_WIDTH - GUI_RESTART_TEXT_WIDTH) / 2))
#define GUI_RESTART_TEXT_Y0                 (GUI_RESTART_BUTTON_Y0 + ((GUI_RESTART_BUTTON_HEIGHT - GUI_CHAR_HEIGHT) / 2))
#define GUI_RESTART_TEXT_COLOR              Black
#define GUI_RESTART_TEXT_BG_COLOR           GUI_RESTART_BUTTON_BG_COLOR

#define GUI_CLEAR_TEXT                      "Clear"
#define GUI_CLEAR_TEXT_WIDTH                ((sizeof(GUI_CLEAR_TEXT) - 1) * GUI_CHAR_WIDTH)
#define GUI_CLEAR_TEXT_HEIGHT               GUI_CHAR_HEIGHT
#define GUI_CLEAR_TEXT_X0                   (GUI_CLEAR_BUTTON_X0 + ((GUI_CLEAR_BUTTON_WIDTH - GUI_CLEAR_TEXT_WIDTH) / 2))
#define GUI_CLEAR_TEXT_Y0                   (GUI_CLEAR_BUTTON_Y0 + ((GUI_CLEAR_BUTTON_HEIGHT - GUI_CHAR_HEIGHT) / 2))
#define GUI_CLEAR_TEXT_COLOR                Black
#define GUI_CLEAR_TEXT_BG_COLOR             GUI_CLEAR_BUTTON_BG_COLOR

#define GUI_START_TEXT                      "Touch to start a new game"
#define GUI_START_TEXT_WIDTH                (sizeof(GUI_START_TEXT) - 1) * GUI_CHAR_WIDTH
#define GUI_START_TEXT_HEIGHT               GUI_CHAR_HEIGHT
#define GUI_START_TEXT_X0                   (GUI_LABYRINTH_X0 + ((GUI_LABYRINTH_WIDTH - GUI_START_TEXT_WIDTH) / 2))
#define GUI_START_TEXT_Y0                   (GUI_LABYRINTH_Y0 + ((GUI_LABYRINTH_HEIGHT - GUI_CHAR_HEIGHT) / 2))
#define GUI_START_TEXT_COLOR                Black
#define GUI_START_TEXT_BG_COLOR             GUI_LABYRINTH_BG_COLOR

#define GUI_WIN_TEXT                        "You win!"
#define GUI_WIN_TEXT_WIDTH                  (sizeof(GUI_WIN_TEXT) - 1) * GUI_CHAR_WIDTH
#define GUI_WIN_TEXT_HEIGHT                 GUI_CHAR_HEIGHT
#define GUI_WIN_TEXT_X0                     (GUI_LABYRINTH_X0 + ((GUI_LABYRINTH_WIDTH - GUI_WIN_TEXT_WIDTH) / 2))
#define GUI_WIN_TEXT_Y0                     (GUI_LABYRINTH_Y0 + ((GUI_LABYRINTH_HEIGHT - GUI_CHAR_HEIGHT) / 2))
#define GUI_WIN_TEXT_COLOR                  White
#define GUI_WIN_TEXT_BG_COLOR               Black

#define GUI_ROBOT_BG_COLOR_EXPLORE          Yellow
#define GUI_ROBOT_BG_COLOR_MOVE             Green
#define GUI_ROBOT_DIRECTION_COLOR           Black
#define GUI_ROBOT_TRAIL_COLOR               Green2

#define GUI_OBSTACLES_BG_COLOR              Red

/**
  \brief  game.c: Functions to handle the game
 */
void GAME_Init(void);
void GAME_Touch(uint16_t x, uint16_t y);
void GAME_ChangeMode(void);
void GAME_Action(uint8_t dir);

#endif
