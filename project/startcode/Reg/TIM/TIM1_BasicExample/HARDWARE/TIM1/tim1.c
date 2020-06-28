////////////////////////////////////////////////////////////////////////////////
/// @file     tim1.c
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
#define _TIM1_C_

// Files includes
#include "tim1.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM1
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM_Exported_Constants
/// @{

/////////////////////////////////////////////////////////////////////////////////
/// @brief    configure timing  device1count-up mode
/// @param    Period  : 16bit count reloadvalue
/// @param    Prescaler : Clock predivision fractional frequency value
/// @retval   None
/////////////////////////////////////////////////////////////////////////////////
void Tim1_UPCount_test(u16 Prescaler, u16 Period)
{
    //enableTIM1clock ,defaultclock sourcePCLK2(PCLK2 register  fractional frequencyregister frequency doubling ,
    //otherwise  PCLK2frequency doubling  output ),can  choose  clock source
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1->ARR = Prescaler;  // set count self-motion reload value
    TIM1->PSC = Period; // fractional frequency deviceset
    TIM1->CR1 |= TIM_CR1_CEN;  //enabletiming  device3


}
/////////////////////////////////////////////////////////////////////////////////
/// @brief   wait timing  deviceover flow
/// @param   None
/// @retval  None
/////////////////////////////////////////////////////////////////////////////////
void Tim1_UPStatusOVCheck_test(void)
{
    //wait event  updata  flag bit
    while((TIM1->SR & TIM_SR_UIF) == RESET);
    //clear event  flag bit

    TIM1->SR = ~TIM_SR_UIF;
}

/// @}


/// @}

/// @}
