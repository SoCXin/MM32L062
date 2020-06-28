////////////////////////////////////////////////////////////////////////////////
/// @file    adcx.h
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
#ifndef __ADCX_H
#define __ADCX_H

// Files includes

#include <string.h>
#include "mm32_device.h"
#include <stdio.h>

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
typedef s32 (*PtrFun)();

////////////////////////////////////////////////////////////////////////////////
/// @brief ADC GPIO enumerate definition.
/// @anchor EM_ADC_PORT
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    emADC1 = 0,
    emADC2 = 1,
} EM_ADC_PORT;


////////////////////////////////////////////////////////////////////////////////
/// @brief ADC enumerate definition.
/// @anchor ADCch
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    ADC1ch0                 = (emADC1 << 8) | (ADC_Channel_0),
    ADC1ch1                 = (emADC1 << 8) | (ADC_Channel_1),
    ADC1ch2                 = (emADC1 << 8) | (ADC_Channel_2),
    ADC1ch3                 = (emADC1 << 8) | (ADC_Channel_3),
    ADC1ch4                 = (emADC1 << 8) | (ADC_Channel_4),
    ADC1ch5                 = (emADC1 << 8) | (ADC_Channel_5),
    ADC1ch6                 = (emADC1 << 8) | (ADC_Channel_6),
    ADC1ch7                 = (emADC1 << 8) | (ADC_Channel_7),

    ADC1ch8                 = (emADC1 << 8) | (ADC_Channel_8),
    ADC1ch9                 = (emADC1 << 8) | (ADC_Channel_9),
    ADCxchTemp              = (emADC1 << 8) | (ADC_Channel_TempSensor),         ///< Temperature sensor(ADC1) or internal
    ADCxchVref              = (emADC1 << 8) | (ADC_Channel_VoltReference),      ///< Internal reference voltage(ADC1) channel



} emADCch;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Variables
/// @{
#ifdef _ADCX_C_
#define GLOBAL




GLOBAL u32 ADC_Result = 0;
GLOBAL u16 ADC_Flag = 0;


#else
#define GLOBAL extern



GLOBAL u32 ADC_Result;
GLOBAL u16 ADC_Flag;



#endif




#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{




void ADC1BasicConfigWithParameter(void);
void ADC1ChannelConfigWithParameter(void);
void ADC1PinConfigWithParameter(void);
void ADC1NvicInitWithParameters(void);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
