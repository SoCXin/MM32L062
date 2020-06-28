////////////////////////////////////////////////////////////////////////////////
/// @file    wkup.c
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
#define _WKUP_C_

// Files includes
#include "wkup.h"
#include "led.h"
#include "delay.h"
#include "core_cm0.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup WKUP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup WKUP_Exported_Constants
/// @{

//////////////////////////////////////////////////////////////////////////////////
/// @brief   entrance the the mode of standby
/// @param   None
/// @retval  None
//////////////////////////////////////////////////////////////////////////////////

void Sys_Standby(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    PWR->CSR |= 0x00000100;

    // Clear Wake-up flag
    PWR->CR |= 0x4;
    // Select STANDBY mode
    PWR->CR |= 0x2;
    // Set SLEEPDEEP bit of Cortex System Control Register

    SCB->SCR |= 0x4;
    // Request Wait Forinterrupt
    __WFI();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   check if  wake up from standby mode
/// @param   None
/// @retval  0:failure; 1: success
////////////////////////////////////////////////////////////////////////////////
u8 Check_WKUP(void)
{
    u16 t = 0;  // record  press down time
    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();

    while(1) {
        if((GPIOA->IDR & 0x1) == 1) {
            t++;
            DELAY_Ms(2);
            if(t >= 1000) { // press down over 2second clock
                LED1_ON();
                LED2_ON();
                LED3_ON();
                LED4_ON();
                return 1;   // press down over 3s
            }
        }
        else {
            LED1_OFF();
            LED2_OFF();
            LED3_OFF();
            LED4_OFF();
            return 0; // press downless than 3s
        }

    }

}


/////////////////////////////////////////////////////////////////////////////////
/// @brief    external interrupt 0,enter Clearinterrupt flag bit
/// @param    None
/// @retval   None
/////////////////////////////////////////////////////////////////////////////////
void EXTI0_1_IRQHandler(void)
{
    EXTI->PR = EXTI_PR_PR0;
}
/////////////////////////////////////////////////////////////////////////////////
/// @brief    configure the NVIC
/// @param    GPIOx : X can be A/B/C/D
/// @param    BITX
/// @param    TRIM
/// @retval   None
/////////////////////////////////////////////////////////////////////////////////

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




/////////////////////////////////////////////////////////////////////////////////
/// @brief   setup NVIC Initialize
/// @param   NVIC_IRQChannelPriority,
/// @param   NVIC_IRQChannel,
/// @param   NVIC_IRQChannelCmd
/// @retval  None
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
/// @brief   initialization of external mode to wake up standby mode
/// @param   None
/// @param   None
/////////////////////////////////////////////////////////////////////////////////
void WKUP_Init(void)
{
    RCC->AHBENR |= 0x7 << 17; //firstly enableexternal IO PORTAclock
    RCC->APB2ENR |= 0x1;

    GPIOA->CRL &= 0XFFFFFFF0; //PA0set  input
    GPIOA->CRL |= GPIO_CRL_CNF0_1;
    //useexternal interrupt
    Ex_NVIC_Config(GPIO_A, 0, RTIR);    //rising edge trigger
    M0_NVIC_Init(2, EXTI0_1_IRQn, ENABLE);
    if(Check_WKUP() == 0) Sys_Standby();  //register boot,enter standby mode  mode
}

/// @}

/// @}

/// @}













