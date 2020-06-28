////////////////////////////////////////////////////////////////////////////////
/// @file     usb_istr.C
/// @author   AE TEAM
/// @brief    ISTR events interrupt service routines.
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
#define _USB_ISTR_C_

// Files includes
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "usb_istr.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{
__IO u8 bIntPackSOF = 0;  // SOFs received between 2 consecutive packets
void (*pEpInt_IN[7])(void) = {
    EP1_IN_Callback,
    EP2_IN_Callback,
    EP3_IN_Callback,
    EP4_IN_Callback,
    EP5_IN_Callback,
    EP6_IN_Callback,
    EP7_IN_Callback,
};

void (*pEpInt_OUT[7])(void) = {
    EP1_OUT_Callback,
    EP2_OUT_Callback,
    EP3_OUT_Callback,
    EP4_OUT_Callback,
    EP5_OUT_Callback,
    EP6_OUT_Callback,
    EP7_OUT_Callback,
};


////////////////////////////////////////////////////////////////////////////////
/// @brief  STR events interrupt service routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_Istr(void)
{
    __IO u16 wIstr;
    wIstr = _GetUSB_INT_STA();
    //USB->INT_STATE = wIstr;

    // fSuspendEnabled = 0;
    if(wIstr & USB_INT_STATE_RSTF) {
        _ClrUSB_INT_STA(USB_INT_STATE_RSTF) ;
        Device_Property.Reset();
    }

    if(wIstr & USB_INT_STATE_SOFF) {
        //    _ClrUSB_INT_STA(USB_INT_STATE_SOFF) ;
        //    bIntPackSOF++;
    }

    if(wIstr & USB_INT_STATE_SUSPENDF) {
        _ClrUSB_INT_STA(USB_INT_STATE_SUSPENDF);
        Suspend();

    }


    if(wIstr & USB_INT_STATE_RESUMF) {
        _ClrUSB_INT_STA(USB_INT_STATE_RESUMF);
        Resume_Init();

    }

    if(wIstr & USB_INT_STATE_EPINTF) {
        CTR_LP();   //Clear interrupt flags in subroutines
    }

} // USB_Istr

/// @}

/// @}

/// @}

