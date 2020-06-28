////////////////////////////////////////////////////////////////////////////////
/// @file     usb_regs.C
/// @author   AE TEAM
/// @brief    Interface functions to USB cell registers.
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
#define _USB_REGS_C_
// Files includes
#include "usb_lib.h"
#include "usb_regs.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_REGS
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{



////////////////////////////////////////////////////////////////////////////////
/// @brief  merge two byte in a word.
/// @param  bh: byte high, bl: bytes low..
/// @retval wRet:resulted word.
////////////////////////////////////////////////////////////////////////////////
u16 ToWord(u8 bh, u8 bl)
{
    u16 wRet;
    wRet = (u16)bl | ((u16)bh << 8);
    return(wRet);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Swap two byte in a word.
/// @param  wSwW: word to Swap.
/// @retval wRet:resulted word.
////////////////////////////////////////////////////////////////////////////////
u16 ByteSwap(u16 wSwW)
{
    u8 bTemp;
    u16 wRet;
    bTemp = (u8)(wSwW & 0xff);
    wRet =  (wSwW >> 8) | ((u16)bTemp << 8);
    return(wRet);
}


/// @}

/// @}

/// @}
