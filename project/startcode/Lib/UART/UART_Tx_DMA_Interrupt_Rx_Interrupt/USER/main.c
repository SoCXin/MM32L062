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


#include "main.h"
#include "delay.h"
#include "led.h"
#include "uart_txdma_rx_interrupt.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{
u8 gSendBuf[] = "0123456789";
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u8 len;
    u16 times = 0;

    DELAY_Init();
    LED_Init();
    UART1_NVIC_Init(115200);
    NVIC_Configure(DMA1_Channel2_3_IRQn, 2, 0);
    DMA_NVIC_Send_Config(DMA1_Channel2, (u32)&UART1->TDR, (u32)gSendBuf, 10);
    while(1) {
        if(gUartRxSta & 0x8000) {
            //receive data length
            len = gUartRxSta & 0x3fff;
            DMA_NVIC_Send_Config(DMA1_Channel2, (u32)&UART1->TDR, (u32)gUartRxBuf, len);
            gUartRxSta = 0;
        }
        else {
            times++;
            if(times % 5000 == 0) {
                UART1_SendString("\r\nMini Board UART test\r\n");
            }
            if(times % 200 == 0) UART1_SendString("Please input Data, End with Enter\r\n");
            if(times % 30 == 0)LED1_TOGGLE();
            DELAY_Ms(100);
        }
    }
}


/// @}

/// @}

/// @}
