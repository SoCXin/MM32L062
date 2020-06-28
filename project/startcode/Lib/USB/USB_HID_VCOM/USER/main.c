////////////////////////////////////////////////////////////////////////////////
/// @file     main.c
/// @author   AE TEAM
/// @brief    Custom HID demo main file.
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
#define _MAIN_C_

// Files includes
#include "mm32_device.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usbio.h"
#include "usb_regs.h"

#include "usb_pwr.h"

#include "uart.h"
#include "string.h"
#include "usb_desc.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{

// Private functions

extern __IO u32 gTxDataCnt;
extern u8 gTxBuff[VIRTUAL_COM_PORT_DATA_SIZE];
extern u8 gUartDataCnt ;

u8 gTableData[BUFF_SIZE] = {
    0x02, 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    CONSOLE_Init(115200);
    ADC_Configuration();
    USB_NVIC_Config();
    USB_ClockConfig();
    USB_Init();
    printf("USB_initial success\r\n");
    memset(gTableData, 0x00, BUFF_SIZE);
    while (1) {
        if(bDeviceState == CONFIGURED) { //HID Update
            if((_GetUSB_CTRL4()&EP4_CTRL_TRANEN ) == 0) {
                gTableData[0] = 0x02;
                gTableData[4] = 0x01;
                UserToPMABufferCopy(gTableData, ENDP4, 8);
                _SetUSB_CTRL4(EP4_CTRL_TRANEN | 8) ;
            }
        }
        if ((gTxDataCnt != 0) && (bDeviceState == CONFIGURED)) { //VCOM
            USB_To_UART_Send_Data(gTxBuff, &gUartDataCnt);
        }
        if(bDeviceState == CONFIGURED) { //HID Update
            if((_GetUSB_CTRL4()&EP4_CTRL_TRANEN ) == 0) {
                gTableData[0] = 0x02;
                gTableData[4] = 0x02;
                UserToPMABufferCopy(gTableData, ENDP4, 8);
                _SetUSB_CTRL4(EP4_CTRL_TRANEN | 8) ;
            }
        }
    }
}


/// @}

/// @}

/// @}
