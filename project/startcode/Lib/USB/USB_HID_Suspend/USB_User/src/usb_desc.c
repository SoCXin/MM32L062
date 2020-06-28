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
/// @addtogroup USB_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

// USB Standard Device Descriptor
const u8 CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] = {
    0x12,                       //bLength
    USB_DEVICE_DESCRIPTOR_TYPE, //bDescriptorType
    0x00,                       //bcdUSB
    0x02,
    0x00,                       //bDeviceClass
    0x00,                       //bDeviceSubClass
    0x00,                       //bDeviceProtocol
    0x40,                       //bMaxPacketSize40
    0x81,                       //idVendor (0x2F81)
    0x2F,
    0x21,                       //idProduct = 0x5721
    0x57,
    0x00,                       //bcdDevice rel. 2.00
    0x02,
    1,                          //Index of string descriptor describing
    //  manufacturer
    2,                          //Index of string descriptor describing
    //   product
    3,                          //Index of string descriptor describing the
    //  device serial number
    0x01                        //bNumConfigurations
}
; // CustomHID_DeviceDescriptor


// USB Configuration Descriptor
//   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor
const u8 CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] = {
    0x09, // bLength: Configuation Descriptor size
    USB_CONFIGURATION_DESCRIPTOR_TYPE, // bDescriptorType: Configuration
    CUSTOMHID_SIZ_CONFIG_DESC,
    // wTotalLength: Bytes returned
    0x00,
    0x01,         // bNumInterfaces: 1 interface
    0x01,         // bConfigurationValue: Configuration value
    0x00,         // iConfiguration: Index of string descriptor describing
    //the configuration
    0xC0,         // bmAttributes: Bus powered
    0x32,         // MaxPower 100 mA: this current is used for detecting Vbus

    ///************ Descriptor of Custom HID interface ***************
    // 09
    0x09,         // bLength: Interface Descriptor size
    USB_INTERFACE_DESCRIPTOR_TYPE,// bDescriptorType: Interface descriptor type
    0x00,         // bInterfaceNumber: Number of Interface
    0x00,         // bAlternateSetting: Alternate setting
    0x02,         // bNumEndpoints
    0x03,         // bInterfaceClass: HID
    0x00,         // bInterfaceSubClass : 1=BOOT, 0=no boot
    0x00,         // nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
    0,            // iInterface: Index of string descriptor
    ///****************** Descriptor of Custom HID HID *******************
    // 18
    0x09,         // bLength: HID Descriptor size
    HID_DESCRIPTOR_TYPE, // bDescriptorType: HID
    0x10,         // bcdHID: HID Class Spec release number
    0x01,
    0x00,         // bCountryCode: Hardware target country
    0x01,         // bNumDescriptors: Number of HID class descriptors to follow
    0x22,         // bDescriptorType
    CUSTOMHID_SIZ_REPORT_DESC,// wItemLength: Total length of Report descriptor
    0x00,
    ///****************** Descriptor of Custom HID endpoints *****************
    // 27
    0x07,          // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE, // bDescriptorType:

    0x81,          // bEndpointAddress: Endpoint Address (IN)
    0x03,          // bmAttributes: Interrupt endpoint
    0x40,          // wMaxPacketSize: 64 Bytes max
    0x00,
    0x02,          // bInterval: Polling Interval (32 ms)
    // 34

    0x07,   // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE,   // bDescriptorType:
    //  Endpoint descriptor type
    0x03,   // bEndpointAddress:
    //  Endpoint Address (OUT)
    0x03,   // bmAttributes: Interrupt endpoint
    0x40,   // wMaxPacketSize: 2 Bytes max
    0x00,
    0x02,   // bInterval: Polling Interval (20 ms)
    // 41
}
; // CustomHID_ConfigDescriptor


const u8 CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] = {
    0x05, 0x8c, // USAGE_PAGE (ST Page)
    0x09, 0x01, // USAGE (Demo Kit)
    0xa1, 0x01, // COLLECTION (Application)

    // The Input report
    0x09, 0x03, // USAGE ID - Vendor defined
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x26, 0x00, 0xFF, // LOGICAL_MAXIMUM (255)
    0x75, 0x08, // REPORT_SIZE (8bit)
    0x95, 0x40, // REPORT_COUNT (64Byte)
    0x81, 0x02, // INPUT (Data,Var,Abs)

    // The Output report
    0x09, 0x04, // USAGE ID - Vendor defined
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x26, 0x00, 0xFF, // LOGICAL_MAXIMUM (255)
    0x75, 0x08, // REPORT_SIZE (8bit)
    0x95, 0x40, // REPORT_COUNT (64Byte)
    0x91, 0x02, // OUTPUT (Data,Var,Abs)

    0xc0 // END_COLLECTION
}; // CustomHID_ReportDescriptor



// USB String Descriptors (optional)
const u8 CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] = {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
}
; // LangID = 0x0409: U.S. English

const u8 CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] = {
    CUSTOMHID_SIZ_STRING_VENDOR, // Size of Vendor string
    USB_STRING_DESCRIPTOR_TYPE,  // bDescriptorType
    // Manufacturer: "MM32"
    'H', 0,
    'E', 0,
    'L', 0,
    'L', 0,
    'O', 0,
    ' ', 0,
    'U', 0,
    'S', 0,
    'B', 0,
    '!', 0,
    ' ', 0,
    ' ', 0,
    ' ', 0,
    ' ', 0,
    ' ', 0,
    'i', 0,
    ' ', 0,
    ' ', 0
};

const u8 CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] = {
    CUSTOMHID_SIZ_STRING_PRODUCT,          // bLength
    USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
    'M', 0,
    'M', 0,
    '3', 0,
    '2', 0,
    ' ', 0,
    ' ', 0,
    'C', 0,
    'u', 0,
    's', 0,
    't', 0,
    'm', 0,
    ' ', 0,
    'H', 0,
    'I', 0,
    'D', 0
};
u8 CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] = {
    CUSTOMHID_SIZ_STRING_SERIAL,           // bLength
    USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
    'M', 0,
    'M', 0,
    '3', 0,
    '2', 0,
    ' ', 0,
    ' ', 0,
    ' ', 0
};

/// @}

/// @}

/// @}

