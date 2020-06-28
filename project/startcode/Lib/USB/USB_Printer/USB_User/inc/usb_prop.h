////////////////////////////////////////////////////////////////////////////////
/// @file     usb_prop.h
/// @author   AE TEAM
/// @brief    All processings related to Custom HID demo.
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
#ifndef __USB_PROP_H
#define __USB_PROP_H

// Includes ------------------------------------------------------------------
// Exported types ------------------------------------------------------------
typedef enum _HID_REQUESTS {
    GET_REPORT = 1,
    GET_IDLE,
    GET_PROTOCOL,

    SET_REPORT = 9,
    SET_IDLE,
    SET_PROTOCOL
} HID_REQUESTS;

// Exported constants --------------------------------------------------------
// Exported macro ------------------------------------------------------------
// Exported functions -------------------------------------------------------
void Printer_init(void);
void Printer_Reset(void);
void Printer_SetConfiguration(void);
void Printer_SetDeviceAddress (void);
void Printer_Status_In (void);
void Printer_Status_Out (void);
RESULT Printer_Data_Setup(u8);
RESULT Printer_NoData_Setup(u8);
RESULT Printer_Get_Interface_Setting(u8 Interface, u8 AlternateSetting);
u8* Printer_GetDeviceDescriptor(u16 );
u8* Printer_GetConfigDescriptor(u16);
u8* Printer_GetStringDescriptor(u16);
RESULT Printer_SetProtocol(void);
u8* Printer_GetProtocolValue(u16 Length);
RESULT Printer_SetProtocol(void);
u8* Printer_GetReportDescriptor(u16 Length);
u8* Printer_GetHIDDescriptor(u16 Length);


//Exported define
#define Printer_GetConfiguration          NOP_Process
//#define Printer_SetConfiguration          NOP_Process
#define Printer_GetInterface              NOP_Process
#define Printer_SetInterface              NOP_Process
#define Printer_GetStatus                 NOP_Process
#define Printer_ClearFeature              NOP_Process
#define Printer_SetEndPointFeature        NOP_Process
#define Printer_SetDeviceFeature          NOP_Process
//#define Printer_SetDeviceAddress          NOP_Process

#define REPORT_DESCRIPTOR                  0x22

#endif // __USB_PROP_H  

/// @}

/// @}

/// @}
