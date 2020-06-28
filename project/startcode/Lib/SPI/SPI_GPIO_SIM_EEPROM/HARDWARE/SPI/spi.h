////////////////////////////////////////////////////////////////////////////////
/// @file    spi.h
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
#ifndef __SPI_H
#define __SPI_H

// Files includes

#include "hal_device.h"
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
#ifdef _BKP_C_
#define GLOBAL







#else
#define GLOBAL extern


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
#define CS_GPIOx              GPIOB
#define CS_Pin_x              GPIO_Pin_12
#define RCC_CS                RCC_AHBPeriph_GPIOB

#define CLK_GPIOx             GPIOB
#define CLK_Pin_x             GPIO_Pin_13
#define RCC_CLK               RCC_AHBPeriph_GPIOB

#define MOSI_GPIOx            GPIOB
#define MOSI_Pin_x            GPIO_Pin_15
#define RCC_MOSI              RCC_AHBPeriph_GPIOB


#define MISO_GPIOx            GPIOB
#define MISO_Pin_x            GPIO_Pin_14
#define RCC_MISO              RCC_AHBPeriph_GPIOB


#define MSB   1                                                                 // Open this MSB==1 set MSB enable, otherwise LSB
#define BITLENGTH   8                                                           //bits 4~16
#define SPI_MODE 3

#if    (SPI_MODE == 0)                                                          //(SPI_MODE == 1)
#define CPOL 0                                                                  // Open this CPOL=0
#define CPHA 0                                                                  // Open this CPHA=0
#elif  (SPI_MODE == 1)                                                          //(SPI_MODE == 1)
#define CPOL 1                                                                  // Open this CPOL=1
#define CPHA 0                                                                  // Open this CPHA=0
#elif  (SPI_MODE == 2)                                                          //(SPI_MODE == 2)
#define CPOL 0                                                                  // Open this CPOL=0
#define CPHA_1                                                                  // Open this CPHA=1
#else                                                                           //(SPI_MODE == 3)
#define CPOL 1                                                                  // Open this CPOL=1
#define CPHA 1                                                                  // Open this CPHA=1
#endif


//----------------- User Config -----------------//
#define CS_L           {CS_GPIOx->BRR = CS_Pin_x;}                              //GPIO_ResetBits(CS_GPIOx,CS_Pin_x)
#define CS_H           {CS_GPIOx->BSRR = CS_Pin_x;}                             //GPIO_SetBits(CS_GPIOx,CS_Pin_x)
#define SCK_L          {CLK_GPIOx->BRR = CLK_Pin_x;}                            //GPIO_ResetBits(CLK_GPIOx,CLK_Pin_x)
#define SCK_H          {CLK_GPIOx->BSRR = CLK_Pin_x;}                           //GPIO_SetBits(CLK_GPIOx,CLK_Pin_x)
#define MOSI_L         {MOSI_GPIOx->BRR = MOSI_Pin_x;}                          //GPIO_ResetBits(MOSI_GPIOx,MOSI_Pin_x)
#define MOSI_H         {MOSI_GPIOx->BSRR = MOSI_Pin_x;}                         //GPIO_SetBits(MOSI_GPIOx,MOSI_Pin_x)
#define MISO_IN        (MISO_GPIOx->IDR&MISO_Pin_x)                             //GPIO_ReadInputData(MISO_GPIOx,MISO_Pin_x)







#endif



#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{


u8 W25xx_ReadWrite8Bits(u8 );
static void W25xx_CS_Low(void);
static void W25xx_CS_High(void);
void SPI_Stop(void);
void GPIO_SIM_SPI_Test(void);
void GPIO_SIM_SPI_Init(void);


/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
