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
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "spi.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI
/// @{

u8 gTxData[256];
u8 gRxData[256];
s32 gRxFlag;
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  IRQ Handler
/// @note    If the program fails, see here.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel4_5_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_IT_TC4)) {
        gRxFlag = 1;
        DMA_ClearITPendingBit(DMA1_IT_TC4);
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set the DMA Channeln's Peripheral address.
/// @param  channel : where n can be 1 to 7 for DMA1 to select the DMA Channel.
/// @param  length : Transmit lengths.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void DMA_Set_TransmitLen(DMA_Channel_TypeDef* channel, u16 length)
{
    channel->CNDTR = length;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set the DMA Channeln's Peripheral address.
/// @param  channel : where n can be 1 to 7 for DMA1 to select the DMA Channel.
/// @param  address : DMA memery address.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void DMA_Set_MemoryAddress(DMA_Channel_TypeDef* channel, u32 address)
{
    channel->CMAR = address;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief   DMA transmits packet
/// @param   ch: Pointer to a DMA channel.
/// @param   addr: The memory Buffer address.
/// @param   len: The length of data to be transferred.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DMA_TransmitPacket(DMA_Channel_TypeDef* ch, u32 addr, u32 len)
{
    DMA_ITConfig(ch, DMA_IT_TC, ENABLE);
    DMA_Cmd(ch, DISABLE);
    DMA_Set_MemoryAddress(ch, addr);
    DMA_Set_TransmitLen(ch, len);
    DMA_Cmd(ch, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC SPI2 Config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void DMA_NVIC_Init(void)
{

    NVIC_InitTypeDef  NVIC_InitStructure;
    // Enable the UARTy_DMA1_IRQn Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialization of DMA Received Data.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI2_DMA_RX_Init(void)
{
    //DMA1  CHANNEL4
    DMA_InitTypeDef  DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel4);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (SPI2->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)gRxData;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 20;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

    SCB->AIRCR = 0x05FA0000 | 0x600;
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    SPI_DMACmd(SPI2, ENABLE);
    DMA_Cmd(DMA1_Channel4, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialization of DMA Send Data.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI2_DMA_TX_Init(void)
{
    //DMA1  CHANNEL5
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel5);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (SPI2->TDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)gTxData;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
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
void SPI_DMA_SlaveInit(void)
{
    SPI2_GPIO_SlaveInit();
    SPI2_SlaveConfig();
    DMA_NVIC_Init();
    SPI2_DMA_RX_Init();

}


/// @}

/// @}

/// @}
