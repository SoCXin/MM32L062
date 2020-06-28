////////////////////////////////////////////////////////////////////////////////
/// @file     adc.c
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _ADC_C_

// Files includes
#include "adc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Constants
/// @{

/////////////////////////////////////////////////////////////////////////////////
/// @brief   configure ADC1single transform  mode
/// @param   None
/// @retval  None
/////////////////////////////////////////////////////////////////////////////////
void ADC1_SingleChannel(void)
{
    RCC->AHBENR |= 1 << 17; //enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //enableADC1clock
    GPIOA->CRL &= 0XFFFFFF0F; //PA1 anolog input

    RCC->APB2RSTR |= RCC_APB2RSTR_ADC1RST; //ADC1reset
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1RST); //reset   end

    //ADC configure single soft trigger  transform mode
    ADC1->ADCFG |= ADCFG_ADCPRE_8 | ADCFG_ADCPRE_10; //8 fractional frequency
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD | ADCR_ADMD_CONTINUE | ADCR_ALIGN_LEFT); //singleregister  mode , Data right-justified
    ADC1->ADCR |= ADCR_ADMD_PERIOD;
    ADC1->ADCHS = ADCHS_CHEN1; //enable channel 1

    ADC1->ADCFG |= ADCFG_ADEN;//ADC1enable
    ADC1->ADCR |= ADCR_ADST;//Start Conversion
}
/////////////////////////////////////////////////////////////////////////////////
/// @brief    getADC1 transform data
/// @param    None
/// @retval   None
/////////////////////////////////////////////////////////////////////////////////
u16 ADC1_SingleChannel_Get(void)
{
    u16 puiADData;
    //ADCR deviceADSTbit enable, soft start  transform
    ADC1->ADCR |= 0x00000100;
    while(((ADC1->ADSTA ) & 0x01) == 0);

    ADC1->ADSTA |= 0x01;
    puiADData = ADC1->ADDATA & 0xfff;
    return puiADData;
}
/// @}


/// @}

/// @}
