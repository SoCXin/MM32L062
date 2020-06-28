////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
/// @author  AE TEAM
/// @brief   DMA receive.
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

#include "uart_receive_dma_polling.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_RECEIVE_DMA
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_RECEIVE_DMA_Exported_Functions
/// @{

//receive buff
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
    DMA_Polling_Config(DMA1_Channel3, (u32)&UART1->RDR, (u32)sRxBuffer, 10);
    while(1) {
        //Accept array zeroing
        for(i = 0; i < 10; i++) {
            sRxBuffer[i] = 0;
        }
        //Waiting for the completion of DMA transmission
        while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));
        DMA_ClearFlag(DMA1_FLAG_TC3);
        UART1_SendString("DST_Buffer:\r\n");
        for(i = 0; i < 10; i++) {
            UART1_Send_Byte(sRxBuffer[i]);
        }
        UART1_SendString("\r\n");
    }
}

/// @}


/// @}

/// @}
