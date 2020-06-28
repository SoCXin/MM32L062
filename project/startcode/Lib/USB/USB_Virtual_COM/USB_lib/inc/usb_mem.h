////////////////////////////////////////////////////////////////////////////////
/// @file     usb_mem.H
/// @author   AE TEAM
/// @brief    Utility prototypes functions for memory/PMA transfers.
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
#ifndef __USB_MEM_H
#define __USB_MEM_H

// Files includes

#include "mm32_device.h"


// Includes ------------------------------------------------------------------
// Exported types ------------------------------------------------------------
// Exported constants --------------------------------------------------------
// Exported macro ------------------------------------------------------------
// Exported functions -------------------------------------------------------
void UserToPMABufferCopy(u8* pbUsrBuf, u16 EPNum, u16 wNBytes);
void PMAToUserBufferCopy(u8* pbUsrBuf, u16 EPNum, u16 wNBytes);

// External variables --------------------------------------------------------

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
