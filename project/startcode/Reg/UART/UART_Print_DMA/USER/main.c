////////////////////////////////////////////////////////////////////////////////
/// @file     main.c
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
#define _MAIN_C_

// Files includes
#include "delay.h"
#include "sys.h"
#include "dma.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Functions
/// @{
u32 TEXT_TO_SEND[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH + 2) * 100];

extern u32 SystemCoreClock;


void UartInitwBaudRate(u32 bound)
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
    //enable receive interrupt
    UART1->GCR = 0X18;
    // receive and sent enable
    // receive  buffer area register  null interrupt enable
    UART1->IER = 0X2;
    // receive interrupt enable
    //
    //UART interrupt enable device(UART_IER)
    //bit 1
    //RXIEN: receive  buffer interrupt enablebit
    //1=interrupt enable
    //0=interrupt forbid
    //
    UART1->ICR = 0X2;
    //clear  receive interrupt
    //
    //UART interrupt clear  device(UART_ICR)
    //bit 1
    //RXICLR:  receive interrupt clear bit
    //1=interrupt clear
    //0=interrupt not clear
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
////////////////////////////////////////////////////////////////////////////////
/// @brief  the main entrance
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u16 i;
    u8 t = 0;

    DELAY_Init();
    UartInitwBaudRate(115200);  // uart initial115200
    MYDMA_Config(DMA1_Channel2, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 10); //DMA1 channel 2,external  uart 1,memory  deviceSendBuff,(TEXT_LENTH+2)*100.

    for(i = 0; i < (TEXT_LENTH + 2) * 100; i++) { //Fill ASCII character set data
        if(t >= TEXT_LENTH) { //Add a newline character
            SendBuff[i++] = 0x0d;
            SendBuff[i] = 0x0a;
            t = 0;
        }
        else SendBuff[i] = TEXT_TO_SEND[t++]; //copy TEXT_TO_SEND statement
    }

    UART1->GCR |= 0x0002;
    DMA1_Channel2->CCR |= 0x00000001;
    //wait DMA transmission complete,Now let's do something else, light a lamp

    while(1) {
        if(!(DMA1->ISR & 0x00000020)) { //wait  channel 2 transmission complete
            DMA1->IFCR = 0x00000020;
            break;
        }
    }
    while(1);

}
/// @}


/// @}

/// @}



