////////////////////////////////////////////////////////////////////////////////
/// @file     usb_lib.h
/// @author   AE TEAM
/// @brief    USB library include files
////////////////////////////////////////////////////////////////////////////////
//THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
//WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
//AS A RESULT, MM32 SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
//INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
//CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
//INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#ifndef __USB_LIB_H
#define __USB_LIB_H

//Includes


#include "usb_type.h"
#include "usb_regs.h"
#include "usb_def.h"
#include "usb_core.h"
#include "usb_init.h"
#include "usb_mem.h"
#include "usb_int.h"
#include "mm32_device.h"


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Example_Layer
/// @brief MM32 Example Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_RESOURCE
/// @brief MM32 Examples resource modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Constants
/// @{

#define BUFF_SIZE  0x40


extern __IO u16 wIstr;  // ISTR register last read value
extern __IO u16 wIstr;  // ISTR register last read value

//Exported types
//Exported constants
//Exported macro
//Exported functions
//External variables

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
