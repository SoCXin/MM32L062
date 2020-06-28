////////////////////////////////////////////////////////////////////////////////
/// @file    adc_dma_vref.h
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
#ifndef __ADC_DMA_Vref_H
#define __ADC_DMA_Vref_H

// Files includes

#include "hal_device.h"
#include "hal_conf.h"
#include "stdio.h"
#include "string.h"
//#include "drv.h"


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



//Reference voltage, the unit is: V
#define REFVOLATGE 3.3

// Several sets of channel data are collected
#define ADCSCANNUM               3
/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Enumeration
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief XXXX enumerate definition.
/// @anchor XXXX
////////////////////////////////////////////////////////////////////////////////


typedef enum {
    ADCch0                 = ADC_Channel_0,
    ADCch1                 = ADC_Channel_1,
    ADCch2                 = ADC_Channel_2,
    ADCch3                 = ADC_Channel_3,
    ADCch4                 = ADC_Channel_4,
    ADCch5                 = ADC_Channel_5,
    ADCch6                 = ADC_Channel_6,
    ADCch7                 = ADC_Channel_7,
    ADCch8                 = ADC_Channel_8,
    ADCch9                 = ADC_Channel_9,
    ADCchTemp              = ADC_Channel_TempSensor,
    ADCchVref              = ADC_Channel_VoltReference,
} ADCch;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Variables
/// @{
#ifdef _ADC_DMA_Vref_C_
#define GLOBAL







#else
#define GLOBAL extern







#endif

GLOBAL __IO  u32 TimingDelay;
GLOBAL __IO    u32 CRCValue;
GLOBAL u16 ADCVAL;
GLOBAL u16 ADC_flag;
GLOBAL float fValue;
GLOBAL u16 ADCValue[20];
GLOBAL float ADCVolatge[3];
GLOBAL u16 ADCFilterValue;
GLOBAL u16 ADCFilterflag;
GLOBAL u8 ADCTrigFilterflag;
GLOBAL float VDDA_Volatge;
#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{





void ADCConfig(ADCch ADC_Channel);
void EXTIX_Init(void);
u16 Get_Adc_Average(u8 times);
u16 ADC1_SingleChannel_Get(void);

void GPIO_PinConfigAnalog(GPIO_TypeDef* GPIOn, u16 gpio_pin);
void ADCSingleChannelInit(ADCch ADC_Channel_x);
void DMAInit(void);
void ADCFilter(void);
void Get_ADCVolatge(void);
void Get_VDDA_Volatge(void);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
