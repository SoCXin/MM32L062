////////////////////////////////////////////////////////////////////////////////
/// @file     usb_endp.C
/// @author   AE TEAM
/// @brief    Endpoint routines.
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
#define _USB_ENDP_C_

// Files includes
#include "platform_config.h"
#include "mm32_device.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "uart.h"
#include "queue.h"
#include "hw_config.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_ENDP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

u8 Flag_free_EP = 0;

// Private typedef -----------------------------------------------------------
// Private define ------------------------------------------------------------
// Private macro -------------------------------------------------------------
// Private variables ---------------------------------------------------------
u8 Receive_Buffer[64];
u8 Send_Buffer[64] = {0x31, 0x32, 0x33, 0x34};


////////////////////////////////////////////////////////////////////////////////
/// @brief  EP1 OUT Callback Routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 test_data[4] = {0x45, 0x67, 0x78, 0xaa};
void EP3_OUT_Callback(void)
{
    u8 Data_Len = 0;
    u16 tmp_len = 0;
    //  u8 tmp = 0;

    tmp_len = QUEUE_PacketLengthGet(&m_QueueUsbRx); //Read queue data length

    if (tmp_len < 896) // Read USB data release EP3 hardware buff allows PC data
        //to be received if the queue has data space
    {
        Data_Len = _GetUSB_AVILn(ENDP3);    // Takes the current endpoint to receive the output length
        PMAToUserBufferCopy(Receive_Buffer, ENDP3, Data_Len); //Fetch the data received by the endpoint
        USB_RxWrite(Receive_Buffer, Data_Len);                //Put the received data into the print queue
    }
    else { //Otherwise do not read USB data no longer receive PC data
        Flag_free_EP = 1; //Flag read USB data if there is space
    }
}

void EP1_IN_Callback(void)
{
    UserToPMABufferCopy(test_data, ENDP1, 4);
    _SetUSB_CTRL1(EP1_CTRL_TRANEN | 4);
}

/// @}

/// @}

/// @}
