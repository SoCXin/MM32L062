////////////////////////////////////////////////////////////////////////////////
/// @file     mm32l0xx_it.c
/// @author   AE TEAM
/// @brief    Main Interrupt Service Routines.
///           This file provides template for all exceptions handler
///           and peripherals interrupt service routine.
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
#define _MM32L0xx_IT_C_

// Files includes
#include "platform_config.h"
#include "mm32l0xx_it.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "hw_config.h"
#include "hal_exti.h"
#include "hal_gpio.h"
#include "hal_adc.h"
#include "hal_uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles NMI exception
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void NMI_Handler(void)
{
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles Hard Fault exception.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void HardFault_Handler(void)
{
    // Go to infinite loop when Hard Fault exception occurs
    while (1) {
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles Memory Manage exception.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void MemManage_Handler(void)
{
    // Go to infinite loop when Memory Manage exception occurs
    while (1) {
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles Bus Fault exception.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void BusFault_Handler(void)
{
    // Go to infinite loop when Bus Fault exception occurs
    while (1) {
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles Usage Fault exception.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UsageFault_Handler(void)
{
    // Go to infinite loop when Usage Fault exception occurs
    while (1) {
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles SVCall exception.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SVC_Handler(void)
{
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles Debug Monitor exception.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DebugMon_Handler(void)
{
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles PendSVC exception.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void PendSV_Handler(void)
{
}





////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles USB Low Priority.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

void USB_IRQHandler(void) //mm32
{

    USB_Istr();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles External lines 0 interrupt request.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(GPIO_TAMPER_EXTI_Line) != RESET) {
        //    Send_Buffer[0] = 0x06;
        //
        //    if (GPIO_ReadInputDataBit(GPIO_TAMPER, GPIO_TAMPER_PIN) == Bit_RESET)
        //    {
        //      Send_Buffer[1] = 0x01;
        //    }
        //    else
        //    {
        //      Send_Buffer[1] = 0x00;
        //    }

        //    UserToPMABufferCopy(Send_Buffer, ENDP1_TXADDR, 2);
        //    SetEPTxCount(ENDP1, 2);
        //    SetEPTxValid(ENDP1);

        // Clear the EXTI line 0 pending bit
        //    EXTI_ClearITPendingBit(GPIO_TAMPER_EXTI_Line);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles External lines 0 interrupt request.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(GPIO_KEY2_EXTI_Line) != RESET) {
        //    Send_Buffer[0] = 0x05;
        //
        //    if (GPIO_ReadInputDataBit(GPIO_KEY, GPIO_KEY2_PIN) == Bit_RESET)
        //    {
        //      Send_Buffer[1] = 0x01;
        //    }
        //    else
        //    {
        //      Send_Buffer[1] = 0x00;
        //    }

        //    UserToPMABufferCopy(Send_Buffer, ENDP1_TXADDR, 2);
        //    SetEPTxCount(ENDP1, 2);
        //    SetEPTxValid(ENDP1);

        // Clear the EXTI 1 line  pending bit
        EXTI_ClearITPendingBit(GPIO_KEY2_EXTI_Line);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles External lines 0 interrupt request.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(GPIO_KEY1_EXTI_Line) != RESET) {
        //    Send_Buffer[0] = 0x07;
        //
        //      Send_Buffer[1] =Get_Adc(ADC_AIN_CHANNEL);
        //      UserToPMABufferCopy(Send_Buffer, ENDP1_TXADDR, 2);
        //      SetEPTxCount(ENDP1, 2);
        //      SetEPTxValid(ENDP1);

        // Clear the EXTI 9 to 5 line  pending bit
        EXTI_ClearITPendingBit(GPIO_KEY1_EXTI_Line);
    }
}




/// @}

/// @}

/// @}
