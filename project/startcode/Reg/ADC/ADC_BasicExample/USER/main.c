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
#include "adc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Functions
/// @{
/////////////////////////////////////////////////////////////////////////////////
/// @brief    print the value of ADC1_SingleChannel
/// @param    None
/// @retval   None
/////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u16 ADCVAL;
    float fValue;

    DELAY_Init();
    CONSOLE_Init(115200);  // uart initial115200

    // configure ADC1single transform  mode , channel 1enable
    ADC1_SingleChannel();
    while(1) {
        //read current transformation data
        ADCVAL = ADC1_SingleChannel_Get();
        fValue = ((float)ADCVAL / 4095) * 3.3;
        printf("ADC1_CH_1=%fV\r\n", fValue);
        DELAY_Ms(500);
    }
}
/// @}


/// @}

/// @}
