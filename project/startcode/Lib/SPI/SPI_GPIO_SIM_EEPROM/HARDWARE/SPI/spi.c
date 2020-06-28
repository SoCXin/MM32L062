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
/// @brief  Initialization  GPIO
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_SimPinConfig(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    //spi2_cs  pb12
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_sck  pb13
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


}

////////////////////////////////////////////////////////////////////////////////
/// @brief  start Initialization  GPIO
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_SIM_SPI_Init(void)
{
    W25xx_SimPinConfig();
    SPI_Stop();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  start flag
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_Start(void)
{
    MOSI_L;
#if (CPOL == 0)
    SCK_L;
    CS_L;
#else
    SCK_H;
    CS_L;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  End flag
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_Stop(void)
{
#if (CPOL == 0)
    SCK_L;
    CS_H;
#else
    SCK_H;
    CS_H;
#endif
    MOSI_L;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Select the Macro definition of SPI to select the mode
/// @note   //bits BITLENGTH = 1~8.
/// @param  senddata:TX data.
/// @retval RX data.
////////////////////////////////////////////////////////////////////////////////
u8 W25xx_ReadWrite8Bits(u8 senddata)
{
    u8 i = 0;
    u8 recvdata = 0x00;

    for(i = 0; i < BITLENGTH; i++) {
#if (((CPOL == 0)&&(CPHA == 0))||((CPOL == 1)&&(CPHA == 1)))
        SCK_L;
#else
        SCK_H;
#endif
#if ( MSB ==1)
        // Tx data use MSB mode
        if(senddata & ((0x01) << (BITLENGTH - 1))) {
            MOSI_H;
        }
        else {
            MOSI_L;
        }
        senddata <<= 1;

#else
        // Tx data use LSB mode
        if(senddata & 0x1) {
            MOSI_H;
        }
        else {
            MOSI_L;
        }
        senddata >>= 1;
#endif

#if (((CPOL == 0)&&(CPHA == 0))||((CPOL == 1)&&(CPHA == 1)))
        SCK_H;
#else
        SCK_L;
#endif

#if ( MSB ==1)               // Rx data use MSB mode
        recvdata <<= 1 ;
        if((MISO_IN) != 0) {
            recvdata++;
        }
#else
        if((MISO_IN) != 0) {
            recvdata = recvdata + (1 << i);// Rx data use LSB mode
        }
#endif
    }

    return recvdata;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void W25xx_CS_Low(void)
{
    //Spi cs assign to this pin,select
    SPI_Start();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void W25xx_CS_High(void)
{
    //Spi cs release
    SPI_Stop();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Transmit and receive data through peripheral SPIx for full duplex mode
///         (simultaneous transceiver)
/// @note   None.
/// @param  tx_data:send data.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 W25xx_ReadWriteByte(u8 tx_data)
{
    return ((u8)W25xx_ReadWrite8Bits((u8)tx_data));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Read ID
/// @note   None.
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_ReadID()
{
    __IO u8 temp[5];
    u32 i;

    W25xx_CS_Low();
    W25xx_ReadWriteByte(RDID);

    for(i = 0; i < 3; i++) {
        temp[i] = W25xx_ReadWriteByte(0x01);
    }
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Turn off the data transmission direction of SPI in bidirectional mode
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_WriteEnable()
{
    W25xx_CS_Low();
    W25xx_ReadWriteByte(WREN);
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  check Status
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CheckStatus()
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
/// @brief  disable TX
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_WriteDisable()
{
    W25xx_CS_Low();
    W25xx_ReadWriteByte(WRDI);
    W25xx_CS_High();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH Read
/// @note   page = 256 bytes.
/// @param  Address:address.
/// @param  number:length.
/// @param  p:data buff.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_PageRead(u32 address, u8* p, u32 number)
{
    u8 addr0, addr1, addr2;
    u32 i;
    //page address
    address = address & 0xffffff00;
    addr0 = (u8)(address >> 16);
    addr1 = (u8)(address >> 8);
    addr2 = (u8)address;

    W25xx_CS_Low();

    W25xx_ReadWriteByte(READ);
    W25xx_ReadWriteByte(addr0);
    W25xx_ReadWriteByte(addr1);
    W25xx_ReadWriteByte(addr2);

    for(i = 0; i < number; i++) {
        gRxData[i] = W25xx_ReadWriteByte(0x00);
    }

    W25xx_CS_High();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI FLASH write
/// @note   page = 256 bytes.
/// @param  Address:address.
/// @param  number:length.
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
    //Spi cs assign to this pin,select
    W25xx_CS_Low();
    W25xx_ReadWriteByte(PP);
    W25xx_ReadWriteByte(addr0);
    W25xx_ReadWriteByte(addr1);
    W25xx_ReadWriteByte(addr2);
    for(j = 0; j < number; j++) {
        W25xx_ReadWriteByte(*p);
        p++;
    }

    W25xx_CS_High();

    W25xx_CheckStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI Sector Erase
/// @note   None.
/// @param  Addr:address.
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
    W25xx_CheckStatus();
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
/// @brief  SPI Sector Erase
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_BlockErase()
{
    W25xx_WriteEnable();
    //Spi cs assign to this pin,select
    W25xx_CS_Low();

    W25xx_ReadWriteByte(BE);

    W25xx_CS_High();

    W25xx_CheckStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  STOP TX/RX
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_Close()
{
    W25xx_CS_High();
    W25xx_ReadWriteByte(0x01);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Test program, using serial port to print 256 pages of data
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_SIM_SPI_Test(void)
{
    s32 result = 0;
    u32 i;

    W25xx_ReadID();
    for(i = 0; i < 5; i++) {
        W25xx_ReadID();
    }
    W25xx_SectorErase(0x400);
    for(i = 0; i < 256; i++) {
        gRxData[i] = 0x0;
    }
    W25xx_PageRead(0x400, gRxData, 256);
    for(i = 0; i < 256; i++) {
        gTxData[i] = i;
    }
    W25xx_PageProgram(0x400, gTxData, 256);

    for(i = 0; i < 256; i++) {
        gRxData[i] = 0x0;
    }
    W25xx_PageRead(0x400, gRxData, 256);

    for(i = 0; i < 256; i++) {
        if(gRxData[i] != gTxData[i]) {
            result = 1;
            break;
        }
    }
    W25xx_SectorErase(0x400);
    for(i = 0; i < 256; i++) {
        gRxData[i] = 0x0;
    }
    W25xx_PageRead(0x400, gRxData, 256);
    for(i = 0; i < 256; i++) {
        gTxData[i] = 2 * i;
    }
    W25xx_PageProgram(0x400, gTxData, 256);

    for(i = 0; i < 256; i++) {
        gRxData[i] = 0x0;
    }
    W25xx_PageRead(0x400, gRxData, 256);

    for(i = 0; i < 256; i++) {
        if(gRxData[i] != gTxData[i]) {
            result = 1;
            break;
        }
    }
    if ( result == 1) {
        LED1_ON();
        LED2_ON();
        LED3_ON();
        LED4_ON();
    }
    else {
        LED1_OFF();
        LED2_OFF();
        LED3_OFF();
        LED4_OFF();
    }
}
/// @}

/// @}

/// @}
