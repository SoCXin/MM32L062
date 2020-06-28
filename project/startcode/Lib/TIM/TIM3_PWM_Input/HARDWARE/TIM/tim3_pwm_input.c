////////////////////////////////////////////////////////////////////////////////
/// @file     tim3_pwm_input.c
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
#define _TIM3_PWM_INPUT_C_

// Files includes
#include "tim3_pwm_input.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM3_PWM_INPUT
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM3_Exported_Functions
/// @{

u16 period = 0;
u16 duty = 0;
u8 CollectFlag = 0;
extern u32 SystemCoreClock;
////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM3 Pin Config
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM2_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM3 Pin Config
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM3_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  time initialization configuration
/// @note   It must be careful of the Chip Version.
/// @param  psc: Clock frequency division value.
/// @param  arr: Preloading value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM2_PWM_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    ///TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    //Select Timer Mode: TIM Pulse Width Modulation Mode 2
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    //Setting the Pulse Value of the Capture Comparison Register to be Loaded
    TIM_OCInitStructure.TIM_Pulse = 0;
    //Output polarity: TIM output is more polar
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_CtrlPWMOutputs(TIM2, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}
void TIM3_PWM_Input_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM3_ICInitStructure;

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    ///TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ICStructInit(&TIM3_ICInitStructure);
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM3_ICInitStructure.TIM_ICFilter = 0x0;
    //Configure PWM
    TIM_PWMIConfig(TIM3, &TIM3_ICInitStructure);
    //Select the valid input
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
    //Configuration in master-slave reset mode
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

    TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    TIM_Cmd(TIM3, ENABLE);
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
/// @brief  TIM3 Handler Funtion
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM3_IRQHandler(void)
{
    //Capture 1 Occurs Capture Event
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
        period = TIM_GetCapture1(TIM3);
        duty = TIM_GetCapture2(TIM3);
        CollectFlag = 1;
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Tim3 pwm Input test
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM3_PWM_Input_Test(void)
{
    TIM2_GPIO_Init();
    TIM3_GPIO_Init();
    //1KHZ Period
    TIM2_PWM_Init(1000 - 1, SystemCoreClock / 1000000 - 1);
    TIM_SetCompare1(TIM2, 600);
    NVIC_Configure(TIM3_IRQn, 1, 1);
    TIM3_PWM_Input_Init(0xffff, SystemCoreClock / 1000000 - 1);
}
/// @}


/// @}

/// @}



