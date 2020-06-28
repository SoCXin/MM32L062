////////////////////////////////////////////////////////////////////////////////
/// @file     dma.c
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _DMA_C_

// Files includes
#include "dma.h"
#include "delay.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DMA
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DMA_Exported_Constants
/// @{


u16 DMA1_MEM_LEN;
////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA  channel  configure
/// @param  DMA_CHx:DMA channel CHx
/// @param  cpar:external  address
/// @param  cmar:memory  device address
/// @param  cndtr: data transmission size
/// @retval None
///////////////////////////////////////////////////////////////////////////////
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;       //start DMA1clock
    DELAY_Ms(5);                //wait DMAclock  stabilization
    DMA_CHx->CPAR = cpar;       //DMA1 external  address
    DMA_CHx->CMAR = (u32)cmar;  //DMA1,memory  device address
    DMA1_MEM_LEN = cndtr;       //conserveDMAtransmission data  size
    DMA_CHx->CNDTR = cndtr;     //DMA1,transmission data  size
    DMA_CHx->CCR = 0X00000000;  //reset
    DMA_CHx->CCR &= ~DMA_CCR1_DIR;      //from memory device read
    DMA_CHx->CCR &= ~DMA_CCR1_CIRC;     //common  mode
    DMA_CHx->CCR &= ~DMA_CCR1_PINC;     //external  address register increment  mode
    DMA_CHx->CCR |= DMA_CCR1_MINC;      //memory  deviceincrement  mode
    DMA_CHx->CCR &= ~DMA_CCR1_PSIZE_0;      //external  data the width is 8 bits
    DMA_CHx->CCR &= ~DMA_CCR1_MSIZE_0;      //memory  device data the width is 8 bits
    DMA_CHx->CCR |= DMA_CCR1_PL_0;      //Medium priority
    DMA_CHx->CCR &= ~DMA_CCR1_MEM2MEM;      //register memory  device to memory  device mode
}

void MYDMA_Enable(DMA_Channel_TypeDef* DMA_CHx)
{
    DMA_CHx->CCR &= ~(DMA_CCR1_EN);     // close DMA transmission
    DMA_CHx->CNDTR = DMA1_MEM_LEN; //DMA1,transmission data  size
    DMA_CHx->CCR |= DMA_CCR1_EN;        //start DMA transmission
}

/// @}


/// @}

/// @}


























