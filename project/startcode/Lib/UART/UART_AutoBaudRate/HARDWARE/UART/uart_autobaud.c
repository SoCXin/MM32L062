////////////////////////////////////////////////////////////////////////////////
/// @file    uart_autobaud.c
/// @author  AE TEAM
/// @brief   Output received data.
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
#define _UART_AUTOBAUD_C_

// Files includes



#include "hal_conf.h"
#include "delay.h"
#include "uart_autobaud.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{
u8 gStartTrim = 0;
u32 gPeriod = 0;
float gTrim = 0.99;
s32 gUartBaudRate = 9600;
s32 gNewBaudRate = 0;
//Input capture value frequency
u16 TIM3CH1_CAPTURE_Period = 0;
//Input capture status
u8  CAPTURE_ATART = 0;
u8  CAPTURE_END = 0;
extern u32 SystemCoreClock;

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
void UART_AutoBaudRateTest(void)
{

    //SetSystemClock(emSYSTICK_On, NULL);
    uart_nvic_init(9600);

    UART_SendData(UART1, 0x55);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
    UART_SendData(UART1, 0x55);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
    UART_SendData(UART1, 0x55);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
    //Input capture
    TIM3_capture_INIT(0xffffffff, 0);

    while(1) {
        //End of TIM3 capture
        if(CAPTURE_ATART && (CAPTURE_END == 0)) {
            CAPTURE_ATART = 0;
            while (CAPTURE_ATART == 0);
            gNewBaudRate = SystemCoreClock / TIM3CH1_CAPTURE_Period;//48000000
            if (gNewBaudRate > gUartBaudRate * 1.03 || gNewBaudRate < gUartBaudRate * 0.97) {
                gUartBaudRate = (s32)(gNewBaudRate * gTrim);
                ChangeBaudRate(gNewBaudRate);
                UART_SendData(UART1, 0x55);
                while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
            }
            CAPTURE_END = 1;
        }

    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Change baud_rate
/// @note   None.
/// @param  baud_rate: baud_rate
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ChangeBaudRate(u32 baud_rate)
{
    u32 apbclock = 0x00;
    RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus);

    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
    UART1->BRR = (apbclock / baud_rate) / 16;
    UART1->FRA = (apbclock / baud_rate) % 16;

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  TIME initialization configuration
/// @note   It must be careful of the Chip Version.
/// @param  bound: Baud rate
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM3_capture_INIT(u32 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM3_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //AF1 with PA6 multiplexing function
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = (TIMCKD_TypeDef) 0;
    //TIM up counting mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ICStructInit(&TIM3_ICInitStructure);
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM3_ICInitStructure.TIM_ICFilter = 0x0;
    //PWM input configuration
    TIM_PWMIConfig(TIM3, &TIM3_ICInitStructure);
    //Select valid input
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

    TIM_ITConfig(TIM3, (TIMIT_TypeDef) (TIM_IT_CC1 | TIM_IT_Update), ENABLE);
    TIM_ClearITPendingBit(TIM3, (TIMIT_TypeDef) (TIM_IT_CC1 | TIM_IT_Update));
    TIM_Cmd(TIM3, ENABLE);


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


}
////////////////////////////////////////////////////////////////////////////////
/// @brief  External interrupt 0, clears interrupt flag after entering.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI4_15_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line6);
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == RESET) {
        if(CAPTURE_END == 1) {
            CAPTURE_END = 0;
            CAPTURE_ATART = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM3 IRQ, clears interrupt flag after entering.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM3_IRQHandler(void)
{
    if(CAPTURE_ATART == 0) {
        if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
            CAPTURE_ATART = 1;
            //capture value
            TIM3CH1_CAPTURE_Period = TIM_GetCapture2(TIM3);
        }
    }

    TIM_ClearITPendingBit(TIM3, (TIMIT_TypeDef) (TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update)); //Clear the interrupt flag
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Interrupt service function
/// @note   The received data is terminated by a carriage return sign.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 buff;
void UART1_IRQHandler(void)
{
    // u8 Res;
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET) {
        //Receiving interrupts (data received must end at 0x0D 0x0a)
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
        //read receive data.
        buff = UART_ReceiveData(UART1);

    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port initialization configuration
/// @note    It must be careful of the Chip Version.
/// @param  bound: Baud rate
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void uart_nvic_init(u32 bound)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    //UART1 NVIC

    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

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
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Clock_Set(GPIO_TypeDef* gpio, FunctionalState state)
{

    if(gpio == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, state);
    }
    if(gpio == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, state);
    }
    if(gpio == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, state);
    }
    if(gpio == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, state);
    }
}





/// @}


/// @}

/// @}


