////////////////////////////////////////////////////////////////////////////////
/// @file    exti.c
/// @author  AE TEAM
/// @brief   key intput.
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
#define _EXTI_C_

// Files includes
#include "exti.h"
#include "delay.h"
#include "led.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize KEY GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE);
#if defined (MINIBOARD)
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //PC13£¬K1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //set as pull up input
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                  //PA0,K2£¨WK_UP£©
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //set as pull down input
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#if defined (EMINIBOARD)
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;                                 //PB1£¬K1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //set as pull up input
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;                                  //PB2, K2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //set as pull down input
    GPIO_Init(GPIOB, &GPIO_InitStructure);

#endif
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;                   //PB10 K3,PB11 K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //set as pull up input
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize exti nvic config
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EXTI_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#if defined (EMINIBOARD)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize exti interrupt Line
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EXTI_NVIC_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
#if defined (MINIBOARD)
    //PC.13 use EXTI line 13
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //PA.0 use EXTI line 0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
#endif
#if defined (EMINIBOARD)
    //PC.13 use EXTI line 13
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //PA.0 use EXTI line 0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
#endif
    //PB.10 use EXTI line 10
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //PB.11 use EXTI line 11
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function EXTI config
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI_Config(void)
{
    EXTI_NVIC_Config();
    EXTI_NVIC_Init();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is EXTI0 1 Handler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI0_1_IRQHandler(void)
{
    u16 EXTI_DELAY = 0;
    for(EXTI_DELAY = 0; EXTI_DELAY < 1000; EXTI_DELAY++);
#if defined (MINIBOARD)
    if(WK_UP == 1) {
        LED2_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line0);
#endif
#if defined (EMINIBOARD)
    if(KEY1 == 1) {
        LED1_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line1);
#endif

}
#if defined (EMINIBOARD)
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is EXTI0 1 Handler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI2_3_IRQHandler(void)
{
    u16 EXTI_DELAY = 0;
    for(EXTI_DELAY = 0; EXTI_DELAY < 1000; EXTI_DELAY++);
    if(WK_UP == 0) {
        LED2_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line2);
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is EXTI4 15 Handler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI4_15_IRQHandler(void)
{
    u16 EXTI_DELAY = 0;
    for(EXTI_DELAY = 0; EXTI_DELAY < 1000; EXTI_DELAY++);
    if(KEY3 == 0) {
        LED3_TOGGLE();
    }
#if defined (MINIBOARD)
    else if(KEY1 == 0) {
        LED1_TOGGLE();
    }
#endif
    else if(KEY4 == 0) {
        LED4_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line10 | EXTI_Line11 | EXTI_Line13);

}


/// @}


/// @}

/// @}

