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
#include "hal_adc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_PROP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

u32 ProtocolValue;

// --------------------------------------------------------------------------
//  Structures initializations
// --------------------------------------------------------------------------

DEVICE Device_Table = {
    EP_NUM,
    1
};

DEVICE_PROP Device_Property = {
    Printer_init,
    Printer_Reset,
    Printer_Status_In,
    Printer_Status_Out,
    Printer_Data_Setup,
    Printer_NoData_Setup,
    Printer_Get_Interface_Setting,
    Printer_GetDeviceDescriptor,
    Printer_GetConfigDescriptor,
    Printer_GetStringDescriptor,
    0,
    0x40 //MAX PACKET SIZE
};
USER_STANDARD_REQUESTS User_Standard_Requests = {
    Printer_GetConfiguration,
    Printer_SetConfiguration,
    Printer_GetInterface,
    Printer_SetInterface,
    Printer_GetStatus,
    Printer_ClearFeature,
    Printer_SetEndPointFeature,
    Printer_SetDeviceFeature,
    Printer_SetDeviceAddress
};

ONE_DESCRIPTOR Device_Descriptor = {
    (u8*)Printer_DeviceDescriptor,
    Printer_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor = {
    (u8*)Printer_ConfigDescriptor,
    Printer_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR Printer_Report_Descriptor = {
    (u8*)Printer_ReportDescriptor,
    Printer_SIZ_REPORT_DESC
};

ONE_DESCRIPTOR Printer_Hid_Descriptor = {
    (u8*)Printer_ConfigDescriptor + Printer_OFF_HID_DESC,
    Printer_SIZ_HID_DESC
};

ONE_DESCRIPTOR String_Descriptor[4] = {
    {(u8*)Printer_StringLangID, Printer_SIZ_STRING_LANGID},
    {(u8*)Printer_StringVendor, Printer_SIZ_STRING_VENDOR},
    {(u8*)Printer_StringProduct, Printer_SIZ_STRING_PRODUCT},
    {(u8*)Printer_StringSerial, Printer_SIZ_STRING_SERIAL}
};

////////////////////////////////////////////////////////////////////////////////
/// @brief  Custom HID init routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void Printer_init(void)
{
    // Update the serial number string descriptor with the data from the unique ID

//  Get_SerialNum();

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

void Printer_Reset(void)
{
    // Set Joystick_DEVICE as not configured
    pInformation->Current_Configuration = 0;
    pInformation->Current_Interface = 0;//the default Interface

    // Current Feature initialization
    pInformation->Current_Feature = Printer_ConfigDescriptor[7];

    // Set this device to response on default address

    _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2));

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE | \
                    EPn_INT_EN_OUTACKIE);//|EPn_INT_EN_OUTACKIE );             //EPn_INT_EN_INACKIE|

    _ClrEP0_INT_STA(EPn_INT_STATE_SETUP | EPn_INT_STATE_INNACK | \
                    EPn_INT_EN_INACKIE | EPn_INT_STATE_OUTACK);


    _SetEP1_INT_EN( EPn_INT_EN_INNACKIE );                         //| EPn_INT_EN_INACKIE
    _ClrEP1_INT_STA(EPn_INT_STATE_INNACK);//| EPn_INT_STATE_INACK );


    _SetEP2_INT_EN(EPn_INT_EN_INNACKIE);                           //| EPn_INT_EN_INACKIE
    _ClrEP2_INT_STA(EPn_INT_EN_INNACKIE);//| EPn_INT_EN_INACKIE );

    _SetEP3_INT_EN(EPn_INT_EN_INNACKIE);                           //| EPn_INT_EN_INACKIE
    _ClrEP3_INT_STA(EPn_INT_EN_INNACKIE);//| EPn_INT_EN_INACKIE );

    _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));
    _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2));

    SetDeviceAddress(0);

    _SetUSB_TOP(USB_TOP_RESET);
    _ClrUSB_TOP(USB_TOP_RESET);

    bDeviceState = ATTACHED;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Udpade the device state to configured and command the ADC
///         conversion.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void Printer_SetConfiguration(void)
{
    if (pInformation->Current_Configuration != 0) {
        // Device configured
        bDeviceState = CONFIGURED;

        // Start ADC1 Software Conversion
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Udpade the device state to addressed.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Printer_SetDeviceAddress (void)
{
    bDeviceState = ADDRESSED;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Udpade the device state to addressed.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Printer_Status_In(void)
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Joystick status IN routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Printer_Status_Out (void)
{
}



////////////////////////////////////////////////////////////////////////////////
/// @brief  Handle the data class specific requests.
/// @param Request Nb.
/// @retval USB_UNSUPPORT or USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT Printer_Data_Setup(u8 RequestNo)
{
    u8* (*CopyRoutine)(u16);

    CopyRoutine = NULL;

    if ((RequestNo == GET_DESCRIPTOR)
            && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
            && (pInformation->USBwIndex0 == 0)) {

        if (pInformation->USBwValue1 == REPORT_DESCRIPTOR) {
            CopyRoutine = Printer_GetReportDescriptor;
        }
        else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE) {
            CopyRoutine = Printer_GetHIDDescriptor;
        }

    } // End of GET_DESCRIPTOR

    // GET_PROTOCOL
    else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
             && RequestNo == GET_PROTOCOL) {
        CopyRoutine = Printer_GetProtocolValue;
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
RESULT Printer_NoData_Setup(u8 RequestNo)
{
    if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
            && (RequestNo == SET_PROTOCOL)) {
        return Printer_SetProtocol();
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
u8* Printer_GetDeviceDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the configuration descriptor.
/// @param  Length.
/// @retval The address of the configuration descriptor.
////////////////////////////////////////////////////////////////////////////////
u8* Printer_GetConfigDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the string descriptors according to the needed index.
/// @param  Length.
/// @retval The address of the string descriptors.
////////////////////////////////////////////////////////////////////////////////
u8* Printer_GetStringDescriptor(u16 Length)
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
/// @brief   Gets the HID report descriptor.
/// @param  Length.
/// @retval The address of the string descriptors.
////////////////////////////////////////////////////////////////////////////////
u8* Printer_GetReportDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &Printer_Report_Descriptor);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief    Gets the HID descriptor.
/// @param  Length.
/// @retval The address of the string descriptors.
////////////////////////////////////////////////////////////////////////////////
u8* Printer_GetHIDDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &Printer_Hid_Descriptor);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  test the interface and the alternate setting according to the
///          supported one.
/// @param  u8: Interface : interface number.
///        : u8: AlternateSetting : Alternate Setting number..
/// @retval The address of the string descriptors.
////////////////////////////////////////////////////////////////////////////////
RESULT Printer_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
    if (AlternateSetting > 0) {
        return USB_UNSUPPORT;
    }
    else if (Interface > 0) {
        return USB_UNSUPPORT;
    }
    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  send the linecoding structure to the PC host.
/// @param  Length
/// @retval Inecoding structure base address
////////////////////////////////////////////////////////////////////////////////
RESULT Printer_SetProtocol(void)
{
    u8 wValue0 = pInformation->USBwValue0;
    ProtocolValue = wValue0;
    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set the linecoding structure fields.
/// @param  Length
/// @retval Linecoding structure base address.
////////////////////////////////////////////////////////////////////////////////
u8* Printer_GetProtocolValue(u16 Length)
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
