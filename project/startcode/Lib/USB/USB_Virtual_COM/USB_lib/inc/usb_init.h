////////////////////////////////////////////////////////////////////////////////
/// @file     usb_init.h
/// @author   AE TEAM
/// @brief    Initialization routines & global variables.
/// @attention
////////////////////////////////////////////////////////////////////////////////
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
#ifndef __USB_INIT_H
#define __USB_INIT_H


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Example_Layer
/// @brief MM32 Example Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_RESOURCE
/// @brief MM32 Examples resource modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Constants
/// @{
// Exported types ------------------------------------------------------------
// Exported constants --------------------------------------------------------
// Exported macro ------------------------------------------------------------
// Exported functions -------------------------------------------------------
void USB_Init(void);

// External variables --------------------------------------------------------
//  The number of current endpoint, it will be used to specify an endpoint
extern u8  EPindex;
//  The number of current device, it is an index to the Device_Table
//extern u8    Device_no;
//  Points to the DEVICE_INFO structure of current device
//  The purpose of this register is to speed up the execution
extern DEVICE_INFO* pInformation;
//  Points to the DEVICE_PROP structure of current device
//  The purpose of this register is to speed up the execution
extern DEVICE_PROP* pProperty;
//  Temporary save the state of Rx & Tx status.
//  Whenever the Rx or Tx state is changed, its value is saved
//  in this variable first and will be set to the EPRB or EPRA
//  at the end of interrupt process
extern USER_STANDARD_REQUESTS* pUser_Standard_Requests;

extern u16 SaveState ;
extern u16 wInterrupt_Mask;

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////

