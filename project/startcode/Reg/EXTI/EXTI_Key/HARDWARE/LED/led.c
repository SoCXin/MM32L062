////////////////////////////////////////////////////////////////////////////////
/// @file     led.c
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
#define _LED_C_

// Files includes
#include "led.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup LED
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup LED_Exported_Constants
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  LED initialization
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void LED_Init(void)
{

    RCC->AHBENR |= 0x7 << 17; //enable GPIOA clock
    //JTAG_Set(SWD_ENABLE); // close JTAG,start SWD

    GPIOA->CRH |= GPIO_CRH_MODE15;         //PA15 push-pull output
    GPIOA->ODR |= GPIO_ODR_ODR15;          // GPIOA->ODR|=1<<15;   //PA15  output high

    GPIOB->CRL |= GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5; ////PB3,PB4,PB5push-pull output
    GPIOB->ODR |= GPIO_ODR_ODR3 | GPIO_ODR_ODR4 | GPIO_ODR_ODR5;    ////PB.3,PB4,PB5 output high

    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    LED4_OFF();
}

/// @}

/// @}

/// @}
