////////////////////////////////////////////////////////////////////////////////
/// @file    bkp.c
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
#define _BKP_C_

// Files includes

#include "bkp.h"
#include "led.h"
#include "delay.h"



////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{



////////////////////////////////////////////////////////////////////////////////
/// @brief  : Write data to register library
/// @param  : FirstBackupData
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void WriteToBackupReg(u16 FirstBackupData)
{
    u32 index = 0;

    for (index = 0; index < 10; index++) {
        // Write data into the corresponding register
        BKP_WriteBackupRegister(BKPDataReg[index], FirstBackupData);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  : Determine if the write data is correct
/// @param  : FirstBackupData
/// @retval : index + 1,error
///           0,success
////////////////////////////////////////////////////////////////////////////////
u8 CheckBackupReg(u16 FirstBackupData)
{
    u32 index = 0;

    for (index = 0; index < 10; index++) {
        if (BKP_ReadBackupRegister(BKPDataReg[index]) != FirstBackupData) {
            return (index + 1);
        }
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  : Configure the NVIC
/// @param  : None
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    // Enable TAMPER IRQChannel
    NVIC_InitStructure.NVIC_IRQChannel = BKP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  : Start intrusion detection
/// @param  : Portx , State
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void  BKP_DATA(void)
{
    NVIC_Configuration();
    // Enable PWR and BKP clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // Enable write access to Backup domain
    PWR_BackupAccessCmd(ENABLE);
    // Disable Tamper pin
    BKP_TamperPinCmd(DISABLE);

    // Disable Tamper interrupt
    BKP_ITConfig(DISABLE);

    // Tamper pin active on low level
    BKP_TamperPinLevelConfig(BKP_TamperPinLevel_Low);

    // Clear Tamper pin Event(TE) pending flag
    BKP_ClearFlag();

    // Enable Tamper interrupt
    BKP_ITConfig(ENABLE);
    // Enable Tamper pin
    BKP_TamperPinCmd(ENABLE);
    // Write data to register library
    WriteToBackupReg(0xabcd);
    // Check if the written data are correct
    if(CheckBackupReg(0xabcd) == 0x00) {
        // Turn off LED1
        LED1_OFF();//Correct data reading and writing
    }
    else {
        // Turn off LED3
        LED3_OFF();//Data read-write error
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Tamper test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Tampertest(void)
{

    BKP_DATA();
    while(1) {

    }
}




/// @}

/// @}

/// @}
