////////////////////////////////////////////////////////////////////////////////
/// @file    led.h
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FIRMWARE FUNCTIONS.
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
#ifndef __LED_H
#define __LED_H

// Files includes
#include "hal_conf.h"

//////////////////////////////////////////////////////////////////////////////////
//Development board
//LED Driver code
//////////////////////////////////////////////////////////////////////////////////
#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)    // PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15) // PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))    // PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3) // PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)  // PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))   // PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4) // PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)  // PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))   // PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5) // PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)  // PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))   // PB5




////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Variables
/// @{
#ifdef _LED_C_
#define GLOBAL







#else
#define GLOBAL extern







#endif





#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{




void LED_Init(void);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
