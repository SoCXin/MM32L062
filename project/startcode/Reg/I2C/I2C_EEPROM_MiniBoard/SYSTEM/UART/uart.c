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

extern u32 SystemCoreClock;

#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE s32 __io_putchar(s32 ch)
#else
#define PUTCHAR_PROTOTYPE s32 fputc(s32 ch, FILE *f)

#endif

#ifdef USE_IAR
PUTCHAR_PROTOTYPE {
    while((UART1->CSR & UART_IT_TXIEN) == 0); //The loop is sent until it is finished
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

#else
void _sys_exit(s32 x)
{
    x = x;
}
//redefine fputcfunction
s32 fputc(s32 ch, FILE* f)
{
    while((UART1->CSR & UART_IT_TXIEN) == 0); //The loop is sent until it is finished
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

#endif


void CONSOLE_Init(u32 bound)
{

    u32 tempBaud;
    RCC->AHBENR |= RCC_AHBENR_GPIOA;
    //enable GPIOA clock(RCC_AHBENR_GPIOA = 1 << 17)
    RCC->APB2ENR |= RCC_APB2ENR_UART1;
    //enable uart clock(RCC_APB2ENR_UART1= 1 << 14)
    RCC->APB2RSTR |= RCC_APB2RSTR_UART1;
    //reset   uart 1(RCC_APB2RSTR_UART1 =1 << 14)
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_UART1);
    //stop reset
    GPIOA->AFRH &= (~0x0FF0);
    GPIOA->AFRH |= 0x0110;
    //Set PA9 as AF1 and PA10 as AF1


    // Determine the uart_baud
    tempBaud = (SystemCoreClock / bound) / 16;
    UART1->FRA = (SystemCoreClock / bound) % 16;
    // Write to UART BRR
    UART1->BRR = tempBaud;
    UART1->CCR |= 0X30;
    //set UART 1 as 8-N-1
    //
    //enable receive s32errupt
    UART1->GCR = 0X18;
    // receive and sent enable
    // receive  buffer area register  null s32errupt enable
    UART1->IER = 0X2;
    // receive s32errupt enable
    //
    //UART s32errupt enable device(UART_IER)
    //bit 1
    //RXIEN: receive  buffer s32errupt enablebit
    //1=s32errupt enable
    //0=s32errupt forbid
    //
    UART1->ICR = 0X2;
    //clear  receive s32errupt
    //
    //UART s32errupt clear  device(UART_ICR)
    //bit 1
    //RXICLR:  receive s32errupt clear bit
    //1=s32errupt clear
    //0=s32errupt not clear
    //
    UART1->GCR |= 0X01;
    //UART1 enable
    //
    //bit 0
    //UART1EN:UART1 enable
    //Soft set '1' or clear '0'
    //0:UART1 clock  close ;
    //1:UART1 clock start .
    //
    GPIOA->CRH &= 0XFFFFF00F; //IO status set
    GPIOA->CRH |= 0X000008B0;
    //Set PA9 as Alternate function output Push-pull
    //Set PA10 as Floating input
}

/// @}


/// @}

/// @}

