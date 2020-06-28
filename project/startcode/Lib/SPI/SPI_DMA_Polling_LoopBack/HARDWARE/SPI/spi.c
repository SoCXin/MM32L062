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
#include "uart.h"
#include "spi.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI
/// @{

#define DISSWDIO

static u8 gRxData[64];
static u8 gTxData[64] = {0x55, 0xaa, 0xbb, 0xdd, 0x33, 0x77, 0x22, 0x11, 0x55, 0xaa, 0xbb, 0xdd, 0x33, 0x77, 0x22, 0x11};

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize SPI1 MODE1(master)
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI1_GPIO_Mode1(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);
    //spi1_cs  pa4
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //spi1_sck  pa5
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //spi1_mosi  pa7
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //spi1_miso  pa6
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize SPI2 MODE1(master)
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI2_GPIO_Mode1(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);
    //spi2_cs    pb12
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_sck   pb13
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Modifiable parameter initialization SPI.
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @param  spi_baud_div:Specifies the Baud Rate prescaler value which will be.
/// @param  datawidth:data byte length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI_Config(SPI_TypeDef* SPIx, u16 spi_baud_div, u8 datawidth)
{
    SPI_InitTypeDef SPI_InitStructure;
    SPI_StructInit(&SPI_InitStructure);
    if(datawidth >= 32)
        datawidth = 0;

    if(SPIx == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        SPI1_GPIO_Mode1();
    }
    if(SPIx == SPI2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        SPI2_GPIO_Mode1();
    }
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = datawidth;
#define SPIMODE3
#if  defined(SPIMODE0)
    // mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    // mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
#elif  defined(SPIMODE3)
    // mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    // mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
#else

#endif
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = (SPI_BaudRatePrescaler_TypeDef) spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
    SPI_Cmd(SPIx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Transmit and receive data through peripheral SPIx.
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @param  tx_data:TX DATA.
/// @retval RX DATA.
////////////////////////////////////////////////////////////////////////////////
u32 SPIMReadWriteByte(SPI_TypeDef* SPIx, u8 tx_data)
{
    SPI_SendData(SPIx, tx_data);
    while (1) {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)) {
            return SPI_ReceiveData(SPIx);
        }
    }
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
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = (DMA_memory_incremented_mode_TypeDef) DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
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
    DMA_InitStructure.DMA_MemoryInc = (DMA_memory_incremented_mode_TypeDef) DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
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
/// @brief   DMA transmit and receive packet
/// @param   ptx_buf: Pointer to SPI DMA send buffer(include send and recv bytes).
/// @param   prx_buf: Pointer to SPI DMA recv buffer(include send and recv bytes).
/// @param   len: The length of data , length equal send + recv len).
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void SPI2_ReadWriteBuf(u8* ptx_buf, u8* prx_buf, u32 len)
{
    DRV_DMA_TransmitPacket(DMA1_Channel5, (u32)ptx_buf, len);
    DRV_DMA_TransmitPacket(DMA1_Channel4, (u32)prx_buf, len);
    SPI_CS_Low(SPI2);
    SPI_DMACmd(SPI2, ENABLE);
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));
    DMA_ClearFlag(DMA1_FLAG_TC5);
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
    DMA_ClearFlag(DMA1_FLAG_TC4);
    SPI_DMACmd(SPI2, DISABLE);
    SPI_CS_High(SPI2);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Transceiver/Receiver Init.
/// @note    MISO & MOSI need to be s16-connected.
/// @param   None.
/// @retval  None.
////////////////////////////////////////////////////////////////////////////////
void SPI_DMA_LoopInit(void)
{
    SPI_Config(SPI2, 0x2, 8);
    SPI2_DMA_RX_Init();
    SPI2_DMA_TX_Init();

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Transceiver/Receiver Test.
/// @note    MISO & MOSI need to be s16-connected.
/// @param   None.
/// @retval  None.
////////////////////////////////////////////////////////////////////////////////
void SPI_DMA_LoopTest(void)
{
    u16 j;

    printf("\r\nStart SPI test\r\n");
    SPI_CS_Low(SPI2);
    SPI2_ReadWriteBuf(gTxData, gRxData, 5);
    SPI_CS_High(SPI2);
    if(gRxData[0] != gTxData[0]) {
        j = 1;
    }
    if(j == 1) {
        printf( "SPI2 TxRx Loopback test fail\r\n");
    }
    else {
        printf( "SPI2 TxRx Loopback test Sucessful\r\n");
    }
    printf("SPI2 test over\r\n");

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_CS_Low(SPI_TypeDef* SPIx)
{
    SPI_CSInternalSelected(SPIx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_CS_High(SPI_TypeDef* SPIx)
{
    SPI_CSInternalSelected(SPIx, DISABLE);
}

/// @}

/// @}

/// @}
