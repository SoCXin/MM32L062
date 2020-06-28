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
//////////////////////////////////////////////////////////////////////////////////
//Development board
//Key-driven code
//////////////////////////////////////////////////////////////////////////////////
#define MINIBOARD

#if defined (MINIBOARD)
#define KEY1    ((GPIOC->IDR&0x2000)?1:0)  //input //Read button1  Read exit value 1 is 1 otherwise 0
#define WK_UP   ((GPIOA->IDR&0x1)?1:0)  //input //Read button2 
#endif
#if defined (EMINIBOARD)
#define KEY1    ((GPIOB->IDR&0x2)?1:0)  //input //Read button1  Read exit value 1 is 1 otherwise 0
#define WK_UP   ((GPIOB->IDR&0x4)?1:0)  //input //Read button2 
#endif
#define KEY3    ((GPIOB->IDR&0x400)?1:0)  //input //Read button3
#define KEY4    ((GPIOB->IDR&0x800)?1:0) //input //Read button4

#define KEY1_PRES   1       //KEY1 
#define WKUP_PRES   2       //WK_UP  
#define KEY3_PRES   3       //KEY3 
#define KEY4_PRES   4       //KEY4 
void KEY_Init(void);
u8 GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
u8 KEY_Scan(u8 mode);
/// @}

/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
