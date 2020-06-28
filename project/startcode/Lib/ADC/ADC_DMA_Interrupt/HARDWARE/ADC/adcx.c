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



#include "hal_exti.h"
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
/// @param  ADCn: select the ADC peripheral.
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
/// @param  ADCn:  select the ADC peripheral.
/// @param  channel: the ADC channel to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCxChannelDisable(ADC_TypeDef* ADCn, u32 channel)
{
    ADCn->CHSR &= ~(1 << channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set ADCn sample time.
/// @param  ADCn: select the ADC peripheral.
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
/// @param  ADCn: select the ADC peripheral.
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
    ADC_InitStructure.ADC_Mode = ADC_Mode_Continue;                             //Set ADC mode to continuous conversion mode
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //AD data right-justified
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

    ADC_Init(ADCn, &ADC_InitStructure);
    ADCxSampleTimeConfig(ADCn, ADC_Samctl_13_5);
    ADC_Cmd(ADCn, ENABLE);                                                      //Enable AD conversion
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
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    }
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  gpio_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOn, &GPIO_InitStructure);
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
        ADCSingleChannelInit(ADCch0);
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
/// @brief  Configure ADC1 single conversion mode,external interrupt source
///         interrupt priority
/// @note   Configure parameters according to requirements.
/// @param  ADC_Channel_x: The sampling channel
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCSingleChannelInit(ADCch ADC_Channel_x)
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    //Initialize PA1 to analog input mode
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
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, ADC_Samctl_239_5);

    //Enable ADCDMA
    ADC_DMACmd(ADC1, ENABLE);
    //Enable AD conversion
    ADC_Cmd(ADC1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DMA and interrupt priority configuration
/// @note   Configure parameters according to requirements.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMAInit(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1);
    DMA_StructInit(&DMA_InitStructure);
    //DMA transfer peripheral address
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (ADC1->DR);
    //DMA transfer memory address
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCValue;
    //DMA transfer direction from peripheral to memory
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //DMA cache size
    DMA_InitStructure.DMA_BufferSize = 20;
    //After receiving the data, the peripheral address is forbidden to move
    //backward
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //After receiving the data, the memory address is shifted backward
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //Define the peripheral data width to 16 bits
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    //Define the memory data width to 16 bits
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //Cycle conversion mode
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //DMA priority is high
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //M2M mode is disabled
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    //DMA interrupt initialization
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Process values read from DMA,ADC second-order filter,
///         when the filter is completed erection filter end marker.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCFilter(void)
{
    static u16 cntFilter;
    static u32 lADCFilterValue = 0;
    for(cntFilter = 0; cntFilter < 20; cntFilter++) {
        lADCFilterValue += ADCValue[cntFilter];
    }
    cntFilter = 0;

    ADCFilterflag = 1;
    ADCFilterValue = (lADCFilterValue / 20 * 30 + ADCFilterValue * 70) / 100;
    lADCFilterValue = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Transform the optimized data to get the final result.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Get_ADCVolatge(void)
{
    ADCVolatge = ((float)ADCFilterValue / 4095) * 3.3;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  When DMA send data, set ADCflag, PA8, clear the interrupt flag,
///         stop the conversion.
/// @note   If error occurs,Simulation to see if you can enter the interrupt
///         function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel1_IRQHandler(void)
{
    //Stop Conversion
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    //Clear interrupt flag
    DMA_ClearITPendingBit(DMA1_IT_TC1);

    ADCFilter();
}




/// @}


/// @}

/// @}


