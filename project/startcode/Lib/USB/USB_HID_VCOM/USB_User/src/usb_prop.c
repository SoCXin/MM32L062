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

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"


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
__IO u8 Request = 0;
u8 Report_Buf[2];


LINE_CODING linecoding = {
    115200, // baud rate
    0x00,   // stop bits-1
    0x00,   // parity - none
    0x08    // no. of bits 8
};

// --------------------------------------------------------------------------
//  Structures initializations
// --------------------------------------------------------------------------

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
    CustomHID_Get_String,  //mm32
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

ONE_DESCRIPTOR CustomHID_Hid_Descriptor = { ////////////////////////////////////////////////////////////////////////////
    (u8*)CustomHID_ConfigDescriptor + CUSTOMHID_OFF_HID_DESC,
    CUSTOMHID_SIZ_HID_DESC
};

ONE_DESCRIPTOR String_Descriptor[4] = {
    {(u8*)CustomHID_StringLangID, CUSTOMHID_SIZ_STRING_LANGID},
    {(u8*)CustomHID_StringVendor, CUSTOMHID_SIZ_STRING_VENDOR},
    {(u8*)CustomHID_StringProduct, CUSTOMHID_SIZ_STRING_PRODUCT},
    {(u8*)CustomHID_StringSerial, CUSTOMHID_SIZ_STRING_SERIAL}
};

ONE_DESCRIPTOR CustomHID_String[5] = {
    {(u8*)CustomHID_String_0, CUSTOMHID_SIZ_STRING},
    {(u8*)CustomHID_String_1, CUSTOMHID_SIZ_STRING},
    {(u8*)CustomHID_String_2, CUSTOMHID_SIZ_STRING},
    {(u8*)CustomHID_String_3, CUSTOMHID_SIZ_STRING},
    {(u8*)CustomHID_String_4, CUSTOMHID_SIZ_STRING},
};



u8* CustomHID_SetReport_Feature(u16 Length);

////////////////////////////////////////////////////////////////////////////////
/// @brief  Custom HID init routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void CustomHID_init(void)
{
    // Update the serial number string descriptor with the data from the unique
    //ID
    Get_SerialNum();

    pInformation->Current_Configuration = 0;
    // Connect the device
    PowerOn();

//   USB interrupts initialization

//  _SetISTR(0);
//   clear pending interrupts

    bDeviceState = UNCONNECTED;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Custom HID reset routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CustomHID_Reset(void)
{

    // Set Joystick_DEVICE as not configured
    pInformation->Current_Configuration = 0;
    pInformation->Current_Interface = 0;//the default Interface

    // Current Feature initialization
    pInformation->Current_Feature = CustomHID_ConfigDescriptor[7];

    // Set this device to response on default address


    _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));


    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INACKIE | \
                    EPn_INT_EN_OUTACKIE/*|EPn_INT_EN_OUTACKIE*/);             //EPn_INT_EN_INACKIE|

    _ClrEP0_INT_STA(EPn_INT_STATE_SETUP | EPn_INT_STATE_INACK | \
                    EPn_INT_EN_INACKIE | EPn_INT_STATE_OUTACK);

    //VCOM EP

    _SetEP1_INT_EN( EPn_INT_EN_INNACKIE );//| EPn_INT_EN_INACKIE
    _ClrEP1_INT_STA(EPn_INT_STATE_INNACK);//| EPn_INT_STATE_INACK


    _SetEP2_INT_EN(EPn_INT_EN_INNACKIE);//| EPn_INT_EN_INACKIE
    _ClrEP2_INT_STA(EPn_INT_EN_INNACKIE);//| EPn_INT_EN_INACKIE


    _SetEP3_INT_EN( EPn_INT_EN_OUTACKIE);
    _ClrEP3_INT_STA( EPn_INT_STATE_OUTACK);

    //HID  EP4 IN  OUT

    _SetEP4_INT_EN( EPn_INT_EN_INNACKIE );//| EPn_INT_EN_INACKIE
    _ClrEP4_INT_STA(EPn_INT_STATE_INNACK | EPn_INT_STATE_INACK);


    _SetEP4_INT_EN( EPn_INT_EN_OUTNACKIE );//| EPn_INT_EN_INACKIE
    _ClrEP4_INT_STA(EPn_INT_STATE_OUTNACK | EPn_INT_STATE_OUTACK);

    _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));
    _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3) | (1 << ENDP4));

    // Set this device to response on default address


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
void CustomHID_SetConfiguration(void)
{
    if (pInformation->Current_Configuration != 0) {
        // Device configured
        bDeviceState = CONFIGURED;

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
    if (Request == SET_LINE_CODING) {

        //USART_Config();
        Request = 0;
    }
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

//  if (pInformation->USBwIndex != 0) ???????????????????????????????????????????????????
//    return USB_UNSUPPORT;

    CopyRoutine = NULL;

    if (RequestNo == GET_LINE_CODING) {
        if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) {
            CopyRoutine = Virtual_Com_Port_GetLineCoding;
        }
    }
    else if (RequestNo == SET_LINE_CODING) {
        if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) {
            CopyRoutine = Virtual_Com_Port_SetLineCoding;
        }
        Request = SET_LINE_CODING;
    }

    else if (RequestNo == GET_DESCRIPTOR) {
        if(Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT)) {
            if (pInformation->USBwValue1 == REPORT_DESCRIPTOR) {
                CopyRoutine = CustomHID_GetReportDescriptor;
            }
            else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE) {
                CopyRoutine = CustomHID_GetHIDDescriptor;
            }
        }
    }


    // GET_PROTOCOL, GET_REPORT, SET_REPORT
    else if ( (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) ) {
        switch( RequestNo ) {
            case GET_PROTOCOL:
                CopyRoutine = CustomHID_GetProtocolValue;
                break;
            case SET_REPORT:
                CopyRoutine = CustomHID_SetReport_Feature;
                Request = SET_REPORT;
                break;
            default:
                break;
        }
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
/// @brief     Set Feature request handling
/// @param     Length.
/// @retval    Buffer
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_SetReport_Feature(u16 Length)
{
    if (Length == 0) {
        pInformation->Ctrl_Info.Usb_wLength = 2;
        return NULL;
    }
    else {
        return Report_Buf;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  handle the no data class specific requests.
/// @param  Request Nb.
/// @retval USB_UNSUPPORT or USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////
RESULT CustomHID_NoData_Setup(u8 RequestNo)
{
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) {
        if(RequestNo == SET_PROTOCOL) {
            return CustomHID_SetProtocol();
        }
        else if(RequestNo == SET_COMM_FEATURE) {
            return USB_SUCCESS;
        }
        else if (RequestNo == SET_CONTROL_LINE_STATE) {
            return USB_SUCCESS;
        }
    }

    return USB_UNSUPPORT;
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
/// @brief    Gets the string descriptors according to the needed index
/// @param    Length
/// @retval   The address of the string descriptors.
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_Get_String(u16 Length)
{
    u8 wValue0 = pInformation->USBbRequest;
    wValue0 = (wValue0 & 0x7F);
    //if (wValue0 > 3)
    {
        // return NULL;
    }
    //else
    {
        return Standard_GetDescriptorData(Length, &CustomHID_String[wValue0]);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Gets the HID report descriptor.
/// @param    Length
/// @retval   The address of the configuration descriptor.
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_GetReportDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &CustomHID_Report_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Gets the HID descriptor.
/// @param    Length
/// @retval   The address of the configuration descriptor.
////////////////////////////////////////////////////////////////////////////////
u8* CustomHID_GetHIDDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &CustomHID_Hid_Descriptor);
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief    tests the interface and the alternate setting according to the
///           supported one.
/// @param    Interface : interface number.
/// @param    AlternateSetting : Alternate Setting number.
/// @retval   USB_SUCCESS or USB_UNSUPPORT.
////////////////////////////////////////////////////////////////////////////////
RESULT CustomHID_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)  //mm32
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


////////////////////////////////////////////////////////////////////////////////
/// @brief     send the linecoding structure to the PC host.
/// @param     Length.
/// @retval    Linecoding structure base address.
////////////////////////////////////////////////////////////////////////////////
u8* Virtual_Com_Port_GetLineCoding(u16 Length)
{
    if (Length == 0) {
        pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
        return NULL;
    }
    return(u8*)&linecoding;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Set the linecoding structure fields.
/// @param    Length.
/// @retval   Linecoding structure base address.
////////////////////////////////////////////////////////////////////////////////
u8* Virtual_Com_Port_SetLineCoding(u16 Length)
{
    if (Length == 0) {
        pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
        return NULL;
    }
    return(u8*)&linecoding;
}

/// @}
/// @}

/// @}
