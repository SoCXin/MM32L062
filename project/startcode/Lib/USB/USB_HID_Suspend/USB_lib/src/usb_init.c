////////////////////////////////////////////////////////////////////////////////
/// @file     USB_INIT.C
/// @author   AE TEAM
/// @brief    Initialization routines & global variables.
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
#define _USB_INIT_C_
// Files includes

#include "usb_lib.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_INIT
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

//  The number of current endpoint, it will be used to specify an endpoint
u8    EPindex;
//  The number of current device, it is an index to the Device_Table
// u8  Device_no;
//  Points to the DEVICE_INFO structure of current device
//  The purpose of this register is to speed up the execution
DEVICE_INFO* pInformation;
//  Points to the DEVICE_PROP structure of current device
//  The purpose of this register is to speed up the execution
DEVICE_PROP* pProperty;
//  Temporary save the state of Rx & Tx status.
//  Whenever the Rx or Tx state is changed, its value is saved
//  in this variable first and will be set to the EPRB or EPRA
//  at the end of interrupt process
u16    SaveState ;
u16  wInterrupt_Mask;
DEVICE_INFO Device_Info;
USER_STANDARD_REQUESTS*  pUser_Standard_Requests;

// Extern variables ----------------------------------------------------------
// Private function prototypes -----------------------------------------------
// Private functions ---------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief  USB system initialization.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void USB_Init(void)
{
    pInformation = &Device_Info;
    pInformation->ControlState = 2;
    pProperty = &Device_Property;
    pUser_Standard_Requests = &User_Standard_Requests;
    // Initialize devices one by one
    pProperty->Init();
}
/// @}

/// @}

/// @}

