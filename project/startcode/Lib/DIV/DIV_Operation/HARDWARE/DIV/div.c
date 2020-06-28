////////////////////////////////////////////////////////////////////////////////
/// @file    div.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _DIV_C_

// Files includes


#include "div.h"

#include "delay.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DIV
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DIV_Exported_Constants
/// @{




#define GET_SoftDivider(DVD, DVS) ((DVD) / (DVS))

u32 dataBuf[32] = {0x12345678, 0x9876554, 0x9822346, 0x98734662,
                   0x000001, 0xFFFFFFFF, 0x11111111, 0x22222222,
                   0x33333333, 0x78645323, 0x4399039, 0x4367464,
                   0x42345678, 0x9873554, 0x9822346, 0x98734662,
                   0x100001, 0x1FFFFFFF, 0x11511111, 0x22422222,
                   0x53333333, 0x78645323, 0x4399039, 0x4367464,
                   0x140001, 0x11FFFFFF, 0x12111111, 0x29422222,
                   0x53333333, 0x78644323, 0x4369039, 0x4767464,
                  };

u32 dataBuf1[32] = {9, 7, 8, 2,
                    33, 44, 66, 77,
                    11, 22, 1, 4,
                    44, 44, 11, 8,
                    45, 6, 3, 9,
                    2332, 989, 23, 77,
                    79, 345, 12, 43,
                    67, 89, 78, 23,
                   };
u32 result[2];

////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize DIV
/// @note   if use jtag/swd interface GPIO PIN as DIV, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DIV_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBENR_HWDIV, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  DIV_BAN test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DIV_BANtest(void)
{
    u32 i = 1, j;
    s32 errCnt = 0;

    printf("sprintf ok\r\n");

    HWDivider_Init(true, false);

    for(j = 0; j < 32; j++) {
        if(i == 0)
            i = 1;
        i++;
        SET_HWDivider(dataBuf[j & 0x1f], i);
        result[0] = GET_HWDivider();
        result[1] = GET_SoftDivider(dataBuf[j & 0x1f], i);
        if(result[1] != result[0]) {
            errCnt++;
            printf("\r\n\r\nERROR\r\n\r\n" );
            if(errCnt > 2) {
                while(1);
            }
        }
        if((i % 100000) == 0) {
            printf("i = %d\r\n", i);
        }
    }
    while(1);
}

/// @}

/// @}

/// @}
