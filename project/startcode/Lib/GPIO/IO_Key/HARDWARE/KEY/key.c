////////////////////////////////////////////////////////////////////////////////
/// @file    key.c
/// @author  AE TEAM
/// @brief   key intput.
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
#include "led.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY_Exported_Functions
/// @{



////////////////////////////////////////////////////////////////////////////////
/// @brief  Keyboard Scanning
/// @note   None.
/// @param  None.
/// @retval Keyboard value.
////////////////////////////////////////////////////////////////////////////////
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1;
    if(mode)
        key_up = 1;
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
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize KEY GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful, can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE);
#if defined(MINIBOARD)
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = KEY1_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = KEY2_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStructure);
#endif
#if defined(EMINIBOARD)
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = KEY1_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = KEY2_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStructure);
#endif
    GPIO_InitStructure.GPIO_Pin  = KEY3_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY3_GPIO_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = KEY4_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY4_GPIO_Port, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is key test.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void KEY_Test(void)
{
    u8 t = 0;
    while(1) {
        t = KEY_Scan(0);
        if(KEY1_PRES == t) {
            LED1_TOGGLE();
        }
        else if(KEY2_PRES == t) {
            LED2_TOGGLE();
        }
        else if(KEY3_PRES == t) {
            LED3_TOGGLE();
        }
        else if(KEY4_PRES == t) {
            LED4_TOGGLE();
        }
        else {
            DELAY_Ms(10);
        }
    }
}

/// @}


/// @}

/// @}

