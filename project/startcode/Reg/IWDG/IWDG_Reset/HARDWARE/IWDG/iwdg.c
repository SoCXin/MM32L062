////////////////////////////////////////////////////////////////////////////////
/// @file     iwdg.c
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
#define _IWDG_C_

// Files includes
#include "iwdg.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup IWDG
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup IWDG_Exported_Constants
/// @{

/////////////////////////////////////////////////////////////////////////////////
/// @brief   start  independent watchdog
/// @param   IWDG_Prescaler: can  choose IWDG_Prescaler_X, X4,8,16,32,64,128,256, fractional frequencyvalue and X value
/// @param   Reload:  Reload<=0xfff,count reloadvalue
/// @retval  None
/////////////////////////////////////////////////////////////////////////////////
void Write_Iwdg_ON(u16 IWDG_Prescaler, u16 Reload)
{
    //start the internal low speedclock ,wait clock  ready
    RCC->CSR |= RCC_CSR_LSION;

    //set clock  fractional frequency
    IWDG->KR = 0X5555;
    IWDG->PR = IWDG_Prescaler;

    //set reload devicevalue
    //RVU_CheckStatus();
    IWDG->KR = 0X5555;
    IWDG->RLR = (Reload & 0xfff);
    //load and enablecount
    IWDG->KR = 0XAAAA;
    IWDG->KR = 0XCCCC;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief     feed  independent watchdog
/// @param     None
/// @retval    None
////////////////////////////////////////////////////////////////////////////////
void Write_Iwdg_RL(void)
{
    IWDG->KR = 0XAAAA; //reload
}
/// @}

/// @}

/// @}
