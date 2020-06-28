////////////////////////////////////////////////////////////////////////////////
/// @file    uart.c
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
#define _UART_C_

// Files includes
#include <stdio.h>
#include "uart.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Functions
/// @{

#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE s32 __io_putchar(s32 ch)
#else
#define PUTCHAR_PROTOTYPE s32 fputc(s32 ch, FILE *f)

#endif

#ifdef USE_IAR
PUTCHAR_PROTOTYPE {
    while((UART1->CSR & UART_IT_TXIEN) == 0); //Send circularly until the transmission is complete
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

#else
#pragma import(__use_no_semihosting)
//Support functions required by the standard library
struct __FILE {
    s32 handle;

};

FILE __stdout;
//Define _sys_exit () to avoid using semi-hosted mode
_sys_exit(s32 x)
{
    x = x;
}
//Redefine fputc function
s32 fputc(s32 ch, FILE* f)
{
    while((UART1->CSR & UART_IT_TXIEN) == 0); //Send circularly until the transmission is complete
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

#endif


void uart_initwBaudRate(u32 bound)
{
    //GPIO Port settings
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    UART_StructInit(&UART_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);   //Enable UART1,GPIOA clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //open GPIOA,GPIOB clock
    //UART initialization config
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;//Serial baud rate
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;//Word length is 8-bit data format
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//A stop bit
    UART_InitStructure.UART_Parity = UART_Parity_No;//No parity
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//No hardware data flow control
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; //Send and receive mode

    UART_Init(UART1, &UART_InitStructure); //initialization UART1
    UART_Cmd(UART1, ENABLE);                    //Enable  UART1

    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //Multiplexed push-pull output
    GPIO_Init(GPIOA, &GPIO_InitStructure);//initializationGPIOA.9

    //UART1_RX    GPIOA.10initialization
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//Floating input
    GPIO_Init(GPIOA, &GPIO_InitStructure);//initializationGPIOA.10

}
/// @}

/// @}

/// @}

