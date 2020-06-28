////////////////////////////////////////////////////////////////////////////////
/// @file       usb_pwr.c
/// @author     AE TEAM
/// @brief      Connection/disconnection & power management
////////////////////////////////////////////////////////////////////////////////
// THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
// WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
// AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
// INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
// CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
// INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _USB_PWR_C_

// Files includes
#include "mm32_device.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_pwr.h"
#include "hw_config.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_PWR
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

// Private typedef
// Private define
// Private macro
// Private variables
__IO u32 bDeviceState = UNCONNECTED; // USB device status
__IO bool fSuspendEnabled = true;  // True when suspend is possible

struct {
    __IO RESUME_STATE eState;
    __IO u8 bESOFcnt;
}
ResumeS;

// Extern variables
// Private function prototypes
// Extern function prototypes
// Private functions

////////////////////////////////////////////////////////////////////////////////
/// @brief      USB IP power ON Routine.
/// @param      None.
/// @retval     None
////////////////////////////////////////////////////////////////////////////////
RESULT PowerOn(void)
{

    USB_CableConfig(DISABLE);          //USB connection 1.5K internal pull-up resistor

    _SetUSB_TOP(USB_TOP_RESET) ;        //USB reset
    _ClrUSB_TOP(USB_TOP_RESET) ;

    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_EPINTF); //Clear USB interruptUSB_INT_STATE_SOFF|

    _SetUSB_INT_EN(USB_INT_EN_RSTIE | USB_INT_EN_EPINTIE);      //Enabler USB interruptUSB_INT_EN_SOFIE|

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_OUTACKIE); //|EPn_INT_EN_OUTNACKIE

    _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | \
                    EPn_INT_EN_OUTACKIE);//|EPn_INT_EN_OUTNACKIE|//EPn_INT_EN_INACKIE|
    _ClrEP0_INT_STA(EPn_INT_STATE_SETUP | EPn_INT_STATE_INNACK | \
                    EPn_INT_EN_INACKIE | EPn_INT_STATE_OUTACK | EPn_INT_STATE_OUTNACK);


    _SetEP1_INT_EN( EPn_INT_EN_INNACKIE );                      // EPn_INT_EN_INACKIE
    _ClrEP1_INT_STA(EPn_INT_STATE_INNACK);                      // EPn_INT_STATE_INACK


    _SetEP2_INT_EN(EPn_INT_EN_INNACKIE );                       // EPn_INT_EN_INACKIE
    _ClrEP2_INT_STA(EPn_INT_EN_INNACKIE);                       // EPn_INT_EN_INACKIE


    _SetEP3_INT_EN( EPn_INT_EN_OUTACKIE);
    _ClrEP3_INT_STA( EPn_INT_STATE_OUTACK);


    _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));
    _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));

    USB_CableConfig(ENABLE);                                   //USB connection 1.5K internal pull-up resistor

    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     Handles switch-off conditions
/// @param     None
/// @retval    USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT PowerOff()
{
    _SetUSB_TOP(USB_TOP_RESET) ;      //USB reset
    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_SOFF | USB_INT_STATE_EPINTF); //Clear USB interrupt

    USB_CableConfig(DISABLE);        //Disconnect

    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   sets suspend mode operating conditions
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
void Suspend(void)
{

}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Handles wake-up restoring normal operations
/// @param    None.
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void Resume_Init(void)
{

}

////////////////////////////////////////////////////////////////////////////////
/// @brief    This is the state machine handling resume operations and
///           timing sequence. The control is based on the Resume structure
///           variables and on the ESOF interrupt calling this subroutine
///           without changing machine state.
/// @param    eResumeSetVal:  a state machine value (RESUME_STATE)
///           RESUME_ESOF doesn't change ResumeS.eState allowing
///           decrementing of the ESOF counter in different states.
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void Resume(RESUME_STATE eResumeSetVal)
{
    if (eResumeSetVal != RESUME_ESOF)
        ResumeS.eState = eResumeSetVal;

    switch (ResumeS.eState) {
        case RESUME_EXTERNAL:
            Resume_Init();
            ResumeS.eState = RESUME_OFF;
            break;
        case RESUME_INTERNAL:
            Resume_Init();
            ResumeS.eState = RESUME_START;
            break;
        case RESUME_LATER:
            ResumeS.bESOFcnt = 2;
            ResumeS.eState = RESUME_WAIT;
            break;
        case RESUME_WAIT:
            ResumeS.bESOFcnt--;
            if (ResumeS.bESOFcnt == 0)
                ResumeS.eState = RESUME_START;
            break;
        case RESUME_START:

            ResumeS.eState = RESUME_ON;
            ResumeS.bESOFcnt = 10;
            break;
        case RESUME_ON:
            ResumeS.bESOFcnt--;
            if (ResumeS.bESOFcnt == 0) {
                ResumeS.eState = RESUME_OFF;
            }
            break;
        case RESUME_OFF:
        case RESUME_ESOF:
        default:
            ResumeS.eState = RESUME_OFF;
            break;
    }
}

/// @}


/// @}

/// @}
