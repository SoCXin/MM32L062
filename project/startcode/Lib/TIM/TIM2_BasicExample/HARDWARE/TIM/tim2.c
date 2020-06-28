////////////////////////////////////////////////////////////////////////////////
/// @file     tim2.c
/// @author   AE TEAM
/// @brief    Using timer to generate delays.
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
#define _TIM2_C_

// Files includes
#include "tim2.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief  Waiting for the timer to overflow and interrupt
/// @note   Pay attention to clearing interruption marks.
/// @param  TxMessage:Sent message.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    ucTim2Flag++;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  time initialization configuration
/// @note    It must be careful of the Chip Version.
/// @param  Prescaler: Clock frequency division value.
/// @param  Period: Preloading value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM2_UPCount_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_StructInit;

    //Enable TIM1 clock, default clock source is PCLK2,Alternative Clock Sources
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = psc;
    TIM_TimeBaseStructure.TIM_Prescaler = arr;
    //Sampling frequency of digital filter does not affect timer clock
    //Sampling Frequency Dividing Value
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // interrupt priority configuration

    NVIC_StructInit.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_StructInit.NVIC_IRQChannelPriority = 1;
    NVIC_StructInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_StructInit);


    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Tim2_UPStatusOVCheck_test1(void)
/// @note
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM2_UPStatusOVCheck(void)
{
    //Waiting for Event Update Flag Bit
    while(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET);
    //Clear the marker
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}
/// @}

/// @}

/// @}
