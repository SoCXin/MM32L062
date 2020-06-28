////////////////////////////////////////////////////////////////////////////////
/// @file    key.h
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
#ifndef __KEY_H
#define __KEY_H

// Files includes
#include "sys.h"
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
#define   MINIBOARD

#if defined(MINIBOARD)
#define KEY1                            ((GPIOC->IDR&0x2000)?1:0)               //Read button1
#define KEY2                            ((GPIOA->IDR&0x0001)?1:0)               //Read button2 
#define KEY3                            ((GPIOB->IDR&0x0400)?1:0)               //Read button3
#define KEY4                            ((GPIOB->IDR&0x0800)?1:0)               //Read button4
#define WK_UP                           ((GPIOA->IDR&0x0001)?1:0)               //Read buttonwkup 
#define KEY1_PRES                       1
#define KEY2_PRES                       2
#define KEY3_PRES                       3
#define KEY4_PRES                       4
#define WKUP_PRES                       2
#define KEY1_DOWN_VALUE                 0
#define KEY2_DOWN_VALUE                 1
#define KEY3_DOWN_VALUE                 0
#define KEY4_DOWN_VALUE                 0
#define WKUP_DOWN_VALUE                 1
#endif

#if defined(EMINIBOARD)

#define KEY1                            ((GPIOB->IDR&0x0002)?1:0)               //Read button1
#define KEY2                            ((GPIOB->IDR&0x0004)?1:0)               //Read button2 
#define KEY3                            ((GPIOB->IDR&0x0400)?1:0)               //Read button3
#define KEY4                            ((GPIOB->IDR&0x0800)?1:0)               //Read button4

#define WK_UP                           ((GPIOA->IDR&0x0001)?1:0)               //Read buttonwkup 

#define KEY1_PRES                       1
#define KEY2_PRES                       2
#define KEY3_PRES                       3
#define KEY4_PRES                       4

#define WKUP_PRES                       2


#define KEY1_DOWN_VALUE                 1
#define KEY2_DOWN_VALUE                 0
#define KEY3_DOWN_VALUE                 0
#define KEY4_DOWN_VALUE                 0
#define WKUP_DOWN_VALUE                 1
#endif

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
