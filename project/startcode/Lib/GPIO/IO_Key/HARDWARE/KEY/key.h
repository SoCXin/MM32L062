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
#include <string.h>

#include "mm32_device.h"



#include "hal_conf.h"


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


#define KEY1_GPIO_Port                  GPIOC
#define KEY1_Pin                        GPIO_Pin_13
#define KEY2_GPIO_Port                  GPIOA
#define KEY2_Pin                        GPIO_Pin_0
#define KEY3_GPIO_Port                  GPIOB
#define KEY3_Pin                        GPIO_Pin_10
#define KEY4_GPIO_Port                  GPIOB
#define KEY4_Pin                        GPIO_Pin_11
#define WK_UP_GPIO_Port                 GPIOA
#define WK_UP_Pin                       GPIO_Pin_0

#define KEY1_DOWN_VALUE                 0                                               //KEY1
#define KEY2_DOWN_VALUE                 1                                               //KEY2
#define KEY3_DOWN_VALUE                 0                                               //KEY3
#define KEY4_DOWN_VALUE                 0                                               //KEY4
#define WKUP_DOWN_VALUE                 1                                               //WK_UP

#endif

#if defined(EMINIBOARD)

#define KEY1_GPIO_Port                  GPIOB
#define KEY1_Pin                        GPIO_Pin_1
#define KEY2_GPIO_Port                  GPIOB
#define KEY2_Pin                        GPIO_Pin_2
#define KEY3_GPIO_Port                  GPIOB
#define KEY3_Pin                        GPIO_Pin_10
#define KEY4_GPIO_Port                  GPIOB
#define KEY4_Pin                        GPIO_Pin_11
#define WK_UP_GPIO_Port                 GPIOA
#define WK_UP_Pin                       GPIO_Pin_0

#define KEY1_DOWN_VALUE                 1                                               //KEY1
#define KEY2_DOWN_VALUE                 0                                               //KEY2
#define KEY3_DOWN_VALUE                 0                                               //KEY3
#define KEY4_DOWN_VALUE                 0                                               //KEY4
#define WKUP_DOWN_VALUE                 1                                               //WK_UP
#endif

#define KEY1                    GPIO_ReadInputDataBit(KEY1_GPIO_Port,KEY1_Pin)  //read key1
#define KEY2                    GPIO_ReadInputDataBit(KEY2_GPIO_Port,KEY2_Pin)  //read key2
#define KEY3                    GPIO_ReadInputDataBit(KEY3_GPIO_Port,KEY3_Pin)  //read key3
#define KEY4                    GPIO_ReadInputDataBit(KEY4_GPIO_Port,KEY4_Pin)  //read key4
#define WK_UP                   GPIO_ReadInputDataBit(WK_UP_GPIO_Port,WK_UP_Pin)//read wk_up

#define KEY1_PRES               1                                               //KEY1
#define KEY2_PRES               2                                               //KEY2
#define KEY3_PRES               3                                               //KEY3
#define KEY4_PRES               4                                               //KEY4
#define WKUP_PRES               2                                               //WK_UP

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Enumeration
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief XXXX enumerate definition.
/// @anchor XXXX
////////////////////////////////////////////////////////////////////////////////



/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Variables
/// @{
#ifdef _KEY_C_
#define GLOBAL







#else
#define GLOBAL extern







#endif





#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{

void KEY_Init(void);
void KEY_Test(void);
u8 KEY_Scan(u8 mode);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
