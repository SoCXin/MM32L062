////////////////////////////////////////////////////////////////////////////////
/// @file     usb_desc.C
/// @author   AE TEAM
/// @brief    Descriptors for Custom HID Demo.
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
#define _USB_DESC_C_

// Files includes
#include "usb_lib.h"
#include "usb_desc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_DESC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

// USB Standard Device Descriptor
const u8 Virtual_Com_Port_DeviceDescriptor[] = {
    0x12,   // bLength
    USB_DEVICE_DESCRIPTOR_TYPE,     // bDescriptorType
    0x10,
    0x01,   // bcdUSB = 2.00
    0x02,   // bDeviceClass: CDC
    0x00,   // bDeviceSubClass
    0x00,   // bDeviceProtocol
    0x40,   // bMaxPacketSize0
    0x81,
    0x2F,   // idVendor = 0x2F81
    0x09,
    0x72,   // idProduct = 0x7209
    0x00,
    0x02,   // bcdDevice = 2.00
    1,              // Index of string descriptor describing manufacturer
    2,              // Index of string descriptor describing product
    3,              // Index of string descriptor describing the device's serial number
    0x01    // bNumConfigurations
};

const u8 Virtual_Com_Port_ConfigDescriptor[] = {
    //Configuation Descriptor
    0x09,   // bLength: Configuation Descriptor size
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      // bDescriptorType: Configuration
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,       // wTotalLength:no of returned bytes
    0x00,
    0x02,   // bNumInterfaces: 2 interface
    0x01,   // bConfigurationValue: Configuration value
    0x00,   // iConfiguration: Index of string descriptor describing the configuration
    0xC0,   // bmAttributes: self powered
    0x32,   // MaxPower 0 mA
    //Interface Descriptor
    0x09,   // bLength: Interface Descriptor size
    USB_INTERFACE_DESCRIPTOR_TYPE,  // bDescriptorType: Interface
    // Interface descriptor type
    0x00,   // bInterfaceNumber: Number of Interface
    0x00,   // bAlternateSetting: Alternate setting
    0x01,   // bNumEndpoints: One endpoints used
    0x02,   // bInterfaceClass: Communication Interface Class
    0x02,   // bInterfaceSubClass: Abstract Control Model
    0x01,   // bInterfaceProtocol: Common AT commands
    0x00,   // iInterface:
    //Header Functional Descriptor
    0x05,   // bLength: Endpoint Descriptor size
    0x24,   // bDescriptorType: CS_INTERFACE
    0x00,   // bDescriptorSubtype: Header Func Desc
    0x10,   // bcdCDC: spec release number
    0x01,
    //Call Managment Functional Descriptor
    0x05,   // bFunctionLength
    0x24,   // bDescriptorType: CS_INTERFACE
    0x01,   // bDescriptorSubtype: Call Management Func Desc
    0x00,   // bmCapabilities: D0+D1
    0x00,   // bDataInterface: 1
    //ACM Functional Descriptor
    0x04,   // bFunctionLength
    0x24,   // bDescriptorType: CS_INTERFACE
    0x02,   // bDescriptorSubtype: Abstract Control Management desc
    0x02,   // bmCapabilities
    //Union Functional Descriptor
    0x05,   // bFunctionLength
    0x24,   // bDescriptorType: CS_INTERFACE
    0x06,   // bDescriptorSubtype: Union func desc
    0x00,   // bMasterInterface: Communication class interface
    0x01,   // bSlaveInterface0: Data Class Interface
    //Endpoint 2 Descriptor
    0x07,   // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE,   // bDescriptorType: Endpoint
    0x82,   // bEndpointAddress: (IN2)
    0x03,   // bmAttributes: Interrupt
    VIRTUAL_COM_PORT_INT_SIZE,      // wMaxPacketSize:
    0x00,
    0xFF,   // bInterval:
    //Data class interface descriptor
    0x09,   // bLength: Endpoint Descriptor size
    USB_INTERFACE_DESCRIPTOR_TYPE,  // bDescriptorType:
    0x01,   // bInterfaceNumber: Number of Interface
    0x00,   // bAlternateSetting: Alternate setting
    0x02,   // bNumEndpoints: Two endpoints used
    0x0A,   // bInterfaceClass: CDC
    0x00,   // bInterfaceSubClass:
    0x00,   // bInterfaceProtocol:
    0x00,   // iInterface:
    //Endpoint 3 Descriptor
    0x07,   // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE,   // bDescriptorType: Endpoint
    0x03,   // bEndpointAddress: (OUT3)
    0x02,   // bmAttributes: Bulk
    VIRTUAL_COM_PORT_DATA_SIZE,             // wMaxPacketSize:
    0x00,
    0x00,   // bInterval: ignore for Bulk transfer
    //Endpoint 1 Descriptor
    0x07,   // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE,   // bDescriptorType: Endpoint
    0x81,   // bEndpointAddress: (IN1)
    0x02,   // bmAttributes: Bulk
    VIRTUAL_COM_PORT_DATA_SIZE,             // wMaxPacketSize:
    0x00,
    0x00    // bInterval
};

// USB String Descriptors
const u8 Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID] = {
    VIRTUAL_COM_PORT_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 // LangID = 0x0409: U.S. English
};

const u8 Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR] = {
    VIRTUAL_COM_PORT_SIZ_STRING_VENDOR,     // Size of Vendor string
    USB_STRING_DESCRIPTOR_TYPE,             // bDescriptorType
    // Manufacturer: "MindMotion"

    'M', 0,
    'i', 0,
    'n', 0,
    'd', 0,
    'M', 0,
    'o', 0,
    't', 0,
    'i', 0,
    'o', 0,
    'n', 0,

};

const u8 Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT] = {
    VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT,          // bLength
    USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
    // Product name: ""MindMotion  Virtual COM Port"

    'M', 0,
    'i', 0,
    'n', 0,
    'd', 0,
    'M', 0,
    'o', 0,
    't', 0,
    'i', 0,
    'o', 0,
    'n', 0,
    ' ', 0,
    'V', 0,
    'i', 0,
    'r', 0,
    't', 0,
    'u', 0,
    'a', 0,
    'l', 0,
    ' ', 0,
    'C', 0,
    'O', 0,
    'M', 0,
    ' ', 0,
    'P', 0,
    'o', 0,
    'r', 0,
    't', 0,
    ' ', 0,
    ' ', 0

};

u8 Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL] = {
    VIRTUAL_COM_PORT_SIZ_STRING_SERIAL,           // bLength
    USB_STRING_DESCRIPTOR_TYPE,                   // bDescriptorType


    'M', 0,
    'M', 0,
    '3', 0,
    '2', 0,
    '0', 0,
    '6', 0,
    '2', 0,
}
;
/// @}

/// @}

/// @}

