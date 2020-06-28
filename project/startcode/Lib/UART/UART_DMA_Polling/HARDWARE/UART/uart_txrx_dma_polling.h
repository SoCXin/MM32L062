////////////////////////////////////////////////////////////////////////////////
/// @file    uart_txrx_dma_polling.h
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FIRMWARE FUNCTIONS.
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
#ifndef __UART_TXRX_DMA_POLLING_H
#define __UART_TXRX_DMA_POLLING_H
// Files includes
#include "hal_conf.h"
#include  "stdio.h"


void UART1_GPIO_Init(void);
void UART1_DMA_Init(u32 bound);
void DMA_Polling_Send_Config(DMA_Channel_TypeDef* dam_chx, u32 cpar, u32 cmar, u16 cndtr);
void DMA_Polling_Recv_Config(DMA_Channel_TypeDef* dam_chx, u32 cpar, u32 cmar, u16 cndtr);
void NVIC_Configure(u8 ch, u8 pri, u8 sub);
void UART1_Send_Byte(u8 dat);
void UART1_Send_Group(u8* buf, u16 len);
/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
