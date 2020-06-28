////////////////////////////////////////////////////////////////////////////////
/// @file     main.c
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#include "delay.h"
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Functions
/// @{
extern u32 SystemCoreClock;
////////////////////////////////////////////////////////////////////////////////
/// @brief   the main entrance
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{

    RCC->AHBENR |= 0x1 << 17; //enable GPIOA clock
    RCC->APB2ENR |= 0x1;
    GPIOA->CRH &= 0XFFFFFFF0;
    GPIOA->CRH |= 0xb; //PA8 push-pull output
    GPIOA->AFRH &= ~0XF;
    if(SystemCoreClock < 8000000) {
        RCC->CFGR |= (u32)RCC_CFGR_MCO_HSE ;//PLL/2
    }
    else if(SystemCoreClock <= 48000000) {
        RCC->CFGR |= (u32)RCC_CFGR_MCO_SYSCLK ;//PLL/2
    }
    else {
        RCC->CFGR |= (u32)RCC_CFGR_MCO_PLL ;//PLL/2
    }
    while(1);
}
/// @}

/// @}

/// @}


