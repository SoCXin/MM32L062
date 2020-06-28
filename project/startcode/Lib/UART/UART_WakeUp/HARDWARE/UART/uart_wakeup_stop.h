////////////////////////////////////////////////////////////////////////////////
/// @file    uart_wakeup_stop.h
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
#ifndef __UART_WAKEUP_STOP_H
#define __UART_WAKEUP_STOP_H
// Files includes
#include "hal_conf.h"
#include  "stdio.h"

extern u8 gTestStatus;
extern u8 gBuff[10];
void UART1_GPIO_Init(void);
void UART1_NVIC_Init(u32 bound);
void STOP_Wakeup_Init(void);
void UART1_Send_Byte(u8 dat);
void UART1_Send_Group(u8* buf, u16 len);
void DELAY_Nop(u32 time);
/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
