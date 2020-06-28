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
#include "mm32_device.h"

// Exported types ------------------------------------------------------------
// Exported constants --------------------------------------------------------
// Exported macro ------------------------------------------------------------
// Exported define -----------------------------------------------------------
// Exported functions -------------------------------------------------------
void USB_ClockConfig(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_NVIC_Config(void);
void USB_Cable_Config(FunctionalState NewState);
void EXTI_Configuration(void);
void ADC_Configuration(void);
void Get_SerialNum(void);
u16 Get_Adc(u8 ch);
u16 ADC1_SingleChannel_Get(u8 ADC_Channel_x);

void USB_To_UART_Send_Data(u8* data_buffer, u8* Nb_bytes);
void UART_To_USB_Send_Data(void);


#endif //__HW_CONFIG_H

/// @}

/// @}

/// @}
