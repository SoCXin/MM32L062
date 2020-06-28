////////////////////////////////////////////////////////////////////////////////
/// @file    rcc.c
/// @author  AE TEAM
/// @brief    In window comparator mode,The transformation results are detected
///           Set the threshold value from 0 to 3V, and connect PB6 and PA0 with
///           jumper cap to see the effect.
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
#define _RCC_C_

// Files includes
#include "delay.h"
#include "led.h"
#include "UART.h"
#include "rcc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RCC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RCC_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  WWDG_ResetTest
/// @note   When the system starts, the watchdog is disabled. After the watchdog
///         is disabled, it cannot be closed manually. After reset, the watchdog
///         is disabled
/// @param  ucTcnt:which is 7-bit data and the value is 0x40~0x7f
/// @param  ucWcnt: The guard dog counts
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void WWDG_ResetConfig(u8 ucTcnt, u8 ucWcnt)
{
    //Disabled window watchdog clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    //WWDG_Prescaler_x,x can be 1,2,4,8
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(ucWcnt);
    //Assigns a value to the watchdog autodecrement counter
    WWDG_Enable(ucTcnt);
}

////////////////////////////////////////////////////////////////////////////////
/// @name   : Write_Iwdg_ON
/// @brief  : Write_Iwdg_ON
/// @param  : None
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void IWDG_ResetConfig(u16 IWDG_Prescaler, u16 Reload)
{
    RCC_LSICmd(ENABLE);                                                         //enable LSI
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
    PVU_CheckStatus();                                                          //get IWDG status
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);
    RVU_CheckStatus();                                                          //get IWDG status
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);
    IWDG_ReloadCounter();                                                       //load and enable IWDG
    IWDG_Enable();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   Write_Iwdg_RL
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
void IWDG_RL(void)
{
    IWDG_ReloadCounter();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   WWDG reset
/// @note    RCC_CSR 24 RMVF rw 0 RMVF£º clear reset flag
///          The software sets' 1 'to clear the reset flag.
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
s32 WWDG_ResetTest(void)
{
    printf("WWDG Reset Test!\r\n");
    printf("1.RCC->CSR =0x%x\r\n", RCC->CSR);
    deleyNop(100);
    RCC->CSR |= 1 << 24;
    deleyNop(100);
    printf("2.RCC->CSR =0x%x\r\n", RCC->CSR);
    deleyNop(2000);
    //Reset in microseconds to milliseconds
    WWDG_ResetConfig(0x7e, 0x7f);
    while (1) {
        //A watchdog feeds a dog and is out of position
        //WWDG_SetCounter(0x7e);
        deleyNop(1);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief   IWDG reset
/// @note    RCC_CSR 24 RMVF rw 0 RMVF£º clear reset flag
///          The software sets' 1 'to clear the reset flag.
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
s32 IWDG_ResetTest(void)
{
    printf("IWDG Reset Test!\r\n");
    printf("1.RCC->CSR =0x%x\r\n", RCC->CSR);
    deleyNop(100);
    RCC->CSR |= 1 << 24;
    deleyNop(100);
    printf("2.RCC->CSR =0x%x\r\n", RCC->CSR);
    deleyNop(2000);
    IWDG_ResetConfig(IWDG_Prescaler_256, 0xff);
    while (1) {
        //A  independent watchdog feeds a dog and is out of position
        //IWDG_RL();
        deleyNop(1);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief   soft reset
/// @note    RCC_CSR 24 RMVF rw 0 RMVF£º clear reset flag
///          The software sets' 1 'to clear the reset flag.
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
s32 SOFT_ResetTest(void)
{
    printf("SOFT Reset Test!\r\n");
    printf("1.RCC->CSR =0x%x\r\n", RCC->CSR);
    deleyNop(100);
    RCC->CSR |= 1 << 24;
    deleyNop(100);
    printf("2.RCC->CSR =0x%x\r\n", RCC->CSR);
    deleyNop(2000);
    NVIC_SystemReset();
    while (1) {
        //Do not run here, the software has been reset
        deleyNop(1);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief   key or Repeat the electricity to reset
/// @note    RCC_CSR 24 RMVF rw 0 RMVF£º clear reset flag
///          The software sets' 1 'to clear the reset flag.
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
s32 PA0_ResetTest(void)
{
    printf("Pin and POR Reset Test!\r\n");
    printf("1.RCC->CSR =0x%x\r\n", RCC->CSR);
    deleyNop(100);
    RCC->CSR |= 1 << 24;
    deleyNop(100);
    printf("2.RCC->CSR =0x%x\r\n", RCC->CSR);
    deleyNop(2000);
    while (1) {
        //Wait for the button or reset
        deleyNop(1);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  delay nTime
/// @note   get x times.
/// @param  DlyTime  nTime .
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void deleyNop(u32 DlyTime)
{
    u32 i, j;
    for(i = 0; i < DlyTime; i++) {
        for(j = 0; j < 100; j++) {
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Tamper test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_CSR_Test(void)
{
    static u8 i = 0;
    i = 1;
    deleyNop(300);
    printf("RCC_CSR TEST!\r\n");
    switch(i) {
        case 1:
            WWDG_ResetTest();
            break;
        case 2:
            IWDG_ResetTest();
            break;
        case 3:
            SOFT_ResetTest();
            break;
        default:
            PA0_ResetTest();
            break;
    }
}

/// @}

/// @}

/// @}
