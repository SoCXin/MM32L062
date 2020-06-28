////////////////////////////////////////////////////////////////////////////////
/// @file    comp.c
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
#define _COMP_C_

// Files includes


#include "comp.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup COMP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup COMPExported_Constants
/// @{

#define COMP_Selection_COMP1 ((u32)0x00000000)




__IO u8 ucA;

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is comp test entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void COMP_Test(void)
{

    RCC_ConfigInit();

    Comp_Config(COMP_Selection_COMP1);
    EXTIX_Init();
    COMPOut_GPIO_ConfigInit();
    COMPOutPA6_GPIO_ConfigInit();
    COMPOutPA11_GPIO_ConfigInit();

    while(1) {
        //ucA = Comp_StatusCheck(COMP_Selection_COMP1);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTIX config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);                      //Enable SYSCFGEN


    EXTI_InitStructure.EXTI_Line = EXTI_Line19;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //Falling Trigger
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = ADC_COMP_IRQn;                          //Irq config
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //Priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //Enable Irq
    NVIC_Init(&NVIC_InitStructure);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Comp Irq Handler
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1_COMP_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line19) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line19);
        ucA = Comp_StatusCheck(COMP_Selection_COMP1);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void COMPOut_GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_7);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void COMPOutPA6_GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_7);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  RCC config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_ConfigInit(void)
{

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  COMP GPIO config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void COMP_GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}



////////////////////////////////////////////////////////////////////////////////
/// @brief  COMP config
/// @note   None.
/// @param  : COMP_Selection_COMPx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Comp_Config(u32 COMP_Selection_COMPx)
{
    COMP_InitTypeDef COMP_InitStructure;
    COMP_GPIO_ConfigInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_COMP, ENABLE);
    COMP_StructInit(&COMP_InitStructure);
    COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_IO0;
    COMP_InitStructure.COMP_NonInvertingInput = COMP_NonInvertingInput_IO0;
    COMP_InitStructure.COMP_Output = COMP_Output_None;
    COMP_InitStructure.COMP_OutputPol = COMP_OutputPol_NonInverted;
    COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_No;
    COMP_InitStructure.COMP_Mode = COMP_Mode_MediumSpeed;
    COMP_InitStructure.OFLT = COMP_Filter_4_Period;
    COMP_Init(COMP_Selection_COMPx, &COMP_InitStructure);

    COMP_Cmd(COMP_Selection_COMPx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Check COMP status
/// @note   None.
/// @param  : COMP_Selection_COMPx
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 Comp_StatusCheck(u32 COMP_Selection_COMPx)
{
    u8 chCapStatus = 2;
    if(COMP_GetOutputLevel(COMP_Selection_COMPx) == 0) {
        chCapStatus = 0;
    }
    else {
        chCapStatus = 1;
    }
    return chCapStatus;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void COMPOutPA11_GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_7);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/// @}

/// @}

/// @}
