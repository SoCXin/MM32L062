////////////////////////////////////////////////////////////////////////////////
/// @file    spi.c
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
#define _SPI_C_

// Files includes
#include "uart.h"
#include "spi.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI
/// @{

u8 gRxData[256];
s32 gRxFlag;
s32 gRxCnt;
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  Interrupt function in SPI2 to get SPI2 sending and receiving
///         interrupt flag bit.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI2_IRQHandler (void)
{

    if(SPI_GetITStatus(SPI2, SPI_IT_TXEPT)) {
        SPI_ClearITPendingBit(SPI2, SPI_IT_TXEPT);

    }
    if(SPI_GetITStatus(SPI2, SPI_IT_RX)) {
        gRxData[gRxCnt] = SPI_ReceiveData(SPI2);
        SPI_SendData(SPI2, gRxData[gRxCnt]);
        gRxCnt++;
        if(gRxCnt == 20) {
            gRxCnt = 0;
            gRxFlag = 1;
        }
        //clear rx interrupt
        SPI_ClearITPendingBit(SPI2, SPI_IT_RX);

    }

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC SPI2 Config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI2_NVIC_Init(void)
{

    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize SPI2
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI2_GPIO_SlaveInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    //Enable SPI2 clock and GPIO clock for SPI2 and SPI
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);;

    //Configure SPI2 pins: SCK
    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_cs  pb12
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI Slave
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI2_SlaveConfig(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    SPI_StructInit(&SPI_InitStructure);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    // Slave Mode
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = 8;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI2, &SPI_InitStructure);

    // Enable SPI2 RXNE interrupt
    SPI_ITConfig(SPI2, SPI_IT_RX, ENABLE);
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
    SPI_Cmd(SPI2, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  The functional test of SPI.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_NVIC_SlaveInit(void)
{
    SPI2_NVIC_Init();
    SPI2_GPIO_SlaveInit();
    SPI2_SlaveConfig();

}


/// @}

/// @}

/// @}
