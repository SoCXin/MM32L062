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
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "usb_core.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

u8 Request = 0;

LINE_CODING linecoding = {
    9600, // baud rate
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
    Virtual_Com_Port_init,
    Virtual_Com_Port_Reset,
    Virtual_Com_Port_Status_In,
    Virtual_Com_Port_Status_Out,
    Virtual_Com_Port_Data_Setup,
    Virtual_Com_Port_NoData_Setup,
    Virtual_Com_Port_Get_Interface_Setting,
    Virtual_Com_Port_GetDeviceDescriptor,
    Virtual_Com_Port_GetConfigDescriptor,
    Virtual_Com_Port_GetStringDescriptor,
    0,
    0x40 //MAX PACKET SIZE
};

USER_STANDARD_REQUESTS User_Standard_Requests = {
    Virtual_Com_Port_GetConfiguration,
    Virtual_Com_Port_SetConfiguration,
    Virtual_Com_Port_GetInterface,
    Virtual_Com_Port_SetInterface,
    Virtual_Com_Port_GetStatus,
    Virtual_Com_Port_ClearFeature,
    Virtual_Com_Port_SetEndPointFeature,
    Virtual_Com_Port_SetDeviceFeature,
    Virtual_Com_Port_SetDeviceAddress
};

ONE_DESCRIPTOR Device_Descriptor = {
    (u8*)Virtual_Com_Port_DeviceDescriptor,
    VIRTUAL_COM_PORT_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor = {
    (u8*)Virtual_Com_Port_ConfigDescriptor,
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR String_Descriptor[4] = {
    {(u8*)Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID},
    {(u8*)Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR},
    {(u8*)Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT},
    {(u8*)Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL}
};


////////////////////////////////////////////////////////////////////////////////
/// @brief  Custom HID init routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Virtual_Com_Port_init(void)
{
    // Update the serial number string descriptor with the data from the unique ID

//  Get_SerialNum();
    pInformation->Current_Configuration = 0;
    // Connect the device
    PowerOn();
    // USB interrupts initialization

    _ClrUSB_INT_STA(0xff);

    // configure the USART 1 to the default settings

//  USART_Config_Default();
    bDeviceState = UNCONNECTED;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Custom HID reset routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Virtual_Com_Port_Reset(void)
{
    // Set Virtual_Com_Port DEVICE as not configured
    pInformation->Current_Configuration = 0;

    // Current Feature initialization
    pInformation->Current_Feature = Virtual_Com_Port_ConfigDescriptor[7];

    // Set Virtual_Com_Port DEVICE with the default Interface
    pInformation->Current_Interface = 0;


    _ClrEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));

    _SetEP0_INT_EN( EPn_INT_EN_SETUPIE | EPn_INT_EN_INNACKIE\
                    | EPn_INT_EN_OUTACKIE); //EPn_INT_EN_INACKIE|EPn_INT_EN_OUTACKIE
    _ClrEP0_INT_STA(EPn_INT_STATE_SETUP | EPn_INT_STATE_INNACK | \
                    EPn_INT_EN_INACKIE | EPn_INT_STATE_OUTACK);


    _SetEP1_INT_EN( EPn_INT_EN_INNACKIE );//| EPn_INT_EN_INACKIE
    _ClrEP1_INT_STA(EPn_INT_STATE_INNACK);//| EPn_INT_STATE_INACK


    _SetEP2_INT_EN(EPn_INT_EN_INNACKIE);//| EPn_INT_EN_INACKIE
    _ClrEP2_INT_STA(EPn_INT_EN_INNACKIE);//| EPn_INT_EN_INACKIE


    _SetEP3_INT_EN( EPn_INT_EN_OUTACKIE);
    _ClrEP3_INT_STA( EPn_INT_STATE_OUTACK);

    _SetEP_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));
    _SetEP_INT_EN((1 << ENDP0) | (1 << ENDP1) | (1 << ENDP2) | (1 << ENDP3));

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
void Virtual_Com_Port_SetConfiguration(void)
{
    DEVICE_INFO* pInfo = &Device_Info;

    if (pInfo->Current_Configuration != 0) {
        // Device configured
        bDeviceState = CONFIGURED;
    }
}
void Virtual_Com_Port_SetDeviceAddress (void)
{
    bDeviceState = ADDRESSED;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Udpade the device state to addressed.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Virtual_Com_Port_Status_In (void)
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
        UART_Config();
        Request = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Joystick status OUT routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Virtual_Com_Port_Status_Out(void)
{}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Handle the data class specific requests.
/// @param Request Nb.
/// @retval USB_UNSUPPORT or USB_SUCCESS.
////////////////////////////////////////////////////////////////////////////////

RESULT Virtual_Com_Port_Data_Setup(u8 RequestNo)
{
    u8*    (*CopyRoutine)(u16);

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
RESULT Virtual_Com_Port_NoData_Setup(u8 RequestNo)
{

    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) {
        if (RequestNo == SET_COMM_FEATURE) {
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
u8* Virtual_Com_Port_GetDeviceDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the configuration descriptor.
/// @param  Length.
/// @retval The address of the configuration descriptor.
////////////////////////////////////////////////////////////////////////////////
u8* Virtual_Com_Port_GetConfigDescriptor(u16 Length)
{
    return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Gets the string descriptors according to the needed index.
/// @param  Length.
/// @retval The address of the string descriptors.
////////////////////////////////////////////////////////////////////////////////
u8* Virtual_Com_Port_GetStringDescriptor(u16 Length)
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
/// @brief  test the interface and the alternate setting according to the
///          supported one.
/// @param  u8: Interface : interface number.
///        : u8: AlternateSetting : Alternate Setting number..
/// @retval The address of the string descriptors.
////////////////////////////////////////////////////////////////////////////////
RESULT Virtual_Com_Port_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
    if (AlternateSetting > 0) {
        return USB_UNSUPPORT;
    }
    else if (Interface > 1) {
        return USB_UNSUPPORT;
    }
    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  send the linecoding structure to the PC host.
/// @param  Length
/// @retval Inecoding structure base address
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
/// @brief  Set the linecoding structure fields.
/// @param  Length
/// @retval Linecoding structure base address.
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

