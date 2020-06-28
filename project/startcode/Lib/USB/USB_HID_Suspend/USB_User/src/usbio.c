////////////////////////////////////////////////////////////////////////////////
/// @file     usbio.c
/// @author   AE TEAM
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE ADC
///           FIRMWARE LIBRARY.
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
#define _USBIO_C_

// Files includes
// Define to prevent recursive inclusion
//#include "main.h"
#include "usbio.h"
#include "usb_desc.h"
#include "usb_lib.h"
// Private typedef
// Private define
// Private macro

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_IO
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

u8 USB_Receive_Buffer[REPORT_COUNT];
u8 USB_Send_Buffer[REPORT_COUNT];
u8 g_dataSerialNumber;

// Private variables

// Private function prototypes


// Private functions

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send data from USB.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u32 USB_SendData(u8* data, u32 dataNum)
{

    //Send data via USB
    UserToPMABufferCopy(data, ENDP1, dataNum); //Copies data from the USBEP3RX
    // buffer into a user-specified array

    _SetEP_EN(EP_EN_EP1EN);

    _SetUSB_AVIL1(EPn_AVIL_EPXAVIL);


    return dataNum;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive data from USB.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u32 USB_GetData(u8* data, u32 dataNum)
{
    u32 len = 0;
    if(dataNum > sizeof(USB_Receive_Buffer)) {
        dataNum = sizeof(USB_Receive_Buffer);
    }

    for(len = 0; len < dataNum; len++) {
        *data = USB_Receive_Buffer[len];
        data++;
    }
    return dataNum;
}

/// @}

/// @}

/// @}


