////////////////////////////////////////////////////////////////////////////////
/// @file      usb_lib.h
/// @author    AE TEAM
/// @brief     USB library include files
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
#ifndef __USB_LIB_H
#define __USB_LIB_H

// Files includes


#include "usb_type.h"
#include "usb_regs.h"
#include "usb_def.h"
#include "usb_core.h"
#include "usb_init.h"
#include "usb_mem.h"
#include "usb_int.h"
#include "mm32_device.h"


#define BUFF_SIZE  0x40


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
