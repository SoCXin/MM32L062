////////////////////////////////////////////////////////////////////////////////
/// @file    adc_awdg.c
/// @author  AE TEAM
/// @brief   In window comparator mode,The transformation results are detected
///          Set the threshold value from 0 to 3V.
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
#define _ADC_AWDG_C_

// Files includes
#include "delay.h"
#include "adc_awdg.h"
#include "led.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_AWDG
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Analog Watch Test.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1SingleAnalogWatchTest(void)
{
    ADC_Config(ADC_Channel_1);
    while(1) {
        ADCVAL = ADC1GetSingleChannelValue();
        if(ADC_flag == 1) {
            fValue = ((float)ADCVAL / 4095) * 3.3;
            ADC_flag = 0;
            LED1_ON();
        }
        else {
            LED1_OFF();
        }
    }

}



////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the ADC1 single conversion mode to correspond to the PIN
/// @note   It must be careful of the different config for each chipset.
/// @param  gpio: The sampling gpio corresponds to the port
/// @param  gpio_pin: The sampling gpio_pin corresponds to the pin.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_PinConfigAnalog(GPIO_TypeDef* gpio, u16 gpio_pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if(gpio == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    }
    if(gpio == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    }
    if(gpio == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    }
    if(gpio == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    }
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  gpio_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(gpio, &GPIO_InitStructure);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure ADC1 single conversion mode, external interrupt source
///         interrupt priority
/// @note   Configure parameters according to requirements.
/// @param  adc_channel_x: The sampling channel
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1SingleChannelInit(ADCCHANNEL_TypeDef adc_channel_x)
{
    ADC_InitTypeDef  ADC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    //Enable ADC clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    //ADC prescale factor
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
    //Set ADC mode to continuous conversion mode
    ADC_InitStructure.ADC_Mode = ADC_Mode_Continue;
    //AD data right-justified
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);
    //Enable the channel
    ADC_RegularChannelConfig(ADC1, adc_channel_x, 0, ADC_Samctl_239_5);

    ADC_AnalogWatchdogCmd(ADC1, ENABLE);
    ADC_AnalogWatchdogThresholdsConfig(ADC1, 3722, 0);
    ADC_AnalogWatchdogSingleChannelConfig(ADC1, adc_channel_x);


    //Enable an external interrupt source
    ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    //Configure interrupt priority

    NVIC_InitStructure.NVIC_IRQChannel = ADC_COMP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure sampling GPIO with single conversion mode for ADC1
/// @note   Note the corresponding channel selection.
/// @param  adc_channel_x: The sampling channel
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_Config(ADCCHANNEL_TypeDef adc_channel)
{
    if( ADC_Channel_0 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_0);
    }
    else if( ADC_Channel_1 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_1);
    }
    else if( ADC_Channel_2 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_2);
    }
    else if( ADC_Channel_3 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_3);
    }
    else if( ADC_Channel_4 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_4);
    }
    else if( ADC_Channel_5 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_5);
    }
    else if( ADC_Channel_6 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_6);
    }
    else if( ADC_Channel_7 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_7);
    }
    else if( ADC_Channel_8 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOB, GPIO_Pin_0);
    }
    else if( ADC_Channel_9 == adc_channel ) {
        GPIO_PinConfigAnalog(GPIOB, GPIO_Pin_1);
    }
    else {
    }
    ADC1SingleChannelInit(adc_channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the ADC1 transform data
/// @note   Note the register configuration if there is no return value.
/// @param  None.
/// @retval Conversion results.
////////////////////////////////////////////////////////////////////////////////
u16 ADC1GetSingleChannelValue(void)
{
    u16 adc_value;
    //Register ADST bit enable, software start conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    //ADC_IT_EOC
    while(ADC_GetFlagStatus(ADC1, ADC_IT_EOC) == 0);
    ADC_ClearFlag(ADC1, ADC_IT_EOC);
    adc_value = ADC_GetConversionValue(ADC1);
    return adc_value;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the ADC1 average converter value
/// @note   None.
/// @param  times:Sampling frequency
/// @retval temp_val / times:Obtain the average value of several ADC1 samples.
////////////////////////////////////////////////////////////////////////////////
u16 ADC1GetValueAverage(u8 times)
{
    u32 temp_val = 0;
    u8 t;
    u8 delay;
    if(times == 0)
        return 0;
    for(t = 0; t < times; t++) {
        temp_val += ADC1GetSingleChannelValue();
        for(delay = 0; delay < 100; delay++);
    }
    return (temp_val / times);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Interrupt service function
/// @note   If error occurs,Simulation to see if you can enter the interrupt
///         function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1_COMP_IRQHandler(void)
{

    if(ADC_GetFlagStatus(ADC1, ADC_IT_AWD) != RESET) {
        ADC_flag = 1;
    }

    ADC_ClearFlag(ADC1, ADC_IT_AWD);
}




/// @}

/// @}

/// @}
