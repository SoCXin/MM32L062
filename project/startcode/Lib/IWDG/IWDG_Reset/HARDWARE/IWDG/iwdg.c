////////////////////////////////////////////////////////////////////////////////
/// @file    iwdg.c
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
#define _IWDG_C_

// Files includes
#include "delay.h"
#include "led.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup IWDG
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup IWDG_Exported_Functions
/// @{

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
/// @brief  Set IWDG on
/// @param  IWDG_Prescaler     prescaler value
///         Reload      reload value
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Write_Iwdg_ON(u16 IWDG_Prescaler, u16 Reload)
{
    //Start the internal low-speed clock and wait for the clock to be ready
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    //Setting Clock Pre-division Frequency
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);

    //Setting overload register values
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);

    //Loading and Enabling Counter
    IWDG_ReloadCounter();
    IWDG_Enable();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  write IWDG relaod value -- feed watch dog
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  IWDG_Reset test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void IWDG_ResetTest(void)
{
    u32 i;
    // Check if the system has resumed from WWDG reset
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
        // WWDGRST flag set
        // Turn on LED1
        for(i = 0; i < 5; i++) {
            LED1_TOGGLE();
            deleyNop(300);
        }

        // Clear reset flags
        RCC_ClearFlag();
    }
    else {
        // WWDGRST flag is not set
        // Turn off LED1
        for(i = 0; i < 5; i++) {
            LED2_TOGGLE();
            deleyNop(100);
        }
    }
    deleyNop(1000);

    //Configuration LSI32 frequency division, counter initial value is 0x7ff,
    //reset time is about 1.6s.
    Write_Iwdg_ON(IWDG_Prescaler_32, 0xf);

}

/// @}

/// @}

/// @}
