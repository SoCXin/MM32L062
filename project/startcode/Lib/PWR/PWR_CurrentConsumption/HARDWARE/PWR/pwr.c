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
u32 stop_flag;
u8 buff[10];
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
void EXTI0_1_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  External interrupt wake-up standby initialization
/// @note   External Wake-up Pin Selection.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void WKUP_STOP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    EXTI_StructInit(&EXTI_InitStructure);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,  EXTI_PinSource0);


    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize PWR_STOP_Exti
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_STOP_ExtiInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    deleyNop(10000);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB |
                          RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_STOP_ExtiTest(void)
{
    u16 i;

    for(i = 0; i < 5; i++) {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        deleyNop(1000);
    }
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_PWREN, ENABLE);
    WKUP_STOP_Init();
    //important notes
    //need to set those GPIO Pins as AIN exclude the wake up pin;
    //need to disalbe those interrupts (Systick, Timer), exclude the wake interrupt
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    stop_flag = 1;
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
