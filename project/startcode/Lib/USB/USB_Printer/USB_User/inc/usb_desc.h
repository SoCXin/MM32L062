////////////////////////////////////////////////////////////////////////////////
/// @file     usb_desc.h
/// @author   AE TEAM
/// @brief    Descriptor Header for Virtual COM Port Device.
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
#ifndef __USB_DESC_H
#define __USB_DESC_H
// Files includes
#include "mm32_device.h"

// Includes ------------------------------------------------------------------
// Exported types ------------------------------------------------------------
// Exported constants --------------------------------------------------------
// Exported macro ------------------------------------------------------------
// Exported define -----------------------------------------------------------
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define Printer_SIZ_HID_DESC                  0x09
#define Printer_OFF_HID_DESC                  0x12

#define Printer_SIZ_DEVICE_DESC               18
#define Printer_SIZ_CONFIG_DESC               32 //41
#define Printer_SIZ_REPORT_DESC               162
#define Printer_SIZ_STRING_LANGID             11 //4
#define Printer_SIZ_STRING_VENDOR             38
#define Printer_SIZ_STRING_PRODUCT            32
#define Printer_SIZ_STRING_SERIAL             26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

// Exported functions -------------------------------------------------------
extern const u8 Printer_DeviceDescriptor[Printer_SIZ_DEVICE_DESC];
extern const u8 Printer_ConfigDescriptor[Printer_SIZ_CONFIG_DESC];
extern const u8 Printer_ReportDescriptor[Printer_SIZ_REPORT_DESC];
extern const u8 Printer_StringLangID[Printer_SIZ_STRING_LANGID];
extern const u8 Printer_StringVendor[Printer_SIZ_STRING_VENDOR];
extern const u8 Printer_StringProduct[Printer_SIZ_STRING_PRODUCT];
extern u8 Printer_StringSerial[Printer_SIZ_STRING_SERIAL];

#endif // __USB_DESC_H  

/// @}

/// @}

/// @}
