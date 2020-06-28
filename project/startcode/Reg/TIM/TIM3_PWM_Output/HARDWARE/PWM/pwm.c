////////////////////////////////////////////////////////////////////////////////
/// @file     pwm.c
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
#define _PWM_C_

// Files includes
#include "pwm.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWM
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup PWM_Exported_Constants
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief   PWM output initial
/// @param   arr:self-motion reload value
/// @param   psc:Clock predivision coefficient
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
void TIM3_PWM_Init(u16 arr, u16 psc)
{

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //TIM3clock enable
    RCC->AHBENR |= 0x7 << 17; //RCC->APB2ENR|=1<<3;  //enablePORTBclock

    //SYSCFG->CFGR|=0x1<<11;
    GPIOB->AFRL &= (~(0xf << (4 * 4)));
    GPIOB->AFRL |= 0x1 << (4 * 4);
    GPIOB->CRL &= 0XFFF0FFFF; //PB4clear set
    GPIOB->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_CNF4_1; // function output

    TIM3->ARR = arr;
    TIM3->PSC = psc;

    TIM3->CCMR1 |= TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2; //CH1 PWM2 mode
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;     //CH1 preloaded enable
    TIM3->CCER |= TIM_CCER_CC1E;    //OC1  output enable
    TIM3->CR1 = TIM_CR1_ARPE;   //ARPEenable
    TIM3->CR1 |= TIM_CR1_CEN;   //enabletiming  device1

}
/// @}


/// @}

/// @}


