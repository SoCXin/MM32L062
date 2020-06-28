////////////////////////////////////////////////////////////////////////////////
/// @file     sleep.c
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
#define _SLEEP_C_

// Files includes
#include "sleep.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SLEEP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SLEEP_Exported_Constants
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  configure the NVIC
/// @param  GPIOx:
/// @param  BITx
/// @param  TRIM
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM)
{
    u8 EXTADDR;
    u8 EXTOFFSET;
    EXTADDR = BITx / 4; //get  interrupt  device
    EXTOFFSET = (BITx % 4) * 4;
    RCC->APB2ENR |= 0x01; //enable ioclock
    SYSCFG->EXTICR[EXTADDR] &= ~(0x000F << EXTOFFSET);
    SYSCFG->EXTICR[EXTADDR] |= GPIOx << EXTOFFSET; //EXTI.BITx map  to GPIOx.BITx
    //self-motion set
    EXTI->IMR |= 1 << BITx; //  start line BITxinterrupt
    //EXTI->EMR|=1<<BITx;//register shield line BITxevent (if register shield is in hardware iscan, but is in soft emulation,interrupt with none!)
    if(TRIM & 0x01)EXTI->FTSR |= 1 << BITx; //line BITxevent falling edgetrigger
    if(TRIM & 0x02)EXTI->RTSR |= 1 << BITx; //line BITxevent The lift along the trigger
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  configure GPIO
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////

void GPIO_Configuration(void)
{
    RCC->AHBENR |= 0xf << 17;
    RCC->APB2ENR |= 0x00000001;

    // configure PA0bit pull-down input
    GPIOA->CRL = 0x8;

    // choose PA0bit external event trigger  input
    Ex_NVIC_Config(GPIO_A, 0, RTIR);
    //rising edge trigger
    //enableexternal interrupt 0,set event  mode ,falling edgetrigger
    EXTI->EMR = 0x01;
    EXTI->RTSR = 0x01;
    EXTI->PR = 0x01;

    //Register configure can embedinterrupt vector and only do wakeup event, which corresponds to bit not hanging, so none needs to clear out the event bit
}
/////////////////////////////////////////////////////////////////////////////////
/// @brief  go to sleep  mode
/// @param  None
/// @retval None
/////////////////////////////////////////////////////////////////////////////////
void sleep_test(void)
{
    GPIO_Configuration();
    __WFE();
}
/// @}


/// @}

/// @}
