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
#define _HAL_ADC_C_

#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "usb_istr.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_ISTR
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

__IO u16 wIstr;  // ISTR register last read value
__IO u8 bIntPackSOF = 0;  // SOFs received between 2 consecutive packets
__IO u32 esof_counter = 0; // expected SOF counter
__IO u32 wCNTR = 0;

// Extern variables
// Private function prototypes
// Private functions
// function pointers to non-control endpoints service routines
void (*pEpInt_IN[4])(void) = {
    EP1_IN_Callback,
    EP2_IN_Callback,
    EP3_IN_Callback,
    EP4_IN_Callback,
};

void (*pEpInt_OUT[4])(void) = {
    EP1_OUT_Callback,
    EP2_OUT_Callback,
    EP3_OUT_Callback,
    EP4_OUT_Callback,
};


////////////////////////////////////////////////////////////////////////////////
/// @brief  STR events interrupt service routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_Istr(void)
{

    wIstr = _GetUSB_INT_STA();

    if(wIstr & USB_INT_STATE_RSTF) {
        _ClrUSB_INT_STA(USB_INT_STATE_RSTF) ;
        Device_Property.Reset();
    }


    if (wIstr & USB_INT_STATE_RESUMF ) {
        _ClrUSB_INT_STA(USB_INT_STATE_RESUMF) ;

        Resume(RESUME_EXTERNAL);
    }


    if (wIstr & USB_INT_STATE_SUSPENDF ) {

        if (fSuspendEnabled) { // check if SUSPEND is possible
            Suspend();
        }
        else {
            // if not possible then resume after xx ms
            Resume(RESUME_LATER);
        }

        // clear of the ISTR bit must be done after setting of CNTR_FSUSP
        _ClrUSB_INT_STA(USB_INT_STATE_SUSPENDF);

    }


    if(wIstr & USB_INT_STATE_SOFF) {
        _ClrUSB_INT_STA(USB_INT_STATE_SOFF);

        bIntPackSOF++;
    }



    if(wIstr & USB_INT_STATE_EPINTF) {
        CTR_LP(); // servicing of the endpoint correct transfer interrupt
    }



} // USB_Istr

/// @}

/// @}

/// @}

