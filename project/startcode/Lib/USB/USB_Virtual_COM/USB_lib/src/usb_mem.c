////////////////////////////////////////////////////////////////////////////////
/// @file     usb_mem.C
/// @author   AE TEAM
/// @brief    Utility functions for memory transfers to/from PMA.
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
#define _USB_MEM_C_
// Files includes

#include "usb_lib.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_MEN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{



////////////////////////////////////////////////////////////////////////////////
/// @brief  Copy a buffer from user memory area to packet memory area (PMA).
/// @param  pbUsrBuf: pointer to user memory area.
///         wPMABufAddr: address into PMA.
///         wNBytes: no. of bytes to be copied.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UserToPMABufferCopy(u8* pbUsrBuf, u16 EPNum, u16 wNBytes)
{
    u32 i;
    for (i = wNBytes; i != 0; i--) {
        if(EPNum == 0) {
            _SetUSB_FIFO0(*pbUsrBuf);
        }
        else {
            _SetUSB_FIFOn(*pbUsrBuf, EPNum);
        }
        pbUsrBuf++;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Copy a buffer from user memory area to packet memory area (PMA).
/// @param  pbUsrBuf: pointer to user memory area.
///         wPMABufAddr: address into PMA.
///         wNBytes: no. of bytes to be copied.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PMAToUserBufferCopy(u8* pbUsrBuf, u16 EPNum, u16 wNBytes)
{
    u32 i;

    for (i = wNBytes; i != 0; i--) {
        if(EPNum == 0) {
            *pbUsrBuf = _GetUSB_FIFO0() & 0xff;
        }
        else {
            *pbUsrBuf = _GetUSB_FIFOn(EPNum);
        }
        pbUsrBuf++;
    }
}

/// @}

/// @}

/// @}
