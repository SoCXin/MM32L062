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
/// @brief  set MCO, use SYSCLK
/// @param  void
/// @retval None
////////////////////////////////////////////////////////////////////////////////
static void MCO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource9, GPIO_AF_5);
    //MCO use PA9
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_MCOConfig(RCC_MCO_SYSCLK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Tamper test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_OUT_Init(void)
{

    MCO_Config();

}





/// @}

/// @}

/// @}
