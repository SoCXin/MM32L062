////////////////////////////////////////////////////////////////////////////////
/// @file     hw_config.c
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
#include "MM32L0xx_it.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "platform_config.h"
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

UART_InitTypeDef UART_InitStructure;


extern u8 buffer_in[VIRTUAL_COM_PORT_DATA_SIZE];
extern u32 count_in;
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
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures Main system clocks & power.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    USB_HSI48M_Config();
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);             //USB CLK EN

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                   //Analog input
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures Main system clocks & power.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);            //USB CLK EN

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    // Configure UART1 Rx (PA.10) as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure UART1 Tx (PA.09) as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
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
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Software Connection/Disconnection of USB Cable.
/// @param  NewState: new state.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_CableConfig (FunctionalState NewState)
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
void UART_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Configures the different GPIO ports.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_ConfigDefault(void)
{
    UART_StructInit(&UART_InitStructure);
    UART_InitStructure.UART_BaudRate = 9600;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    // Configure the UART1
    UART_Init(UART1, &UART_InitStructure);
    UART_Cmd(UART1, ENABLE);
    // Enable the UART Receive interrupt
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configure the UART 1 according to the linecoding structure.
/// @param  None.
/// @retval Configuration status
///          TRUE : configuration done with success
///          FALSE : configuration aborted.
////////////////////////////////////////////////////////////////////////////////
bool UART_Config(void)
{
    // set the Stop bit
    switch (linecoding.format) {
        case 0:
            UART_InitStructure.UART_StopBits = UART_StopBits_1;
            break;
        case 1:
            UART_InitStructure.UART_StopBits = UART_StopBits_1;//1.5old
            break;
        case 2:
            UART_InitStructure.UART_StopBits = UART_StopBits_2;
            break;
        default : {
                UART_ConfigDefault();
                return (false);
            }
    }
    // set the parity bit
    switch (linecoding.paritytype) {
        case 0:
            UART_InitStructure.UART_Parity = UART_Parity_No;
            break;
        case 1:
            UART_InitStructure.UART_Parity = UART_Parity_Even;
            break;
        case 2:
            UART_InitStructure.UART_Parity = UART_Parity_Odd;
            break;
        default : {
                UART_ConfigDefault();
                return (false);
            }
    }
    //set the data type : only 8bits and 9bits is supported
    switch (linecoding.datatype) {
        case 0x07:
            // With this configuration a parity (Even or Odd) should be set
            UART_InitStructure.UART_WordLength = UART_WordLength_8b;
            break;
        case 0x08:
            if (UART_InitStructure.UART_Parity == UART_Parity_No) {
                UART_InitStructure.UART_WordLength = UART_WordLength_8b;
            }
            else {
                UART_InitStructure.UART_WordLength = UART_WordLength_6b;//
            }

            break;
        default : {
                UART_ConfigDefault();
                return (false);
            }
    }
    UART_InitStructure.UART_BaudRate = linecoding.bitrate;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(UART1, &UART_InitStructure);
    UART_Cmd(UART1, ENABLE);
    // Enable the UART Receive interrupt
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    return (true);
}




extern u8 status_UsbUart;

////////////////////////////////////////////////////////////////////////////////
/// @brief  send the received data from USB to the UART 0.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_To_UART_Send_Data(u8* data_buffer, u8* Nb_bytes)
{
    u32 i;
    if((status_UsbUart >> 1) & 0x01) {
        status_UsbUart |= 1 << 4;
        for (i = 0; i < *Nb_bytes; i++) {
            UART_SendData(UART1, *(data_buffer + i));
            while(UART_GetFlagStatus(UART1, UART_FLAG_TXEMPTY) == RESET);
        }
        *Nb_bytes = 0;
        status_UsbUart &= ~(1 << 4);
        status_UsbUart &= ~(1 << 1);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  send the received data from USB to the UART 0.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_To_USB_Send_Data(void)
{
    if (linecoding.datatype == 7) {
        buffer_in[count_in] = UART_ReceiveData(UART1) & 0x7F;
    }
    else if (linecoding.datatype == 8) {
        buffer_in[count_in] = UART_ReceiveData(UART1);
    }
    if(count_in < VIRTUAL_COM_PORT_DATA_SIZE)
        count_in++;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Create the serial number string descriptor.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Get_SerialNum(void)
{
    u32 Device_Serial0, Device_Serial1, Device_Serial2;

    Device_Serial0 = *(__IO u32*)(0x1FFFF7E8);
    Device_Serial1 = *(__IO u32*)(0x1FFFF7EC);
    Device_Serial2 = *(__IO u32*)(0x1FFFF7F0);

    if (Device_Serial0 != 0) {
        Virtual_Com_Port_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
        Virtual_Com_Port_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
        Virtual_Com_Port_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
        Virtual_Com_Port_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

        Virtual_Com_Port_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
        Virtual_Com_Port_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
        Virtual_Com_Port_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
        Virtual_Com_Port_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

        Virtual_Com_Port_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
        Virtual_Com_Port_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
        Virtual_Com_Port_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
        Virtual_Com_Port_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
    }
}
/// @}

/// @}

/// @}
