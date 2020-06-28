////////////////////////////////////////////////////////////////////////////////
/// @file     tim2.c
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
#define _TIM2_C_

// Files includes
#include "tim2.h"
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM2
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup TIM_Exported_Constants
/// @{

/////////////////////////////////////////////////////////////////////////////////
/// @brief    Tim2  interrupt function
/// @param    None
/// @retval   None
/////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
    // You have to manually Clearinterrupt flag bit
    // TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    //
    // whole situation flag
    // ucTim2Flag++;
    if(TIM2->SR & TIM_SR_UIF) { //over flowinterrupt
        // whole situation flag
        ucTim2Flag++;
    }
    TIM2->SR &= ~(TIM_SR_UIF); //Clearinterrupt flag bit

}


/////////////////////////////////////////////////////////////////////////////////
/// @brief    setup NVIC Initialize
/// @param    NVIC_IRQChannelPriority,
/// @param    NVIC_IRQChannel,
/// @param    NVIC_IRQChannelCmd
/// @retval   None
/////////////////////////////////////////////////////////////////////////////////
void M0_NVIC_Init(u32 NVIC_IRQChannelPriority, IRQn_Type NVIC_IRQChannel, FunctionalState NVIC_IRQChannelCmd)
{
    if (NVIC_IRQChannelCmd != DISABLE) {
        NVIC->IP[NVIC_IRQChannel >> 0x02] =
            (NVIC->IP[NVIC_IRQChannel >> 0x02] &
             (~(((u32)0xFF) << ((NVIC_IRQChannel & 0x03) * 8)))) |
            ((((u32)NVIC_IRQChannelPriority << 6) & 0xFF) << ((NVIC_IRQChannel & 0x03) * 8));

        NVIC->ISER[0] = 0x01 << (NVIC_IRQChannel & 0x1F);
    }
    else {
        NVIC->ICER[0] = 0x01 << (NVIC_IRQChannel & 0x1F);
    }
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief    configure Tim2  count-up mode
/// @param    Period: 16 bit count reload value
/// @param    Prescaler :Clock predivision fractional frequency value
/// @retval   None
/////////////////////////////////////////////////////////////////////////////////
void Tim2_UPCount_test(u16 Prescaler, u16 Period)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //TIM3clock enable
    TIM2->ARR = Prescaler;  // set count self-motion reload value
    TIM2->PSC = Period; // fractional frequency deviceset
    TIM2->DIER |= TIM_DIER_UIE; //allow updata interrupt
    TIM2->CR1 |= TIM_CR1_CEN;  //enabletiming  device3
    TIM2->CR1 |= TIM_CR1_CEN;  //enabletiming  device3

    M0_NVIC_Init(2, TIM2_IRQn, ENABLE);
}
/// @}


/// @}

/// @}

