////////////////////////////////////////////////////////////////////////////////
/// @file     usb_endp.C
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE ADC FIRMWARE FUNCTIONS.
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
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_ENDP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{


u8 gTxData[VIRTUAL_COM_PORT_DATA_SIZE];
u8 buffer_in[VIRTUAL_COM_PORT_DATA_SIZE];

//bit0:USB_TX
//bit1:USB_RX
//bit2:USB_BUSY
//bit3:reserve
//bit4:UART_TX
//bit5:UART_RX
//bit6:UART_BUSY
u8 status_UsbUart = 0;
u8 gUartBuffCnt = 0;
u8 len_UartToUsb = 0;
__IO u32 gTxDataCnt = 0;
u32 count_in = 0;
u8 empty_flag = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief  EP1 OUT Callback Routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EP3_OUT_Callback(void)
{
    u8 tempbuffer[VIRTUAL_COM_PORT_DATA_SIZE];
    gTxDataCnt = _GetUSB_AVILn(ENDP3);
    if((status_UsbUart >> 4) & 0x01) {  //UART Send data
        PMAToUserBufferCopy(tempbuffer, ENDP3, gTxDataCnt);
        gTxDataCnt = 0;
    }
    else {
        PMAToUserBufferCopy(gTxData, ENDP3, gTxDataCnt);
        status_UsbUart |= 0x1 << 1;
    }
    gUartBuffCnt = gTxDataCnt;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  EP1_IN_Callback.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
extern __IO u8 bIntPackSOF ;
void EP1_IN_Callback(void)
{
    if( bIntPackSOF > 1) {              //Receive data length 64byte
        if((_GetUSB_CTRL1()&EP1_CTRL_TRANEN ) == 0) {
            UserToPMABufferCopy(buffer_in, ENDP1, count_in);
            if(count_in > 0) {
                _SetUSB_CTRL1(EP1_CTRL_TRANEN | count_in) ;
            }
            count_in = 0;
            status_UsbUart &= ~(1 << 5);
        }
    }
}

/// @}

/// @}

/// @}

