////////////////////////////////////////////////////////////////////////////////
/// @file     main.c
/// @author   AE TEAM
/// @brief    DMA mode send data.
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
#include "uart_print_dma.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_PRINT_DMA
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_PRINT_DMA_Exported_Functions
/// @{

u32 gText_Send[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 gSendBuff[(TEXT_LENTH + 2) * 100];
//u16 DMA1_MEM_LEN;

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u16 i;
    u8 t = 0;

    UART1_DMA_Init(115200);
    //DMA1 channel 2, peripheral is serial port 1, memory is SendBuff,
    //s32 (TEXT_LENTH+2)*10.
    DMA_Config(DMA1_Channel2, (u32)&UART1->TDR, (u32)gSendBuff, (TEXT_LENTH + 2) * 10);
    for(i = 0; i < (TEXT_LENTH + 2) * 100; i++) {
        //Add line breaks.
        if(t >= TEXT_LENTH) {
            gSendBuff[i++] = 0x0d;
            gSendBuff[i] = 0x0a;
            t = 0;
        }
        else gSendBuff[i] = gText_Send[t++];
    }
    //ENABLE uart1 DMA
    UART_DMACmd(UART1, UART_DMAReq_EN, ENABLE);
    DMA_Enable(DMA1_Channel2);

    while(1) {
        //Waiting for Channel 2 transmission to complete.
        if(DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET) {
            DMA_ClearFlag(DMA1_FLAG_TC2);
            break;
        }
    }
    while(1);

}

/// @}


/// @}

/// @}

