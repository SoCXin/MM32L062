////////////////////////////////////////////////////////////////////////////////
/// @file     tim1_interrupt.c
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
#define _TIM1_INTERRUPT_C_
// Files includes
#include "tim1_interrupt.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM1_INTERRUPT
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM1_Exported_Functions
/// @{

u8 timeflag;
extern u32 SystemCoreClock;
////////////////////////////////////////////////////////////////////////////////
/// @brief  Tim3 Base initialization configuration
/// @note
/// @param  psc: Clock frequency division value.
/// @param  arr: Preloading value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM1_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    //TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC Configure, Applicable to Cortex M0 or M3 processors.
/// @param  NVIC Channel, Priority or SubPriority.
/// @arg    ch: IRQChannel
/// @arg    pri: Priority, Use only Cortex-M3
/// @arg    sub: SubPriority
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void NVIC_Configure(u8 ch, u8 pri, u8 sub)
{

    exNVIC_Init_TypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = ch;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = pri;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = sub;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    exNVIC_Init(&NVIC_InitStruct);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  IRQ Handler
/// @note    If the program fails, see here.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        timeflag = 1;
    }
}
/// @}


/// @}

/// @}



