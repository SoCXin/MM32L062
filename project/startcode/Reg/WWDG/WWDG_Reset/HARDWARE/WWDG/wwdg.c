////////////////////////////////////////////////////////////////////////////////
/// @file     wwdg.c
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
#define _WWDG_C_

// Files includes
#include "mm32_device.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup WWDG
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup WWDG_Exported_Constants
/// @{

/////////////////////////////////////////////////////////////////////////////////
/// @brief  Window dog  interrupt function
/// @param  None
/// @note   enterinterrupt when decrease count equals 0x40
///         count param It must be less than 0x7f and greater than 0x40.
///         If it is close to 0x40can, this interrupt can be frequently entered
/// @retval None
/////////////////////////////////////////////////////////////////////////////////
void WWDG_IRQHandler (void)
{
    if((WWDG->SR)) {
        WWDG->SR = (u32)RESET;
        WWDG->CR = (0x66 & 0x7f);
    }
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief  use Window dog
/// @param  ucTcn: watch dogdecrease count value
/// @param  ucWcnt: window value
/// @note   system  run-up Watchdog disable status ,Watchdog disable register close by hand,
///         reset Watchdog disable
/// @retval None
/////////////////////////////////////////////////////////////////////////////////
void Wwdg_reset_ON(u8 ucTcnt, u8 ucWcnt)
{
    //Window dogclock
    RCC->APB1ENR |= 0x00000800;

    //WWDG_Prescaler_x,x1,2,4,8,WWDG counter clock = (PCLK1/4096)/x
    WWDG->CFR = (WWDG->CFR & ~WWDG_CFGR_WDGTB) | (WWDG_CFGR_WDGTB_8);
    WWDG->CFR = (WWDG->CFR & ~WWDG_CFGR_WINDOW) | (ucWcnt & WWDG_CFGR_WINDOW);

    WWDG->CR = 0x00000080 | ucTcnt;
}

/// @}


/// @}

/// @}

