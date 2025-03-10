/******************************************************************************
 * @file:    game.c
 * @purpose: Blind Labyrinth: the player drives the robot through the
 *           controller in the labyrinth, trying to reach one of four possible
 *           exits. The player does not know the labyrinth structure, nor the
 *           obstacle it contains. Since the game is called "Blind Labyrinth",
 *           the player has not any visual control over the robot. He decides
 *           the robot movements depending on the information returned by the
 *           proximity sensor
 * @version: V1.00
 * @date:    2021-01-20
 ******************************************************************************/

#include "game.h"

#include "LPC17xx.h"
#include "TIMER/timer.h"
#include "JOYSTICK/joystick.h"
#include "GLCD/GLCD.h"
#include "TP/TouchPanel.h"

#define SLOT_POINTS 1000                    // Points in one slot (1 slot = SLOT_POINTS)

#define TOUCHPANEL_TIMER_NUM 0              // TouchPanel TIMER number
#define TOUCHPANEL_TIMER_TIME 1             // TouchPanel TIMER time [ms]
#define TOUCHPANEL_TIMER_PRIORITY 1         // TouchPanel TIMER priority

#define JOYSTICK_TIMER_NUM 1                // Joystick TIMER number
#define JOYSTICK_TIMER_TIME 50              // Joystick TIMER time [ms]
#define JOYSTICK_TIMER_PRIORITY 0           // Joystick TIMER priority

// Labyrinth definition: 0 -> free ground; 1 -> obstacle; 2 -> exit
static const uint8_t LABYRINTH[LABYRINTH_MAX_Y][LABYRINTH_MAX_X] = {
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}
};

// Robot struct definition
struct robot_s {
    uint8_t dir;                            // Direction: 0 -> North; 1 -> East; 2 -> South; 3 -> West
    uint16_t step_pos_x;                    // Step to increment/decrement robot position on the X axis
    uint16_t step_pos_y;                    // Step to increment/decrement robot position on the Y axis
    uint16_t pos_x;                         // Position on the X axis (1 slot = SLOT_POINTS)
    uint16_t pos_y;                         // Position on the Y axis (1 slot = SLOT_POINTS)
    uint16_t dist;                          // Distance from obstacles/walls/exits
    bool on_exit;                           // True -> The robot is pointing towards one of the exits; False -> Otherwise
    uint16_t step_gui_x;                    // Step to increment/decrement robot position in GUI on the X axis
    uint16_t step_gui_y;                    // Step to increment/decrement robot position in GUI on the Y axis
    uint16_t px_x;                          // Position on the X axis of the top-left pixel
    uint16_t px_y;                          // Position on the Y axis of the top-left pixel
    uint16_t left_column;                   // Column occupied by the left extreme of the robot 
    uint16_t right_column;                  // Column occupied by the right extreme of the robot 
    uint16_t top_row;                       // Row occupied by the top extreme of the robot
    uint16_t bottom_row;                    // Row occupied by the bottom extreme of the robot
} robot;

// Game struct definition
struct game_s {
    uint8_t mode;                           // Mode: 0x00 -> MOVE; 0xFF -> EXPLORE
    bool start;                             // Start flag
    bool draw;                              // Draw flag: if true, then draw obstacles/walls in current direction
    bool win;                               // Win flag
} game;

// Internal functions
void Start(void);
void Restart(void);
void Clear(void);
void CalculateDistance(void);
void Win(void);
void DrawRobot(void);
void DrawRobotTrail(uint16_t shift);
void DrawObstacles(void);

/**
 * @brief  Game Initialization
 *
 * @param  none
 * @return none
 */
void GAME_Init(void) {
    // Init JOYSTICK
    JOYSTICK_Init();
    
    // Init LCD
    LCD_Init();
    
    // Init and calibrate TouchPanel
    TP_Init();
    TP_Calibrate();
    
    // Set background
    LCD_Clear(GUI_BG_COLOR);
    
    // Put TITLE Text
    LCD_PutString(GUI_TITLE_TEXT_X0,
                  GUI_TITLE_TEXT_Y0,
                  (uint8_t *)(GUI_TITLE_TEXT),
                  GUI_TITLE_TEXT_COLOR,
                  GUI_TITLE_TEXT_BG_COLOR);
    
    // Draw LABYRINTH Area
    LCD_DrawRectangle(GUI_LABYRINTH_X0,
                      GUI_LABYRINTH_Y0,
                      GUI_LABYRINTH_WIDTH,
                      GUI_LABYRINTH_HEIGHT,
                      GUI_LABYRINTH_BG_COLOR);
    
    // Draw RESTART Button
    LCD_DrawRectangle(GUI_RESTART_BUTTON_X0,
                      GUI_RESTART_BUTTON_Y0,
                      GUI_RESTART_BUTTON_WIDTH,
                      GUI_RESTART_BUTTON_HEIGHT,
                      GUI_RESTART_BUTTON_BG_COLOR);
    
    // Put RESTART Text
    LCD_PutString(GUI_RESTART_TEXT_X0,
                  GUI_RESTART_TEXT_Y0,
                  (uint8_t *)(GUI_RESTART_TEXT),
                  GUI_RESTART_TEXT_COLOR,
                  GUI_RESTART_TEXT_BG_COLOR);
    
    // Draw CLEAR Button
    LCD_DrawRectangle(GUI_CLEAR_BUTTON_X0,
                      GUI_CLEAR_BUTTON_Y0,
                      GUI_CLEAR_BUTTON_WIDTH,
                      GUI_CLEAR_BUTTON_HEIGHT,
                      GUI_CLEAR_BUTTON_BG_COLOR);
    
    // Put CLEAR Text
    LCD_PutString(GUI_CLEAR_TEXT_X0,
                  GUI_CLEAR_TEXT_Y0,
                  (uint8_t *)(GUI_CLEAR_TEXT),
                  GUI_CLEAR_TEXT_COLOR,
                  GUI_CLEAR_TEXT_BG_COLOR);
    
    // Put START Text
    LCD_PutString(GUI_START_TEXT_X0,
                  GUI_START_TEXT_Y0,
                  (uint8_t *)(GUI_START_TEXT),
                  GUI_START_TEXT_COLOR,
                  GUI_START_TEXT_BG_COLOR);
    
    // Set Joystick TIMER and TouchPanel TIMER priority
    TIMER_SetPriority(TOUCHPANEL_TIMER_NUM, TOUCHPANEL_TIMER_PRIORITY);
    TIMER_SetPriority(JOYSTICK_TIMER_NUM, JOYSTICK_TIMER_PRIORITY);
    
    // Init and enable TouchPanel TIMER to generate an interrupt every TOUCHPANEL_TIMER_TIME ms
    TIMER_Init(TOUCHPANEL_TIMER_NUM, TOUCHPANEL_TIMER_TIME, true, true, false);
    TIMER_Enable(TOUCHPANEL_TIMER_NUM);
    
    return;
}

/**
 * @brief  Game Touch
 *
 * @param  uint16_t x
 * @param  uint16_t y
 * @return none
 */
void GAME_Touch(uint16_t x, uint16_t y) {
    if (game.start == false) {
        // Touch LABYRINTH Area
        if ((x >= GUI_LABYRINTH_X0) && (x < (GUI_LABYRINTH_X0 + GUI_LABYRINTH_WIDTH)) && (y >= GUI_LABYRINTH_Y0) && (y < (GUI_LABYRINTH_Y0 + GUI_LABYRINTH_HEIGHT))) {
            Start();
        }
    } else {
        // Touch RESTART Button
        if ((x >= GUI_RESTART_BUTTON_X0) && (x < (GUI_RESTART_BUTTON_X0 + GUI_RESTART_BUTTON_WIDTH)) && (y >= GUI_RESTART_BUTTON_Y0) && (y < (GUI_RESTART_BUTTON_Y0 + GUI_RESTART_BUTTON_HEIGHT))) {
            Restart();
        }
        
        if (game.win == false) {
            // Touch CLEAR Button
            if ((x >= GUI_CLEAR_BUTTON_X0) && (x < (GUI_CLEAR_BUTTON_X0 + GUI_CLEAR_BUTTON_WIDTH)) && (y >= GUI_CLEAR_BUTTON_Y0) && (y < (GUI_CLEAR_BUTTON_Y0 + GUI_CLEAR_BUTTON_HEIGHT))) {
                Clear();
            }
        }
    }
    
    return;
}

/**
 * @brief  Game Change Mode
 *
 * @param  none
 * @param  none
 * @return none
 */
void GAME_ChangeMode(void) {
    // Change robot mode
    game.mode = ~game.mode;
    
    // Draw robot in new mode
    DrawRobot();
    
    return;
}

/**
 * @brief  Game Action
 *
 * @param  uint8_t Direction: 0 -> North; 1 -> East; 2 -> South; 3 -> West
 * @param  none
 * @return none
 */
void GAME_Action(uint8_t dir) {
    // If the current direction is different from the previous one
    if (dir != robot.dir) {
        // Update current direction
        robot.dir = dir;
        
        // Draw robot in new direction
        DrawRobot();
        
        // Calculate distance from obstacles/walls/exits
        CalculateDistance();
        
        // Set draw flag
        game.draw = true;
    }
    
    if (game.mode == 0x00) {               // MOVE mode
        uint16_t step_pos, px;
        
        // Set step to increment/decrement robot position based on current axis
        step_pos = (robot.dir & 1) ? robot.step_pos_x : robot.step_pos_y;
        
        // If the robot isn't in front of an obstacles/walls or on an exit
        if ((robot.dist - step_pos) >= 0) {
            // Decrement the distance from obstacles/walls/exits
            robot.dist -= step_pos;
            
            // robot.dir
            // North -> 0 -> 0000 -> Y Axis - Negative
            // East  -> 1 -> 0001 -> X Axis - Positive
            // South -> 2 -> 0010 -> Y Axis - Positive
            // West  -> 3 -> 0011 -> X Axis - Negative
            if ((robot.dir & 1) == 0) {     // Y Axis (North/South) 
                // Increment (South) or decrement (North) y position of the robot by one step
                robot.pos_y += (robot.dir & 2 ? 1: -1) * step_pos;
                
                // Calculate the robot top-left pixel corresponding to the new position on Y axis
                px = GUI_LABYRINTH_Y0 + ((robot.pos_y / SLOT_POINTS) * GUI_LABYRINTH_SLOT_HEIGHT) + ((robot.pos_y % SLOT_POINTS) / robot.step_gui_y);
                
                // If the current robot top-left pixel on Y axis is different from the previous one
                if (robot.px_y != px) {
                    // Draw robot trail
                    DrawRobotTrail((robot.px_y > px) ? (robot.px_y - px) : (px - robot.px_y));
                    
                    // Update the robot top-left pixel on Y axis
                    robot.px_y = px;
                    
                    // Draw robot in new position
                    DrawRobot();
                }
            } else {                        // X Axis (East/West)
                // Increment (East) or decrement (West) x position of the robot by one step
                robot.pos_x += ((robot.dir & 2 ? -1: 1) * step_pos);
                
                // Calculate the robot top-left pixel corresponding to the new position on X axis
                px = GUI_LABYRINTH_X0 + ((robot.pos_x / SLOT_POINTS) * GUI_LABYRINTH_SLOT_WIDTH) + ((robot.pos_x % SLOT_POINTS) / robot.step_gui_x);
                
                // If the current robot top-left pixel on X axis is different from the previous one
                if (robot.px_x != px) {
                    // Draw robot trail
                    DrawRobotTrail((robot.px_x > px) ? (robot.px_x - px) : (px - robot.px_x));
                    
                    // Update the robot top-left pixel on X axis
                    robot.px_x = px;
                    
                    // Draw robot in new position
                    DrawRobot();
                }
            }
            
            // If the distance is zero and on_exit is true, then the robot is on an exit
            if ((robot.dist == 0) && (robot.on_exit == true)) {
                Win();
                return;
            }
        }
    }
    
    // If draw flag is set and distance from obstacles/wall is less than ROBOT_MAX_RANGE, then draw obstacles/walls
    if (game.draw == true) {
        if (robot.dist <= (ROBOT_MAX_RANGE * SLOT_POINTS)) {
            DrawObstacles();
        }
    }
    
    return;
}

/**
 * @brief  Start the game
 *
 * @param  none
 * @return none
 */
void Start(void) {
    // Set the game starting mode
    game.mode = GAME_START_MODE;
    // Set start flag
    game.start = true;
    // Set draw flag
    game.draw = true;
    // Clear win flag
    game.win = false;
    
    // Set the robot starting direction
    robot.dir = ROBOT_START_DIR;
    // Set the step to increment/decrement robot position
    robot.step_pos_x = ROBOT_SPEED_X * (JOYSTICK_TIMER_TIME / 1000.0) * SLOT_POINTS;
    robot.step_pos_y = ROBOT_SPEED_Y * (JOYSTICK_TIMER_TIME / 1000.0) * SLOT_POINTS;
    // Set the robot starting position on the X axis (East/West) and on Y axis (North/South)
    robot.pos_x = ROBOT_START_X * SLOT_POINTS;
    robot.pos_y = ROBOT_START_Y * SLOT_POINTS;
    // Set the step to increment/decrement robot position in GUI
    robot.step_gui_x = SLOT_POINTS / GUI_LABYRINTH_SLOT_WIDTH;
    robot.step_gui_y = SLOT_POINTS / GUI_LABYRINTH_SLOT_HEIGHT;
    // Set the robot starting position on the X axis and on Y axis of the top-left pixel
    robot.px_x = GUI_LABYRINTH_X0 + ((robot.pos_x / SLOT_POINTS) * GUI_LABYRINTH_SLOT_WIDTH) + ((robot.pos_x % SLOT_POINTS) / robot.step_gui_x);
    robot.px_y = GUI_LABYRINTH_Y0 + ((robot.pos_y / SLOT_POINTS) * GUI_LABYRINTH_SLOT_HEIGHT) + ((robot.pos_y % SLOT_POINTS) / robot.step_gui_y);
    
    // Clear START Text
    LCD_DrawRectangle(GUI_START_TEXT_X0, GUI_START_TEXT_Y0, GUI_START_TEXT_WIDTH, GUI_START_TEXT_HEIGHT, GUI_LABYRINTH_BG_COLOR);
    
    // Draw robot in starting position and direction
    DrawRobot();
    
    // Calculate distance from obstacles/walls/exits
    CalculateDistance();
    
    // If distance from obstacles/walls is less than ROBOT_MAX_RANGE, then draw obstacles/walls
    if (robot.dist <= (ROBOT_MAX_RANGE * SLOT_POINTS)) {
        DrawObstacles();
    }
    
    // Init and enable Joystick TIMER to generate an interrupt every JOYSTICK_TIMER_TIME ms
    TIMER_Init(JOYSTICK_TIMER_NUM, JOYSTICK_TIMER_TIME, true, true, false);
    TIMER_Enable(JOYSTICK_TIMER_NUM);
    
    return;
}

/**
 * @brief  Restart the game
 *
 * @param  none
 * @return none
 */
void Restart(void) {
    // Disable and reset Joystick TIMER
    TIMER_Disable(JOYSTICK_TIMER_NUM);
    TIMER_Reset(JOYSTICK_TIMER_NUM);
    
    // Disable and reset TouchPanel TIMER
    TIMER_Disable(TOUCHPANEL_TIMER_NUM);
    TIMER_Reset(TOUCHPANEL_TIMER_NUM);
    
    // Clear start flag
    game.start = false;
    // Clear win flag
    game.win = false;
    
    // Clear LABYRINTH north walls
    LCD_DrawRectangle(GUI_LABYRINTH_X0 - GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_Y0 - GUI_LABYRINTH_SLOT_HEIGHT,
                      GUI_LABYRINTH_WIDTH + (GUI_LABYRINTH_SLOT_WIDTH * 2),
                      GUI_LABYRINTH_SLOT_HEIGHT,
                      GUI_BG_COLOR);
    // Clear LABYRINTH south walls
    LCD_DrawRectangle(GUI_LABYRINTH_X0 - GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_Y0 + GUI_LABYRINTH_HEIGHT,
                      GUI_LABYRINTH_WIDTH + (GUI_LABYRINTH_SLOT_WIDTH * 2),
                      GUI_LABYRINTH_SLOT_HEIGHT,
                      GUI_BG_COLOR);
    // Clear LABYRINTH east walls
    LCD_DrawRectangle(GUI_LABYRINTH_X0 + GUI_LABYRINTH_WIDTH,
                      GUI_LABYRINTH_Y0,
                      GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_HEIGHT,
                      GUI_BG_COLOR);
    // Clear LABYRINTH west walls
    LCD_DrawRectangle(GUI_LABYRINTH_X0 - GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_Y0,
                      GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_HEIGHT,
                      GUI_BG_COLOR);
    
    // Draw LABYRINTH Area
    LCD_DrawRectangle(GUI_LABYRINTH_X0,
                      GUI_LABYRINTH_Y0,
                      GUI_LABYRINTH_WIDTH,
                      GUI_LABYRINTH_HEIGHT,
                      GUI_LABYRINTH_BG_COLOR);
    
    // Put START Text
    LCD_PutString(GUI_START_TEXT_X0,
                  GUI_START_TEXT_Y0,
                  (uint8_t *)(GUI_START_TEXT),
                  GUI_START_TEXT_COLOR,
                  GUI_START_TEXT_BG_COLOR);
    
    // Enable TouchPanel TIMER
    TIMER_Enable(TOUCHPANEL_TIMER_NUM);
    
    return;
}

/**
 * @brief  Clear the game
 *
 * @param  none
 * @return none
 */
void Clear(void) {
    // Disable and reset Joystick TIMER    
    TIMER_Disable(JOYSTICK_TIMER_NUM);
    TIMER_Reset(JOYSTICK_TIMER_NUM);
    
    // Disable and reset TouchPanel TIMER
    TIMER_Disable(TOUCHPANEL_TIMER_NUM);
    TIMER_Reset(TOUCHPANEL_TIMER_NUM);
    
    // Clear LABYRINTH north walls
    LCD_DrawRectangle(GUI_LABYRINTH_X0 - GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_Y0 - GUI_LABYRINTH_SLOT_HEIGHT,
                      GUI_LABYRINTH_WIDTH + (GUI_LABYRINTH_SLOT_WIDTH * 2),
                      GUI_LABYRINTH_SLOT_HEIGHT,
                      GUI_BG_COLOR);
    // Clear LABYRINTH south walls
    LCD_DrawRectangle(GUI_LABYRINTH_X0 - GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_Y0 + GUI_LABYRINTH_HEIGHT,
                      GUI_LABYRINTH_WIDTH + (GUI_LABYRINTH_SLOT_WIDTH * 2),
                      GUI_LABYRINTH_SLOT_HEIGHT,
                      GUI_BG_COLOR);
    // Clear LABYRINTH east walls
    LCD_DrawRectangle(GUI_LABYRINTH_X0 + GUI_LABYRINTH_WIDTH,
                      GUI_LABYRINTH_Y0,
                      GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_HEIGHT,
                      GUI_BG_COLOR);
    // Clear LABYRINTH west walls
    LCD_DrawRectangle(GUI_LABYRINTH_X0 - GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_Y0,
                      GUI_LABYRINTH_SLOT_WIDTH,
                      GUI_LABYRINTH_HEIGHT,
                      GUI_BG_COLOR);
    // Draw LABYRINTH Area
    LCD_DrawRectangle(GUI_LABYRINTH_X0,
                      GUI_LABYRINTH_Y0,
                      GUI_LABYRINTH_WIDTH,
                      GUI_LABYRINTH_HEIGHT,
                      GUI_LABYRINTH_BG_COLOR);
    
    // Draw robot in latest position and direction
    DrawRobot();
    
    // If distance from obstacles/walls is less than ROBOT_MAX_RANGE, then draaw obstacles/walls
    if (robot.dist <= (ROBOT_MAX_RANGE * SLOT_POINTS)) {
        DrawObstacles();
    }
    
    // Enable TouchPanel TIMER
    TIMER_Enable(TOUCHPANEL_TIMER_NUM);
    
    // Enable Joystick TIMER    
    TIMER_Enable(JOYSTICK_TIMER_NUM);
    
    return;
}

/**
 * @brief  Calculate distance from obstacles/walls/exits
 *
 * @param  none
 * @return none
 */
void CalculateDistance(void) {
    int32_t i;
    
    // robot.dir
    // North -> 0 -> 0000 -> Y Axis - Negative
    // East  -> 1 -> 0001 -> X Axis - Positive
    // South -> 2 -> 0010 -> Y Axis - Positive
    // West  -> 3 -> 0011 -> X Axis - Negative
    if ((robot.dir & 1) == 0) {             // Y Axis (North/South)
        // Based on the dimension and current position of the robot, calculate the two columns occupied by the left and right extremes of the robot
        robot.left_column = robot.pos_x / SLOT_POINTS;
        robot.right_column = (robot.pos_x + (ROBOT_DIM_X * SLOT_POINTS) - 1) / SLOT_POINTS;
        
        if ((robot.dir & 2) == 0) {         // North
            for (i = robot.pos_y / SLOT_POINTS; i >= 0; i--) {
                if ((LABYRINTH[i][robot.left_column] == 2) && (LABYRINTH[i][robot.right_column] == 2)) {
                    robot.on_exit = true;
                    break;
                }
                if ((LABYRINTH[i][robot.left_column] == 1) || (LABYRINTH[i][robot.right_column] == 1)) {
                    robot.on_exit = false;
                    break;
                }
            }
            if (i == -1) {
                robot.on_exit = false;
            }
            robot.dist = robot.pos_y - ((i + 1) * SLOT_POINTS) + (robot.on_exit ? SLOT_POINTS: 0);
        } else {                            // South
            for (i = robot.pos_y / SLOT_POINTS; i < LABYRINTH_MAX_Y; i++) {
                if ((LABYRINTH[i][robot.left_column] == 2) && (LABYRINTH[i][robot.right_column] == 2)) {
                    robot.on_exit = true;
                    break;
                }
                if ((LABYRINTH[i][robot.left_column] == 1) || (LABYRINTH[i][robot.right_column] == 1)) {
                    robot.on_exit = false;
                    break;
                }
            }
            if (i == LABYRINTH_MAX_Y) {
                robot.on_exit = false;
            }
            robot.dist = (i * SLOT_POINTS) - (robot.pos_y + (ROBOT_DIM_Y * SLOT_POINTS)) + (robot.on_exit ? SLOT_POINTS: 0);
        }
    } else {                                // X Axis (East/West)
        // Based on the dimension and current position of the robot, calculate the two rows occupied by the top and bottom extremes of the robot
        robot.top_row = robot.pos_y / SLOT_POINTS;
        robot.bottom_row = (robot.pos_y + (ROBOT_DIM_Y * SLOT_POINTS) - 1) / SLOT_POINTS;
        
        if ((robot.dir & 2) == 0) {         // East
            for (i = robot.pos_x / SLOT_POINTS; i < LABYRINTH_MAX_X; i++) {
                if ((LABYRINTH[robot.top_row][i] == 2) && (LABYRINTH[robot.bottom_row][i] == 2)) {
                    robot.on_exit = true;
                    break;
                }
                if ((LABYRINTH[robot.top_row][i] == 1) || (LABYRINTH[robot.bottom_row][i] == 1)) {
                    robot.on_exit = false;
                    break;
                }
            }
            if (i == LABYRINTH_MAX_X) {
                robot.on_exit = false;
            }
            robot.dist = (i * SLOT_POINTS) - (robot.pos_x + (ROBOT_DIM_X * SLOT_POINTS)) + (robot.on_exit ? SLOT_POINTS: 0);
        } else {                            // West
            for (i = robot.pos_x / SLOT_POINTS; i >= 0; i--) {
                if ((LABYRINTH[robot.top_row][i] == 2) && (LABYRINTH[robot.bottom_row][i] == 2)) {
                    robot.on_exit = true;
                    break;
                }
                if ((LABYRINTH[robot.top_row][i] == 1) || (LABYRINTH[robot.bottom_row][i] == 1)) {
                    robot.on_exit = false;
                    break;
                }
            }
            if (i == -1) {
                robot.on_exit = false;
            }
            robot.dist = robot.pos_x - ((i + 1) * SLOT_POINTS) + (robot.on_exit ? SLOT_POINTS: 0);
        }
    }
    
    return;
}

/**
 * @brief  Win the game
 *
 * @param  none
 * @return none
 */
void Win(void) {
    // Disable and reset Joystick TIMER
    TIMER_Disable(JOYSTICK_TIMER_NUM);
    TIMER_Reset(JOYSTICK_TIMER_NUM);
    
    // Set win flag
    game.win = true;
    
    // Put WIN Text
    LCD_PutString(GUI_WIN_TEXT_X0,
                  GUI_WIN_TEXT_Y0,
                  (uint8_t *)(GUI_WIN_TEXT),
                  GUI_WIN_TEXT_COLOR,
                  GUI_WIN_TEXT_BG_COLOR);
    
    return;
}

/**
 * @brief  Draw the robot
 *
 * @param  none
 * @return none
 */
void DrawRobot(void) {
    uint16_t x0, y0, x1, y1;
    
    // Draw robot background
    LCD_DrawRectangle(robot.px_x, robot.px_y, ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH, ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT, (game.mode == 0 ? GUI_ROBOT_BG_COLOR_MOVE : GUI_ROBOT_BG_COLOR_EXPLORE));
    
    if ((robot.dir & 1) == 0) {             // Y Axis (North/South)
        // Draw straight segment of the arrow
        x0 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2) - 1;
        y0 = robot.px_y;
        x1 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2) - 1;
        y1 = robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1;
        LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
        LCD_DrawLine(x0 + 1, y0, x1 + 1, y1, GUI_ROBOT_DIRECTION_COLOR);
        
        if ((robot.dir & 2) == 0) {         // North (Upwards Arrow)
            // Draw left flap of the arrow
            x0 = robot.px_x;
            y0 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2) - 1;
            x1 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2) - 1;
            y1 = robot.px_y;
            LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
            LCD_DrawLine(x0, y0 + 1, x1 + 1, y1, GUI_ROBOT_DIRECTION_COLOR);
            
            // Draw right flap of the arrow
            x0 = robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1;
            y0 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2) - 1;
            x1 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2);
            y1 = robot.px_y;
            LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
            LCD_DrawLine(x0, y0 + 1, x1 - 1, y1, GUI_ROBOT_DIRECTION_COLOR);
        } else {                            // South (Downwards Arrow)
            // Draw left flap of the arrow
            x0 = robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1;
            y0 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2);
            x1 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2);
            y1 = robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1;
            LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
            LCD_DrawLine(x0, y0 - 1, x1 - 1, y1, GUI_ROBOT_DIRECTION_COLOR);
            
            // Draw right flap of the arrow
            x0 = robot.px_x;
            y0 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2);
            x1 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2) - 1;
            y1 = robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1;
            LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
            LCD_DrawLine(x0, y0 - 1, x1 + 1, y1, GUI_ROBOT_DIRECTION_COLOR);
        }
    } else {                                // X Axis (East/West)
        // Draw straight segment of the arrow
        x0 = robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1;
        y0 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2) - 1;
        x1 = robot.px_x;
        y1 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2) - 1;
        LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
        LCD_DrawLine(x0, y0 + 1, x1, y1 + 1, GUI_ROBOT_DIRECTION_COLOR);
        
        if ((robot.dir & 2) == 0) {         // East (Rightwards Arrow)
            // Draw left flap of the arrow
            x0 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2);
            y0 = robot.px_y;
            x1 = robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1;
            y1 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2) - 1;
            LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
            LCD_DrawLine(x0 - 1, y0, x1, y1 + 1, GUI_ROBOT_DIRECTION_COLOR);
            
            // Draw right flap of the arrow
            x0 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2);
            y0 = robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1;
            x1 = robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1;
            y1 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2);
            LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
            LCD_DrawLine(x0 - 1, y0, x1, y1 - 1, GUI_ROBOT_DIRECTION_COLOR);
        } else {                            // West (Leftwards Arrow)
            // Draw left flap of the arrow
            x0 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2) - 1;
            y0 = robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1;
            x1 = robot.px_x;
            y1 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2);
            LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
            LCD_DrawLine(x0 + 1, y0, x1, y1 - 1, GUI_ROBOT_DIRECTION_COLOR);
            
            // Draw right flap of the arrow
            x0 = robot.px_x + ((ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) / 2) - 1;
            y0 = robot.px_y;
            x1 = robot.px_x;
            y1 = robot.px_y + ((ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) / 2) - 1;
            LCD_DrawLine(x0, y0, x1, y1, GUI_ROBOT_DIRECTION_COLOR);
            LCD_DrawLine(x0 + 1, y0, x1, y1 + 1, GUI_ROBOT_DIRECTION_COLOR);
        }
    }
    
    return;
}

/**
 * @brief  Draw the robot trail
 *
 * @param  uint16_t shift: number of pixels shifted
 * @return none
 */
void DrawRobotTrail(uint16_t shift) {
    uint16_t i;
    
    // robot.dir
    // North -> 0 -> 0000 -> Y Axis - Negative
    // East  -> 1 -> 0001 -> X Axis - Positive
    // South -> 2 -> 0010 -> Y Axis - Positive
    // West  -> 3 -> 0011 -> X Axis - Negative
    if ((robot.dir & 1) == 0) {             // Y Axis (North/South)
        if ((robot.dir & 2) == 0) {         // North
            for (i = 0; i < shift; i++) {
                LCD_DrawLine(robot.px_x,
                             robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1 - i,
                             robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1,
                             robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1 - i,
                             GUI_ROBOT_TRAIL_COLOR);
            }
        } else {                            // South
            for (i = 0; i < shift; i++) {
                LCD_DrawLine(robot.px_x,
                             robot.px_y + i,
                             robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1,
                             robot.px_y + i,
                             GUI_ROBOT_TRAIL_COLOR);
            }
        }
    } else {                                // X Axis (East/West)
        if ((robot.dir & 2) == 0) {         // East
            for (i = 0; i < shift; i++) {
                LCD_DrawLine(robot.px_x + i,
                             robot.px_y,
                             robot.px_x + i,
                             robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1,
                             GUI_ROBOT_TRAIL_COLOR);
            }
        } else {                            // West
            for (i = 0; i < shift; i++) {
                LCD_DrawLine(robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1 - i,
                             robot.px_y,
                             robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH) - 1 - i,
                             robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT) - 1,
                             GUI_ROBOT_TRAIL_COLOR);
            }
        }
    }
    
    return;
}

/**
 * @brief  Draw the obstacles
 *
 * @param  none
 * @return none
 */
void DrawObstacles(void) {
    int32_t row, column;
    uint16_t i;
    uint16_t x0, y0, width, height;
    
    if ((robot.dir & 1) == 0) {         // Y Axis (North/South)
        if ((robot.dir & 2) == 0) {     // North
            row = ((robot.pos_y - robot.dist) / SLOT_POINTS) - 1;
            if (row >= 0) {
                // Draw obstacles
                for (i = robot.left_column; i <= robot.right_column; i++) {
                    if (LABYRINTH[row][i] == 1) {
                        x0 = GUI_LABYRINTH_X0 + (i * GUI_LABYRINTH_SLOT_WIDTH);
                        y0 = GUI_LABYRINTH_Y0 + (row * GUI_LABYRINTH_SLOT_HEIGHT);
                        width = GUI_LABYRINTH_SLOT_WIDTH;
                        height = GUI_LABYRINTH_SLOT_HEIGHT;
                        if (x0 < robot.px_x) {
                            width -= robot.px_x - x0;
                            x0 = robot.px_x;
                        }
                        if ((x0 + width) > (robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH))) {
                            width -= (x0 + width) - (robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH));
                        }
                        LCD_DrawRectangle(x0, y0, width, height, GUI_OBSTACLES_BG_COLOR);
                    }
                }
            } else {
                // Draw walls
                for (i = robot.left_column; i <= robot.right_column; i++) {
                    if (LABYRINTH[row + 1][i] != 2) {
                        x0 = GUI_LABYRINTH_X0 + (i * GUI_LABYRINTH_SLOT_WIDTH);
                        y0 = GUI_LABYRINTH_Y0 - GUI_LABYRINTH_SLOT_HEIGHT;
                        width = GUI_LABYRINTH_SLOT_WIDTH;
                        height = GUI_LABYRINTH_SLOT_HEIGHT;
                        if (x0 < robot.px_x) {
                            width -= robot.px_x - x0;
                            x0 = robot.px_x;
                        }
                        if ((x0 + width) > (robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH))) {
                            width -= (x0 + width) - (robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH));
                        }
                        LCD_DrawRectangle(x0, y0, width, height, GUI_OBSTACLES_BG_COLOR);
                    }
                }
            }
        } else {                        // South
            row = ((robot.pos_y + robot.dist) / SLOT_POINTS) + ROBOT_DIM_Y;
            if (row < LABYRINTH_MAX_Y) {
                // Draw obstacles
                for (i = robot.left_column; i <= robot.right_column; i++) {
                    if (LABYRINTH[row][i] == 1) {
                        x0 = GUI_LABYRINTH_X0 + (i * GUI_LABYRINTH_SLOT_WIDTH);
                        y0 = GUI_LABYRINTH_Y0 + (row * GUI_LABYRINTH_SLOT_HEIGHT);
                        width = GUI_LABYRINTH_SLOT_WIDTH;
                        height = GUI_LABYRINTH_SLOT_HEIGHT;
                        if (x0 < robot.px_x) {
                            width -= robot.px_x - x0;
                            x0 = robot.px_x;
                        }
                        if ((x0 + width) > (robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH))) {
                            width -= (x0 + width) - (robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH));
                        }
                        LCD_DrawRectangle(x0, y0, width, height, GUI_OBSTACLES_BG_COLOR);
                    }
                }
            } else {
                // Draw walls
                for (i = robot.left_column; i <= robot.right_column; i++) {
                    if (LABYRINTH[row - 1][i] != 2) {
                        x0 = GUI_LABYRINTH_X0 + (i * GUI_LABYRINTH_SLOT_WIDTH);
                        y0 = GUI_LABYRINTH_Y0 + (LABYRINTH_MAX_Y * GUI_LABYRINTH_SLOT_HEIGHT);
                        width = GUI_LABYRINTH_SLOT_WIDTH;
                        height = GUI_LABYRINTH_SLOT_HEIGHT;
                        if (x0 < robot.px_x) {
                            width -= robot.px_x - x0;
                            x0 = robot.px_x;
                        }
                        if ((x0 + width) > (robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH))) {
                            width -= (x0 + width) - (robot.px_x + (ROBOT_DIM_X * GUI_LABYRINTH_SLOT_WIDTH));
                        }
                        LCD_DrawRectangle(x0, y0, width, height, GUI_OBSTACLES_BG_COLOR);
                    }
                }
            }
        }
    } else {                            // X Axis (East/West)
        if ((robot.dir & 2) == 0) {     // East
            column = ((robot.pos_x + robot.dist) / SLOT_POINTS) + ROBOT_DIM_X;
            if (column < LABYRINTH_MAX_X) {
                // Draw obstacles
                for (i = robot.top_row; i <= robot.bottom_row; i++) {
                    if (LABYRINTH[i][column] == 1) {
                        x0 = GUI_LABYRINTH_X0 + (column * GUI_LABYRINTH_SLOT_WIDTH);
                        y0 = GUI_LABYRINTH_Y0 + (i * GUI_LABYRINTH_SLOT_HEIGHT);
                        width = GUI_LABYRINTH_SLOT_WIDTH;
                        height = GUI_LABYRINTH_SLOT_HEIGHT;
                        if (y0 < robot.px_y) {
                            height -= robot.px_y - y0;
                            y0 = robot.px_y;
                        }
                        if ((y0 + height) > (robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT))) {
                            height -= (y0 + height) - (robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT));
                        }
                        LCD_DrawRectangle(x0, y0, width, height, GUI_OBSTACLES_BG_COLOR);
                    }
                }
            } else {
                // Draw walls
                for (i = robot.top_row; i <= robot.bottom_row; i++) {
                    if (LABYRINTH[i][column - 1] != 2) {
                        x0 = GUI_LABYRINTH_X0 + (LABYRINTH_MAX_X * GUI_LABYRINTH_SLOT_WIDTH);
                        y0 = GUI_LABYRINTH_Y0 + (i * GUI_LABYRINTH_SLOT_HEIGHT);
                        width = GUI_LABYRINTH_SLOT_WIDTH;
                        height = GUI_LABYRINTH_SLOT_HEIGHT;
                        if (y0 < robot.px_y) {
                            height -= robot.px_y - y0;
                            y0 = robot.px_y;
                        }
                        if ((y0 + height) > (robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT))) {
                            height -= (y0 + height) - (robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT));
                        }
                        LCD_DrawRectangle(x0, y0, width, height, GUI_OBSTACLES_BG_COLOR);
                    }
                }
            }
        } else {                        // West
            column = ((robot.pos_x - robot.dist) / SLOT_POINTS) - 1;
            if (column >= 0) {
                // Draw obstacles
                for (i = robot.top_row; i <= robot.bottom_row; i++) {
                    if (LABYRINTH[i][column] == 1) {
                        x0 = GUI_LABYRINTH_X0 + (column * GUI_LABYRINTH_SLOT_WIDTH);
                        y0 = GUI_LABYRINTH_Y0 + (i * GUI_LABYRINTH_SLOT_HEIGHT);
                        width = GUI_LABYRINTH_SLOT_WIDTH;
                        height = GUI_LABYRINTH_SLOT_HEIGHT;
                        if (y0 < robot.px_y) {
                            height -= robot.px_y - y0;
                            y0 = robot.px_y;
                        }
                        if ((y0 + height) > (robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT))) {
                            height -= (y0 + height) - (robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT));
                        }
                        LCD_DrawRectangle(x0, y0, width, height, GUI_OBSTACLES_BG_COLOR);
                    }
                }
            } else {
                // Draw walls
                for (i = robot.top_row; i <= robot.bottom_row; i++) {
                    if (LABYRINTH[i][column + 1] != 2) {
                        x0 = GUI_LABYRINTH_X0 - GUI_LABYRINTH_SLOT_WIDTH;
                        y0 = GUI_LABYRINTH_Y0 + (i * GUI_LABYRINTH_SLOT_HEIGHT);
                        width = GUI_LABYRINTH_SLOT_WIDTH;
                        height = GUI_LABYRINTH_SLOT_HEIGHT;
                        if (y0 < robot.px_y) {
                            height -= robot.px_y - y0;
                            y0 = robot.px_y;
                        }
                        if ((y0 + height) > (robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT))) {
                            height -= (y0 + height) - (robot.px_y + (ROBOT_DIM_Y * GUI_LABYRINTH_SLOT_HEIGHT));
                        }
                        LCD_DrawRectangle(x0, y0, width, height, GUI_OBSTACLES_BG_COLOR);
                    }
                }
            }
        }
    }
    
    // Clear draw flag
    game.draw = false;
    
    return;
}
