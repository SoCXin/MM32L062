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

#define   MINIBOARD

#if defined (MINIBOARD)
#define I2C1_SCL_PORT               GPIOB
#define I2C1_SCL_PIN                GPIO_Pin_8
#define I2C1_SCL_BUSCLK             RCC_AHBPeriph_GPIOB

#define I2C1_SDA_PORT               GPIOB
#define I2C1_SDA_PIN                GPIO_Pin_9
#define I2C1_SDA_BUSCLK             RCC_AHBPeriph_GPIOB
#endif
#if defined (EMINIBOARD)
#define I2C1_SCL_PORT               GPIOB
#define I2C1_SCL_PIN                GPIO_Pin_6
#define I2C1_SCL_BUSCLK             RCC_AHBPeriph_GPIOB

#define I2C1_SDA_PORT               GPIOB
#define I2C1_SDA_PIN                GPIO_Pin_7
#define I2C1_SDA_BUSCLK             RCC_AHBPeriph_GPIOB
#endif

#define SCL_H                       I2C1_SCL_PORT->BSRR = I2C1_SCL_PIN
#define SCL_L                       I2C1_SCL_PORT->BRR  = I2C1_SCL_PIN
#define SCL_read                    (I2C1_SCL_PORT-IDR  & I2C1_SCL_PIN)

#define SDA_H                       I2C1_SDA_PORT->BSRR = I2C1_SDA_PIN
#define SDA_L                       I2C1_SDA_PORT->BRR  = I2C1_SDA_PIN
#define SDA_read                    (I2C1_SDA_PORT->IDR & I2C1_SDA_PIN)


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
extern  u16 gDataCnt;







#endif



#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{


void I2C_GPIO_Config(void);
static void EEPROM_Write(u16 addr, u8* ptr, u16 cnt);
static void EEPROM_Read(u16 addr, u8* ptr, u16 cnt);
static void EEPROM_Writeany(u16 addr, u8* ptr, u16 cnt);
bool EEPROM_ByteWrite(u8 SendByte, u16 WriteAddress, u8 DeviceAddress);
bool EEPROM_SequentialRead(u8* pBuffer, u8 length, u16 ReadAddress, u8 DeviceAddress);

void GPIO_SIM_I2C_Test(void);
void GPIO_SIM_I2C_Init(void);


/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
