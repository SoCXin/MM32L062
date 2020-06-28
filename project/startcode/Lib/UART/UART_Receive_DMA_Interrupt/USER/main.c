////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
/// @author  AE TEAM
/// @brief   DMA Interrupt receive mode.
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

#include "uart_receive_dma_interrupt.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_RECEIVE_DMA_INTERRUPT
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_RECEVICE_DMA_INTERRUPT_Exported_Functions
/// @{

u8 sRxBuffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u8 i;
    UART1_DMA_Init(115200);
    DMA_NVIC_Config(DMA1_Channel3, (u32)&UART1->RDR, (u32)sRxBuffer, 10);
    NVIC_Configure(DMA1_Channel2_3_IRQn, 0, 0);
    while(1) {
        if(gTestStatus == 1) {
            gTestStatus = 0;
            UART1_SendString("sRxBuffer:\r\n");
            for(i = 0; i < 10; i++) {
                UART1_Send_Byte(sRxBuffer[i]);
            }
            UART1_SendString("\r\n");
        }
    }
}
/// @}


/// @}

/// @}
