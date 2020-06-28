////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
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
#define _MAIN_C_

// Files includes


#include "led.h"




////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{



u32 Counter = 0;
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is called back by SysTick Handler
///         call series function to run systick cycle running
///         for example , delay, led blink, key in scan , etc.
/// @note   This function always run after power on.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SysTick_Handler(void)
{
    Counter++;
    if(Counter >= 1000) {
        Counter = 0;
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is called back by SysTick Handler
///         call series function to run systick cycle running
///         for example , delay, led blink, key in scan , etc.
/// @note   This function always run after power on.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Systick_init()
{
    if (SysTick_Config(RCC_GetSysClockFreq() / 1000)) {
        // Capture error
        while (1);
    }
    // Configure the SysTick handler priority
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTickinterrupt priority class set
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    Systick_init();
    LED_Init();
    while(1) {
        ;
    }

    //return 0;
}



/// @}

/// @}

/// @}
