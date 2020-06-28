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
#define RTC_IRQHandler RTC_BKP_IRQHandler

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
void RTC_Irg_ON(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_DeInit();
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17 ;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt ;
    EXTI_InitStructure.EXTI_Trigger =   EXTI_Trigger_Rising_Falling  ;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line17);

    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize RTC clock and check whether it works properly
/// @note    It must be careful of the Chip Version.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 RTC_Init(void)
{
    //Check if the clock is configured for the first time
    u8 temp = 0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    //Enabling backup register access
    PWR_BackupAccessCmd(ENABLE);
    // Reset Backup Domain
    //Read data from a specified backup register: Read data that is not identical
    //to the specified data written
    if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050) {
        //Reset backup area
        BKP_DeInit();
        RCC_LSICmd(ENABLE);
        deleyNop(2000);
        //Check whether the specified RCC marker is set or not, and wait for the
        //low-speed crystal oscillator to be ready
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) {
            temp++;
            if(temp >= 250)return 1;
            deleyNop(10);
        }
        //Setting RTC Clock
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        RCC_RTCCLKCmd(ENABLE);
        //Waiting for the last write to RTC registers to complete
        RTC_WaitForLastTask();
        //Waiting for RTC register synchronization
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_ALR, ENABLE);
        //Allow configuration
        RTC_WaitForLastTask();
        RTC_EnterConfigMode();
        //Setting the value of RTC pre-frequency Division
        RTC_SetPrescaler(32767);
        RTC_SetAlarm(0x05);
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();
        //Exit configuration mode
        RTC_ExitConfigMode();
        //Write user program data to a specified backup register
        BKP_WriteBackupRegister(BKP_DR1, 0X5050);
        RTC_WaitForLastTask();
    }
    //System Continuation Timing
    else {
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
    RTC_Irg_ON();
    return 0;
}
void RTC_IRQHandler(void)
{
    //Alarm clock interrupt
    if(RTC_GetITStatus(RTC_IT_ALR) != RESET) {
        RTC_ClearITPendingBit(RTC_IT_ALR);
    }
    RTC_ClearITPendingBit((RTC_IT_TypeDef)(RTC_IT_SEC | RTC_IT_OW));
    RTC_WaitForLastTask();
    EXTI_ClearFlag(EXTI_Line17);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PWR_STOP_RTC_Init(void)
{
    RTC_Init();
//    important notes
//    need to set those GPIO Pins as AIN exclude the wake up pin;
//    need to disalbe those interrupts (Systick, Timer), exclude the wake interrupt
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_PWREN, ENABLE);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}


/// @}

/// @}

/// @}
