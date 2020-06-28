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

u8 gTxData[256];
u8 gRxData[256];

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CS_Low(SPI_TypeDef* SPIx)
{
    SPI_CSInternalSelected(SPIx, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CS_High(SPI_TypeDef* SPIx)
{
    SPI_CSInternalSelected(SPIx, DISABLE);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialization of DMA Received Data.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI2_DMA_RX_Init()
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
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
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
void SPI2_DMA_TX_Init()
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
/// @brief  Set the DMA Channeln's Peripheral address.
/// @param  channel : where n can be 1 to 7 for DMA1 to select the DMA Channel.
/// @param  length : Transmit lengths.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void DMA_Set_MemoryInc_Enable(DMA_Channel_TypeDef* channel, FunctionalState NewState)
{
    if(NewState == ENABLE) {
        channel->CCR |= DMA_MemoryInc_Enable;
    }
    else {
        channel->CCR  &= ~(DMA_MemoryInc_Enable);
    }
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
void SPI2_MasterWritebuf(u8* ptx_buf,  u32 len)
{
    u32 uByte[2];
    DMA_Set_MemoryInc_Enable(DMA1_Channel5, ENABLE);
    DRV_DMA_TransmitPacket(DMA1_Channel5, (u32)ptx_buf, len);
    DMA_Set_MemoryInc_Enable(DMA1_Channel4, DISABLE);
    DRV_DMA_TransmitPacket(DMA1_Channel4, (u32)uByte, len);
    SPI_DMACmd(SPI2, ENABLE);
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));
    DMA_ClearFlag(DMA1_FLAG_TC5);
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
    DMA_ClearFlag(DMA1_FLAG_TC4);
    SPI_DMACmd(SPI2, DISABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief   DMA transmit and receive packet
/// @param   ptx_buf: Pointer to SPI DMA send buffer(include send and recv bytes).
/// @param   prx_buf: Pointer to SPI DMA recv buffer(include send and recv bytes).
/// @param   len: The length of data , length equal send + recv len).
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void SPI2_MasterReadbuf( u8* prx_buf, u32 len)
{
    u32 uByte[2];
    DMA_Set_MemoryInc_Enable(DMA1_Channel5, DISABLE);
    DRV_DMA_TransmitPacket(DMA1_Channel5, (u32)uByte, len);
    DMA_Set_MemoryInc_Enable(DMA1_Channel4, ENABLE);
    DRV_DMA_TransmitPacket(DMA1_Channel4, (u32)prx_buf, len);

    SPI_DMACmd(SPI2, ENABLE);
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
    DMA_ClearFlag(DMA1_FLAG_TC4);
    SPI_DMACmd(SPI2, DISABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Read data.
/// @note   None.
/// @param  pBuf:Rx buff.
/// @param  len:data length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 W25xx_ReadBurst(u8* pBuf, u16 len)
{
    if(len == 0)
        return 0;
    SPI2_MasterReadbuf(pBuf, len);
    return len;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Write data.
/// @note   None.
/// @param  pBuf:tx buff.
/// @param  len:data length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_WriteBurst(u8* pBuf, u16 len)
{
    if(len == 0)
        return ;
    SPI2_MasterWritebuf(pBuf, len);
    return ;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  check Status
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void W25xx_CheckStatus(void)
{
    u8 temp[5];
    u32 i = 0;
    W25xx_CS_Low(SPI2);
    temp[i++] = RDSR;
    W25xx_WriteBurst(temp, i);
    while(1) {
        W25xx_ReadBurst(&temp[i], 1);
        if(((temp[i]) & 0x01) == 0x0)
            break;
    }
    W25xx_CS_High(SPI2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable write
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_WriteEnable(void)
{
    u8 temp[5];
    u32 i = 0;
    temp[i++] = WREN;
    //Spi cs assign to this pin,select
    W25xx_CS_Low(SPI2);

    W25xx_WriteBurst(temp, i);
    W25xx_CS_High(SPI2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Read ID
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_ReadID(void)
{
    u8 temp[5];
    u32 i = 0;
    temp[i++] = RDID;
    //Spi cs assign to this pin,select
    W25xx_CS_Low(SPI2);

    W25xx_WriteBurst(temp, i);
    W25xx_ReadBurst(&temp[i], 3);

    //Spi cs release
    W25xx_CS_High(SPI2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI Sector Erase
/// @note   None.
/// @param  Addr:address.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_SectorErase(u32 address)
{
    u8 temp[5];
    u32 i = 0;

    address = address & 0xffff0000;
    temp[i++] = SE;
    temp[i++] = ((u8)(address >> 16)) & 0xff;
    temp[i++] = ((u8)(address >> 8)) & 0xff;
    temp[i++] = ((u8)address) & 0xff;
    W25xx_WriteEnable();
    //Spi cs assign to this pin,select
    W25xx_CS_Low(SPI2);
    W25xx_WriteBurst(temp, i);
    //Spi cs release
    W25xx_CS_High(SPI2);
    W25xx_CheckStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH Read
/// @note   None.
/// @param  Address:address.
/// @param  number:length.
/// @param  p:data buff.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_PageRead(u32 address, u8* p, u32 number)
{

    u8 temp[5];
    u32 i = 0;
    address = address & 0xffff0000;
    temp[i++] = READ;
    temp[i++] = ((u8)(address >> 16)) & 0xff;
    temp[i++] = ((u8)(address >> 8)) & 0xff;
    temp[i++] = ((u8)address) & 0xff;
    W25xx_CheckStatus();
    //Spi cs assign to this pin,select
    W25xx_CS_Low(SPI2);
    W25xx_WriteBurst(temp, i);
    W25xx_ReadBurst(p, number);
    W25xx_CS_High(SPI2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH Write
/// @note   None.
/// @param  Address:address.
/// @param  number:length.
/// @param  p:data buff.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_PageProgram(u32 address, u8* p, u32 number)
{
    u8 temp[5];
    u32 i = 0;

    address = address & 0xffff0000;
    temp[i++] = PP;
    temp[i++] = ((u8)(address >> 16)) & 0xff;
    temp[i++] = ((u8)(address >> 8)) & 0xff;
    temp[i++] = ((u8)address) & 0xff;
    W25xx_WriteEnable();
    W25xx_CS_Low(SPI2);
    W25xx_WriteBurst(temp, i);
    W25xx_WriteBurst(p, number);
    W25xx_CS_High(SPI2);
    W25xx_CheckStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialization DMA TX/RX
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_DMA_25xxInit(void)
{
    SPI2_DMA_RX_Init();
    SPI2_DMA_TX_Init();
    SPI2_GPIO_Config();
    W25xx_Init(SPI2, 0x04);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize SPI2 MODE1(master)
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI2_GPIO_Config(void)
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
/// @param  spi_baud_div:Specifies the Baud Rate prescaler value which will be..
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_Init(SPI_TypeDef* SPIx, u16 spi_baud_div)
{
    SPI_InitTypeDef SPI_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = 8;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = (SPI_BaudRatePrescaler_TypeDef)spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);
    W25xx_CS_High(SPIx);
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
    SPI_Cmd(SPIx, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_DMA_25xxTest(void)
{
    u32 i, result = 0;
    printf("\r\nsprintf ok\r\n");
    printf("\r\nStart SPI test\r\n");

    for(i = 0; i < 256; i++) {
        gTxData[i] = i * 2;
    }
    printf("SPI2 test\r\n");
    W25xx_ReadID();
    W25xx_SectorErase(0);
    W25xx_PageProgram(0, gTxData, 256);
    memset(gRxData, 0x0, 256);
    W25xx_PageRead(0, gRxData, 256);
    for(i = 0; i < 10; i++) {
        printf( "rx[%d]=0x%x\r\n", i, gRxData[i]);
    }
    for(i = 0; i < 256; i++) {
        if(gTxData[i] != gRxData[i]) {
            result = 1;
            break;
        }

    }
    if(result == 1) {
        printf("SPI2 WR 25xx Fail\r\n");
    }
    else {
        printf("SPI2 WR 25xx Successful\r\n");

    }
    printf("SPI2 test over\r\n");
}

/// @}

/// @}

/// @}
