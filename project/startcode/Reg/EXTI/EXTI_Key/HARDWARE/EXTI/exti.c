////////////////////////////////////////////////////////////////////////////////
/// @file     exti.c
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
#define _EXTI_C_

// Files includes
#include "exti.h"
#include "led.h"
#include "key.h"
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXTI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXTI_Exported_Constants
/// @{

extern void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM);



/////////////////////////////////////////////////////////////////////////////////
/// @brief  setup M0 core  NVIC Initialize
/// @param  NVIC_IRQChannelPriority,
/// @param  NVIC_IRQChannel,
/// @param  NVIC_IRQChannelCmd
/// @retval None
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
//external interrupt initialprocedure
void EXTIX_Init(void)
{
    RCC->APB2ENR |= 0x00000001;       //enableAFIOclock
#if defined (MINIBOARD)
    Ex_NVIC_Config(GPIO_A, 0, RTIR);    //rising edge trigger
    Ex_NVIC_Config(GPIO_C, 13, FTIR);   //falling edgetrigger
#endif
#if defined (EMINIBOARD)
    Ex_NVIC_Config(GPIO_B, 1, RTIR);    //rising edge trigger
    Ex_NVIC_Config(GPIO_B, 2, FTIR);   //falling edgetrigger
#endif
    Ex_NVIC_Config(GPIO_B, 10, FTIR);   //falling edgetrigger
    Ex_NVIC_Config(GPIO_B, 11, FTIR);   //falling edgetrigger
    M0_NVIC_Init(2, EXTI0_1_IRQn, ENABLE);
#if defined (EMINIBOARD)
    M0_NVIC_Init(1, EXTI2_3_IRQn, ENABLE);
#endif
    M0_NVIC_Init(2, EXTI4_15_IRQn, ENABLE);
}

u16 EXTI_DELAY = 0;

void EXTI0_1_IRQHandler(void)
{
    for(EXTI_DELAY = 0; EXTI_DELAY < 10000; EXTI_DELAY++);
#if defined (MINIBOARD)
    if(WK_UP == 1) {
        LED2_TOGGLE();
    }
    EXTI->PR = 1 << 0; //clear LINE0interrupt  flag bit
#endif
#if defined (EMINIBOARD)
    if(KEY1 == 1) {
        LED1_TOGGLE();
    }
    EXTI->PR = 1 << 1; //clear LINE0interrupt  flag bit
#endif
}
#if defined (EMINIBOARD)
void EXTI2_3_IRQHandler(void)
{
    for(EXTI_DELAY = 0; EXTI_DELAY < 10000; EXTI_DELAY++);
    if(WK_UP == 0) {
        LED2_TOGGLE();
    }
    EXTI->PR = 1 << 2; //clear LINE0interrupt  flag bit
}

#endif
void EXTI4_15_IRQHandler (void)
{
    for(EXTI_DELAY = 0; EXTI_DELAY < 10000; EXTI_DELAY++);
#if defined (MINIBOARD)
    if(KEY1 == 0) {
        LED1_TOGGLE();
    }
#endif
    if(KEY3 == 0) {
        LED3_TOGGLE();
    }
    else if(KEY4 == 0) {
        LED4_TOGGLE();
    }
#if defined (MINIBOARD)
    EXTI->PR = 1 << 13; //clear LINE10interrupt  flag bit
#endif
    EXTI->PR = 1 << 11; //clear LINE11interrupt  flag bit
    EXTI->PR = 1 << 10; //clear LINE13interrupt  flag bit
}

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
/// @}

/// @}

/// @}


