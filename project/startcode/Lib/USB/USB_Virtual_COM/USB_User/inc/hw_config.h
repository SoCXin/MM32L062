////////////////////////////////////////////////////////////////////////////////
/// @file     hw_config.h
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
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

// Includes ------------------------------------------------------------------
#include "usb_type.h"

// Exported types ------------------------------------------------------------
// Exported constants --------------------------------------------------------
// Exported macro ------------------------------------------------------------
// Exported define -----------------------------------------------------------
#define MASS_MEMORY_START     0x04002000
#define BULK_MAX_PACKET_SIZE  0x00000040
#define LED_ON                0xF0
#define LED_OFF               0xFF

// Exported functions -------------------------------------------------------
void USB_GPIO_Config(void);
void UART_GPIO_Config(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_NVIC_Config(void);
void USB_CableConfig (FunctionalState NewState);
void UART_ConfigDefault(void);
void UART_NVIC_Config(void);
bool UART_Config(void);
void USB_To_UART_Send_Data(u8* data_buffer, u8* Nb_bytes);
void UART_To_USB_Send_Data(void);
void Get_SerialNum(void);

// External variables --------------------------------------------------------

#endif  //__HW_CONFIG_H 
/// @}

/// @}

/// @}
