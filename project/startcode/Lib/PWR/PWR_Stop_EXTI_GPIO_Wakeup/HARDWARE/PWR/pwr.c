////////////////////////////////////////////////////////////////////////////////
/// @file    pwr.c
/// @author  AE TEAM
/// @brief    In window comparator mode,The transformation results are detected
///           Set the threshold value from 0 to 3V, and connect PB6 and PA0 with
///           jumper cap to see the effect.
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
#define _PWR_C_

// Files includes
#include "delay.h"
#include "led.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWR_Exported_Functions
/// @{
void SystemInit(void);
////////////////////////////////////////////////////////////////////////////////
/// @brief  delay nTime ms
/// @note   get x times.
/// @param  nTime  nTime ms.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void deleyNop(u32 DlyTime)
{
    u32 i, j;
    for(i = 0; i < DlyTime; i++) {
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
////////////////////////////////////////////////////////////////////////////////
/// @brief  External interrupt 0, clears interrupt flag after entering.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI4_15_IRQHandler(void)
{
    EXTI_ClearFlag(EXTI_Line10 | EXTI_Line11 | EXTI_Line13);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is EXTI0 1 Handler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI0_1_IRQHandler(void)
{
    EXTI_ClearFlag(EXTI_Line0);
}
void GPIO_AIN_Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB |
                          RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC EXTI Config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EXTI_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //ENABLE  External interrupt
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  EXTI Config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EXTI_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    //PA.0 use EXTI line 0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

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
/// @brief  EXTI Config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EXTI_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                  //PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //set as pull down input
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;                   //PB10 K3,PB11 K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //set as pull up input
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  External interrupt wake-up standby initialization
/// @note   External Wake-up Pin Selection.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void WKUP_STOP_Init(void)
{
    // EXTI  SYSTEM CLOLK ENABLE
    GPIO_AIN_Init();
    EXTI_NVIC_Init();
    EXTI_Config();
    EXTI_GPIO_Config();

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_STOP_Exti_Init(void)
{
    u16 i;
    deleyNop(10000);

    for(i = 0; i < 10; i++) {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        deleyNop(1000);
    }
    WKUP_STOP_Init();
    //important notes
    //need to set those GPIO Pins as AIN exclude the wake up pin;
    //need to disalbe those interrupts (Systick, Timer), exclude the wake interrupt
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_PWREN, ENABLE);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    //After enter STOP mode, the SYSCLK changed to HSI/6
    //Need re-initialize the SYSCLK , set HSI/4+PLL to 48Mhz as SYSCLK
    SystemInit();
    //re-initialize those GPIO Pins as previous setting before the wake up pin;
    //re-enalbe those interrupts (Systick, Timer) as previous setting before
    //the wake up pin

}




/// @}

/// @}

/// @}
