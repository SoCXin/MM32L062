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

// Define to prevent recursive inclusion  --------------------------------------
#define _HW_CONFIG_C_

// Files includes  -------------------------------------------------------------
#include "mm32_device.h"
#include "platform_config.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "queue.h"
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


QUEUE8_t m_QueueUsbRx = {0};

u8 m_UsbRxBuf[USB_RX_BUF_SIZE] = {0};

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures Main system clocks & power.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void USB_GPIO_Config(void)
{
    QUEUE_PacketCreate(&m_QueueUsbRx, m_UsbRxBuf, sizeof(m_UsbRxBuf)); //Create a receive queue
    // Configure the used GPIOs
    GPIO_Configuration();
    // Configure the EXTI lines for Key and Tamper push buttons
    EXTI_Configuration();
    // Configure the ADC
    ADC_Configuration();
}

void Set_CRS()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CRS, ENABLE);
    CRS->CFGR &= ~0xffff; //RELOAD
    CRS->CFGR |= 0xBB7F;  //
    // Select USB SOF as synchronization source
    //  CRS_SynchronizationSourceConfig(CRS_SYNCSource_USB);
    CRS->CFGR &= 0xcfffffff; //
    //Enables the automatic hardware adjustment of TRIM bits: AUTOTRIMEN:
    CRS->CFGR |= 0x2 << 28; //GPIO
    CRS->CR |= 0x40;        //autoen
    //Enables the oscillator clock for frequency error counter CEN
    CRS->CR |= 0x20; //cen
}

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
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures USB Clock input (48MHz).
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_ClockConfig(void)
{
    USB_HSI48M_Config();
    Set_CRS(); // The calibration of vibration
    // Select USBCLK source
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);
    // Enable USB clock
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
        // Device configured
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
    // Enable the EXTI0 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Enable the EXTI9_5 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
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
/// @brief  Configures the different GPIO ports.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIO_DISCONNECT | RCC_AHBPeriph_GPIO_KEY | RCC_AHBPeriph_GPIO_TAMPER | RCC_AHBPeriph_GPIO_IOAIN | RCC_AHBPeriph_GPIO_LED, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USB_DISCONNECT used as USB pull-up
    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

    // Configure the KEY IO as Input Floating
    GPIO_InitStructure.GPIO_Pin = GPIO_KEY1_PIN | GPIO_KEY2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIO_KEY, &GPIO_InitStructure);

    // Configure the Tamper IO as Input Floating
    GPIO_InitStructure.GPIO_Pin = GPIO_TAMPER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIO_TAMPER, &GPIO_InitStructure);

    // Configure Potentiometer IO as analog input -------------------------
    GPIO_InitStructure.GPIO_Pin = GPIO_IOAIN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIO_IOAIN, &GPIO_InitStructure);

    // Configure the LED1 IOs as Output PP
    GPIO_InitStructure.GPIO_Pin = GPIO_LED1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIO_LED1, &GPIO_InitStructure);

    // Configure the LED2 IOs as Output PP
    GPIO_InitStructure.GPIO_Pin = GPIO_LED2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIO_LED2, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the UART 1 according to the linecoding structure.
/// @param  None.
/// @retval Configuration status
///          TRUE : configuration done with success
///          FALSE : configuration aborted.
////////////////////////////////////////////////////////////////////////////////

void EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_StructInit(&EXTI_InitStructure);

    // Configure Key1 EXTI line to generate an interrupt on rising & falling edges
    EXTI_InitStructure.EXTI_Line = GPIO_KEY1_EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Clear the Key1 EXTI line pending bit
    EXTI_ClearITPendingBit(GPIO_KEY1_EXTI_Line);

    // Connect Key2 to the correspondent EXTI line
    //  GPIO_EXTILineConfig(GPIO_KEY2_PORTSOURCE, GPIO_KEY2_PINSOURCE);

    // Configure Key2 EXTI line to generate an interrupt on rising & falling edges
    EXTI_InitStructure.EXTI_Line = GPIO_KEY2_EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Clear the Key1 EXTI line pending bit
    EXTI_ClearITPendingBit(GPIO_KEY2_EXTI_Line);

    // Connect Tamper puch button to the correspondent EXTI line
    //  GPIO_EXTILineConfig(GPIO_TAMPER_PORTSOURCE, GPIO_TAMPER_PINSOURCE);

    // Configure Tamper EXTI Line to generate an interrupt rising & falling edges
    EXTI_InitStructure.EXTI_Line = GPIO_TAMPER_EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Clear the Tamper EXTI line pending bit
    EXTI_ClearITPendingBit(GPIO_TAMPER_EXTI_Line);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the ADC and DMA.
/// @param  None.
/// @retval The direction value.
////////////////////////////////////////////////////////////////////////////////
void ADC_Configuration(void)
{

}


////////////////////////////////////////////////////////////////////////////////
/// @brief  send the received data from USB to the UART 0.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

u32 USB_RxRead(u8* buffter, u32 buffterSize)
{
    return QUEUE_PacketOut(&m_QueueUsbRx, buffter, buffterSize);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  send the received data from USB to the UART 0.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u32 USB_RxWrite(u8* buffter, u32 writeLen)
{
    return QUEUE_PacketIn(&m_QueueUsbRx, buffter, writeLen);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Create the serial number string descriptor.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void Get_SerialNum(void)
{
    u32 Device_Serial0, Device_Serial1, Device_Serial2;

    Device_Serial0 = *(u32*)(0x1FFFF7E8);
    Device_Serial1 = *(u32*)(0x1FFFF7EC);
    Device_Serial2 = *(u32*)(0x1FFFF7F0);

    if (Device_Serial0 != 0) {
        Printer_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
        Printer_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
        Printer_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
        Printer_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

        Printer_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
        Printer_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
        Printer_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
        Printer_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

        Printer_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
        Printer_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
        Printer_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
        Printer_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
    }
}
/// @}

/// @}

/// @}
