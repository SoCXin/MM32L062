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

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures Main system clocks & power.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Set_System(void)
{
    // Configure the used GPIOs
    GPIO_Configuration();
}


//*******************************************************************


void SetUSBSysClockTo48M(void)
{
    __IO u32 StartUpCounter = 0, HSEStatus = 0;
    RCC_DeInit();
    // SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------
    // Enable HSE
    RCC->CR |= ((u32)RCC_CR_HSEON);

    // Wait till HSE is ready and if Time out is reached exit
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
        HSEStatus = (u32)0x01;
    }
    else {
        HSEStatus = (u32)0x00;
    }

    if (HSEStatus == (u32)0x01) {
        // Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        // Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~0x07;
        FLASH->ACR |= 0x02;
        // HCLK = SYSCLK
        RCC->CFGR |= (u32)RCC_CFGR_HPRE_DIV1;

        // PCLK2 = HCLK
        RCC->CFGR |= (u32)RCC_CFGR_PPRE2_DIV1;

        // PCLK1 = HCLK
        RCC->CFGR |= (u32)RCC_CFGR_PPRE1_DIV2;

        //  PLL configuration:  = (HSE ) * (5+1) = 48MHz
        RCC->CFGR &= (u32)0xFFFCFFFF;
        RCC->CR &= (u32)0x000FFFFF;

        RCC->CFGR |= (u32 ) RCC_CFGR_PLLSRC ;
        RCC->CR |= 0x14000000;//pll = 6/1
        //RCC->CFGR &= (u32)((u32)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        //RCC->CFGR |= (u32)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE_Div2 | RCC_CFGR_PLLMULL6);

        // Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        // Wait till PLL is ready
        while((RCC->CR & RCC_CR_PLLRDY) == 0) {
        }

        // Select PLL as system clock source
        RCC->CFGR &= (u32)((u32)~(RCC_CFGR_SW));
        RCC->CFGR |= (u32)RCC_CFGR_SW_PLL;

        // Wait till PLL is used as system clock source
        while ((RCC->CFGR & (u32)RCC_CFGR_SWS) != (u32)0x08) {
        }
    }
    else {
        // If HSE fails to start-up, the application will have wrong clock
        //  configuration. User can add here some code to deal with this error
    }
}


void Set_CRS()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CRS, ENABLE);
    CRS->CFGR &= ~0xffff; //??RELOAD
    CRS->CFGR |= 0xBB7F; //RELOAD????47999,?????:RELOAD=????48M/??????-1=48000000/1000-1=47999(?????????1KHZ?
    // Select USB SOF as synchronization source
    //  CRS_SynchronizationSourceConfig(CRS_SYNCSource_USB);
    CRS->CFGR &= 0xcfffffff; //
    //Enables the automatic hardware adjustment of TRIM bits: AUTOTRIMEN:
    CRS->CFGR |= 0x2 << 28; //GPIO
    CRS->CR |= 0x40; //autoen
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
/// @brief  Configures USB Clock input (48MHz)..
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_ClockConfig(void)
{
    USB_HSI48M_Config();
    Set_CRS();      // The calibration of vibration
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
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
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
        // (USB->TOP  &= 0x02);
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
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_DISCONNECT, ENABLE);

    // USB_DISCONNECT used as USB pull-up
    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

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

    if(Device_Serial0 != 0) {
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
