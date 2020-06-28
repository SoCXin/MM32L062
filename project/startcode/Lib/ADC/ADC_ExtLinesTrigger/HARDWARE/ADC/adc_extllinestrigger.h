////////////////////////////////////////////////////////////////////////////////
/// @file    adc_extllinestrigger.h
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
#ifndef __ADC_ExtllinesTrigger_C
#define __ADC_ExtllinesTrigger_C

// Files includes

#include "hal_device.h"
#include "hal_conf.h"
#include "stdio.h"
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




/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Enumeration
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief XXXX enumerate definition.
/// @anchor XXXX
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// @brief ADC_Channels
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
#ifdef _ADC_ExtllinesTrigger_C_
#define GLOBAL







#else
#define GLOBAL extern







#endif

GLOBAL __IO  u32 TimingDelay;
GLOBAL __IO    u32 CRCValue;
GLOBAL u16 ADCVAL;
GLOBAL u16 ADC_flag;
GLOBAL float fValue;

#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{





void ADCConfig(ADCch ADC_Channel);
void EXTIX_Init(void);
u16 Get_Adc_Average(u8 times);


/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
