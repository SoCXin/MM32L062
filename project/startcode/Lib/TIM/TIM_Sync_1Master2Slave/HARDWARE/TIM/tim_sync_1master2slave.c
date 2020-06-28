////////////////////////////////////////////////////////////////////////////////
/// @file     tim_sync_1master2slave.c
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
#define _TIM_SYNC_1MASTER2SLAVE_C_

// Files includes
#include "tim_sync_1master2slave.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM_SYNC_1MASTER2SLAVE
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM_Exported_Functions
/// @{

extern u32 SystemCoreClock;
////////////////////////////////////////////////////////////////////////////////
/// @brief  Timer Pin Config
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM1_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void TIM2_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_2);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void TIM3_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Time Base initialization configuration
/// @note
/// @param  psc: Clock frequency division value.
/// @param  arr: Preloading value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM1_Master_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    ///TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    //Select Timer Mode: TIM Pulse Width Modulation Mode 2
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    //Setting the Pulse Value of the Capture Comparison Register to be Loaded
    TIM_OCInitStructure.TIM_Pulse = 499;
    //Output polarity: TIM output is more polar
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    //Enable TIMx preload register on ARR
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    //Tim1 enable output as a trigger signal
    TIM_SelectOutputTrigger(TIM1, TIM_TRIGSource_Enable);
    //Master-slave mode MSM
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);

    TIM_SetCounter(TIM1, 0);
    // Enable TIM1 PWM Complementary Deadband Output
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Time Base initialization configuration
/// @note
/// @param  psc: Clock frequency division value.
/// @param  arr: Preloading value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM2_Slave_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

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
    TIM_OCInitStructure.TIM_Pulse = 499;
    //Output polarity: TIM output is more polar
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    //Enable TIMx preload register on ARR
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    //Trigger selection, ITR0 is TIM1
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);
    //Tim2 enable output as a trigger signal
    TIM_SelectOutputTrigger(TIM2, TIM_TRIGSource_Enable);
    //Trigger mode only starts; door mode starts and stops are controlled
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);
    TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

    TIM_SetCounter(TIM2, 0);

    TIM_Cmd(TIM2, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Time Base initialization configuration
/// @note
/// @param  psc: Clock frequency division value.
/// @param  arr: Preloading value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void TIM3_Slave_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
    ///TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    //Select Timer Mode: TIM Pulse Width Modulation Mode 2
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    //Setting the Pulse Value of the Capture Comparison Register to be Loaded
    TIM_OCInitStructure.TIM_Pulse = 499;
    //Output polarity: TIM output is more polar
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    //Enable TIMx preload register on ARR
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    //Trigger selection, ITR0 is TIM1
    TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);
    //Trigger mode only starts; door mode starts and stops are controlled
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Gated);
    TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);

    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
}
/// @}


/// @}

/// @}



