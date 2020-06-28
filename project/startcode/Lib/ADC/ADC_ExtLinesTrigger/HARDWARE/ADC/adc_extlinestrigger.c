////////////////////////////////////////////////////////////////////////////////
/// @file     adc_extlinestrigger.c
/// @author   AE TEAM
/// @brief    Use EXTI_Line11 (PB11) as the external trigger source for AD sampling
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
#define _ADC_ExtllinesTrigger_C_

// Files includes
#include "adc_extllinestrigger.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_EXTINESTRIGGER
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is ADC ExtLines Trigger test entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_ExtLinesTrigger(void)
{
    EXTIX_Init();
    ADCConfig(ADCch0);
    while(1) {
        if(ADC_flag == 1) {
            //use 3.3V as VDD
            ADC_flag = 0;
            fValue = ((float)ADCVAL / 4095) * 3.3;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the ADC1 single conversion mode to correspond to the PIN
/// @note    It must be careful of the Chip Version.
/// @param  GPIOn: The sampling GPIOn corresponds to the port
/// @param  gpio_pin: The sampling gpio_pin corresponds to the pin.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_PinConfigAnalog(GPIO_TypeDef* GPIOn, u16 gpio_pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if(GPIOn == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    }
    if(GPIOn == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    }
    if(GPIOn == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    }
    if(GPIOn == GPIOD) {
        //GPIO clock starts
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    }
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  gpio_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOn, &GPIO_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure ADC1 single conversion mode¡¢external interrupt source
///         interrupt priority
/// @note   Configure parameters according to requirements.
/// @param  ADC_Channel_x: The sampling channel
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCSingleChannelInit(ADCch ADC_Channel_x)
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
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_EXTI_11;
    ADC_Init(ADC1, &ADC_InitStructure);
    //Enable the channel
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, ADC_Samctl_239_5);


    //Enable an external interrupt source
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC1_ExternalTrigConv_EXTI_11);
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    // Configure interrupt priority

    NVIC_InitStructure.NVIC_IRQChannel = ADC_COMP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure sampling GPIO with single conversion mode for ADC1
/// @note    Note the corresponding channel selection.
/// @param  ADC_Channel_x: The sampling channel
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCConfig(ADCch ADC_Channel)
{
    if( ADCch0 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_0);
    }
    else if( ADCch1 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_1);
    }
    else if( ADCch2 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_2);
    }
    else if( ADCch3 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_3);
    }
    else if( ADCch4 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_4);
    }
    else if( ADCch5 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_5);
    }
    else if( ADCch6 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_6);
    }
    else if( ADCch7 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOA, GPIO_Pin_7);
    }
    else if( ADCch8 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOB, GPIO_Pin_0);
    }
    else if( ADCch9 == ADC_Channel ) {
        GPIO_PinConfigAnalog(GPIOB, GPIO_Pin_1);
    }
    else {
    }
    ADCSingleChannelInit(ADC_Channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the ADC1 transform data
/// @note   If no return value is generated, note the register configuration.
/// @param  None.
/// @retval puiADData:Conversion results.
////////////////////////////////////////////////////////////////////////////////
u16 ADC1_SingleChannel_Get(void)
{
    u16 puiADData;
    //Register ADST bit enable, software start conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    //ADC_IT_EOC
    while(ADC_GetFlagStatus(ADC1, ADC_IT_EOC) == 0);
    ADC_ClearFlag(ADC1, ADC_IT_EOC);
    puiADData = ADC_GetConversionValue(ADC1);
    return puiADData;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the ADC1 transform data
/// @note   None.
/// @param  times:Sampling frequency
/// @retval temp_val / times:Obtain the average value of several ADC1 samples.
////////////////////////////////////////////////////////////////////////////////
u16 Get_Adc_Average(u8 times)
{
    u32 temp_val = 0;
    u8 t;
    u8 delay;
    for(t = 0; t < times; t++) {
        temp_val += ADC1_SingleChannel_Get();
        for(delay = 0; delay < 100; delay++);
    }
    return temp_val / times;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  External trigger source configuration
/// @note   The IO is set to input mode
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    // Set to pull-up input
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIOB.11
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    //Falling edge trigger
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
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

    if(ADC_GetFlagStatus(ADC1, ADC_IT_EOC) != RESET) {

        ADCVAL = Get_Adc_Average(5);
        ADC_flag = 1;
    }

    ADC_ClearFlag(ADC1, ADC_IT_EOC);
}

/// @}

/// @}

/// @}

