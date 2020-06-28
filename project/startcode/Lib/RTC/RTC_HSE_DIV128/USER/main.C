////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
/// @author  AE TEAM
/// @brief   Read and write flash using SPI2 and print the first ten characters.
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

#include "rtc.h"
#include "delay.h"
#include "uart.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_REALTIME
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    u8 t;

    CONSOLE_Init(9600);
    DELAY_Init();
    RTC_HSE_Init();
    while(1) {
        if(t != calendar.sec) {
            t = calendar.sec;
            printf("%d", calendar.w_year);
            printf("%d", calendar.w_month);
            printf("%d", calendar.w_date);
            printf("%d", calendar.hour);
            printf("%d", calendar.min);
            printf("%d", calendar.sec);
        }
        DELAY_Ms(10);
    }
}




/// @}

/// @}

/// @}


