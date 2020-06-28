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
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Functions
/// @{
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
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
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

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    W25xx_CS_High();

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
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
    SPI_Cmd(SPI2, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Modifiable parameter initialization SPI.
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @param  spi_baud_div:Specifies the Baud Rate prescaler value which will be..
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_NSS_Init(u16 spi_baud_div)
{
    SPI2_Config(spi_baud_div);
    SPI2_GPIO_Config();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Read ID
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_ReadID(void)
{
    u8 temp;
    u32 i;
    //Spi cs assign to this pin,select
    W25xx_CS_Low();
    W25xx_ReadWriteByte(RDID);
    for(i = 0; i < 3; i++) {
        temp = W25xx_ReadWriteByte(0x01);
        printf("temp=0x%x\r\n", temp);
    }
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH Write
/// @note   None.
/// @param  Address:address.
/// @param  number:data length.
/// @param  p:data buff.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_PageProgram(u32 address, u8* p, u32 number)
{
    u32 j;
    u8 addr0, addr1, addr2;
    //page address
    address = address & 0xffffff00;
    addr0 = (u8)(address >> 16);
    addr1 = (u8)(address >> 8);
    addr2 = (u8)address;

    W25xx_WriteEnable();
    W25xx_CS_Low();
    W25xx_ReadWriteByte(PP);
    W25xx_ReadWriteByte(addr0);
    W25xx_ReadWriteByte(addr1);
    W25xx_ReadWriteByte(addr2);
    for(j = 0; j < number; j++) {
        W25xx_ReadWriteByte(*(p++));
    }
    W25xx_CS_High();
    W25xx_CheckStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI Sector Erase
/// @note   None.
/// @param  address:address.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_SectorErase(u32 address)
{
    u8 addr0, addr1, addr2;
    address = address & 0xffff0000;
    addr0 = ((u8)(address >> 16)) & 0xff;
    addr1 = ((u8)(address >> 8)) & 0xff;
    addr2 = ((u8)address) & 0xff;

    W25xx_WriteEnable();
    //Spi cs assign to this pin,select
    W25xx_CS_Low();
    W25xx_ReadWriteByte(SE);
    W25xx_ReadWriteByte(addr0);
    W25xx_ReadWriteByte(addr1);
    W25xx_ReadWriteByte(addr2);
    W25xx_CS_High();
    W25xx_CheckStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH Read
/// @note   None.
/// @param  Address:address.
/// @param  number:data length.
/// @param  p:data buff.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_PageRead(u32 address, u8* p, u32 number)
{
    u8 addr0, addr1, addr2;
    u32 i;
    address = address & 0xffffff00;
    addr0 = (u8)(address >> 16);
    addr1 = (u8)(address >> 8);
    addr2 = (u8)address;

    W25xx_CS_Low();
    W25xx_ReadWriteByte(READ);
    W25xx_ReadWriteByte(addr0);
    W25xx_ReadWriteByte(addr1);
    W25xx_ReadWriteByte(addr2);
    for(i = 0; i < 256; i++) {
        gRxData[i] = W25xx_ReadWriteByte(0x00);
    }
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable FLASH Read
/// @note   None.
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_WriteEnable(void)
{
    W25xx_CS_Low();
    W25xx_ReadWriteByte(WREN);
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  check Status
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CheckStatus(void)
{
    u8 temp;
    W25xx_CS_Low();
    W25xx_ReadWriteByte(RDSR);
    while(1) {
        temp = W25xx_ReadWriteByte(0x00);
        if((temp & 0x01) == 0x0)
            break;
    }
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CS_Low(void)
{
    GPIO_ResetBits( GPIOB, GPIO_Pin_12 );
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
    GPIO_SetBits( GPIOB, GPIO_Pin_12 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH Write Read
/// @note   None.
/// @param  tx_data:data.
/// @retval rx data.
////////////////////////////////////////////////////////////////////////////////
u32 W25xx_ReadWriteByte(u8 tx_data)
{
    SPI_SendData(SPI2, tx_data);
    while (1) {
        if(SPI_GetFlagStatus(SPI2, SPI_FLAG_RXAVL)) {
            return SPI_ReceiveData(SPI2);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  The functional test of SPI.
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_NSS_Test(void)
{
    u32 i;
    for(i = 0; i < 256; i++) {
        gTxData[i] = i * 2;
    }
    printf("SPI2 test\r\n");
    W25xx_ReadID();
    W25xx_SectorErase(0);
    W25xx_PageProgram(0, gTxData, 256);

    for(i = 0; i < 256; i++) {
        gRxData[i] = 0x0;
    }
    W25xx_PageRead(0, gRxData, 256);

    for(i = 0; i < 10; i++) {
        printf("rx[%d]=0x%x\r\n", i, gRxData[i]);
    }
    printf("SPI2 test over\r\n");
}


/// @}

/// @}

/// @}
