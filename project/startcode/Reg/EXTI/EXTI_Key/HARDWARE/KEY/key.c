////////////////////////////////////////////////////////////////////////////////
/// @file     key.c
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
#define _KEY_C_

// Files includes
#include "key.h"
#include "delay.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY_Exported_Constants
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief   the function is the key initialization
/// @param   None
/// @retval  None
///////////////////////////////////////////////////////////////////////////////
void KEY_Init(void)
{
    RCC->AHBENR |= 0x7 << 17;    //RCC->APB2ENR|=1<<2;  //enable GPIOA clock
#if defined (MINIBOARD)
    GPIOA->CRL &= 0XFFFFFFF0; //PA0set  input     ,K2(WK_UP)
    GPIOA->CRL |= GPIO_CRL_CNF0_1;
    GPIOC->CRH &= 0XFF0FFFFF; //PC13set  input ,K1
    GPIOC->CRH |= GPIO_CRH_CNF13_1;
    GPIOC->ODR |= GPIO_ODR_ODR13;   //PC13pull-down
#endif
#if defined (EMINIBOARD)
    GPIOB->CRL &= 0XFFFFF00F; //PA0set  input     ,K2(WK_UP)
    GPIOB->CRL |= (GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1);
    GPIOB->ODR |= GPIO_ODR_ODR2;
#endif
    GPIOB->CRH &= 0XFFFF00FF; //PB10,PB11set  input ,K3,K4
    GPIOB->CRH |= GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1;
    GPIOB->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR11;  //PB10,PB11pull-down
}
////////////////////////////////////////////////////////////////////////////////
/// @brief   press  key deal function
/// @param   mode:0,register  supportcontinuous press ;1,supportcontinuous press;
/// @retval  press  key value
////////////////////////////////////////////////////////////////////////////////
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; // press  key  press  flag
    if(mode)key_up = 1; // support continuous press
    if(key_up && (KEY1 == 0 || WK_UP == 1 || KEY3 == 0 || KEY4 == 0)) {
        DELAY_Ms(10);
        key_up = 0;
        if(KEY1 == 0)return KEY1_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
        else if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
    }
    else if(KEY1 == 1 && KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;// none press  key  press down
}
/// @}

/// @}

/// @}



















