/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           TouchPanel.c
** Descriptions:        The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:          AVRman
** Created date:        2010-11-7
** Version:             v1.0
** Descriptions:        The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "../GLCD/GLCD.h"
#include "../TP/TouchPanel.h"

/* Private variables ---------------------------------------------------------*/
Matrix matrix;
Coordinate display;

/* Display Sample */
Coordinate ScreenSample[3];
/* LCD Sample */
Coordinate DisplaySample[3] = {{ 45,  45},
                               { 45, 270},
                               {190, 190}};

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2

/*******************************************************************************
* Function Name  : LPC17xx_SPI_SetSpeed
* Description    : Set clock speed to desired value
* Input          : - speed: speed
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LPC17xx_SPI_SetSpeed(uint8_t speed) {
    speed &= 0xFE;
    if (speed < 2) {
        speed = 2;
    }
    LPC_SSP1->CPSR = speed;
}

/*******************************************************************************
* Function Name  : ADS7843_SPI_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void ADS7843_SPI_Init(void) {
    volatile uint32_t dummy;

    /* Initialize and enable the SSP1 Interface module. */
    LPC_SC->PCONP |= (1 << 10);                                                     /* Enable power to SSPI1 block */

    /* P0.7 SCK, P0.8 MISO, P0.9 MOSI are SSP pins. */
    LPC_PINCON->PINSEL0 &= ~((3UL<<14) | (3UL<<16) | (3UL<<18));                    /* P0.7,P0.8,P0.9 cleared */
    LPC_PINCON->PINSEL0 |= (2UL<<14) | (2UL<<16) | (2UL<<18);                       /* P0.7 SCK1,P0.8 MISO1,P0.9 MOSI1 */

    /* PCLK_SSP1=CCLK */
    LPC_SC->PCLKSEL0 &= ~(3<<20);                                                   /* PCLKSP0 = CCLK/4 (18MHz) */
    LPC_SC->PCLKSEL0 |=  (1<<20);                                                   /* PCLKSP0 = CCLK   (72MHz) */

    LPC_SSP1->CR0 = 0x0007;                                                         /* 8Bit, CPOL=0, CPHA=0  */
    LPC_SSP1->CR1 = 0x0002;                                                         /* SSP1 enable, master */

    LPC17xx_SPI_SetSpeed(SPI_SPEED_500kHz);

    /* wait for busy gone */
    while (LPC_SSP1->SR & (1 << SSPSR_BSY));

    /* drain SPI RX FIFO */
    while (LPC_SSP1->SR & (1 << SSPSR_RNE)) {
        dummy = LPC_SSP1->DR;
    }
}

/*******************************************************************************
* Function Name  : TP_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TP_Init(void) {
    LPC_GPIO0->FIODIR |= (1<<6);                                                    /* P0.6 CS is output */
    LPC_GPIO2->FIODIR |= (0<<13);                                                   /* P2.13 TP_INT is input */
    TP_CS(1);
    ADS7843_SPI_Init();
}

/*******************************************************************************
* Function Name  : DelayUS
* Description    : 
* Input          : - cnt: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void DelayUS(uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        uint8_t us = 12;
        while (us--);
    }
}

/*******************************************************************************
* Function Name  : WR_CMD
* Description    : 
* Input          : - cmd: 
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static uint8_t WR_CMD(uint8_t cmd) {
    uint8_t byte_r;

    while (LPC_SSP1->SR & (1 << SSPSR_BSY));                                        /* Wait for transfer to finish */
    LPC_SSP1->DR = cmd;
    while (LPC_SSP1->SR & (1 << SSPSR_BSY));                                        /* Wait for transfer to finish */
    while (!(LPC_SSP1->SR & (1 << SSPSR_RNE)));                                     /* Wait until Rx FIFO not empty */
    byte_r = LPC_SSP1->DR;

    return byte_r;                                                                  /* Return received value */
}

/*******************************************************************************
* Function Name  : RD_AD
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
static int RD_AD(void) {
    unsigned short buf, temp;

    temp = WR_CMD(0x00);
    buf = temp << 8;
    DelayUS(1);
    temp = WR_CMD(0x00);
    buf |= temp;
    buf >>= 4;
    buf &= 0xfff;
    return buf;
}

/*******************************************************************************
* Function Name  : Read_X
* Description    : Read X+ Position Input (ADC input Channel 1) in ADS7843
* Input          : None
* Output         : None
* Return         : X+ Position Input (ADC input Channel 1) in ADS7843
* Attention      : None
*******************************************************************************/
int Read_X(void) {
    int i;
    TP_CS(0);
    DelayUS(1);
    WR_CMD(CHX);
    DelayUS(1);
    i = RD_AD();
    TP_CS(1);
    return i;
}

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Read Y+ Position Input (ADC input Channel 2) in ADS7843
* Input          : None
* Output         : None
* Return         : Y+ Position Input (ADC input Channel 2) in ADS7843
* Attention      : None
*******************************************************************************/
int Read_Y(void) {
    int i;
    TP_CS(0);
    DelayUS(1);
    WR_CMD(CHY);
    DelayUS(1);
    i = RD_AD();
    TP_CS(1);
    return i;
}

/*******************************************************************************
* Function Name  : GetAdXY
* Description    : ��ȡADS7843 ͨ��X+ ͨ��Y+��ADCֵ
* Input          : None
* Output         : None
* Return         : ADS7843���� ͨ��X+ ͨ��Y+��ADCֵ
* Attention      : None
*******************************************************************************/
void GetAdXY(int *x, int *y) {
    int adx,ady;
    adx = Read_X();
    DelayUS(1);
    ady = Read_Y();
    *x = adx;
    *y = ady;
}

/*******************************************************************************
* Function Name  : TP_ReadAds7846
* Description    : �õ��˲�֮���X Y
* Input          : None
* Output         : None
* Return         : Coordinate�ṹ���ַ
* Attention      : None
*******************************************************************************/
Coordinate *TP_ReadAds7846(void) {
    static Coordinate screen;
    int m0, m1, m2, TP_X[1], TP_Y[1], temp[3];
    uint8_t count = 0;
    int buffer[2][9] = {{0}, {0}};

    do {
        GetAdXY(TP_X, TP_Y);
        buffer[0][count] = TP_X[0];
        buffer[1][count] = TP_Y[0];
        count++;
    } while (!TP_INT_IN && (count < 9));

    if (count == 9) {
        temp[0] = (buffer[0][0] + buffer[0][1] + buffer[0][2]) / 3;
        temp[1] = (buffer[0][3] + buffer[0][4] + buffer[0][5]) / 3;
        temp[2] = (buffer[0][6] + buffer[0][7] + buffer[0][8]) / 3;
        m0 = temp[0] - temp[1];
        m1 = temp[1] - temp[2];
        m2 = temp[2] - temp[0];
        m0 = (m0 > 0) ? m0 : (-m0);
        m1 = (m1 > 0) ? m1 : (-m1);
        m2 = (m2 > 0) ? m2 : (-m2);
        if ((m0 > THRESHOLD) && (m1 > THRESHOLD) && (m2 > THRESHOLD)) {
            return 0;
        }
        if (m0 < m1) {
            if (m2 < m0) {
                screen.x = (temp[0] + temp[2]) / 2;
            } else {
                screen.x = (temp[0] + temp[1]) / 2;
            }
        } else if (m2 < m1) {
            screen.x = (temp[0] + temp[2]) / 2;
        } else {
            screen.x = (temp[1] + temp[2]) / 2;
        }

        temp[0] = (buffer[1][0] + buffer[1][1] + buffer[1][2]) / 3;
        temp[1] = (buffer[1][3] + buffer[1][4] + buffer[1][5]) / 3;
        temp[2] = (buffer[1][6] + buffer[1][7] + buffer[1][8]) / 3;

        m0 = temp[0] - temp[1];
        m1 = temp[1] - temp[2];
        m2 = temp[2] - temp[0];
        m0 = (m0 > 0) ? m0 : (-m0);
        m1 = (m1 > 0) ? m1 : (-m1);
        m2 = (m2 > 0) ? m2 : (-m2);
        if((m0 > THRESHOLD) && (m1 > THRESHOLD) && (m2 > THRESHOLD)) {
            return 0;
        }

        if (m0 < m1) {
            if (m2 < m0) {
                screen.y = (temp[0] + temp[2]) / 2;
            } else {
                screen.y = (temp[0] + temp[1]) / 2;
            }
        } else if (m2 < m1) {
            screen.y = (temp[0] + temp[2]) / 2;
        } else {
            screen.y = (temp[1] + temp[2]) / 2;
        }

        return &screen;
    }
    return 0;
}

/*******************************************************************************
* Function Name  : DrawCross
* Description    : ��ָ�����껭ʮ��׼��
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DrawCross(uint16_t Xpos, uint16_t Ypos) {
    LCD_DrawLine(Xpos - 15, Ypos, Xpos - 2, Ypos, 0xffff);
    LCD_DrawLine(Xpos + 2, Ypos, Xpos + 15, Ypos, 0xffff);
    LCD_DrawLine(Xpos, Ypos - 15, Xpos, Ypos - 2, 0xffff);
    LCD_DrawLine(Xpos, Ypos + 2, Xpos, Ypos + 15, 0xffff);

    LCD_DrawLine(Xpos - 15, Ypos + 15, Xpos - 7, Ypos + 15, RGB565CONVERT(184, 158, 131));
    LCD_DrawLine(Xpos - 15, Ypos + 7, Xpos - 15, Ypos + 15, RGB565CONVERT(184, 158, 131));

    LCD_DrawLine(Xpos - 15, Ypos-15, Xpos - 7, Ypos - 15, RGB565CONVERT(184, 158, 131));
    LCD_DrawLine(Xpos - 15, Ypos-7, Xpos - 15, Ypos - 15, RGB565CONVERT(184, 158, 131));

    LCD_DrawLine(Xpos + 7, Ypos + 15, Xpos + 15, Ypos + 15, RGB565CONVERT(184, 158, 131));
    LCD_DrawLine(Xpos + 15, Ypos + 7, Xpos + 15, Ypos + 15, RGB565CONVERT(184, 158, 131));

    LCD_DrawLine(Xpos + 7, Ypos - 15, Xpos + 15, Ypos - 15, RGB565CONVERT(184, 158, 131));
    LCD_DrawLine(Xpos + 15, Ypos - 15, Xpos + 15, Ypos - 7, RGB565CONVERT(184, 158, 131));
}

/*******************************************************************************
* Function Name  : DeleteCross
* Description    : ��ָ�����껭ʮ��׼��
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DeleteCross(uint16_t Xpos, uint16_t Ypos) {
    LCD_DrawLine(Xpos - 15, Ypos, Xpos - 2, Ypos, 0x0000);
    LCD_DrawLine(Xpos + 2, Ypos, Xpos + 15, Ypos, 0x0000);
    LCD_DrawLine(Xpos, Ypos - 15, Xpos, Ypos - 2, 0x0000);
    LCD_DrawLine(Xpos, Ypos + 2, Xpos, Ypos + 15, 0x0000);

    LCD_DrawLine(Xpos - 15, Ypos + 15, Xpos - 7, Ypos + 15, RGB565CONVERT(0, 0, 0));
    LCD_DrawLine(Xpos - 15, Ypos + 7, Xpos - 15, Ypos + 15, RGB565CONVERT(0, 0, 0));

    LCD_DrawLine(Xpos - 15, Ypos-15, Xpos - 7, Ypos - 15, RGB565CONVERT(0, 0, 0));
    LCD_DrawLine(Xpos - 15, Ypos-7, Xpos - 15, Ypos - 15, RGB565CONVERT(0, 0, 0));

    LCD_DrawLine(Xpos + 7, Ypos + 15, Xpos + 15, Ypos + 15, RGB565CONVERT(0, 0, 0));
    LCD_DrawLine(Xpos + 15, Ypos + 7, Xpos + 15, Ypos + 15, RGB565CONVERT(0, 0, 0));

    LCD_DrawLine(Xpos + 7, Ypos - 15, Xpos + 15, Ypos - 15, RGB565CONVERT(0, 0, 0));
    LCD_DrawLine(Xpos + 15, Ypos - 15, Xpos + 15, Ypos - 7, RGB565CONVERT(0, 0, 0));
}

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : computation of K A B C D E F
* Input          : None
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
uint8_t setCalibrationMatrix(Coordinate *displayPtr, Coordinate *screenPtr, Matrix *matrixPtr) {
    uint8_t retTHRESHOLD = 0;

    matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                         ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));
    if (matrixPtr->Divider == 0) {
        retTHRESHOLD = 1;
    } else {
        matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));
        matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
                        ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x));
        matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y;
        matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y));
        matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
                        ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x));
        matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y;
    }
    return retTHRESHOLD;
}

/*******************************************************************************
* Function Name  : TP_Calibrate
* Description    : У׼������
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TP_Calibrate(void) {
    uint8_t i;
    Coordinate *Ptr;

    LCD_Clear(Black);
    LCD_PutString(10, 10, (uint8_t *)("Touch crosshair to calibrate"), 0xffff, Black);

    for (i = 0; i < 3; i++) {
        //DelayUS(1000 * 500);
        DrawCross(DisplaySample[i].x, DisplaySample[i].y);
        do {
            Ptr = TP_ReadAds7846();
        } while (Ptr == (void*)0);
        ScreenSample[i].x = Ptr->x;
        ScreenSample[i].y = Ptr->y;
        DeleteCross(DisplaySample[i].x, DisplaySample[i].y);
    }
    setCalibrationMatrix(&DisplaySample[0], &ScreenSample[0], &matrix);
}

/*******************************************************************************
* Function Name  : TP_GetDisplayPoint
* Description    : using K A B C D E F
* Input          : None
* Output         : None
* Return         : ����1��ʾ�ɹ� 0ʧ��
* Attention      : None
*******************************************************************************/
uint8_t TP_GetDisplayPoint(Coordinate *displayPtr, Coordinate *screenPtr, Matrix *matrixPtr) {
    uint8_t retTHRESHOLD = 0;

    if (matrixPtr->Divider != 0) {
        /* XD = AX+BY+C */
        displayPtr->x = ((matrixPtr->An * screenPtr->x) +
                         (matrixPtr->Bn * screenPtr->y) +
                         matrixPtr->Cn
                        ) / matrixPtr->Divider;
        /* YD = DX+EY+F */
        displayPtr->y = ((matrixPtr->Dn * screenPtr->x) +
                         (matrixPtr->En * screenPtr->y) +
                         matrixPtr->Fn
                        ) / matrixPtr->Divider;
    } else {
        retTHRESHOLD = 1;
    }
    return retTHRESHOLD;
}
