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

typedef struct {
    u32 bitrate;
    u8 format;
    u8 paritytype;
    u8 datatype;
} LINE_CODING;


#define SEND_ENCAPSULATED_COMMAND   0x00
#define GET_ENCAPSULATED_RESPONSE   0x01
#define SET_COMM_FEATURE            0x02
#define GET_COMM_FEATURE            0x03
#define CLEAR_COMM_FEATURE          0x04
#define SET_LINE_CODING             0x20
#define GET_LINE_CODING             0x21
#define SET_CONTROL_LINE_STATE      0x22
#define SEND_BREAK                  0x23


//Exported constants
//Exported macro
//Exported functions
void CustomHID_init(void);
void CustomHID_Reset(void);
void CustomHID_SetConfiguration(void);
void CustomHID_SetDeviceAddress (void);
void CustomHID_Status_In (void);
void CustomHID_Status_Out (void);
RESULT CustomHID_Data_Setup(u8);
RESULT CustomHID_NoData_Setup(u8);
RESULT CustomHID_Get_Interface_Setting(u8 Interface, u8 AlternateSetting);
u8* CustomHID_GetDeviceDescriptor(u16 );
u8* CustomHID_GetConfigDescriptor(u16);
u8* CustomHID_GetStringDescriptor(u16);
u8* CustomHID_Get_String(u16);  //mm32
RESULT CustomHID_SetProtocol(void);
u8* CustomHID_GetProtocolValue(u16 Length);
RESULT CustomHID_SetProtocol(void);
u8* CustomHID_GetReportDescriptor(u16 Length);
u8* CustomHID_GetHIDDescriptor(u16 Length);

u8* Virtual_Com_Port_GetLineCoding(u16 Length);
u8* Virtual_Com_Port_SetLineCoding(u16 Length);
// Exported define
#define CustomHID_GetConfiguration          NOP_Process
//#define CustomHID_SetConfiguration          NOP_Process
#define CustomHID_GetInterface              NOP_Process
#define CustomHID_SetInterface              NOP_Process
#define CustomHID_GetStatus                 NOP_Process
#define CustomHID_ClearFeature              NOP_Process
#define CustomHID_SetEndPointFeature        NOP_Process
#define CustomHID_SetDeviceFeature          NOP_Process
//#define CustomHID_SetDeviceAddress          NOP_Process

#define REPORT_DESCRIPTOR                  0x22

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
