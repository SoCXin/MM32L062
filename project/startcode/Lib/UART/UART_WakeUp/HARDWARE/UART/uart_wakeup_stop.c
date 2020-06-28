////////////////////////////////////////////////////////////////////////////////
/// @file     uart_wakeup_stop.c
/// @author   AE TEAM
/// @brief    PWM output.
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
#define _UART_WAKEUP_STOP_C_

// Files includes
#include "uart_wakeup_stop.h"

u16 DMA1_MEM_LEN;
u8 gTestStatus = 0;
u8 gBuff[10];
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
void UART1_NVIC_Init(u32 bound)
{
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

    //UART1 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //Baud rate
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
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART1, ENABLE);

    UART1_GPIO_Init();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  External interrupt wake-up standby initialization
/// @note   External Wake-up Pin Selection.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void STOP_Wakeup_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_StructInit(&EXTI_InitStructure);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,  EXTI_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Interrupt service function
/// @note   The received data is terminated by a carriage return sign.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_IRQHandler(void)
{
    // u8 Res;
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET) {
        //Receiving interrupts (data received must end at 0x0D 0x0a)
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
        //read receive data.
        gBuff[0] = UART_ReceiveData(UART1);


    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  External interrupt 0, clears interrupt flag after entering.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI4_15_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line10);
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
////////////////////////////////////////////////////////////////////////////////
/// @brief  delay nTime ms
/// @note   get x times.
/// @param  nTime  nTime ms.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DELAY_Nop(u32 time)
{
    u32 i, j;
    for(i = 0; i < time; i++) {
        for(j = 0; j < 100; j++) {
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
        }
    }
}

/// @}


/// @}

/// @}

