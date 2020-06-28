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
/// @brief     key initial
/// @param     None
/// @retval    None
////////////////////////////////////////////////////////////////////////////////
void KEY_Init(void)
{

#if defined(MINIBOARD)
    RCC->AHBENR |= 0xf << 17;                                                   //enable GPIOA clock

    GPIOC->CRH &= 0XFF0FFFFF;                                                   //PC13 set input ,K1
    GPIOC->CRH |= GPIO_CRH_CNF13_1;
    GPIOC->ODR |= GPIO_ODR_ODR13;                                               //PC13 pull-down

    GPIOA->CRL &= 0XFFFFFFF0;                                                   //PA0 set input ,K2(KEY2)
    GPIOA->CRL |= GPIO_CRL_CNF0_1;

    GPIOB->CRH &= 0XFFFF00FF;                                                   //PB10,PB11 set input ,K3,K4
    GPIOB->CRH |= GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1;
    GPIOB->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR11;                              //PB10,PB11 pull-down
#endif

#if defined(EMINIBOARD)
    RCC->AHBENR |= 0xf << 17;                                                   //enable GPIOA clock

    GPIOB->CRL &= 0XFFFFFF0F;                                                   //PB1 set as input K1(KEY1)
    GPIOB->CRL |= GPIO_CRL_CNF1_1;

    GPIOB->CRL &= 0XFFFFF0FF;                                                   //PB2 set as input K2(KEY2)
    GPIOB->CRL |= GPIO_CRL_CNF2_1;
    GPIOB->ODR |= GPIO_ODR_ODR2;                                                //PB2 pull-down

    GPIOB->CRH &= 0XFFFF00FF;                                                   //PB10,PB11 set  input ,K3,K4
    GPIOB->CRH |= GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1;
    GPIOB->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR11;                              //PB10,PB11pull-down
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief    Key processing functions
/// @param    mode:0,register supportcontinuous press;1, supportcontinuous press
/// @retval   return  press  key value
////////////////////////////////////////////////////////////////////////////////
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; // press  key  press  flag
    if(mode)key_up = 1; // support continuous press
    if(key_up && ((KEY1 == KEY1_DOWN_VALUE) || (KEY2 == KEY2_DOWN_VALUE) || \
                  (KEY3 == KEY3_DOWN_VALUE) || (KEY4 == KEY4_DOWN_VALUE))) {
        DELAY_Ms(10);
        key_up = 0;
        if(KEY1 == KEY1_DOWN_VALUE)
            return KEY1_PRES;
        else if(KEY2 == KEY2_DOWN_VALUE)
            return KEY2_PRES;
        else if(KEY3 == KEY3_DOWN_VALUE)
            return KEY3_PRES;
        else if(KEY4 == KEY4_DOWN_VALUE)
            return KEY4_PRES;
    }
    else if((KEY1 != KEY1_DOWN_VALUE) && (KEY3 != KEY3_DOWN_VALUE) && \
            (KEY4 != KEY4_DOWN_VALUE) && (KEY2 != KEY2_DOWN_VALUE))
        key_up = 1;
    return 0;                                                                   // none press  key  press down
}


/// @}


/// @}

/// @}
















