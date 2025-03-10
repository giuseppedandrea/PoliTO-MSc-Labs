/****************************************Copyright (c)**************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:           GLCD.c
** Descriptions:        Has been tested SSD1289��ILI9320��R61505U��SSD1298��ST7781��SPFD5408B��ILI9325��ILI9328��
**                      HX8346A��HX8347A
**------------------------------------------------------------------------------------------------------
** Created by:          AVRman
** Created date:        2012-3-10
** Version:             v1.3
** Descriptions:        The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:         Paolo Bernardi
** Modified date:       03/01/2020
** Version:             v2.0
** Descriptions:        simple arrangement for screen usage
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h" 
#include "AsciiLib.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Code;

/* Private define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */
#define  HX8346A    10 /* 0x0046 */
#define  HX8347D    11 /* 0x0047 */
#define  HX8347A    12 /* 0x0047 */
#define  LGDP4535   13 /* 0x4535 */
#define  SSD2119    14 /* 3.5 LCD 0x9919 */

/*******************************************************************************
* Function Name  : Lcd_Configuration
* Description    : Configures LCD Control lines
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void LCD_Configuration(void) {
    /* Configure the LCD Control pins */

    /* EN = P0.19 , LE = P0.20 , DIR = P0.21 , CS = P0.22 , RS = P0.23 , RS = P0.23 */
    /* RS = P0.23 , WR = P0.24 , RD = P0.25 , DB[0.7] = P2.0...P2.7 , DB[8.15]= P2.0...P2.7 */
    LPC_GPIO0->FIODIR |= 0x03f80000;
    LPC_GPIO0->FIOSET = 0x03f80000;
}

/*******************************************************************************
* Function Name  : LCD_Send
* Description    : 
* Input          : - byte: byte to be sent
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_Send(uint16_t byte) {
    LPC_GPIO2->FIODIR |= 0xFF;              /* P2.0...P2.7 Output */
    LCD_DIR(1)                              /* Interface A->B */
    LCD_EN(0)                               /* Enable 2A->2B */
    LPC_GPIO2->FIOPIN = byte;               /* Write D0..D7 */
    LCD_LE(1)
    LCD_LE(0)                               /* latch D0..D7	*/
    LPC_GPIO2->FIOPIN = byte >> 8;          /* Write D8..D15 */
}

/*******************************************************************************
* Function Name  : wait_delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void wait_delay(int count) {
    while(count--);
}

/*******************************************************************************
* Function Name  : LCD_Read
* Description    : 
* Input          : - byte: byte to be read
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_Read(void) {
    uint16_t value;

    LPC_GPIO2->FIODIR &= ~(0xFF);               /* P2.0...P2.7 Input */
    LCD_DIR(0);                                 /* Interface B->A */
    LCD_EN(0);                                  /* Enable 2B->2A */
    wait_delay(30);                             /* delay some times */
    value = LPC_GPIO2->FIOPIN0;                 /* Read D8..D15 */
    LCD_EN(1);                                  /* Enable 1B->1A */
    wait_delay(30);                             /* delay some times */
    value = (value << 8) | LPC_GPIO2->FIOPIN0;  /* Read D0..D7 */
    LCD_DIR(1);
    return value;
}

/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Description    : 
* Input          : - index: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteIndex(uint16_t index) {
    LCD_CS(0);
    LCD_RS(0);
    LCD_RD(1);
    LCD_Send(index);
    wait_delay(22);
    LCD_WR(0);
    wait_delay(1);
    LCD_WR(1);
    LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : 
* Input          : - index: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteData(uint16_t data) {
    LCD_CS(0);
    LCD_RS(1);
    LCD_Send(data);
    LCD_WR(0);
    wait_delay(1);
    LCD_WR(1);
    LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 
* Input          : None
* Output         : None
* Return         : ]
* Attention      : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_ReadData(void) {
    uint16_t value;

    LCD_CS(0);
    LCD_RS(1);
    LCD_WR(1);
    LCD_RD(0);
    value = LCD_Read();

    LCD_RD(1);
    LCD_CS(1);

    return value;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue) {
    /* Write 16-bit Index, then Write Reg */
    LCD_WriteIndex(LCD_Reg);
    /* Write 16-bit Reg */
    LCD_WriteData(LCD_RegValue);
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention      : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_ReadReg(uint16_t LCD_Reg) {
    uint16_t LCD_RAM;

    /* Write 16-bit Index (then Read Reg) */
    LCD_WriteIndex(LCD_Reg);
    /* Read 16-bit Reg */
    LCD_RAM = LCD_ReadData();
    return LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 1141; j++);
    }
}

/*******************************************************************************
* Function Name  : LCD_Init
* Description    : Initialize TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Init(void) {
    uint16_t DeviceCode;

    LCD_Configuration();
    delay_ms(100);
    DeviceCode = LCD_ReadReg(0x0000);

    if ((DeviceCode == 0x9325) || (DeviceCode == 0x9328)) {
        LCD_Code = ILI9325;
        LCD_WriteReg(0x00e7, 0x0010);
        LCD_WriteReg(0x0000, 0x0001);                               /* start internal osc */
        LCD_WriteReg(0x0001, 0x0100);
        LCD_WriteReg(0x0002, 0x0700);                               /* power on sequence */
        LCD_WriteReg(0x0003, (1<<12) | (1<<5) | (1<<4) | (0<<3));   /* importance */
        LCD_WriteReg(0x0004,0x0000);
        LCD_WriteReg(0x0008, 0x0207);
        LCD_WriteReg(0x0009, 0x0000);
        LCD_WriteReg(0x000a, 0x0000);                               /* display setting */
        LCD_WriteReg(0x000c, 0x0001);                               /* display setting */
        LCD_WriteReg(0x000d, 0x0000);
        LCD_WriteReg(0x000f, 0x0000);
        /* Power On sequence */
        LCD_WriteReg(0x0010, 0x0000);
        LCD_WriteReg(0x0011, 0x0007);
        LCD_WriteReg(0x0012, 0x0000);
        LCD_WriteReg(0x0013, 0x0000);
        delay_ms(50);                                               /* delay 50 ms */
        LCD_WriteReg(0x0010, 0x1590);
        LCD_WriteReg(0x0011, 0x0227);
        delay_ms(50);                                               /* delay 50 ms */
        LCD_WriteReg(0x0012, 0x009c);
        delay_ms(50);                                               /* delay 50 ms */
        LCD_WriteReg(0x0013, 0x1900);
        LCD_WriteReg(0x0029, 0x0023);
        LCD_WriteReg(0x002b, 0x000e);
        delay_ms(50);                                               /* delay 50 ms */
        LCD_WriteReg(0x0020, 0x0000);
        LCD_WriteReg(0x0021, 0x0000);
        delay_ms(50);                                               /* delay 50 ms */
        LCD_WriteReg(0x0030, 0x0007);
        LCD_WriteReg(0x0031, 0x0707);
        LCD_WriteReg(0x0032, 0x0006);
        LCD_WriteReg(0x0035, 0x0704);
        LCD_WriteReg(0x0036, 0x1f04);
        LCD_WriteReg(0x0037, 0x0004);
        LCD_WriteReg(0x0038, 0x0000);
        LCD_WriteReg(0x0039, 0x0706);
        LCD_WriteReg(0x003c, 0x0701);
        LCD_WriteReg(0x003d, 0x000f);
        delay_ms(50);                                               /* delay 50 ms */
        LCD_WriteReg(0x0050, 0x0000);
        LCD_WriteReg(0x0051, 0x00ef);
        LCD_WriteReg(0x0052, 0x0000);
        LCD_WriteReg(0x0053, 0x013f);
        LCD_WriteReg(0x0060, 0xa700);
        LCD_WriteReg(0x0061, 0x0001);
        LCD_WriteReg(0x006a, 0x0000);
        LCD_WriteReg(0x0080, 0x0000);
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);
        LCD_WriteReg(0x0090, 0x0010);
        LCD_WriteReg(0x0092, 0x0000);
        LCD_WriteReg(0x0093, 0x0003);
        LCD_WriteReg(0x0095, 0x0110);
        LCD_WriteReg(0x0097, 0x0000);
        LCD_WriteReg(0x0098, 0x0000);
        /* display on sequence */
        LCD_WriteReg(0x0007, 0x0133);
        LCD_WriteReg(0x0020, 0x0000);
        LCD_WriteReg(0x0021, 0x0000);
    }

    delay_ms(50);                                                   /* delay 50 ms */
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - x0: specifies the X position.
*                  - y0: specifies the Y position.
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void LCD_SetCursor(uint16_t x0, uint16_t y0) {
    #if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

    uint16_t temp = Xpos;

    Xpos = Ypos;
    Ypos = (MAX_X - 1) - temp;

    #elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

    #endif
    
    switch (LCD_Code) {
        default:                            /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
            LCD_WriteReg(0x0020, x0);
            LCD_WriteReg(0x0021, y0);
            break; 
        case SSD1298:                       /* 0x8999 */
        case SSD1289:                       /* 0x8989 */
            LCD_WriteReg(0x004e, x0);
            LCD_WriteReg(0x004f, y0);
            break;
        case HX8346A:                       /* 0x0046 */
        case HX8347A:                       /* 0x0047 */
        case HX8347D:                       /* 0x0047 */
            LCD_WriteReg(0x02, x0 >> 8);
            LCD_WriteReg(0x03, x0);
            LCD_WriteReg(0x06, y0 >> 8);
            LCD_WriteReg(0x07, y0);
            break;
        case SSD2119:                        /* 3.5 LCD 0x9919 */
            break;
    }
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 
* Input          : - color: Screen Color
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Clear(uint16_t color) {
    uint32_t i;

    LCD_SetCursor(0, 0);
    LCD_WriteIndex(0x0022);
    
    for (i = 0; i < (MAX_X * MAX_Y); i++) {
        LCD_WriteData(color);
    }
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : 
* Input          : - color: 
* Output         : None
* Return         : 
* Attention      : 
*******************************************************************************/
static uint16_t LCD_BGR2RGB(uint16_t color) {
    uint16_t  r, g, b, rgb;

    b = (color >> 0) & 0x1f;
    g = (color >> 5) & 0x3f;
    r = (color >> 11) & 0x1f;

    rgb = (b << 11) + (g << 5) + (r << 0);

    return rgb;
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : 
* Input          : - x0: Row Coordinate
*                  - y0: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention      : None
*******************************************************************************/
uint16_t LCD_GetPoint(uint16_t x0, uint16_t y0) {
    uint16_t dummy;

    LCD_SetCursor(x0, y0);
    LCD_WriteIndex(0x0022);

    switch (LCD_Code) {
        case ST7781:
        case LGDP4531:
        case LGDP4535:
        case SSD1289:
        case SSD1298:
            dummy = LCD_ReadData();         /* Empty read */
            dummy = LCD_ReadData();
            return dummy;
        case HX8347A:
        case HX8347D: {
            uint8_t red,green,blue;
            
            dummy = LCD_ReadData();         /* Empty read */

            red = LCD_ReadData() >> 3;
            green = LCD_ReadData() >> 2;
            blue = LCD_ReadData() >> 3;
            dummy = (uint16_t)((red << 11) | (green << 5) | blue);
            return dummy;
        }
        default:                            /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x9919 */
            dummy = LCD_ReadData();         /* Empty read */
            dummy = LCD_ReadData();
            return LCD_BGR2RGB(dummy);
    }
}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 
* Input          : - x0: Row Coordinate
*                  - y0: Line Coordinate 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_SetPoint(uint16_t x0, uint16_t y0, uint16_t point) {
    if ((x0 >= MAX_X) || (y0 >= MAX_Y)) {
        return;
    }
    
    LCD_SetCursor(x0, y0);
    LCD_WriteReg(0x0022, point);
}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: 
*                  - y1: 
*                  - x2: 
*                  - y2: 
*                  - color: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/	 
void LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    short dx,dy;
    short temp;

    dx = (x1 >= x0) ? (x1 - x0) : (x0 - x1);
    dy = (y1 >= y0) ? (y1 - y0) : (y0 - y1);

    if (dx == 0) {
        if (y0 > y1) {
            temp = y1;
            y1 = y0;
            y0 = temp;
        }
        do {
            LCD_SetPoint(x0, y0, color);
            y0++;
        } while (y1 >= y0);
        return;
    }
    if (dy == 0) {
        if (x0 > x1) {
            temp = x1;
            x1 = x0;
            x0 = temp;
        }
        do {
            LCD_SetPoint(x0, y0, color);
            x0++;
        } while (x1 >= x0);
        return;
    }

    if (dx > dy) {
        temp = 2 * dy - dx;
        while (x0 != x1) {
            LCD_SetPoint(x0, y0, color);
            (x1 > x0) ? x0++ : x0--;
            if (temp > 0) {
                (y1 > y0) ? y0++ : y0--;
                temp += 2 * dy - 2 * dx;
            } else {
                temp += 2 * dy;
            }
        }
        LCD_SetPoint(x0, y0, color);
    } else {
        temp = 2 * dx - dy;
        while (y0 != y1) {
            LCD_SetPoint(x0, y0, color);
            (y1 > y0) ? y0++ : y0--;
            if (temp > 0) {
                (x1 > x0) ? x0++ : x0--;
                temp += 2 * dy - 2 * dx;
            } else {
                temp += 2 * dy;
            }
        }
        LCD_SetPoint(x0, y0, color);
    }
}

/******************************************************************************
* Function Name  : LCD_DrawRectangle
* Description    : 
* Input          : - x0: 
*                  - y0: 
*                  - width: 
*                  - height: 
*                  - color: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/	 
void LCD_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color) {
    uint16_t i, j;
    
    for (i = 0; i < height; i++) {
        LCD_SetCursor(x0, y0 + i);
        LCD_WriteIndex(0x0022);
        
        for (j = 0; j < width; j++) {
            LCD_WriteData(color);
        }
    }
}

/******************************************************************************
* Function Name  : LCD_PutChar
* Description    : 
* Input          : - x0: 
*                  - y0: 
*                  - ASCI: 
*                  - charColor: 
*                  - bkColor: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_PutChar(uint16_t x0, uint16_t y0, uint8_t ASCI, uint16_t charColor, uint16_t bkColor) {
    uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    
    GetASCIICode(buffer, ASCI);
    
    for (i = 0; i < 16; i++) {
        LCD_SetCursor(x0, y0 + i);
        LCD_WriteIndex(0x0022);
        
        tmp_char = buffer[i];
        for (j = 0; j < 8; j++) {
            if (((tmp_char >> (7 - j)) & 0x01) == 0x01) {
                LCD_WriteData(charColor);
            } else {
                LCD_WriteData(bkColor);
            }
        }
    }
}

/******************************************************************************
* Function Name  : LCD_PutString
* Description    : 
* Input          : - x0: 
*                  - y0: 
*                  - str: 
*                  - charColor: 
*                  - bkColor: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_PutString(uint16_t x0, uint16_t y0, uint8_t *str,uint16_t color, uint16_t bkColor) {
    uint8_t TempChar;
    do {
        TempChar = *str++;
        LCD_PutChar(x0, y0, TempChar, color, bkColor);
        if (x0 < (MAX_X - 8)) {
            x0 += 8;
        } else if (y0 < (MAX_Y - 16)) {
            x0 = 0;
            y0 += 16;
        } else {
            x0 = 0;
            y0 = 0;
        }
    }
    while (*str != 0);
}
