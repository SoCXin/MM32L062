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

// Files includes--
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


//Private typedef
//Private define
//Private macro
//Private variables
//Extern variables
//Private function prototypes
//Private functions

// USB Standard Device Descriptor
const u8 CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] = {
    0x12,                       //bLength:Length, the length of the device descriptor is 18 bytes
    USB_DEVICE_DESCRIPTOR_TYPE, //bDescriptorType
    0x00, 0x02,                  //bcdUSB = 2.00
    0xEF,                       //bDeviceClass
    0x02,                       //bDeviceSubClass
    0x01,                       //bDeviceProtocol
    0x40,                       //bMaxPacketSize40
    0x81,                       //idVendor (0x2F81)
    0x2F,
    0x21,                       //idProduct = 0x5721
    0x57,
    0x00,                       //bcdDevice rel. 2.00
    0x02,
    1,                          //Index of string descriptor describing
    //manufacturer
    2,                          //Index of string descriptor describing
    //           product
    3,                          //Index of string descriptor describing the
    //           device serial number
    0x01                        //bNumConfigurations
}
; // CustomHID_DeviceDescriptor


// USB Configuration Descriptor
//   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor
const u8 CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] = {
    0x09, // bLength: Configuration Descriptor size
    USB_CONFIGURATION_DESCRIPTOR_TYPE, // bDescriptorType: Configuration
    CUSTOMHID_SIZ_CONFIG_DESC,
    // wTotalLength: Bytes returned
    0x00,
    0x03,         // bNumInterfaces: 1 interface
    0x01,         // bConfigurationValue: Configuration value
    0x00,         // iConfiguration: Index of string descriptor describing
    //             the configuration
    0xC0,         // bmAttributes: Self powered
    0x96,         // MaxPower 100 mA: this current is used for detecting Vbus

    //-----------------------Function 1 HID keyboard------------------------------
    //IAD descriptor
    0x08,   //bLength:IAD descriptor size
    0x0B,   //bDescriptorType:IAD descriptor type
    0x00,   //bFirstInterface:The first interface descriptor of the Function 1 HID keyboard is the number of zeros in the total config descriptor.
    0x01,   //bInferfaceCount:Function 1 HID keyboard has 1 interface descriptor
    0x03,   //bFunctionClass:Function 1 HID keyboard has 1 interface descriptor
    0x00,   //bFunctionSubClass:When using the same HID function, b Device SubClass in the device symbol
    0x01,   //bFunctionProtocol:When using the same HID function, b Device Protocol in the device symbol
    0x00,   //iFunction: The index of this device in the String description (personally, it is understood that there is a String descriptor such as 0 ~ 5 is a String with function 1,
    //6 ~ 10 is the String of Function 2, if it is Function 2, this value is 6)


    //------------------------ Descriptor of Custom HID interface ------------------------
    // 09
    0x09,         // bLength: Interface Descriptor size
    USB_INTERFACE_DESCRIPTOR_TYPE,// bDescriptorType: Interface descriptor type
    0x00,         // bInterfaceNumber: Number of Interface
    0x00,         // bAlternateSetting: Alternate setting
    0x02,         // bNumEndpoints
    0x03,         // bInterfaceClass: HID
    0x01,         // bInterfaceSubClass : 1=BOOT, 0=no boot
    0x01,         // nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
    0,            // iInterface: Index of string descriptor

    //----------------------- Descriptor of Custom HID HID -----------------------//
    // 18
    0x09,         // bLength: HID Descriptor size
    HID_DESCRIPTOR_TYPE, //bDescriptorType: HID
    0x10,         // bcdHID: HID Class Spec release number
    0x01,
    0x00,         // bCountryCode: Hardware target country
    0x01,         // bNumDescriptors: Number of HID class descriptors to follow
    0x22,         // bDescriptorType
    CUSTOMHID_SIZ_REPORT_DESC,
    //KEYBOARD_SIZ_REPORT_DESC,// wItemLength: Total length of Report descriptor
    0x00,

    //----------------------Descriptor of Custom HID endpoints -------------------//
    // 27
    0x07,          // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE, //bDescriptorType:
    0x84,          //bEndpointAddress: Endpoint Address (IN)  //EP4  IN
    0x03,          //bmAttributes: Interrupt endpoint
    0x40,          //wMaxPacketSize: 64 Bytes max
    0x00,
    0x02,          //bInterval: Polling Interval (32 ms)
    // 34


    0x07,   // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE,   // bDescriptorType:
    //  Endpoint descriptor type
    0x04,   // bEndpointAddress:                    //EP4  OUT
    //  Endpoint Address (OUT)
    0x03,   // bmAttributes: Interrupt endpoint
    0x40,   // wMaxPacketSize: 64 Bytes max
    0x00,
    0x02,   // bInterval: Polling Interval (20 ms)
    // 41


    //-------------------------Function 2 VCP virtual serial port-------------------//
    //IAD descriptor
    // Interface Association Descriptor(IAD Descriptor)
    0x08,   // bLength
    0x0B,   // bDescriptorType
    0x01,   // bFirstInterface
    0x02,   // bInterfaceCount
    0x02,   // bFunctionClass --CDC
    0x02,   // bFunctionSubClass
    0x01,   // bFunctionProtocoll
    0x00,   // iFunction

    //VCP virtual serial port
    //Interface DescriptorInterface descriptor
    0x09,   // bLength: Interface Descriptor size
    USB_INTERFACE_DESCRIPTOR_TYPE,  // bDescriptorType: Interface
    // Interface descriptor type
    0x01,   //bInterfaceNumber: Number of Interface    //<interface 1>
    0x00,   //bAlternateSetting: Alternate setting
    0x01,   //bNumEndpoints: One endpoints used Non-zero breakpoints of this interface
    0x02,   //bInterfaceClass: Communication Interface Class
    0x02,   //bInterfaceSubClass: Abstract Control Model
    0x01,   //bInterfaceProtocol: Common AT commands
    0x00,   //iInterface:
    //Header Functional Descriptor class descriptor
    0x05,   // bLength: Endpoint Descriptor size
    0x24,   // bDescriptorType: CS_INTERFACE
    0x00,   // bDescriptorSubtype: Header Func Desc
    0x10,   // bcdCDC: spec release number
    0x01,
    //Call Management Functional Descriptor
    0x05,   // bFunctionLength
    0x24,   // bDescriptorType: CS_INTERFACE
    0x01,   // bDescriptorSubtype: Call Management Func Desc
    0x00,   // bmCapabilities: D0+D1
    0x01,   // bDataInterface: 1
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


    //Endpoint 2 Descriptorport  descriptor
    0x07,   // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE,   // bDescriptorType: Endpoint
    0x82,   // bEndpointAddress: (IN2)
    0x03,   // bmAttributes: Interrupt
    VIRTUAL_COM_PORT_INT_SIZE,      // wMaxPacketSize:
    0x00,
    0xFF,   // bInterval:

    //Data class interface descriptor class  descriptor
    0x09,   // bLength: Endpoint Descriptor size
    USB_INTERFACE_DESCRIPTOR_TYPE,  // bDescriptorType:
    0x02,   // bInterfaceNumber: Number of Interface
    0x00,   // bAlternateSetting: Alternate setting
    0x02,   // bNumEndpoints: Two endpoints used
    0x0A,   // bInterfaceClass: CDC
    0x00,   // bInterfaceSubClass:
    0x00,   // bInterfaceProtocol:
    0x00,   // iInterface:


    //Endpoint 3 Descriptorport  descriptor
    0x07,   // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE,   // bDescriptorType: Endpoint
    0x03,   // bEndpointAddress: (OUT3)
    0x02,   // bmAttributes: Bulk
    VIRTUAL_COM_PORT_DATA_SIZE,             // wMaxPacketSize:
    0x00,
    0x00,   // bInterval: ignore for Bulk transfer
    //Endpoint 1 Descriptor port  descriptor
    0x07,   // bLength: Endpoint Descriptor size
    USB_ENDPOINT_DESCRIPTOR_TYPE,   // bDescriptorType: Endpoint
    0x81,   // bEndpointAddress: (IN1)
    0x02,   // bmAttributes: Bulk
    VIRTUAL_COM_PORT_DATA_SIZE,             // wMaxPacketSize:
    0x00,
    0x00    // bInterval
}
; // CustomHID_ConfigDescriptor
const u8 CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] = {
//s16 Item   D7~D4:bTag;D3~D2:bType;D1~D0:bSize
// bTag -- -Main entry    1000: Input  1001: Output  1011: Feature   1010: Collection 1100: End Collection
//        Global entry  0000: Usage Page 0001: (Logical Minimum) 0010: (Logical Maximum) 0011: (Physical Minimum)
//                  0100: (Physical Maximum) 0101: (Unit Exponet) 0110: (Unit) 0111:  (Report Size)
//                  1000: (Report ID) 1001: (Report Count) 1010: (Push) 1011: (Pop) 1100~1111: (Reserved)
//        local entry   0000: (Usage) 0001: (Usage Minimum) 0010: (Usage Maximum) 0011: (Designator Index)
//                  0100: (Designator Minimum) 0101: (Designator Maximum) 0111: (String Index) 1000: (String Minimum)
//                 1001: (String Maximum) 1010: (Delimiter) others: (Reserved)
//        bType-- -00: Main entry (main)  01: Global entry(globle)  10: (local)  11: (reserved)
//        bSize-- -00: 0byte   01: 1byte   10: 2byte   11: 4byte

    //0x05:0000 01 01 This is a Global entry, the purpose page is selected as a normal desktop page
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    //0x09:0000 10 01 This is a Global entry, the use choice is keyboard
    0x09, 0x06, // USAGE (Keyboard)
    //0xa1:1010 00 01 This is a Main entry, selected as the application collection,
    0xa1, 0x01, // COLLECTION (Application)
    //0x05:0000 01 11 This is a Global entry, use keyboard / keys for the purpose page
    0x05, 0x07, // USAGE_PAGE (Keyboard/Keypad)

    //0x19:0001 10 01 This is a local entry with a minimum value of 0xe0, corresponding to the left ctrl key on the keyboard
    0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
    //0x29:0010 10 01 This is a local entry with a maximum value of 0xe7, corresponding to the GUI (WIN) key on the keyboard
    0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
    //0x15:0001 01 01 This is a Global entry, indicating that the minimum logical value of Data is 0
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    //0x25:0010 01 01This is a Global entry, indicating that the maximum logical value of Data is 1
    0x25, 0x01, // LOGICAL_MAXIMUM (1)

    //0x95:1001 01 01 This is a Global entry, the number of Data fields is 8.
    0x95, 0x08, // REPORT_COUNT (8)
    //0x75:0111 01 01 This is a Global entry, and the length of each Data field is 1 bit
    0x75, 0x01, // REPORT_SIZE (1)
    //0x81:1000 00 01 This is a Main entry, with 8 * 1bit Data field as input, the attributes are: Data, Var, Abs
    0x81, 0x02, // INPUT (Data,Var,Abs)

    //0x95:1001 01 01 This is a Global entry, the number of Data fields is 1.
    0x95, 0x01, // REPORT_COUNT (1)
    //0x75:0111 01 01 This is a Global entry, each Data field is 8 bits s32
    0x75, 0x08, // REPORT_SIZE (8)
    //0x81:1000 00 01 It is a Main entry, with 1 * 8bit Data field as input, the attributes are: Cnst, Var, Abs
    0x81, 0x03, // INPUT (Cnst,Var,Abs)

    //0x95:1001 01 01 This is a Global entry, the number of Data fields is 6.
    0x95, 0x06, // REPORT_COUNT (6)
    //0x75:0111 01 01 This is a Global entry, each Data field is 8 bits s32
    0x75, 0x08, // REPORT_SIZE (8)
    //0x25:0010 01 01 This is a Global entry with a logical maximum value of 255
    0x25, 0xFF, // LOGICAL_MAXIMUM (255)
    //0x19:0001 10 01 This is a local entry with a minimum value of 0 for use
    0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
    //0x29:0010 10 01This is a local entry with a maximum value of 0x65
    0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
    //0x81:1000 00 01 This is a Main entry with a 6 * 8bit Data field as input. The attributes are: Data, Var, Abs
    0x81, 0x00, // INPUT (Data,Ary,Abs)

    //0x25:0010 01 01 This is a Global entry, the maximum value of logic is 1
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    //0x95:1001 01 01 This is a Global entry, the number of Data fields is 2
    0x95, 0x02, // REPORT_COUNT (2)
    //0x75:0111 01 01 This is a Global entry, and the length of each Data field is 1 bit
    0x75, 0x01, // REPORT_SIZE (1)
    //0x05:0000 01 01 This is a Global entry, the use page is selected as the LED page
    0x05, 0x08, // USAGE_PAGE (LEDs)
    //0x19:0001 10 01 This is a local entry with a minimum value of 0 for usex01,Corresponds to Num Lock on the keyboard
    0x19, 0x01, // USAGE_MINIMUM (Num Lock)
    //0x29:0010 10 01 This is a local entry with a maximum value of 0x02, corresponding to Caps Lock on the keyboard
    0x29, 0x02, // USAGE_MAXIMUM (Caps Lock)
    //0x91:1001 00 01 This is a Main entry, with 2 * 1bit Data field as output, the attributes are: Data, Var, Abs
    0x91, 0x02, // OUTPUT (Data,Var,Abs)

    //0x95:1001 01 01 This is a Global entry, the number of Data fields is 1.
    0x95, 0x01, // REPORT_COUNT (1)
    //0x75:0111 01 01 This is a Global entry.The length of each Data field is 6bit, which is exactly 1byte with the previous 2bit.
    0x75, 0x06, // REPORT_SIZE (6)
    //0x91:1001 00 01 This is a Main entry.The 1 * 6bit Data field is the most output.The attributes are: Cnst, Var, Abs
    0x91, 0x03, // OUTPUT (Cnst,Var,Abs)

    0xc0        // END_COLLECTION
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
    KEYBOARD_SIZ_STRING_VENDOR,     //bLength:Vendor String descriptor length
    USB_STRING_DESCRIPTOR_TYPE,     //bDescriptorType: String descriptor type is 0x03
    'B', 0, 'y', 0, ':', 0, 'z', 0, 'i', 0, 'y', 0, 'e', 0, '3', 0, '3', 0, '4', 0   //customize
};

const u8 CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] = {
    KEYBOARD_SIZ_STRING_PRODUCT,    // bLength:String descriptor for the product
    USB_STRING_DESCRIPTOR_TYPE,     // bDescriptorType: String descriptor type is 0x03
    'z', 0, 'i', 0, 'y', 0, 'e', 0, '3', 0, '3', 0, '4', 0 //customize
};
u8 CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] = {
    KEYBOARD_SIZ_STRING_SERIAL,     // bLength:Product Serial Number
    USB_STRING_DESCRIPTOR_TYPE,     // bDescriptorType: String descriptor type is 0x03
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0 //customize
};

// USB String Descriptors (optional)
const u8 CustomHID_String_0[CUSTOMHID_SIZ_STRING] = {

    0x11,
    0x22
}
;

// USB String Descriptors (optional)
const u8 CustomHID_String_1[CUSTOMHID_SIZ_STRING] = {

    0x33,
    0x44
}
;

const u8 CustomHID_String_2[CUSTOMHID_SIZ_STRING] = {
    0x00,
    0x80

}
;


const u8 CustomHID_String_3[CUSTOMHID_SIZ_STRING] = {
    0xFF,
    0x7F

}
;
const u8 CustomHID_String_4[CUSTOMHID_SIZ_STRING] = {
    0x00,
    0x04

}
;
/// @}

/// @}

/// @}

