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
#include "delay.h"
#include "flash.h"
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



////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    s32 status;
    LED_Init();
    DELAY_Init();
    //must disable all interrupt
    // Enable the Read Protect
    status = FLASH_CheckReadProtect();
    if (status == 0) { //not protect
        FLASH_Unlock();
        status = FLASH_EnableReadProtect();
        FLASH_Lock();
        if (status == 0) {
            status = 3;
        }
    }
    if (status == 3) { //success
        while (1) {
            LED3_TOGGLE();
            LED4_TOGGLE();
            DELAY_Ms(100);
        }
    }
    else {   //fail
        while (1) {
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            DELAY_Ms(200);
        }
    }
}

/// @}

/// @}

/// @}


