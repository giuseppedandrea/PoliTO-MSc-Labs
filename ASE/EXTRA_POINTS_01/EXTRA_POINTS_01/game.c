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
 * @date:    2021-01-12
 ******************************************************************************/

#include "game.h"

#include "LPC17xx.h"
#include "LED/led.h"
#include "BUTTON/button.h"
#include "TIMER/timer.h"
#include "RIT/RIT.h"

#define RIT_TIME 50                         // RIT time in milliseconds (ms)
#define SLOT_POINTS 1000                    // Points in one slot (1 slot = SLOT_POINTS)
#define TIMER_DIRECTION_LED 0               // Timer number of the DIRECTION LED 
#define TIMER_RUNNING_LED 1                 // Timer number of the RUNNING LED
#define TIMER_WIN 2                         

static const uint8_t MAP_LED_DIR[] = {LED_DIR_N, LED_DIR_E, LED_DIR_S, LED_DIR_W};  // HW mapping of the DIRECTION LED: 0 -> North; 1 -> East; 2 -> South; 3 -> West

// Labyrinth definition: 0 -> free ground; 1 -> obstacle; 2 -> exit
static const uint8_t LABYRINTH[MAX_Y][MAX_X] = {
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

// Robot definition
struct robot_s {
    uint32_t pos_x;                         // Position on the X axis (1 slot = SLOT_POINTS)
    uint32_t pos_y;                         // Position on the Y axis (1 slot = SLOT_POINTS)
    uint8_t dir;                            // Direction: 0 -> North; 1 -> East; 2 -> South; 3 -> West
    uint32_t dist;                          // Distance from obstacles/walls/exits
    bool on_exit;                           // True -> The robot is pointing towards one of the exits; False -> Otherwise
    uint32_t step;                          // Step to increment/decrement robot position
} robot;

// Internal functions
void CalculateDistance(void);
void ManageRunningLED(bool reset);
void ManageDirectionLED(bool reset);
void Win(void);

/**
 * @brief  Game Initialization
 *
 * @param  none
 * @return none
 */
void GAME_Init(void) {
    LED_Init();                             // Init LEDs
    
    // Set interrupt priorities
    RIT_SetPriority(0);
    BUTTON_SetPriority(BUTTON_START, 1);
    BUTTON_SetPriority(BUTTON_RUN, 2);
    BUTTON_SetPriority(BUTTON_ROTATE, 1);
    TIMER_SetPriority(TIMER_DIRECTION_LED, 3);
    TIMER_SetPriority(TIMER_RUNNING_LED, 3);
    TIMER_SetPriority(TIMER_WIN, 3);
    
    RIT_Init(RIT_TIME, true, true);         // Init RIT
    RIT_Enable();                           // Enable RIT
    
    BUTTON_Init(BUTTON_START);              // Init START button
    BUTTON_Enable(BUTTON_START);            // Enable START button
    
    return;
}

/**
 * @brief  Game Start
 *
 * @param  none
 * @return none
 */
void GAME_Start(void) {    
    BUTTON_Init(BUTTON_RUN);                // Init RUN button
    BUTTON_Enable(BUTTON_RUN);              // Enable RUN button
    
    BUTTON_Init(BUTTON_ROTATE);             // Init ROTATE button
    BUTTON_Enable(BUTTON_ROTATE);           // Enable ROTATE button
    
    robot.pos_x = START_X * SLOT_POINTS;    // Set the robot starting position on the X axis (East/West)
    robot.pos_y = START_Y * SLOT_POINTS;    // Set the robot starting position on the Y axis (North/South)
    
    robot.dir = (START_DIR + 3) % 4;        // Set the robot starting direction with START_DIR rotated anticlockwise (the correct starting direction will set in the next instruction)
    GAME_RobotRotate();                     // Rotate the robot clockwise (also calculate step and distance and manage DIRECTION LED)
    
    return;
}

/**
 * @brief  Change the robot direction (clockwise rotation)
 *
 * @param  none
 * @return none
 */
void GAME_RobotRotate(void) {
    robot.dir = (robot.dir + 1) % 4;        // Clockwise rotation of the robot direction: 0 -> North; 1 -> East; 2 -> South; 3 -> West
    
    robot.step = (uint32_t)(((robot.dir & 1) ? SPEED_X: SPEED_Y) * (RIT_TIME / 1000.0) * SLOT_POINTS);  // Set the step (speed * RIT time) to increment/decrement robot position
    
    CalculateDistance();                    // Calculate distance from obstacles/walls/exits
    
    ManageDirectionLED(true);               // Reset DIRECTION LED
    ManageDirectionLED(false);              // Set DIRECTION LED
    
    return;
}

/**
 * @brief  Move the robot in the current direction
 *
 * @param  none
 * @return none
 */
void GAME_RobotMove(void) {
    // If the robot isn't in front of an obstacles/walls or on an exit
    if ((int32_t)(robot.dist - robot.step) >= 0) {
        robot.dist -= robot.step;                                                   // Decrement the distance from obstacles/walls/exits
        
        // robot.dir
        // North -> 0 -> 0000 -> Y Axis - Negative
        // East  -> 1 -> 0001 -> X Axis - Positive
        // South -> 2 -> 0010 -> Y Axis - Positive
        // West  -> 3 -> 0011 -> X Axis - Negative
        if ((robot.dir & 1) == 0) {
            robot.pos_y += ((robot.dir & 2 ? 1: -1) * robot.step);                  // Increment (South) or decrement (North) y position of the robot by one step
        } else {
            robot.pos_x += ((robot.dir & 2 ? -1: 1) * robot.step);                  // Increment (East) or decrement (West) x position of the robot by one step
        }
    }
    
    // If the distance is zero and on_exit is true, then the robot is on an exit
    if ((robot.dist == 0) && (robot.on_exit == true)) {
        Win();
    } else {
        ManageDirectionLED(false);          // Check DIRECTION LED
        ManageRunningLED(false);            // Check RUNNING LED
    }
    
    return;
}

/**
 * @brief  Stop the robot movement
 *
 * @param  none
 * @return none
 */
void GAME_RobotStop(void) {
    ManageRunningLED(true);                 // Reset RUNNING LED
    
    return;
}

/**
 * @brief  Get current HW mapping of the DIRECTION LED
 *
 * @param  none
 * @return uint8_t Current HW mapping of the DIRECTION LED
 */
uint8_t GAME_GetDirectionLED(void) {
    return MAP_LED_DIR[robot.dir];
}

/**
 * @brief  Get current HW mapping of the RUNNING LED
 *
 * @param  none
 * @return uint8_t Current HW mapping of the RUNNING LED
 */
uint8_t GAME_GetRunningLED(void) {
    return LED_RUNNING;
}

/**
 * @brief  Calculate distance from obstacles/walls/exits
 *
 * @param  none
 * @return none
 */
void CalculateDistance(void) {
    int32_t i;
    uint32_t left_column, right_column;
    uint32_t top_row, bottom_row;
    
    // robot.dir
    // North -> 0 -> 0000 -> Y Axis - Negative
    // East  -> 1 -> 0001 -> X Axis - Positive
    // South -> 2 -> 0010 -> Y Axis - Positive
    // West  -> 3 -> 0011 -> X Axis - Negative
    
    if ((robot.dir & 1) == 0) {             // Y Axis (North/South)
        // Based on the dimension and current position of the robot, calculate the two columns occupied by the left and right extremes of the robot
        left_column = robot.pos_x / SLOT_POINTS;
        right_column = (robot.pos_x + (DIM_X * SLOT_POINTS) - 1) / SLOT_POINTS;
        
        if ((robot.dir & 2) == 0) {         // North
            for (i = (uint32_t)(robot.pos_y / SLOT_POINTS); i >= 0; i--) {
                if ((LABYRINTH[i][left_column] == 2) && (LABYRINTH[i][right_column] == 2)) {
                    robot.on_exit = true;
                    break;
                }
                if ((LABYRINTH[i][left_column] == 1) || (LABYRINTH[i][right_column] == 1)) {
                    robot.on_exit = false;
                    break;
                }
            }
            if (i == -1) {
                robot.on_exit = false;
            }
            robot.dist = ((robot.pos_y - ((DIM_Y * SLOT_POINTS) / 2.0)) - ((i + 0.5) * SLOT_POINTS)) + (robot.on_exit ? SLOT_POINTS: 0);
        } else {                            // South
            for (i = (uint32_t)(robot.pos_y / SLOT_POINTS); i < MAX_Y; i++) {
                if ((LABYRINTH[i][left_column] == 2) && (LABYRINTH[i][right_column] == 2)) {
                    robot.on_exit = true;
                    break;
                }
                if ((LABYRINTH[i][left_column] == 1) || (LABYRINTH[i][right_column] == 1)) {
                    robot.on_exit = false;
                    break;
                }
            }
            if (i == MAX_Y) {
                robot.on_exit = false;
            }
            robot.dist = (((i - 0.5) * SLOT_POINTS) - (robot.pos_y + ((DIM_Y * SLOT_POINTS) / 2.0))) + (robot.on_exit ? SLOT_POINTS: 0);
        }
    } else {                                // X Axis (East/West)
        // Based on the dimension and current position of the robot, calculate the two rows occupied by the top and bottom extremes of the robot
        top_row = robot.pos_y / SLOT_POINTS;
        bottom_row = (robot.pos_y + (DIM_Y * SLOT_POINTS) - 1) / SLOT_POINTS;
        
        if ((robot.dir & 2) == 0) {         // East
            for (i = (uint32_t)(robot.pos_x / SLOT_POINTS); i < MAX_X; i++) {
                if ((LABYRINTH[top_row][i] == 2) &&| (LABYRINTH[bottom_row][i] == 2)) {
                    robot.on_exit = true;
                    break;
                }
                if ((LABYRINTH[top_row][i] == 1) || (LABYRINTH[bottom_row][i] == 1)) {
                    robot.on_exit = false;
                    break;
                }
            }
            if (i == MAX_X) {
                robot.on_exit = false;
            }
            robot.dist = (((i - 0.5) * SLOT_POINTS) - (robot.pos_x + ((DIM_X * SLOT_POINTS) / 2.0))) + (robot.on_exit ? SLOT_POINTS: 0);
        } else {                            // West
            for (i = (uint32_t)(robot.pos_x / SLOT_POINTS); i >= 0; i--) {
                if ((LABYRINTH[top_row][i] == 2) && (LABYRINTH[bottom_row][i] == 2)) {
                    robot.on_exit = true;
                    break;
                }
                if ((LABYRINTH[top_row][i] == 1) || (LABYRINTH[bottom_row][i] == 1)) {
                    robot.on_exit = false;
                    break;
                }
            }
            if (i == -1) {
                robot.on_exit = false;
            }
            robot.dist = ((robot.pos_x - ((DIM_X * SLOT_POINTS) / 2.0)) - ((i + 0.5) * SLOT_POINTS)) + (robot.on_exit ? SLOT_POINTS: 0);
        }
    }
    
    return;
}

/**
 * @brief  Manage Running LED
 *
 * @param  Bool If reset is true, disable RUNNING LED
 * @return none
 */
void ManageRunningLED(bool reset) {
    static uint8_t flag = 0;
    
    if (reset == true) {
        TIMER_Disable(TIMER_RUNNING_LED);
        TIMER_Reset(TIMER_RUNNING_LED);
        LED_Off(LED_RUNNING);
        
        flag = 0;
    } else {
        if (robot.dist == 0) {
            if ((flag & 1) == 0) {
                ManageRunningLED(true);
                
                TIMER_Init(TIMER_RUNNING_LED, (uint32_t)((1000.0 / FREQ_RUNNING_BLOCK) / 2), true, true, false);
                TIMER_Enable(TIMER_RUNNING_LED);
                
                flag = 1;
            }
        } else {
            if ((flag & 2) == 0) {
                ManageRunningLED(true);
                
                TIMER_Init(TIMER_RUNNING_LED, (uint32_t)((1000.0 / FREQ_RUNNING_MOVE) / 2), true, true, false);
                TIMER_Enable(TIMER_RUNNING_LED);
                
                flag = 2;
            }
        }
    }
    
    return;
}

/**
 * @brief  Manage Direction LED
 *
 * @param  Bool If reset is true, disable DIRECTION LED
 * @return none
 */
void ManageDirectionLED(bool reset) {
    static uint8_t flag = 0;
    
    if (reset == true) {
        TIMER_Disable(TIMER_DIRECTION_LED);
        TIMER_Reset(TIMER_DIRECTION_LED);
        LED_Off(LED_DIR_N);
        LED_Off(LED_DIR_E);
        LED_Off(LED_DIR_S);
        LED_Off(LED_DIR_W);
        
        flag = 0;
    } else {
        if ((robot.dist > (RANGE_FAR_SUP * 1000)) || (robot.on_exit == true)) {
            if ((flag & 1) == 0) {
                ManageDirectionLED(true);
                
                LED_On(GAME_GetDirectionLED());
                
                flag = 1;
            }
        } else if (robot.dist <= (RANGE_FAR_SUP * SLOT_POINTS) && robot.dist >= (RANGE_FAR_INF * SLOT_POINTS)) {
            if ((flag & 2) == 0) {
                ManageDirectionLED(true);
                
                TIMER_Init(TIMER_DIRECTION_LED, (uint32_t)((1000.0 / FREQ_DIRECTION_RANGE_FAR) / 2), true, true, false);
                TIMER_Enable(TIMER_DIRECTION_LED);
                
                flag = 2;
            }
        } else if (robot.dist <= (RANGE_NEAR_SUP * SLOT_POINTS) && robot.dist >= (RANGE_NEAR_INF * SLOT_POINTS)) {
            if ((flag & 4) == 0) {
                ManageDirectionLED(true);
                
                TIMER_Init(TIMER_DIRECTION_LED, (uint32_t)((1000.0 / FREQ_DIRECTION_RANGE_NEAR) / 2), true, true, false);
                TIMER_Enable(TIMER_DIRECTION_LED);
                
                flag = 4;
            }
        } else if (robot.dist == 0) {
            if ((flag & 8) == 0) {
                ManageDirectionLED(true);
                
                TIMER_Init(TIMER_DIRECTION_LED, (uint32_t)((1000.0 / FREQ_DIRECTION_BLOCK) / 2), true, true, false);
                TIMER_Enable(TIMER_DIRECTION_LED);
                
                flag = 8;
            }
        }
    }
    
    return;
}

/**
 * @brief  Win
 *
 * @param  none
 * @return none
 */
void Win(void) {
    RIT_Disable();
    
    BUTTON_Disable(BUTTON_RUN);
    BUTTON_Disable(BUTTON_ROTATE);
    
    ManageDirectionLED(true);
    ManageRunningLED(true);
    
    TIMER_Init(TIMER_WIN, (uint32_t)((1000.0 / FREQ_WIN) / 2), true, true, false);
    TIMER_Enable(TIMER_WIN);
    
    return;
}
