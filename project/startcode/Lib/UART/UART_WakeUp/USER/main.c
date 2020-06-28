////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _MAIN_C_

// Files includes


#include "led.h"
#include "delay.h"
#include "main.h"
#include "uart_wakeup_stop.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{

static u8 sStopFlag;
//u8 buff[10];
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u16 i, j;
    DELAY_Nop(10000);
    LED_Init();
    for(i = 0; i < 10; i++) {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        DELAY_Nop(1000);
    }
    sStopFlag = 1;
    while(1) {
        if(sStopFlag == 1) {
            UART1_NVIC_Init(115200);
            sStopFlag = 0;
            exEXTI_LineDisable(EXTI4_15_IRQn);
            UART1_Send_Byte(0x10);
        }
        LED3_TOGGLE();
        LED4_TOGGLE();
        LED2_TOGGLE();
        LED1_TOGGLE();
        DELAY_Nop(1000);
        if(gBuff[0] == 0x31) {
            memset(gBuff, 0, sizeof gBuff);
            UART_Cmd(UART1, DISABLE);
            for( j = 0; j < 5; j++) {
                LED3_TOGGLE();
                LED4_TOGGLE();
                LED2_TOGGLE();
                LED1_TOGGLE();
                DELAY_Nop(100);
            }
            DELAY_Nop(2000);
            STOP_Wakeup_Init();
            PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
            LED_Init();
            sStopFlag = 1;
        }

    }
}
/// @}

/// @}

/// @}
