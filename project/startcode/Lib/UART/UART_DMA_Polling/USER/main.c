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
#include "uart_txrx_dma_polling.h"

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
u8 gRecvBuf[100];
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    DELAY_Init();
    UART1_DMA_Init(115200);
    DMA_Polling_Send_Config(DMA1_Channel2, (u32)&UART1->TDR, (u32)gSendBuf, 10);
    while(!DMA_GetFlagStatus(DMA_ISR_TCIF2));
    DMA_ClearFlag(DMA_IFCR_CTCIF2);
    DMA_Cmd(DMA1_Channel3, DISABLE);
    while(1) {
        DMA_Polling_Recv_Config(DMA1_Channel3, (u32)&UART1->RDR, (u32)gRecvBuf, 10);
        while(!DMA_GetFlagStatus(DMA_ISR_TCIF3));
        DMA_ClearFlag(DMA_ISR_TCIF3);
        DMA_Cmd(DMA1_Channel2, DISABLE);

        DMA_Polling_Send_Config(DMA1_Channel2, (u32)&UART1->TDR, (u32)gRecvBuf, 10);
        while(!DMA_GetFlagStatus(DMA_ISR_TCIF2));
        DMA_ClearFlag(DMA_IFCR_CTCIF2);
        DMA_Cmd(DMA1_Channel3, DISABLE);
    }
}



/// @}

/// @}

/// @}
