////////////////////////////////////////////////////////////////////////////////
/// @file     uart_loop.c
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
#define _UART_LOOP_C_

// Files includes
#include "sys.h"
#include "uart_loop.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_LOOP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_LOOP_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  uart initialization
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void UartInit_Loop(void)
{

    //GPIO port set
    RCC->AHBENR |= 1 << 17; //enable GPIOA clock
    RCC->APB2ENR |= 1 << 14; //enable uart clock


    GPIOA->AFRH &= (~0x0FF0);
    GPIOA->AFRH |= 0x110;


    //UART initialset
    UART1->BRR = (SystemCoreClock / 115200) / 16;
    UART1->FRA = (SystemCoreClock / 115200) % 16;
    UART1->CCR |= 0x30; //110000
    UART1->GCR |= 0x19; //11001;
    UART1->GCR |= 0x0001;                    //enable uart 1

    GPIOA->CRH &= 0XFFFFF00F; //IO status set
    GPIOA->CRH |= 0X000008B0; //IO status set


}


////////////////////////////////////////////////////////////////////////////////
/// @brief  uart sent byte
/// @param  UARTx : x can be 1 or 2 or 3
/// @param  c :uart sent byte
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void uart_send(s8 c)
{
    UART1->TDR = c;
    while(1) {
        if(UART1->ISR & 0x1) {
            UART1->ICR = 0x1;
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  uart receive function
/// @param  UARTx:x can be 1 or 2 or 3
/// @retval None
////////////////////////////////////////////////////////////////////////////////
u8 inbyte(UART_TypeDef* UARTx)
{
    u8 temp;

    while(1) {
        if(UART1->ISR & 0x2) {
            UART1->ICR = 0x2;
            break;
        }
    }
    temp = (u8)UART1->RDR;
    if(temp == 0xd) { //clear  error receive  data
        return 0;
    }
    return temp;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  uart receive function  test
/// @param  UARTx , x can be 1 or 2 or 3
/// @retval None
////////////////////////////////////////////////////////////////////////////////

void Uart1RxTest(UART_TypeDef* UARTx)
{
    u8 temp;
    temp = inbyte(UARTx);
    if(temp != 0) {

    }
}
/// @}


/// @}

/// @}





