////////////////////////////////////////////////////////////////////////////////
/// @file    main.h
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
#ifndef __MAIN_H
#define __MAIN_H

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
#ifdef _MAIN_C_
#define GLOBAL







#else
#define GLOBAL extern







#endif

#define SectorErase           0x20
#define PP                    0x02
#define ReadData              0x03
#define ChipErase             0xC7
#define RDSR                  0x05
#define Dummy_Byte            0x00
#define W25X_BUSY             0
#define W25X_NotBUSY          1
#define FlashSize             0x400
#define ReadStatusReg         0x05


#define READ                  0x03
#define FAST_READ             0x0B
#define RDID                  0x9F
#define WREN                  0x06
#define WRDI                  0x04
#define SE                    0xD8
#define BE                    0xC7
#define PP                    0x02
#define RDSR                  0x05
#define WRSR                  0x01
#define DP                    0xB9
#define RES                   0xAB
#define GPIO_Pin_0_2  GPIO_Pin_2



#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{


void GPIO_Clock_Set(GPIO_TypeDef* GPIOx, FunctionalState NewState);
void LED_Init(void);
void WKUP_STOP_Init(void);

void uart_nvic_init(u32 bound);
static void deleyNop(u32 DlyTime);
/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
