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
#include "uart.h"
#include "tim1.h"

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
/// @brief    the main entrance
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u32 uiCnt = 0;

    DELAY_Init();
    CONSOLE_Init(115200);
    Tim1_UPCount_test(SystemCoreClock / 10000 - 1, 9999);

    while(1) {
        for(uiCnt = 0; ; uiCnt++) {
            // wait timing  deviceover flow
            Tim1_UPStatusOVCheck_test();

            // print current  numbersvalue
            printf("0x%d\r\n", uiCnt);
        }
    }
}
/// @}


/// @}

/// @}
