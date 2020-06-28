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
////////////////////////////////////////////////////////////////////////////////
/// @brief  delay config.
/// @note   None.
/// @param  None.
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
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_PVD_PollInit(void)
{
    u16 i;
    deleyNop(10000);

    LED_Init();

    for(i = 0; i < 10; i++) {
        LED2_TOGGLE();
        deleyNop(3000);
    }
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    // Configure EXTI Line to generate an interrupt on falling edge
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);


    // Configure the PVD Level to 1.8 ~ 4.8V
//    PWR_PVDLevelConfig(PWR_CR_PLS_1V8);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V1);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V4);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V7);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V0);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V3);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V6);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V9);
//    PWR_PVDLevelConfig(PWR_CR_PLS_4V2);
//    PWR_PVDLevelConfig(PWR_CR_PLS_4V5);
    PWR_PVDLevelConfig(PWR_CR_PLS_4V8);
    // Enable the PVD Output
    PWR_PVDCmd(ENABLE);


}
/// @}

/// @}

/// @}
