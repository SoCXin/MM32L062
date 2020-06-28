////////////////////////////////////////////////////////////////////////////////
/// @file    uart_print_loop.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _UART_PRINT_LOOP_C_

// Files includes
#include "uart_print_loop.h"


////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM3 Pin Config
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    //UART1_TX   GPIOA.9
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //UART1_RX    GPIOA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port initialization configuration
/// @note    It must be careful of the Chip Version.
/// @param  bound: Baud rate
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_Loop_Init(u32 bound)
{
    UART_InitTypeDef UART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

    UART_StructInit(&UART_InitStructure);
    UART_InitStructure.BaudRate = bound;
    //The word length is in 8-bit data format.
    UART_InitStructure.WordLength = UART_WordLength_8b;
    UART_InitStructure.StopBits = UART_StopBits_1;
    //No even check bit.
    UART_InitStructure.Parity = UART_Parity_No;
    //No hardware data flow control.
    UART_InitStructure.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStructure.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART1, &UART_InitStructure);
    UART_Cmd(UART1, ENABLE);

    UART1_GPIO_Init();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Output received data.
/// @note   None.
/// @param  UARTx: UART1/UART2
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_RxTest(UART_TypeDef* uartx)
{
    u8 temp;
    temp = Receice_Byte(uartx);
    if(temp != 0) {
        UART1_Send_Byte(temp);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  received data.
/// @note   None.
/// @param  uartx: UART1/UART2
/// @retval temp :data.
////////////////////////////////////////////////////////////////////////////////
u8 Receice_Byte(UART_TypeDef* uartx)
{
    u8 temp;
    while(1) {
        if(UART_GetITStatus(uartx, UART_IT_RXIEN)) {
            UART_ClearITPendingBit(uartx, UART_IT_RXIEN);
            break;
        }
    }
    temp = (u8)UART_ReceiveData(uartx);
    if(temp == 0xd) {
        return 0;
    }
    return temp;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART send byte.
/// @note   None.
/// @param  dat(A byte data).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_Send_Byte(u8 dat)
{
    UART_SendData(UART1, dat);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART send byte.
/// @note   None.
/// @param  buf:buffer address.
/// @param  len:data length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_Send_Group(u8* buf, u16 len)
{
    while(len--)
        UART1_Send_Byte(*buf++);
}

