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


#include "led.h"
#include "delay.h"
#include "main.h"
#include "uart.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{


//for LED test
void WWDGtest(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    WWDGtest();

    //return 0;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the GPIO port clock
/// @note   This function should affected by chip version.
/// @param  GPIOx : Select GPIO port.
/// @param  NewState : Enable or disable the GPIO clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Wwdg_reset_ON(u8 ucTcnt, u8 ucWcnt)
{
    // WWDG configuration
    // Enable WWDG clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    //WWDG_Prescaler_x,x?1,2,4,8,WWDG counter clock = (PCLK1/4096)/x
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(ucWcnt);

    // Enable WWDG and set counter value from 0x40  to 0x7F
    // Enable WWDG and set counter value to 127, WWDG timeout = ~683 us * 64 = 43.7 ms
    //In this case the refresh window is: ~683 * (127-80)= 32.1ms < refresh window < ~683 * 64 = 43.7ms
    WWDG_Enable(ucTcnt);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the GPIO port clock
/// @note   This function should affected by chip version.
/// @param  GPIOx : Select GPIO port.
/// @param  NewState : Enable or disable the GPIO clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Wwdg_irq_ON(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;


    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetWindowValue(0x70);
    WWDG_EnableIT();
    //Here is the configuration. The parameter value must be less than the
    //window value, otherwise the reset will occur immediately.
    WWDG_Enable(0x66);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the GPIO port clock
/// @note   This function should affected by chip version.
/// @param  GPIOx : Select GPIO port.
/// @param  NewState : Enable or disable the GPIO clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void WWDG_IRQHandler (void)
{
    if(WWDG_GetFlagStatus()) {
        WWDG_ClearFlag();
        WWDG_SetCounter(0x66);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  WWDG test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void WWDGtest(void)
{
    CONSOLE_Init(115200);
    printf("uart ok!\r\n");

    //ENABLE WWDG
    Wwdg_reset_ON(0x7e, 0x7f);

    while(1) {
        //No reset program goes into the dead cycle, anyway, the system
        //has been printing serial data.
        WWDG_SetCounter(0x7e);
//        DELAY_Ms(1);
    }
}

/// @}

/// @}

/// @}
