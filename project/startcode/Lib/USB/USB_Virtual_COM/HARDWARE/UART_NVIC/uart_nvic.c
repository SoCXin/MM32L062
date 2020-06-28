////////////////////////////////////////////////////////////////////////////////
/// @file    uart_nvic.c
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
#define _UART_NVIC_C_

// Files includes

#include "uart_nvic.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_NVIC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_NVIC_Exported_Functions
/// @{


//Serial port 1 interrupt service routine
u8 UART_RX_BUF[UART_REC_LEN];     //Receive buffer, maximum UART_REC_LEN bytes.
//Receiving status
//bit15,   Reception completion flag
//bit14,   Received 0x0d
//bit13~0, Number of valid bytes received
u16 UART_RX_STA = 0;     //Receive status flag

void uart_nvic_init(u32 bound)
{
    //GPIO Port settings
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);   //Enable UART1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //open GPIOA clock

    //UART1 NVIC  config

    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;     //Sub-priority is 3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ Channel Enable
    NVIC_Init(&NVIC_InitStructure); //Initialize the VIC register according to the specified parameters

    //UART initialization config
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    UART_StructInit(&UART_InitStructure);
    UART_InitStructure.UART_BaudRate = bound;//Serial baud rate
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;//Word length is 8-bit data format
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//A stop bit
    UART_InitStructure.UART_Parity = UART_Parity_No;//No parity
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//No hardware data flow control
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; //Send and receive mode

    UART_Init(UART1, &UART_InitStructure); //initialization UART1
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);//open Serial port accepts interrupt
    UART_Cmd(UART1, ENABLE);                    //Enable  UART1

    //UART1_TX   GPIOA.9
    GPIO_StructInit(&GPIO_InitStructure);
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

