////////////////////////////////////////////////////////////////////////////////
/// @file    adc_tim1trig_dma_multichanit_anychan.c
/// @author  AE TEAM
/// @brief   Using TIMER1 as the trigger source, the default value is reached
///          and the DMA interrupt is generated for sampling.It can be observed
///          through simulation or LED flashing.
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
#define _ADC_TIM1Trig_DMA_MultiChanIT_AnyChan_C_

// Files includes
#include "adc_tim1trig_dma_multichanit_anychan.h"
#include "led.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_TIM1TRIG_DMA_MULTICHANIT_ANYCHAN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

u8 ADCflag = 0;
u32 adc_value = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure sampling GPIO with single conversion mode for ADC1
/// @note   Note the corresponding channel selection.
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
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialization Settings to configure the timing time,
///         Configure timer 1 up count mode.
/// @note   Frequency division and preload value setting according to
///         time requirements.
/// @param  Period: 16 Bit counter overloads the value.
/// @param  Prescaler: Clock predivision value.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Tim1_CC1_init(u16 Prescaler, u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    //Enable clock source
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    //16 Bit counter overloads the value
    TIM_TimeBaseStructure.TIM_Period = Period;
    //Clock predivision value
    TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;
    //Digital filter sampling frequency, does not affect the timer clock
    //Sampling frequency division value
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //Up counting mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_OCStructInit(&TIM_OCInitStructure);
    //Select timer mode
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    //Compare output enablement
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    //Sets the pulse value of the capture comparison register to be loaded
    TIM_OCInitStructure.TIM_Pulse = (Period + 1) / 2;
    //Output polarity setting
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_AnyChanTest(void)
{
    u16 chan, averagenumber = 0;
    ADCch vchan[] = {
        ADCch0,
        ADCch4,
        ADCch5
    };
    ADCCHANNEL_TypeDef anynum[] = {ADC_Channel_4, ADC_Channel_0, ADC_Channel_4, ADC_Channel_0, ADC_Channel_5, ADC_Channel_4};
    //ADCCHANNEL_TypeDef anynum[] = {ADC_Channel_0, ADC_Channel_0, ADC_Channel_4, ADC_Channel_4, ADC_Channel_5, ADC_Channel_5};

    LED_Init();
    DMAInit();
    ADCMultiChannelInit(&vchan[0], 3);
    ADC_AnyChannelConfigInit(&anynum[0], 6);
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    Tim1_CC1_init((u16)(RCC_GetSysClockFreq() / 10000 - 1), 9);
    while(1) {
        if(ADCflag == 1) {
            ADCflag = 0;
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            for(chan = 0; chan < ADCSCANNUM; chan++) {
                varADCavarage[averagenumber][chan] = ADCValue[chan];
            }
            averagenumber++;
            if(averagenumber >= AVERAGELEN) {
                averagenumber = 0;
                ADCTrigFilterflag = 1;
            }
        }
        //Determine whether the second-order filter is over
        if(ADCTrigFilterflag) {
            ADCFilter();
            //Clear the filter end flag
            ADCTrigFilterflag = 0;
            //Convert the filtered value to voltage
            Get_ADCVolatge();

        }

    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the ADC1 single conversion mode to correspond to the PIN
/// @note   It must be careful of the Chip Version.
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
/// @brief  Configure ADC1 single conversion mode¡¢external interrupt source
///         interrupt priority
/// @note   Configure parameters according to requirements.
/// @param  ADC_Channel_x: The sampling channel
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCMultiChannelInit(ADCch* pvchan, u8 number)
{
    u8  i;
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    //Initialize PA1 to analog input mode
    //Enable ADC clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    //ADC prescale factor
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
    //Set ADC mode to continuous conversion mode
    ADC_InitStructure.ADC_Mode = ADC_Mode_Scan;
    //AD data right-justified
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);


    for(i = 0; i < number; i++) {
        if((ADCCHANNEL_TypeDef)(*pvchan) == ADC_Channel_TempSensor) {
            ADC_TempSensorVrefintCmd(ENABLE);
        }
        else if((ADCCHANNEL_TypeDef)(*pvchan) == ADC_Channel_VoltReference) {
            ADC_TempSensorVrefintCmd(ENABLE);
        }
        else {
            ADCConfig(*pvchan);
            //Enable the channel
            ADC_RegularChannelConfig(ADC1, *pvchan, 0, ADC_Samctl_7_5);
        }
        pvchan++;
    }
    //Enable ADCDMA
    ADC_DMACmd(ADC1, ENABLE);
    //Enable AD conversion
    ADC_Cmd(ADC1, ENABLE);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure ADC1 Anychan Channels
/// @note   Configure parameters according to requirements.
/// @param  pvchan: The sampling rank and channel array point
/// @param  number: The max rank number
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_AnyChannelConfigInit(ADCCHANNEL_TypeDef* pvchan, u8 number)
{
    u8  i;
    ADC_ANY_NUM_Config(ADC1, number - 1);
    for(i = 0; i < number; i++) {
        ADC_ANY_CH_Config(ADC1, i, *pvchan);
        pvchan++;
    }
    ADC_ANY_Cmd(ADC1, ENABLE);
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
    DMA_InitStructure.DMA_BufferSize = ADCSCANNUM;
    //After receiving the data, the peripheral address is forbidden to move backward
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
    u16 chan;
    u16 cntFilter;
    u32 lADCFilterValue[ADCSCANNUM] = {0, 0, 0};
    for(chan = 0; chan < ADCSCANNUM; chan++) {
        for(cntFilter = 0; cntFilter < AVERAGELEN; cntFilter++) {
            lADCFilterValue[chan] += varADCavarage[cntFilter][chan];
        }
        ADCFilterValue[chan] = (lADCFilterValue[chan] / 20 * 30 + ADCFilterValue[chan] * 70) / 100;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Convert the filtered value to voltage.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Get_ADCVolatge(void)
{
    u16 chan;
    for(chan = 0; chan < ADCSCANNUM; chan++) {
        ADCVolatge[chan] = (((float)(ADCFilterValue[chan])) / 4095.0) * REFVOLATGE;
    }
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

    //Clear interrupt flag
    DMA_ClearITPendingBit(DMA1_IT_TC1);
    //Erected transmission complete flag
    ADCflag = 1;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  RCC clock Enable to use the GPIO port clock
/// @note   It must be careful of the Chip Version.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Clock_Set(GPIO_TypeDef* GPIOn, FunctionalState state)
{

    if(GPIOn == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, state);
    }
    if(GPIOn == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, state);
    }
    if(GPIOn == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, state);
    }
    if(GPIOn == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, state);
    }
}

/// @}

/// @}

/// @}

