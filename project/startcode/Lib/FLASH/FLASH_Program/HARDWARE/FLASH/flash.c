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



#define BASED_FLASH_SECTOR_ADDRESS   0x08003000
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SIM_EEPROM
/// @{

static u32 gData = 0x12345679;
static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SIM_EEPROM_Exported_Constants
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  FLASH Program
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
u8 FLASH_Program(void)
{
    // Porgram FLASH Bank1
    // Unlock the Flash Bank1 Program Erase controller
    FLASH_Unlock();

    // Clear All pending flags
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    FLASH_ErasePage(BANK1_WRITE_START_ADDR);

    FLASH_ClearFlag(FLASH_FLAG_EOP );

    gFlashStatus = FLASH_ProgramWord(BANK1_WRITE_START_ADDR, gData);

    FLASH_ClearFlag(FLASH_FLAG_EOP );

    FLASH_Lock();

    if((*(__IO u32*) BANK1_WRITE_START_ADDR) != gData) {
        return 1;
    }
    return 0;
}


/// @}

/// @}

/// @}




