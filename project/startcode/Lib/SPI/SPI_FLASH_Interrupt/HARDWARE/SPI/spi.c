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
volatile u8 gRxFlag;
volatile u8 gTxFlag;
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH Read
/// @note   page = 256 bytes.
/// @param  Address:address.
/// @param  number:length.
/// @param  p:data buff.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_PageRead(SPI_TypeDef* SPIx, u32 address, u8* p, u32 number)
{
    u8 addr0, addr1, addr2;
    u32 i;
    address = address & 0xffffff00;
    addr0 = (u8)(address >> 16);
    addr1 = (u8)(address >> 8);
    addr2 = (u8)address;
    W25xx_CS_Low();
    W25xx_ReadWriteByte(SPIx, READ);
    W25xx_ReadWriteByte(SPIx, addr0);
    W25xx_ReadWriteByte(SPIx, addr1);
    W25xx_ReadWriteByte(SPIx, addr2);

    for(i = 0; i < 256; i++) {
        gRxData[i] = W25xx_ReadWriteByte(SPIx, 0x00);
    }
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH Write
/// @note   page = 256 bytes.
/// @param  Address:address.
/// @param  number:length.
/// @param  p:data buff.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_PageProgram(SPI_TypeDef* SPIx, u32 address, u8* p, u32 number)
{
    u32 j;
    u8 addr0, addr1, addr2;
    address = address & 0xffffff00;
    addr0 = (u8)(address >> 16);
    addr1 = (u8)(address >> 8);
    addr2 = (u8)address;
    W25xx_WriteEnable(SPIx);
    W25xx_CS_Low();
    W25xx_ReadWriteByte(SPIx, PP);
    W25xx_ReadWriteByte(SPIx, addr0);
    W25xx_ReadWriteByte(SPIx, addr1);
    W25xx_ReadWriteByte(SPIx, addr2);
    for(j = 0; j < number; j++) {
        W25xx_ReadWriteByte(SPIx, *(p++));
    }
    W25xx_CS_High();
    W25xx_CheckStatus(SPIx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI Sector Erase
/// @note   None.
/// @param  Addr:address.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_SectorErase(SPI_TypeDef* SPIx, u32 address)
{
    u8 addr0, addr1, addr2;
    address = address & 0xffff0000;
    addr0 = ((u8)(address >> 16)) & 0xff;
    addr1 = ((u8)(address >> 8)) & 0xff;
    addr2 = ((u8)address) & 0xff;
    W25xx_WriteEnable(SPIx);
    W25xx_CS_Low();
    W25xx_ReadWriteByte(SPIx, SE);
    W25xx_ReadWriteByte(SPIx, addr0);
    W25xx_ReadWriteByte(SPIx, addr1);
    W25xx_ReadWriteByte(SPIx, addr2);
    W25xx_CS_High();
    W25xx_CheckStatus(SPIx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI send a byte data
/// @note   None.
/// @param  tx_data:send data.
/// @retval RX data.
////////////////////////////////////////////////////////////////////////////////
u32 W25xx_ReadWriteByte(SPI_TypeDef* SPIx, u8 tx_data)
{
    SPI_SendData(SPIx, tx_data);
    while(1) {
        if(gTxFlag == 1) {
            gTxFlag = 0;
            break;
        }
    }
    while (1) {
        if(gRxFlag == 1) {
            gRxFlag = 0;
            return SPI_ReceiveData(SPIx);
        }
    }
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
/// @brief  SPI Config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI2_Config(u16 spi_baud_div)
{
    SPI_InitTypeDef SPI_InitStructure;
    //SPI2 clk enable
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
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_ITConfig(SPI2, SPI_IT_RX | SPI_IT_TXEPT, ENABLE);

    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
    SPI_Cmd(SPI2, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC SPI2 Config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI2_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Modifiable parameter initialization SPI.
/// @note   None.
/// @param  datawidth:data byte length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_NVIC_FlashInit(u16 spi_baud_div)
{
    SPI2_NVIC_Init();
    SPI2_GPIO_Config();
    SPI2_Config(spi_baud_div);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Read ID
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_ReadID(SPI_TypeDef* SPIx)
{
    u8 temp;
    u32 i;
    W25xx_CS_Low();
    W25xx_ReadWriteByte(SPIx, RDID);
    for(i = 0; i < 3; i++) {
        temp = W25xx_ReadWriteByte(SPIx, 0x01);
        printf("temp=0x%x\r\n", temp);
    }
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable Read
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_WriteEnable(SPI_TypeDef* SPIx)
{
    W25xx_CS_Low();
    W25xx_ReadWriteByte(SPIx, WREN);
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  check Status
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CheckStatus(SPI_TypeDef* SPIx)
{
    u8 temp;
    W25xx_CS_Low();
    W25xx_ReadWriteByte(SPIx, RDSR);
    while(1) {
        temp = W25xx_ReadWriteByte(SPIx, 0x00);
        if((temp & 0x01) == 0x0)
            break;
    }
    W25xx_CS_High();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Interrupt function in SPI2 to get SPI2 sending and receiving
///         interrupt flag bit.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI2_IRQHandler (void)
{
    while(1) {
        if(SPI_GetITStatus(SPI2, SPI_IT_TXEPT)) {
            SPI_ClearITPendingBit(SPI2, SPI_IT_TXEPT);
            gTxFlag = 1;
            break;
        }

        if(SPI_GetITStatus(SPI2, SPI_IT_RX)) {
            SPI_ClearITPendingBit(SPI2, SPI_IT_RX);                             //clear rx interrupt
            gRxFlag = 1;
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CS_Low(void)
{
    //Spi cs assign to this pin,select
    SPI_CSInternalSelected(SPI2, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CS_High(void)
{
    //Spi cs release
    SPI_CSInternalSelected(SPI2, DISABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_NVIC_FlashTest(void)
{
    u32 i;
    gRxFlag = 0;
    gTxFlag = 0;
    for(i = 0; i < 256; i++) {
        gTxData[i] = i + 2;
    }
    printf("SPI2 test\r\n");
    W25xx_ReadID(SPI2);
    W25xx_SectorErase(SPI2, 0);
    W25xx_PageProgram(SPI2, 0, gTxData, 256);
    W25xx_PageRead(SPI2, 0, gRxData, 256);
    for(i = 0; i < 8; i++) {
        printf("rx[%d]=0x%x\r\n", i, gRxData[i]);
    }
    printf("SPI2 test over\r\n");
}
/// @}

/// @}

/// @}
