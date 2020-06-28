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
#include "uart_nvic.h"
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
/// @brief  the main entrance
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u8 t;
    u8 len;
    u16 times = 0;

    DELAY_Init();
    LED_Init();
    uart_nvic_init(115200);  // uart initial115200
    while(1) {
        if(UART_RX_STA & 0x8000) {
            len = UART_RX_STA & 0x3fff; //get   receive  to  data length

            for(t = 0; t < len; t++) {
                while((UART1->CSR & 0x1) == 0); //wait  send  end
                UART1->TDR = UART_RX_BUF[t];
            }

            UART_RX_STA = 0;
        }
        else {
            times++;
            if(times % 5000 == 0) {
                LED3_TOGGLE();
            }
            if(times % 200 == 0) {
                LED2_TOGGLE();
            }
            if(times % 30 == 0) {
                LED1_TOGGLE(); // flickerLED,system  run.
            }
            DELAY_Ms(10);
        }
    }
}

/// @}


/// @}

/// @}



