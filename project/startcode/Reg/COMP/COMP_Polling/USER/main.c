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


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Functions
/// @{
__IO u8 ucA;
void RCC_ConfigInit(void);
void Comp_Config(void);
void COMPOut_GPIO_ConfigInit(void);
////////////////////////////////////////////////////////////////////////////////
/// @brief  main entrance
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{

    RCC_ConfigInit();
    Comp_Config();
    COMPOut_GPIO_ConfigInit();
    while(1) {
        if((*(u32*)(COMP_BASE ) & COMP_CSR_STA) != 0) {
            ucA = 1;
        }
        else {
            ucA = 0;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  RCC configure
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////

void RCC_ConfigInit(void)
{

    RCC->AHBENR &= ~(0xf << 17);
    RCC->AHBENR |= 0x0f << 17;

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  COMP configure
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Comp_Config(void)
{
    RCC->APB2ENR &= ~(0x01 << 15);  //COMP ENABLE
    RCC->APB2ENR |= 0x01 << 15;
    *(u32*)(COMP_BASE ) =  0x00 << 4 | 0x00 << 7 | 0x02 << 18 | 0x02 << 2; //COMP config
    (*(u32*)(COMP_BASE  ) |=  COMP_CSR_EN);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO  configure initialization
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void COMPOut_GPIO_ConfigInit(void)
{
    u32* ptr;
    RCC->APB2ENR &= ~(RCC_APB2Periph_SYSCFG);
    RCC->APB2ENR |= RCC_APB2Periph_SYSCFG;

    ptr = (0 < 8) ? (u32*)&GPIOA->AFRL : (u32*)&GPIOA->AFRH;   //AF ENABLE
    *ptr = (*ptr & ~(0x0F )) | (0x7U );

    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
}
/// @}


/// @}

/// @}
