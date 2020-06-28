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

// Private typedef -----------------------------------------------------------
// Private define ------------------------------------------------------------
// Private macro -------------------------------------------------------------
// Private variables ---------------------------------------------------------
// Extern variables ----------------------------------------------------------
// Private function prototypes -----------------------------------------------
// Private functions ---------------------------------------------------------

// USB Standard Device Descriptor
const u8 Printer_DeviceDescriptor[Printer_SIZ_DEVICE_DESC] = {
//    0x12,                       //bLength
//    USB_DEVICE_DESCRIPTOR_TYPE, //bDescriptorType
//    0x00,                       //bcdUSB
//    0x02,
//    0x00,                       //bDeviceClass
//    0x00,                       //bDeviceSubClass
//    0x00,                       //bDeviceProtocol
//    0x40,                       //bMaxPacketSize40
//    0x83,                       //idVendor (0x0483)
//    0x04,
//    0x50,                       //idProduct = 0x5750
//    0x57,
//    0x00,                       //bcdDevice rel. 2.00
//    0x02,
//    1,                          //Index of string descriptor describing
//                                              manufacturer
//    2,                          //Index of string descriptor describing
//                                             product
//    3,                          //Index of string descriptor describing the
//                                             device serial number
//    0x01                        //bNumConfigurations
    0x12,
    0x01,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x85,
    0x04,
    0x41,
    0x75,
    0x00,
    0x03,
    0x01,
    0x02,
    0x03,
    0x01

}
; // Printer_DeviceDescriptor


// USB Configuration Descriptor
//   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor
const u8 Printer_ConfigDescriptor[Printer_SIZ_CONFIG_DESC] = {
//    0x09, // bLength: Configuation Descriptor size
//    USB_CONFIGURATION_DESCRIPTOR_TYPE, // bDescriptorType: Configuration
//    Printer_SIZ_CONFIG_DESC,
//    // wTotalLength: Bytes returned
//    0x00,
//    0x01,         // bNumInterfaces: 1 interface
//    0x01,         // bConfigurationValue: Configuration value
//    0x00,         // iConfiguration: Index of string descriptor describing
//                                 the configuration
//    0xC0,         // bmAttributes: Bus powered
//    0x32,         // MaxPower 100 mA: this current is used for detecting Vbus

//    //  Descriptor of Custom HID interface
//    // 09
//    0x09,         // bLength: Interface Descriptor size
//    USB_INTERFACE_DESCRIPTOR_TYPE,// bDescriptorType: Interface descriptor type
//    0x00,         // bInterfaceNumber: Number of Interface
//    0x00,         // bAlternateSetting: Alternate setting
//    0x02,         // bNumEndpoints
//    0x03,         // bInterfaceClass: HID
//    0x00,         // bInterfaceSubClass : 1=BOOT, 0=no boot
//    0x00,         // nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
//    0,            // iInterface: Index of string descriptor
//    //  Descriptor of Custom HID HID
//    // 18
//    0x09,         // bLength: HID Descriptor size
//    HID_DESCRIPTOR_TYPE, // bDescriptorType: HID
//    0x10,         // bcdHID: HID Class Spec release number
//    0x01,
//    0x00,         // bCountryCode: Hardware target country
//    0x01,         // bNumDescriptors: Number of HID class descriptors to follow
//    0x22,         // bDescriptorType
//    Printer_SIZ_REPORT_DESC,// wItemLength: Total length of Report descriptor
//    0x00,
//    //  Descriptor of Custom HID endpoints
//    // 27
//    0x07,          // bLength: Endpoint Descriptor size
//    USB_ENDPOINT_DESCRIPTOR_TYPE, // bDescriptorType:

//    0x81,          // bEndpointAddress: Endpoint Address (IN)
//    0x03,          // bmAttributes: Interrupt endpoint
//    0x40,          // wMaxPacketSize: 64 Bytes max
//    0x00,
//    0x02,          // bInterval: Polling Interval (32 ms)
//    // 34
//
//    0x07, // bLength: Endpoint Descriptor size
//    USB_ENDPOINT_DESCRIPTOR_TYPE, // bDescriptorType:
//          //  Endpoint descriptor type
//    0x03, // bEndpointAddress:
//          //  Endpoint Address (OUT)
//    0x03, // bmAttributes: Interrupt endpoint
//    0x40, // wMaxPacketSize: 2 Bytes max
//    0x00,
//    0x02, // bInterval: Polling Interval (20 ms)
//    // 41
    0x09,
    0x02,
    0x20,
    0x00,
    0x01,
    0x01,
    0x05,
    0xc0,
    0x32,
    0x09,
    0x04,
    0x00,
    0x00,
    0x02,
    0x07,
    0x01,
    0x02, 0x00, 0x07, 0x05, 0x03, 0x02, 0x40, 0x00, 0x00, 0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00
}
; // Printer_ConfigDescriptor
const u8 Printer_ReportDescriptor[Printer_SIZ_REPORT_DESC] = {
    0x05, 0x8c,            // USAGE_PAGE (ST Page)
    0x09, 0x01,            // USAGE (Demo Kit)
    0xa1, 0x01,            // COLLECTION (Application)
    // 6

    // Led 1
    0x85, 0x01,            //     REPORT_ID (1)
    0x09, 0x01,            //     USAGE (LED 1)
    0x15, 0x00,            //     LOGICAL_MINIMUM (0)
    0x25, 0x01,            //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,            //     REPORT_SIZE (8)
    0x95, 0x01,            //     REPORT_COUNT (1)
    0xB1, 0x82,             //    FEATURE (Data,Var,Abs,Vol)

    0x85, 0x01,            //     REPORT_ID (1)
    0x09, 0x01,            //     USAGE (LED 1)
    0x91, 0x82,            //     OUTPUT (Data,Var,Abs,Vol)
    // 26

    // Led 2
    0x85, 0x02,            //     REPORT_ID 2
    0x09, 0x02,            //     USAGE (LED 2)
    0x15, 0x00,            //     LOGICAL_MINIMUM (0)
    0x25, 0x01,            //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,            //     REPORT_SIZE (8)
    0x95, 0x01,            //     REPORT_COUNT (1)
    0xB1, 0x82,             //    FEATURE (Data,Var,Abs,Vol)

    0x85, 0x02,            //     REPORT_ID (2)
    0x09, 0x02,            //     USAGE (LED 2)
    0x91, 0x82,            //     OUTPUT (Data,Var,Abs,Vol)
    // 46

    // Led 3
    0x85, 0x03,            //     REPORT_ID (3)
    0x09, 0x03,            //     USAGE (LED 3)
    0x15, 0x00,            //     LOGICAL_MINIMUM (0)
    0x25, 0x01,            //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,            //     REPORT_SIZE (8)
    0x95, 0x01,            //     REPORT_COUNT (1)
    0xB1, 0x82,             //    FEATURE (Data,Var,Abs,Vol)

    0x85, 0x03,            //     REPORT_ID (3)
    0x09, 0x03,            //     USAGE (LED 3)
    0x91, 0x82,            //     OUTPUT (Data,Var,Abs,Vol)
    // 66

    // Led 4
    0x85, 0x04,            //     REPORT_ID 4)
    0x09, 0x04,            //     USAGE (LED 4)
    0x15, 0x00,            //     LOGICAL_MINIMUM (0)
    0x25, 0x01,            //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,            //     REPORT_SIZE (8)
    0x95, 0x01,            //     REPORT_COUNT (1)
    0xB1, 0x82,            //     FEATURE (Data,Var,Abs,Vol)

    0x85, 0x04,            //     REPORT_ID (4)
    0x09, 0x04,            //     USAGE (LED 4)
    0x91, 0x82,            //     OUTPUT (Data,Var,Abs,Vol)
    // 86

    // key Push Button
    0x85, 0x05,            //     REPORT_ID (5)
    0x09, 0x05,            //     USAGE (Push Button)
    0x15, 0x00,            //     LOGICAL_MINIMUM (0)
    0x25, 0x01,            //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,            //     REPORT_SIZE (1)
    0x81, 0x82,            //     INPUT (Data,Var,Abs,Vol)

    0x09, 0x05,            //     USAGE (Push Button)
    0x75, 0x01,            //     REPORT_SIZE (1)
    0xb1, 0x82,            //     FEATURE (Data,Var,Abs,Vol)

    0x75, 0x07,            //     REPORT_SIZE (7)
    0x81, 0x83,            //     INPUT (Cnst,Var,Abs,Vol)
    0x85, 0x05,            //     REPORT_ID (2)

    0x75, 0x07,            //     REPORT_SIZE (7)
    0xb1, 0x83,            //     FEATURE (Cnst,Var,Abs,Vol)
    // 114

    // Tamper Push Button
    0x85, 0x06,            //     REPORT_ID (6)
    0x09, 0x06,            //     USAGE (Tamper Push Button)
    0x15, 0x00,            //     LOGICAL_MINIMUM (0)
    0x25, 0x01,            //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,            //     REPORT_SIZE (1)
    0x81, 0x82,            //     INPUT (Data,Var,Abs,Vol)

    0x09, 0x06,            //     USAGE (Tamper Push Button)
    0x75, 0x01,            //     REPORT_SIZE (1)
    0xb1, 0x82,            //     FEATURE (Data,Var,Abs,Vol)

    0x75, 0x07,            //     REPORT_SIZE (7)
    0x81, 0x83,            //     INPUT (Cnst,Var,Abs,Vol)
    0x85, 0x06,            //     REPORT_ID (6)

    0x75, 0x07,            //     REPORT_SIZE (7)
    0xb1, 0x83,            //     FEATURE (Cnst,Var,Abs,Vol)
    // 142

    // ADC IN
    0x85, 0x07,            //     REPORT_ID (7)
    0x09, 0x07,            //     USAGE (ADC IN)
    0x15, 0x00,            //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,      //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,            //     REPORT_SIZE (8)
    0x81, 0x82,            //     INPUT (Data,Var,Abs,Vol)
    0x85, 0x07,            //     REPORT_ID (7)
    0x09, 0x07,            //     USAGE (ADC in)
    0xb1, 0x82,            //     FEATURE (Data,Var,Abs,Vol)
    // 161

    0xc0              //     END_COLLECTION
}; // Printer_ReportDescriptor

// USB String Descriptors (optional)
const u8 Printer_StringLangID[Printer_SIZ_STRING_LANGID] = {
//    Printer_SIZ_STRING_LANGID,
//    USB_STRING_DESCRIPTOR_TYPE,
//    0x09,
//    0x04
    0x00, 0x09, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x30, 0x30, 0x30, 0x00
}
; // LangID = 0x0409: U.S. English

const u8 Printer_StringVendor[Printer_SIZ_STRING_VENDOR] = {
    Printer_SIZ_STRING_VENDOR, // Size of Vendor string
    USB_STRING_DESCRIPTOR_TYPE,  // bDescriptorType
    // Manufacturer: " "
    'H', 0, 'E', 0, 'L', 0, 'L', 0, 'O', 0, ' ', 0, 'U', 0, 'S', 0,
    'B', 0, '!', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, 'i', 0,
    ' ', 0, ' ', 0
};

const u8 Printer_StringProduct[Printer_SIZ_STRING_PRODUCT] = {
    Printer_SIZ_STRING_PRODUCT,          // bLength
    USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
    'M', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, ' ', 0, 'C', 0,
    'u', 0, 's', 0, 't', 0, 'm', 0, ' ', 0, 'H', 0, 'I', 0,
    'D', 0
};
u8 Printer_StringSerial[Printer_SIZ_STRING_SERIAL] = {
//    Printer_SIZ_STRING_SERIAL,           // bLength
//    USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
//    'M', 0, 'M', 0, '3', 0,'2', 0,'2', 0, ' ', 0, ' ', 0

    0x16, 0x03, 0x44, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x31, 0x00, 0x2e, 0x00,
    0x30, 0x00, 0x30, 0x00, 0x30, 0x00,
};


/// @}

/// @}

/// @}

