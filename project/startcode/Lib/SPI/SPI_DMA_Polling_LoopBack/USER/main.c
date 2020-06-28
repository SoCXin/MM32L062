////////////////////////////////////////////////////////////////////////////////
/// @file    SPI_DMApoll_LoopBack.c
/// @author  AE TEAM
/// @brief   Use DMA to send and receive data, compare data,
///          MISO & MOSI need to be s16-connected.
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
#define _SPI_DMAPOLL_LOOPBACK_C_

// Files includes
#include "SPI.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_DMAPOOL_LOOPBACK
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{



////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    CONSOLE_Init(115200);
    SPI_DMA_LoopInit();
    SPI_DMA_LoopTest();
    while(1) {


    }
}



/// @}


/// @}

/// @}

