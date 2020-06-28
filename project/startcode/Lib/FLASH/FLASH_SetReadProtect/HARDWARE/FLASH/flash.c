////////////////////////////////////////////////////////////////////////////////
/// @file    flash.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES SIM-EEPROM FUNCTIONS.
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
#define _FLASH_C_

// Files includes
#include "flash.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SIM_EEPROM
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is check the read protect status.
/// @note   None.
/// @param  None.
/// @retval protectstatus.
////////////////////////////////////////////////////////////////////////////////
u32 FLASH_CheckReadProtect(void)
{
    u32 protectstatus = 0;
    u16 data1;
    s32 i = 0;

    if ((FLASH->OBR & 0x02) != (u32)RESET) {
        // Read Protect on 0x1FFFF800 is set
        protectstatus = 1;
    }
    else {
        for (i = 0; i < 8; i++) {
            data1 = *(u16*)(0x1ffe0000 + i * 2);  //Address must be an integer multiple of 2
            if (data1 != 0xFFFF) {
                protectstatus = 2;
                break;
            }
        }
    }
    return protectstatus;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is portect Full main Flash enable.
/// @note   None.
/// @param  None.
/// @retval ret.
////////////////////////////////////////////////////////////////////////////////
//
s32 FLASH_EnableReadProtect(void)
{
    FLASH_Status status = FLASH_COMPLETE;
    s32 ret = 0;
    status = FLASH_ProgramOptionHalfWord(0x1ffe0000, 0x7F80);
    if (status != FLASH_COMPLETE)
        ret = 1;
    status = FLASH_ProgramOptionHalfWord(0x1ffe0002, 0xFF00);
    if (status != FLASH_COMPLETE)
        ret = 1;
    return ret;
}





/// @}

/// @}

/// @}




