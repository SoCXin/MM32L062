////////////////////////////////////////////////////////////////////////////////
/// @file     hw_config.C
/// @author   AE TEAM
/// @brief    Hardware Configuration & Setup.
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
#define _HW_CONFIG_C_
// Define to prevent recursive inclusion

//Includes
//#include "MM32L0xxs.h"
//#include "platform_config.h"
//#include "hw_config.h"
//#include "usb_lib.h"
//#include "usb_desc.h"
//#include "usb_pwr.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "hal_rcc.h"
#include "hal_misc.h"
#include "hal_exti.h"
#include "hal_gpio.h"
#include "hal_adc.h"
#include "hal_uart.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

extern u8 gRxBuff[VIRTUAL_COM_PORT_DATA_SIZE];
extern u32 gRxDataCnt;
extern LINE_CODING linecoding;


void USB_HSI48M_Config(void)
{
    u8 temp = 0;
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));
    RCC->CFGR = RCC_CFGR_PPRE1_2;
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;
    RCC->CR &= ~(RCC_CR_PLLON);
    RCC->CR &= ~(0x1f << 26);
    RCC->CR |= (4 - 1) << 26;
    FLASH->ACR = FLASH_ACR_LATENCY_1 | FLASH_ACR_PRFTBE;
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while(temp != 0x02) {
        temp = RCC->CFGR >> 2;
        temp &= 0x03;
    }
}

void Set_CRS()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CRS, ENABLE);
    CRS->CFGR &= ~0xffff;
    CRS->CFGR |= 0xBB7F;
    // Select USB SOF as synchronization source
    //  CRS_SynchronizationSourceConfig(CRS_SYNCSource_USB);
    CRS->CFGR &= 0xcfffffff; //
    //Enables the automatic hardware adjustment of TRIM bits: AUTOTRIMEN:
    CRS->CFGR |= 0x2 << 28; //GPIO
    CRS->CR |= 0x40;        //autoen
    //Enables the oscillator clock for frequency error counter CEN
    CRS->CR |= 0x20; //cen
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures USB Clock input (48MHz)..
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_ClockConfig(void)
{
//  Set_CRS(); // Internal vibration calibration
    USB_HSI48M_Config();
    //Select USBCLK source
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);
    //Enable USB clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Power-off system clocks and power while entering suspend mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Enter_LowPowerMode(void)
{
    // Set the device state to suspend
    bDeviceState = SUSPENDED;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Restores system clocks and power while exiting suspend mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Leave_LowPowerMode(void)
{
    DEVICE_INFO* pInfo = &Device_Info;

    // Set the device state to the correct state
    if (pInfo->Current_Configuration != 0) {
        //Device configured
        bDeviceState = CONFIGURED;
    }
    else {
        bDeviceState = ATTACHED;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the USB interrupts.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USB_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Software Connection/Disconnection of USB Cable.
/// @param  NewState: new state.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_Cable_Config (FunctionalState NewState)
{
    if (NewState != DISABLE) {
        _SetUSB_TOP(USB_TOP_CONNECT);
    }
    else {
        _ClrUSB_TOP(USB_TOP_CONNECT);
    }
}

////////////////////////////////////////////////////////////////////////////////
//Function Name : EXTI_Configuration.
//Description   : Configure the EXTI lines for Key and Tamper push buttons.
//Input         : None.
//Output        : None.
//Return value  : The direction value.
////////////////////////////////////////////////////////////////////////////////
void EXTI_Configuration(void)
{
//  EXTI_InitTypeDef EXTI_InitStructure;

    // Enable the AFIO Clock
    //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   mm32

    // Connect Key1 to the correspondent EXTI line
    //  GPIO_EXTILineConfig(GPIO_KEY1_PORTSOURCE, GPIO_KEY1_PINSOURCE);
    //    GPIO_EXTILineConfig(GPIO_KEY1_PORTSOURCE, GPIO_KEY1_PINSOURCE);

//  // Configure Key1 EXTI line to generate an interrupt on rising & falling edges
//  EXTI_InitStructure.EXTI_Line = GPIO_KEY1_EXTI_Line;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);

//  // Clear the Key1 EXTI line pending bit
//  EXTI_ClearITPendingBit(GPIO_KEY1_EXTI_Line);

//  // Connect Key2 to the correspondent EXTI line
//  //  GPIO_EXTILineConfig(GPIO_KEY2_PORTSOURCE, GPIO_KEY2_PINSOURCE);

//  // Configure Key2 EXTI line to generate an interrupt on rising & falling edges
//  EXTI_InitStructure.EXTI_Line = GPIO_KEY2_EXTI_Line;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);

//  // Clear the Key1 EXTI line pending bit
//  EXTI_ClearITPendingBit(GPIO_KEY2_EXTI_Line);

//  // Connect Tamper puch button to the correspondent EXTI line
//  //  GPIO_EXTILineConfig(GPIO_TAMPER_PORTSOURCE, GPIO_TAMPER_PINSOURCE);

//  // Configure Tamper EXTI Line to generate an interrupt rising & falling edges
//  EXTI_InitStructure.EXTI_Line = GPIO_TAMPER_EXTI_Line;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);

//  // Clear the Tamper EXTI line pending bit
//  EXTI_ClearITPendingBit(GPIO_TAMPER_EXTI_Line);
}

////////////////////////////////////////////////////////////////////////////////
// Function Name : ADC_Configuration.
// Description   : Configure the ADC and DMA.
// Input         : None.
// Output        : None.
// Return value  : The direction value.
////////////////////////////////////////////////////////////////////////////////
void ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_StructInit(&ADC_InitStructure);
    GPIO_StructInit(&GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Enable ADC1 channel  clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    //PA1 As an analog channel input pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //Analog input pin
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    ADC_DeInit(ADC1);
    ADC_StructInit(&ADC_InitStructure);
    // Initialize the ADC_PRESCARE values
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_8;
    // Initialize the ADC_Mode member
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
    // Initialize the ADC_DataAlign member
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    // Initialize the ADC_ExternalTrigConv member
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init(ADC1, &ADC_InitStructure);
    //ADC_RegularChannelConfig(ADC1, DISABLE_ALL_CHANNEL, 0, 0);
    ADC1->ADCHS &= (u32)0xFFFFFE00; //
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 0, ADC_CFGR_SAMCTL_28_5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 0, ADC_CFGR_SAMCTL_28_5);
    ADC_Cmd(ADC1, ENABLE); //Enable Specified ADC1
}

////////////////////////////////////////////////////////////////////////////////
//Function Name : ADC_Configuration.
//Description   : Configure the ADC and DMA.
//Input         : None.
//Output        : None.
//Return value  : The direction value.
////////////////////////////////////////////////////////////////////////////////

u16 ADC1_SingleChannel_Get(u8 ADC_Channel_x)
{
    u16 puiADData;

    //ADC_RegularChannelConfig(ADC1, DISABLE_ALL_CHANNEL, 0, 0);
    ADC1->ADCHS &= (u32)0xFFFFFE00; //

    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, ADC_CFGR_SAMCTL_28_5);
    ADC_Cmd(ADC1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
        ;
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    puiADData = ADC_GetConversionValue(ADC1);
    return puiADData;
}

//get ADC value
u16 Get_Adc(u8 ch)
{
    // config specifies the ADC rule group channel, a sequence, and the sampling time
    ADC_RegularChannelConfig(ADC1, ch, 0, 0); //ADC1,ADC channel ,Sampling time is 239.5 cycles

    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //Enable software conversion start function for Specified ADC1

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        ; //Wait for conversion to end

    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);

    return ADC_GetConversionValue(ADC1); //Returns the last conversion result of the ADC1 rule set
}


extern u8 status_UsbUart;
/////////////////////////////////////////////////////////////////////////////////
// Function Name  : USB_To_UART_Send_Data.
// Description    : send the received data from USB to the UART 0.
// Input          : data_buffer: data address.
//                  Nb_bytes: number of bytes to send.
// Return         : none.
////////////////////////////////////////////////////////////////////////////////
void USB_To_UART_Send_Data(u8* data_buffer, u8* Nb_bytes)
{
    u32 i;
    if((status_UsbUart >> 1) & 0x01) {
        status_UsbUart |= 1 << 4;     //UART is transmitting
        for (i = 0; i < *Nb_bytes; i++) {
            while((UART1->CSR & UART_IT_TXIEN) == 0) { //Send circularly until the transmission is complete
            }
            UART1->TDR = ( *(data_buffer + i) & (u16)0x00FF);

        }
        *Nb_bytes = 0;
        status_UsbUart &= ~(1 << 4);  //UART transmission completed
        status_UsbUart &= ~(1 << 1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//Function Name  : UART_To_USB_Send_Data.
//Description    : send the received data from UART 0 to USB.
//Input          : None.
//Return         : none.
////////////////////////////////////////////////////////////////////////////////
void UART_To_USB_Send_Data(void)
{
    if (linecoding.datatype == 7) {
        gRxBuff[gRxDataCnt] = UART_ReceiveData(UART1) & 0x7F;
    }
    else if (linecoding.datatype == 8) {
        gRxBuff[gRxDataCnt] = UART_ReceiveData(UART1);
    }
    if(gRxDataCnt < VIRTUAL_COM_PORT_DATA_SIZE)
        gRxDataCnt++;
}



////////////////////////////////////////////////////////////////////////////////
//Function Name  : Get_SerialNum.
//Description    : Create the serial number string descriptor.
//Input          : None.
//Output         : None.
//Return         : None.
////////////////////////////////////////////////////////////////////////////////
void Get_SerialNum(void)
{
    u32 Device_Serial0, Device_Serial1, Device_Serial2;

    Device_Serial0 = *(u32*)(0x1FFFF7E8);
    Device_Serial1 = *(u32*)(0x1FFFF7EC);
    Device_Serial2 = *(u32*)(0x1FFFF7F0);

    if (Device_Serial0 != 0) {
        CustomHID_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
        CustomHID_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
        CustomHID_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
        CustomHID_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

        CustomHID_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
        CustomHID_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
        CustomHID_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
        CustomHID_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

        CustomHID_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
        CustomHID_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
        CustomHID_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
        CustomHID_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
    }
}
/// @}

/// @}

/// @}
