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


#include "platform_config.h"
#include "mm32_device.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "uart.h"
#include "usb_desc.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_ENDP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

u8 USB_Receive_Buffer1[BUFF_SIZE];
u8 USB_Receive_Buffer2[BUFF_SIZE];

//Private typedef
//Private define
//Private macro
//Private variables
//u8 Receive_Buffer[BUFF_SIZE];
u8 Flag_Receive1;
u8 Flag_Receive2;
u8 Flag_Send;

//Private typedef
//Private define
//Private macro
//Private variables
u8 gTxBuff[VIRTUAL_COM_PORT_DATA_SIZE];
u8 gRxBuff[VIRTUAL_COM_PORT_DATA_SIZE];

//bit0:USB_TX
//bit1:USB_RX
//bit2:USB_BUSY
//bit3:reserve
//bit4:UART_TX
//bit5:UART_RX
//bit6:UART_BUSY
u8 status_UsbUart = 0;
u8 gUartDataCnt = 0;
u8 len_UartToUsb = 0;
__IO u32 gTxDataCnt = 0;
u32 gRxDataCnt = 0;
u8 empty_flag = 0;


//Private function prototypes
//Private functions

extern __IO u8 bIntPackSOF ;

extern __IO u8 bIntPackSOF ;
////////////////////////////////////////////////////////////////////////////////
/// @brief     EP1 IN Callback Routine.
/// @param     None.
/// @retval    None.
////////////////////////////////////////////////////////////////////////////////
void EP1_IN_Callback(void)
{

    if( bIntPackSOF > 1) {              //Receive a frame complete or no data, maximum 64byte
        if((_GetUSB_CTRL1()&EP1_CTRL_TRANEN ) == 0) {
            UserToPMABufferCopy(gRxBuff, ENDP1, gRxDataCnt);
            if(gRxDataCnt > 0) {
                _SetUSB_CTRL1(EP1_CTRL_TRANEN | gRxDataCnt) ;
                if(gRxDataCnt == 64)
                    _SetUSB_CTRL1(EP1_CTRL_TRANEN | 0) ;
            }
            gRxDataCnt = 0;
            status_UsbUart &= ~(1 << 5);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  EP1 OUT Callback Routine.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void EP3_OUT_Callback(void)
{
    u8 tempbuffer[VIRTUAL_COM_PORT_DATA_SIZE];
    gTxDataCnt = _GetUSB_AVILn(ENDP3);
    if((status_UsbUart >> 4) & 0x01) {  //UART is transmitting
        PMAToUserBufferCopy(tempbuffer, ENDP3, gTxDataCnt);
        gTxDataCnt = 0;
    }
    else {
        PMAToUserBufferCopy(gTxBuff, ENDP3, gTxDataCnt);
        status_UsbUart |= 0x1 << 1;
    }
    gUartDataCnt = gTxDataCnt;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief     EP1 OUT Callback Routine.
/// @param     None.
/// @retval    None
////////////////////////////////////////////////////////////////////////////////
void EP4_OUT_Callback(void)
{
//  BitAction Led_State;
    u8 ReadCnt;
    ReadCnt = USB->EP4_AVIL;   //Number of Data received by the current endpoint
    PMAToUserBufferCopy(USB_Receive_Buffer2, ENDP4, ReadCnt);   //Receive data copy from endpoint to user buff

    Flag_Receive2 = 1;
//  printf("EP4_OUT\r\n");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     EP1 IN Callback Routine.
/// @param     None
/// @retval    None.
////////////////////////////////////////////////////////////////////////////////
void EP4_IN_Callback(void)
{
//  PrevXferComplete = 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     EP1 IN Callback Routine.
/// @param     None.
/// @retval    None
////////////////////////////////////////////////////////////////////////////////
void EP2_IN_Callback(void)
{

}
/// @}

/// @}

/// @}
