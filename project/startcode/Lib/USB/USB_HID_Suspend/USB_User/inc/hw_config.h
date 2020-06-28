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
// Exported functions -------------------------------------------------------
void Set_System(void);
void USB_ClockConfig(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_NVIC_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void GPIO_Configuration(void);
void Get_SerialNum(void);
//u16 Get_Adc(u8 ch);
#endif  //__HW_CONFIG_H 

/// @}

/// @}

/// @}
