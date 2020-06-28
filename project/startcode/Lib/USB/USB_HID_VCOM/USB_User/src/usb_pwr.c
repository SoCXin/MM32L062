////////////////////////////////////////////////////////////////////////////////
/// @file     usb_pwr.C
/// @author   AE TEAM
/// @brief    Connection/disconnection & power management.
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
__IO u32 bDeviceState = UNCONNECTED; // USB device status
__IO bool fSuspendEnabled = true;  // true when suspend is possible
__IO u32 EP[8];

struct {
    __IO RESUME_STATE eState;
    __IO u8 bESOFcnt;
}
ResumeS;

__IO u32 remotewakeupon = 0;
////////////////////////////////////////////////////////////////////////////////
/// @brief  PowerOn.
/// @param  None.
/// @retval USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////

RESULT PowerOn(void)
{
    USB_Cable_Config(DISABLE);

    _SetUSB_TOP(USB_TOP_RESET) ;
    _ClrUSB_TOP(USB_TOP_RESET) ;


    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_EPINTF);

    _SetUSB_INT_EN(USB_INT_EN_RSTIE | USB_INT_EN_EPINTIE);

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | EPn_INT_EN_OUTACKIE); //|EPn_INT_EN_OUTNACKIE

    _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | \
                    EPn_INT_EN_OUTACKIE);//EPn_INT_EN_INACKIE|EPn_INT_EN_OUTNACKIE|
    _ClrEP0_INT_STA(EPn_INT_STATE_SETUP | EPn_INT_STATE_INNACK | \
                    EPn_INT_EN_INACKIE | EPn_INT_STATE_OUTACK | EPn_INT_STATE_OUTNACK);


    _SetEP1_INT_EN( EPn_INT_EN_INNACKIE );                      // EPn_INT_EN_INACKIE
    _ClrEP1_INT_STA(EPn_INT_STATE_INNACK);                      // EPn_INT_STATE_INACK


    _SetEP2_INT_EN(EPn_INT_EN_INNACKIE );                       // EPn_INT_EN_INACKIE
    _ClrEP2_INT_STA(EPn_INT_EN_INNACKIE);                       // EPn_INT_EN_INACKIE


    _SetEP3_INT_EN( EPn_INT_EN_OUTACKIE);
    _ClrEP3_INT_STA( EPn_INT_STATE_OUTACK);

    //HID EP4  IN  OUT

    _SetEP4_INT_EN( EPn_INT_EN_INNACKIE );                      // EPn_INT_EN_INACKIE
    _ClrEP4_INT_STA(EPn_INT_STATE_INNACK);                      // EPn_INT_STATE_INACK


    _SetEP4_INT_EN( EPn_INT_EN_OUTNACKIE );                      // EPn_INT_EN_INACKIE
    _ClrEP4_INT_STA(EPn_INT_STATE_OUTNACK);                      // EPn_INT_STATE_INACK


    _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));
    _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));


    USB_Cable_Config(ENABLE);

//   CNTR_PWDN = 0
//  wRegVal = CNTR_FRES;
//  _SetCNTR(wRegVal);

//   CNTR_FRES = 0
//  wInterrupt_Mask = 0;
//  _SetCNTR(wInterrupt_Mask);
//    Clear pending interrupts
//  _SetISTR(0);
//  Set interrupt mask
//  wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;
//  _SetCNTR(wInterrupt_Mask);

    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  handles switch-off conditions.
/// @param  None.
/// @retval USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT PowerOff()
{
    _SetUSB_TOP(USB_TOP_RESET) ;
    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_SOFF | USB_INT_STATE_EPINTF);

    USB_Cable_Config(DISABLE);

    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  sets suspend mode operating conditions.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Suspend(void)
{
//  u32 i =0;
//  u16 wCNTR;
//  u32 tmpreg = 0;
//  __IO u32 savePWR_CR=0;
//  // suspend preparation
//
//
//  //Store CNTR value
//  wCNTR = _GetCNTR();

//    // This a sequence to apply a force RESET to handle a robustness case
//
//  //Store endpoints registers status
//    for (i=0;i<8;i++) EP[i] = _GetENDPOINT(i);
//
//  // unmask RESET flag
//  wCNTR|=CNTR_RESETM;
//  _SetCNTR(wCNTR);
//
//  //apply FRES
//  wCNTR|=CNTR_FRES;
//  _SetCNTR(wCNTR);
//
//  //clear FRES
//  wCNTR&=~CNTR_FRES;
//  _SetCNTR(wCNTR);
//
//  //poll for RESET flag in ISTR
//  while((_GetISTR()&ISTR_RESET) == 0);
//
//  // clear RESET flag in ISTR
//  _SetISTR((u16)CLR_RESET);
//
//  //restore Enpoints
//  for (i=0;i<8;i++)
//  _SetENDPOINT(i, EP[i]);
//
//  // Now it is safe to enter macrocell in suspend mode
//  wCNTR |= CNTR_FSUSP;
//  _SetCNTR(wCNTR);
//
//  // force low-power mode in the macrocell
//  wCNTR = _GetCNTR();
//  wCNTR |= CNTR_LPMODE;
//  _SetCNTR(wCNTR);
//
//  //prepare entry in low power mode (STOP mode)*/
//  // Select the regulator state in STOP mode
//  savePWR_CR = PWR->CR;
//  tmpreg = PWR->CR;
//  // Clear PDDS and LPDS bits
//  tmpreg &= ((u32)0xFFFFFFFC);
//  // Set LPDS bit according to PWR_Regulator value
//  tmpreg |= PWR_Regulator_LowPower;
//  // Store the new value
//  PWR->CR = tmpreg;
//  // Set SLEEPDEEP bit of Cortex System Control Register
//#if defined (STM32F30X) || defined (STM32F37X)
//        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
//#else
//        SCB->SCR |= SCB_SCR_SLEEPDEEP;
//#endif
//
//  // enter system in STOP mode, only when wakeup flag in not set
//  if((_GetISTR()&ISTR_WKUP)==0)
//  {
//      __WFI();
//      // Reset SLEEPDEEP bit of Cortex System Control Register
//#if defined (STM32F30X) || defined (STM32F37X)
//                SCB->SCR &= (u32)~((u32)SCB_SCR_SLEEPDEEP_Msk);
//#else
//                SCB->SCR &= (u32)~((u32)SCB_SCR_SLEEPDEEP);
//#endif
//  }
//  else
//  {
//      // Clear Wakeup flag
//      _SetISTR(CLR_WKUP);
//      // clear FSUSP to abort entry in suspend mode
//        wCNTR = _GetCNTR();
//        wCNTR&=~CNTR_FSUSP;
//        _SetCNTR(wCNTR);
//
//      //restore sleep mode configuration
//      // restore Power regulator config in sleep mode
//      PWR->CR = savePWR_CR;
//
//      // Reset SLEEPDEEP bit of Cortex System Control Register
//#if defined (STM32F30X) || defined (STM32F37X)
//                SCB->SCR &= (u32)~((u32)SCB_SCR_SLEEPDEEP_Msk);
//#else
//                SCB->SCR &= (u32)~((u32)SCB_SCR_SLEEPDEEP);
//#endif
//    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Handles wake-up restoring normal operations.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Resume_Init(void)
{
//  u16 wCNTR;
//
// restart the clocks
//  // ...

//  // CNTR_LPMODE = 0
//  wCNTR = _GetCNTR();
//  wCNTR &= (~CNTR_LPMODE);
//  _SetCNTR(wCNTR);
//
//  // restore full power
//  // ... on connected devices
//  Leave_LowPowerMode();

//  // reset FSUSP bit
//  _SetCNTR(IMR_MSK);

//  // reverse suspend preparation
//  // ...

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This is the state machine handling resume operations and
///                timing sequence. The control is based on the Resume structure
///                variables and on the ESOF interrupt calling this subroutine
///                without changing machine state.
/// @param  a state machine value (RESUME_STATE)
///                 RESUME_ESOF doesn't change ResumeS.eState allowing
///                decrementing of the ESOF counter in different states..
/// @retval None.
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

