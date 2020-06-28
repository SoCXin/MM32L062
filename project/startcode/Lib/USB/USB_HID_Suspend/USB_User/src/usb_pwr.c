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
#include "hal_exti.h"
#include "hal_gpio.h"
#include "hal_adc.h"
#include "hal_uart.h"
#include "hal_rcc.h"
#include "hal_misc.h"


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
__IO bool fSuspendEnabled = false;  // true when suspend is possible

struct {
    __IO RESUME_STATE eState;
    __IO u8 bESOFcnt;
} ResumeS;



////////////////////////////////////////////////////////////////////////////////
/// @brief  PowerOn.
/// @param  None.
/// @retval USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT PowerOn(void)
{
    USB_Cable_Config(DISABLE);          //USB connection 1.5k internal pull-up resistor

    _SetUSB_TOP(USB_TOP_RESET) ;        //USB reset
    _ClrUSB_TOP(USB_TOP_RESET) ;

    USB->INT_STATE |= 0;
    //clear USB_INT_STATE_SOFF|
    _SetUSB_INT_EN(USB_INT_EN_RSTIE | USB_INT_EN_EPINTIE | USB_INT_EN_RESUMIE);    //Enable USB_INT_EN_SOFIE|
    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_EPINTF | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE); //disable USB_INT_STATE_SOFF|

    _SetEP0_INT_EN(EPn_INT_EN_SETUPIE | EPn_INT_EN_INACKIE | EPn_INT_EN_OUTACKIE); //|EPn_INT_EN_OUTNACKIE

    _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));

    _SetEP0_INT_EN(EPn_INT_EN_SETUPIE | EPn_INT_EN_INACKIE | \
                   EPn_INT_EN_OUTACKIE);
    _ClrEP0_INT_STA(EPn_INT_STATE_SETUP | EPn_INT_STATE_INACK | \
                    EPn_INT_EN_INACKIE | EPn_INT_STATE_OUTACK | EPn_INT_STATE_OUTNACK);


    _SetEP1_INT_EN(EPn_INT_EN_INACKIE);                        // EPn_INT_EN_INACKIE
    _ClrEP1_INT_STA(EPn_INT_STATE_INACK);                      // EPn_INT_STATE_INACK


    _SetEP3_INT_EN(EPn_INT_EN_OUTACKIE);
    _ClrEP3_INT_STA(EPn_INT_STATE_OUTACK);


    _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP3));
    _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP3));

    USB_Cable_Config(ENABLE);    //USB connection 1.5k internal pull-up resistor
    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_EPINTF | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE);
    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  handles switch-off conditions.
/// @param  None.
/// @retval USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT PowerOff()
{
    _SetUSB_TOP(USB_TOP_RESET) ;      //USB Reset
    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_SOFF | USB_INT_STATE_EPINTF);

    USB_Cable_Config(DISABLE);        //disconnect interrupt

    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  sets suspend mode operating conditions.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Suspend(void)
{

    if(bDeviceState == CONFIGURED) {
        //Frequency the bus clock 512
        USB->POWER &= ~USB_POWER_SUSP;

        bDeviceState = SUSPENDED;
        fSuspendEnabled = true;

        RCC->CFGR &= ~RCC_CFGR_HPRE;
        RCC->CFGR |=  RCC_CFGR_HPRE_DIV512;
        RCC->CR &= ~(0x1f << 26);

        RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div4);

        UART_DeInit(UART1);

    }


}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Handles wake-up restoring normal operations.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Resume_Init(void)
{

    //HSI 48M
    RCC->CR |= (4 - 1) << 26; //PLL 2~16

    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);

    RCC->CFGR &= ~RCC_CFGR_HPRE;

    USB->POWER |=   USB_POWER_SUSP;

    bDeviceState = CONFIGURED;
    fSuspendEnabled = false;

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
    if(eResumeSetVal != RESUME_ESOF)
        ResumeS.eState = eResumeSetVal;

    switch(ResumeS.eState) {
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
            if(ResumeS.bESOFcnt == 0)
                ResumeS.eState = RESUME_START;
            break;
        case RESUME_START:

            ResumeS.eState = RESUME_ON;
            ResumeS.bESOFcnt = 10;
            break;
        case RESUME_ON:
            ResumeS.bESOFcnt--;
            if(ResumeS.bESOFcnt == 0) {
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

