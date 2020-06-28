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
#include "sys.h"
#include "uart.h"
#include "queue.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{
u8 gTableData[64] = {

    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x01, 0x02, 0x03, 0x04
};
u16 gTxCnt = 0;
u8 gTxBuff[64] = {0};
u8 gRxCnt = 0;
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u32 index_tt = 0;
    u8 tmp = 0;
    u8 tmp_len = 0;
    USB_GPIO_Config();
    CONSOLE_Init(115200);
    printf("System Initial Success");
    USB_NVIC_Config();
    USB_ClockConfig();
    USB_Init();
    while (1) {
        __disable_irq();
        gTxCnt = QUEUE_PacketLengthGet(&m_QueueUsbRx); //Takes the queue data length
        __enable_irq();
        if (gTxCnt < 896) {
            if (Flag_free_EP) { //Release an EP3 hardware buff to allow data to be received
                NVIC_DisableIRQ(USB_IRQn);
                tmp_len = _GetUSB_AVILn(ENDP3);
                PMAToUserBufferCopy(Receive_Buffer, ENDP3, tmp_len);
                USB_RxWrite(Receive_Buffer, tmp_len); //Write USB data to the print queue
                Flag_free_EP = 0;
                NVIC_EnableIRQ(USB_IRQn);
            }
        }
        if (gTxCnt) {
            if (gTxCnt <= 64) {
                gRxCnt = gTxCnt;
            }
            else {
                gRxCnt = 64;
            }
            __disable_irq();
            USB_RxRead(gTxBuff, gRxCnt); //Read queue data
            __enable_irq();
            for (tmp = 0; tmp < gRxCnt; tmp++) {
                printf("%02x ", gTxBuff[tmp]);
                index_tt++;
            }
            printf("\r\n");
            printf("%06d ", index_tt);
            printf("\r\n");
            memset(gTxBuff, 0, 64);
            gTxCnt = 0;
        }
    }
}


/// @}

/// @}

/// @}
