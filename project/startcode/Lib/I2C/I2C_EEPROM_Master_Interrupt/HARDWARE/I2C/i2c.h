////////////////////////////////////////////////////////////////////////////////
/// @file    i2c.h
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
#ifndef __I2C_H
#define __I2C_H

// Files includes

#include "hal_device.h"
#include "hal_conf.h"
#include "stdio.h"


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

//The size of each EEPROM page
#define PAGESIZE 16
//Device address of EEPROM
#define EEPROM_ADDR 0xA8
#define   MINIBOARD

#if defined(MINIBOARD)
#define I2C1_SCL_BUSCLK                  RCC_AHBPeriph_GPIOB
#define I2C1_SCL_PIN                     GPIO_Pin_8
#define I2C1_SCL_PORT                    GPIOB
#define I2C1_SCL_AFSOURCE                GPIO_PinSource8
#define I2C1_SCL_AFMODE                  GPIO_AF_1

#define I2C1_SDA_BUSCLK                  RCC_AHBPeriph_GPIOB
#define I2C1_SDA_PIN                     GPIO_Pin_9
#define I2C1_SDA_PORT                    GPIOB
#define I2C1_SDA_AFSOURCE                GPIO_PinSource9
#define I2C1_SDA_AFMODE                  GPIO_AF_1
#endif
#if defined(EMINIBOARD)
#define I2C1_SCL_BUSCLK                  RCC_AHBPeriph_GPIOB
#define I2C1_SCL_PIN                     GPIO_Pin_6
#define I2C1_SCL_PORT                    GPIOB
#define I2C1_SCL_AFSOURCE                GPIO_PinSource6
#define I2C1_SCL_AFMODE                  GPIO_AF_1

#define I2C1_SDA_BUSCLK                  RCC_AHBPeriph_GPIOB
#define I2C1_SDA_PIN                     GPIO_Pin_7
#define I2C1_SDA_PORT                    GPIOB
#define I2C1_SDA_AFSOURCE                GPIO_PinSource7
#define I2C1_SDA_AFMODE                  GPIO_AF_1
#endif

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
#ifdef _I2C_C_
#define GLOBAL







#else
#define GLOBAL extern






#endif



#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{

static void I2C1_GPIO_Config(void);
static void I2C1_NVIC_Init(void);
static u8 EEPROM_ReadByte(I2C_TypeDef* I2Cx);
static void EEPROM_WriteByte(I2C_TypeDef* I2Cx, u8 data);
static void EEPROM_SendByte(u8 dat);
static void EEPROM_ReadBuff(I2C_TypeDef* I2Cx, u16 mem_byte_addr, u16 rx_count, u8* rx_data );
static u8 EEPROM_WriteBuff(u8 sub, u8* ptr, u16 cnt);
void I2C_MasterModeInit(I2C_TypeDef* I2Cx, u32 uiI2C_speed);
void I2C_SetDeviceAddr(I2C_TypeDef* I2Cx, u8 deviceaddr);
void I2C_NVIC_EepromInit(void);
void I2C_NVIC_EepromTest(void);


/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
