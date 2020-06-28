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
#include "mm32_device.h"


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Example_Layer
/// @brief MM32 Example Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_RESOURCE
/// @brief MM32 Examples resource modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Constants
/// @{


#define LED1_ON()     GPIOA->BRR=0x8000;// PA15
#define LED1_OFF()    GPIOA->BSRR = 0x8000// PA15
#define LED1_TOGGLE()  (GPIOA->ODR&0x8000)?(GPIOA->BRR=0x8000):(GPIOA->BSRR = 0x8000)


#define LED2_ON()   GPIOB->BRR=0x0008// PB3
#define LED2_OFF()  GPIOB->BSRR = 0x0008;// PB3
#define LED2_TOGGLE()  (GPIOB->ODR&0x0008)?(GPIOB->BRR=0x0008):(GPIOB->BSRR = 0x0008)

#define LED3_ON()   GPIOB->BRR=0x0010// PB4
#define LED3_OFF()  GPIOB->BSRR = 0x0010// PB4
#define LED3_TOGGLE()  (GPIOB->ODR&0x0010)?(GPIOB->BRR=0x0010):(GPIOB->BSRR = 0x0010)

#define LED4_ON()   GPIOB->BRR=0x0020// PB5
#define LED4_OFF()  GPIOB->BSRR=0x0020// PB5
#define LED4_TOGGLE()  (GPIOB->ODR&0x0020)?(GPIOB->BRR=0x0020):(GPIOB->BSRR = 0x0020)

void LED_Init(void);


/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////

