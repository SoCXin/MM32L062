////////////////////////////////////////////////////////////////////////////////
/// @file     usb_prop.C
/// @author   AE TEAM
/// @brief    All processings related to Custom HID Demo.
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
#define _USB_PROP_C_

// Files includes
#include "mm32_device.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "hal_rcc.h"
#include "hal_misc.h"
#include "hal_exti.h"
#include "hal_gpio.h"
#include "hal_adc.h"
#include "hal_uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{
u32 ProtocolValue;

DEVICE Device_Table = {
    EP_NUM,
    1
};

DEVICE_PROP Device_Property = {
    CustomHID_init,
    CustomHID_Reset,
    CustomHID_Status_In,
    CustomHID_Status_Out,
    CustomHID_Data_Setup,
    CustomHID_NoData_Setup,
    CustomHID_Get_Interface_Setting,
    CustomHID_GetDeviceDescriptor,
    CustomHID_GetConfigDescriptor,
    CustomHID_GetStringDescriptor,
    0,
    0x40 //MAX PACKET SIZE
};
USER_STANDARD_REQUESTS User_Standard_Requests = {
    CustomHID_GetConfiguration,
    CustomHID_SetConfiguration,
    CustomHID_GetInterface,
    CustomHID_SetInterface,
    CustomHID_GetStatus,
    CustomHID_ClearFeature,
    CustomHID_SetEndPointFeature,
    CustomHID_SetDeviceFeature,
    CustomHID_SetDeviceAddress
};

ONE_DESCRIPTOR Device_Descriptor = {
    (u8*)CustomHID_DeviceDescriptor,
    CUSTOMHID_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor = {
    (u8*)CustomHID_ConfigDescriptor,
    CUSTOMHID_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR CustomHID_Report_Descriptor = {
    (u8*)CustomHID_ReportDescriptor,
    CUSTOMHID_SIZ_REPORT_DESC
};

ONE_DESCRIPTOR CustomHID_Hid_Descriptor = {
    (u8*)CustomHID_ConfigDescriptor + CUSTOMHID_OFF_HID_DESC,
    CUSTOMHID_SIZ_HID_DESC
};

ONE_DESCRIPTOR String_Descriptor[4] = {
    {(u8*)CustomHID_StringLangID, CUSTOMHID_SIZ_STRING_LANGID},
    {(u8*)CustomHID_StringVendor, CUSTOMHID_SIZ_STRING_VENDOR},
    {(u8*)CustomHID_StringProduct, CUSTOMHID_SIZ_STRING_PRODUCT},
    {(u8*)CustomHID_StringSerial, CUSTOMHID_SIZ_STRING_SERIAL}
};


////////////////////////////////////////////////////////////////////////////////
/// @brief  Custom HID init routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CustomHID_init(void)
{
    // Update the serial number string descriptor with the data from the unique ID
    // Get_SerialNum();
    pInformation->Current_Configuration = 0;
    // Connect the device
    PowerOn();
    // USB interrupts initialization
    bDeviceState = UNCONNECTED;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Custom HID reset routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CustomHID_Reset(void)
{

//     u8 temp;
    // Set Joystick_DEVICE as not configured
    pInformation->Current_Configuration = 0;
    pInformation->Current_Interface = 0;//the default Interface

    // Current Feature initialization
    pInformation->Current_Feature = CustomHID_ConfigDescriptor[7];

    // Set this device to response on default address

    //Clockinitialization to HSI 48M
    RCC->CR |= (4 - 1) << 26;
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);
    RCC->CFGR &= ~RCC_CFGR_HPRE;

    _SetUSB_TOP(USB_TOP_RESET);
    _ClrUSB_TOP(USB_TOP_RESET);

    USB->POWER = USB_POWER_SUSPEN | USB_POWER_SUSP;//Enter working mode
    USB->POWER = USB_POWER_WKUP | USB_POWER_SUSPEN | USB_POWER_SUSP;//wake up

    USB->POWER = USB_POWER_SUSPEN | USB_POWER_SUSP;//Enter working mode

    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_EPINTF | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE);
    _SetUSB_INT_EN(USB_INT_EN_RSTIE | USB_INT_EN_EPINTIE | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE);  //Enable USB interrupt USB_INT_EN_SOFIE|
    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_EPINTF | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE); //Clear USB interrupt USB_INT_STATE_SOFF|


    _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2));

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INACKIE | \
                    EPn_INT_EN_OUTACKIE);//|EPn_INT_EN_OUTACKIE );             //EPn_INT_EN_INACKIE|

    _ClrEP0_INT_STA(EPn_INT_STATE_SETUP | EPn_INT_STATE_INACK | \
                    EPn_INT_EN_INACKIE);//|EPn_INT_STATE_OUTACK);


    _SetEP1_INT_EN( EPn_INT_EN_INACKIE );                         //| EPn_INT_EN_INACKIE
    _ClrEP1_INT_STA(EPn_INT_STATE_INACK);//| EPn_INT_STATE_INACK );


    _SetEP2_INT_EN(EPn_INT_EN_INACKIE);                           //| EPn_INT_EN_INACKIE
    _ClrEP2_INT_STA(EPn_INT_EN_INACKIE);//| EPn_INT_EN_INACKIE );

    _SetEP3_INT_EN(EPn_INT_EN_INACKIE);                           //| EPn_INT_EN_INACKIE
    _ClrEP3_INT_STA(EPn_INT_EN_INACKIE);//| EPn_INT_EN_INACKIE );

    _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));
    _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2));

    SetDeviceAddress(0);


    _ClrUSB_INT_STA(USB_INT_STATE_RSTF | USB_INT_STATE_EPINTF | USB_INT_EN_SUSPENDIE | USB_INT_EN_RESUMIE);

    bDeviceState = ATTACHED;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Udpade the device state to configured and command the ADC
///         conversion.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CustomHID_SetConfiguration(void)
{
    if (pInformation->Current_Configuration != 0) {
        // Device configured
        bDeviceState = CONFIGURED;

        // Start ADC1 Software Conversion
        //ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Udpade the device state to addressed.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CustomHID_SetDeviceAddress (void)
{
    bDeviceState = ADDRESSED;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Joystick status IN routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CustomHID_Status_In(void)
{
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Joystick status OUT routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CustomHID_Status_Out (void)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Handle the data class specific requests.
/// @param Request Nb.
/// @retval USB_UNSUPPORT or USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT CustomHID_Data_Setup(u8 RequestNo)
{
    u8* (*CopyRoutine)(u16);

    CopyRoutine = NULL;

    if ((RequestNo == GET_DESCRIPTOR)
            && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
            && (pInformation->USBwIndex0 == 0)) {

        if (pInformation->USBwValue1 == REPORT_DESCRIPTOR) {
            CopyRoutine = CustomHID_GetReportDescriptor;
        }
        else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE) {
            CopyRoutine = CustomHID_GetHIDDescriptor;
        }

    } // End of GET_DESCRIPTOR

    // GET_PROTOCOL
    else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
             && RequestNo == GET_PROTOCOL) {
        CopyRoutine = CustomHID_GetProtocolValue;
    }
    else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))//0x21 0x01
             && RequestNo == CLEAR_FEATURE) {
        CopyRoutine = CustomHID_GetProtocolValue;

    }

    else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))//0x21 0x09
             && RequestNo == SET_CONFIGURATION) {
        CopyRoutine = CustomHID_GetProtocolValue;

    }
    if (CopyRoutine == NULL) {
        return USB_UNSUPPORT;
    }

    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    (*CopyRoutine)(0);
    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  handle the no data class specific requests.
/// @param  Request Nb.
/// @retval USB_UNSUPPORT or USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT CustomHID_NoData_Setup(u8 RequestNo)
{
    if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
            && (RequestNo == SET_PROTOCOL)) {
        return CustomHID_SetProtocol();
    }

    else {
        return USB_UNSUPPORT;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the device descriptor.
/// @param  Length.
/// @retval The address of the device descriptor.
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_GetDeviceDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the configuration descriptor.
/// @param  Length.
/// @retval The address of the configuration descriptor.
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_GetConfigDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the string descriptors according to the needed index.
/// @param  Length.
/// @retval The address of the string descriptors.
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_GetStringDescriptor(u16 Length)
{
    u8 wValue0 = pInformation->USBwValue0;
    if (wValue0 > 4) {
        return NULL;
    }
    else {
        return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the HID report descriptor.
/// @param  Lnegth.
/// @retval The address of the configuration descriptor..
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_GetReportDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &CustomHID_Report_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the HID descriptor.
/// @param  Length.
/// @retval The address of the configuration descriptor.
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_GetHIDDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &CustomHID_Hid_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  tests the interface and the alternate setting according to the
///                supported one..
/// @param  - Interface : interface number.
///         - AlternateSetting : Alternate Setting number..
/// @retval USB_SUCCESS or USB_UNSUPPORT..
////////////////////////////////////////////////////////////////////////////////
RESULT CustomHID_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
//  if (AlternateSetting > 0)
//  {
//    return USB_UNSUPPORT;
//  }
//  else if (Interface > 0)
//  {
//    return USB_UNSUPPORT;
//  }
    return USB_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Joystick Set Protocol request routine.
/// @param  None.
/// @retval USB SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT CustomHID_SetProtocol(void)
{
    u8 wValue0 = pInformation->USBwValue0;
    ProtocolValue = wValue0;
    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  get the protocol value.
/// @param  Length.
/// @retval address of the protcol value.
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_GetProtocolValue(u16 Length)
{
    if (Length == 0) {
        pInformation->Ctrl_Info.Usb_wLength = 1;
        return NULL;
    }
    else {
        return (u8*)(&ProtocolValue);
    }
}

/// @}

/// @}

/// @}

