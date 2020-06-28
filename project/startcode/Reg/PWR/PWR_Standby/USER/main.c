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
#include "wkup.h"
#include "led.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Functions
/// @{
////////////////////////////////////////////////////////////////////////////////
/// @brief   the main entrance
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u8   i;
    DELAY_Init();
    LED_Init();
    CONSOLE_Init(115200);     // uart initial115200
    printf("please  press downwakeup press  key 3s register release\r\n");
    WKUP_Init();            //initialWK_UP press  key ,simultaneous detection whether normal

    while(1) {

        for(i = 0; i < 6; i++) {
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            DELAY_Ms(250);// delay250ms
            printf("from standby mode  wakeup \r\n");
        }

        printf("please  press wakeup key to wakup from Standby!\r\n");
        printf("Enter Standby!\r\n");
        WKUP_Init();            //initialWK_UP press  key ,simultaneous detection whether normal

        while(1) {

        }
    }
}
/// @}

/// @}

/// @}
