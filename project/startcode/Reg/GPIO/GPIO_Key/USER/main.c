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
#include "led.h"
#include "key.h"

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
/// @brief    The corresponding light is on when you press the button
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u8 t = 0;

    DELAY_Init();
    LED_Init();
    KEY_Init();             //initialand  press  key  connect port

    while(1) {
        t = KEY_Scan(0);    //get   key value
        switch(t) {
            case KEY1_PRES://K1defaultregister  continuousPC13,default connect resetreset   press  key , press downK1 reset
                LED1_TOGGLE();
                break;
            case WKUP_PRES:
                LED2_TOGGLE();
                break;
            case KEY3_PRES:
                LED3_TOGGLE();
                break;
            case KEY4_PRES:
                LED4_TOGGLE();
                break;
            default:
                DELAY_Ms(10);
        }
    }
}
/// @}


/// @}

/// @}




