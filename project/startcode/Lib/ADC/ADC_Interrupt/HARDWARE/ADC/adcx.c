////////////////////////////////////////////////////////////////////////////////
/// @file    adcx.c
/// @author  AE TEAM
/// @brief   Output received data.
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
#define _ADCX_C_

// Files includes



#include "main.h"
#include "delay.h"
#include "adcx.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADCX
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the ADC port clock
/// @note   This function should affected by chip version.
/// @param  ADCn : Select ADC port.
/// @param  state : Enable or disable the ADC clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_RCC_ClockSet(ADC_TypeDef* ADCn, FunctionalState state)
{

    if(ADCn == ADC1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);                    //Enable ADC clock
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the GPIO port clock
/// @note   This function should affected by chip version.
/// @param  gpio : Select GPIO port.
/// @param  state : Enable or disable the GPIO clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void GPIO_ClockSet(GPIO_TypeDef* gpio, FunctionalState state)
{
    if(gpio == GPIOA) {
        //CFG_IpClock(emIPCLOCK_GPIOA, state);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, state);
    }
    else if(gpio == GPIOB) {
        //CFG_IpClock(emIPCLOCK_GPIOB, state);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, state);
    }
    else if(gpio == GPIOC) {
        //CFG_IpClock(emIPCLOCK_GPIOC, state);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, state);
    }
    else if(gpio == GPIOD) {
        //CFG_IpClock(emIPCLOCK_GPIOD, state);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, state);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC Pin Config
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCxAssignPin(GPIO_TypeDef* GPIOn, u16 pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_ClockSet(GPIOn, ENABLE);
    GPIO_InitStructure.GPIO_Pin  =  pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           //Output speed
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                               //GPIO mode
    GPIO_Init(GPIOn, &GPIO_InitStructure);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable the selected ADC channel
/// @param  ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param  channel: the ADC channel to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCxChannelEnable(ADC_TypeDef* ADCn, u32 channel)
{
    ADCn->CHSR &= ~(1 << channel);
    ADCn->CHSR |=  (1 << channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Disable the selected ADC channel
/// @param  ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param  channel: the ADC channel to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCxChannelDisable(ADC_TypeDef* ADCn, u32 channel)
{
    ADCn->CHSR &= ~(1 << channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set ADCn sample time.
/// @param  ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param  channel: the ADC channel to configure.
/// @param  sample_time: the ADC Channel n Sample time to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void ADCxSampleTimeConfig(ADC_TypeDef* ADCn, ADCSAM_TypeDef sample_time)
{
    ADCn->CFGR &= ~ADC_CFGR_SAMCTL;
    ADCn->CFGR |= sample_time;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1 Pin Config
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1PinConfigWithParameter(void)
{
    //customer can change below config based Pin assign
    //sample No.1
    ADCxAssignPin(GPIOA, GPIO_Pin_1);
    //ADCxAssignPin(GPIOA, GPIO_Pin_4);
    //ADCxAssignPin(GPIOA, GPIO_Pin_5);
    //sample No.2
    //ADCxAssignPin(GPIOA, GPIO_Pin_4);
    //ADCxAssignPin(GPIOA, GPIO_Pin_5);
    //ADCxAssignPin(GPIOA, GPIO_Pin_6);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable the selected ADC channel
/// @param  ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param  channel: the ADC channel to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1ChannelConfigWithParameter(void)
{
    ADC_TypeDef* ADCn;
    ADCn = ADC1;
    ADCxChannelEnable(ADCn, ADC_Channel_1);
//    ADCxChannelEnable(ADCn,ADC_Channel_1);
//    ADCxChannelEnable(ADCn,ADC_Channel_2);
//    ADCxChannelEnable(ADCn,ADC_Channel_3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial GPIOn initialization configuration
/// @note
/// @param  bound: Baud rate
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1BasicConfigWithParameter(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_TypeDef* ADCn;
    ADCn = ADC1;
    ADC_StructInit(&ADC_InitStructure);

    ADC_RCC_ClockSet(ADCn, ENABLE);                                                //Enable ADC clock

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;                     //ADC prescale factor
    ADC_InitStructure.ADC_Mode = ADC_Mode_Imm;                             //Set ADC mode to continuous conversion mode
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //AD data right-justified
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

    ADC_Init(ADCn, &ADC_InitStructure);
    ADCxSampleTimeConfig(ADCn, ADC_Samctl_13_5);
    ADC_Cmd(ADCn, ENABLE);                                                      //Enable AD conversion
}


//==============================================================================
//ADC interrupt
//==============================================================================
////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC Configure, Applicable to Cortex M0 or M3 processors.
/// @param  NVIC Channel, Priority or SubPriority.
/// @arg    ch: IRQChannel
/// @arg    pri: Priority, Use only Cortex-M3
/// @arg    sub: SubPriority
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void NVIC_Configure(u8 ch, u8 pri, u8 sub)
{

    exNVIC_Init_TypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = ch;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = pri;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = sub;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    exNVIC_Init(&NVIC_InitStruct);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief
/// @param
/// @retval UART IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_ADC_IRQ_Flag(ADC_TypeDef* ADCn)
{
    if      (ADCn == ADC1)      return ADC_COMP_IRQn;//ADC1_IRQn;

    return ADC_COMP_IRQn;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC Nvic initialization
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCxNvicInit(ADC_TypeDef* ADCn, u8 pri, u8 sub)
{
    NVIC_Configure(Get_ADC_IRQ_Flag(ADCn), pri, sub);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1 Nvic initialization
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1NvicInitWithParameters(void)
{
    ADCxNvicInit(ADC1, 0, 1);
}


void ADC1_COMP_IRQHandler(void)
{
    if(RESET != ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
        ADC_SoftwareStartConvCmd(ADC1, DISABLE);//ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
        ADC_Result = 0xFFF & ADC_GetConversionValue(ADC1);
        ADC_Flag = 1;
    }
}
u16 ADC1_SingleChannel_Get(void)
{
    u16 uiADData;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                     //Software start conversion
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_Flag = 0;
    while(1) {
        if(1 == ADC_Flag) {
            uiADData = (u16)ADC_Result;
            ADC_Flag = 0;
            break;
        }
    }
    //ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    return uiADData;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is getting the average of ADC
/// @note   None.
/// @param  times.
/// @retval average.
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
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1SingleChannelTest(void)
{
    printf("ADC1_CH1 test\r\n");
    ADC1BasicConfigWithParameter();
    ADC1ChannelConfigWithParameter();
    ADC1PinConfigWithParameter();
    ADC1NvicInitWithParameters();
    while(1) {
        ADCVAL = Get_Adc_Average(5);
        fValue = ((float)ADCVAL / 4095) * 3.3; //use 3.3V as VDD
        //fValue = (((float)ADCVAL)/4095)*5; //use 5V as VDD
        printf("ADC1_CH_1=%fV\r\n", fValue);
        DELAY_Ms(200);
    }
}

/// @}


/// @}

/// @}


