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

u8 gTxData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
u8 gRxData[10];
////////////////////////////////////////////////////////////////////////////////
/// @brief  Turn off the data transmission direction of SPI in bidirectional mode
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_TXEn(SPI_TypeDef* SPIx)
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Turn off the data transmission direction of SPI in bidirectional mode
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_RXEn(SPI_TypeDef* SPIx)
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Reset internal NSS pins for selected SPI software
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void W25xx_CS_Low()
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
void W25xx_CS_High()
{
    //Spi cs release
    SPI_CSInternalSelected(SPI2, DISABLE);
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
    GPIO_PinAFConfig(SPI_MOSI_PORT, SPI_MOSI_AFSOURCE, SPI_MOSI_AFMODE);
    GPIO_PinAFConfig(SPI_NSS_PORT, SPI_NSS_AFSOURCE, SPI_NSS_AFMODE);
    GPIO_PinAFConfig(SPI_MISO_PORT, SPI_MISO_AFSOURCE, SPI_MISO_AFMODE);
    GPIO_PinAFConfig(SPI_SCK_PORT, SPI_SCK_AFSOURCE, SPI_SCK_AFMODE);
    W25xx_CS_High();
    //spi2_cs  pb12
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = SPI_NSS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_NSS_PORT, &GPIO_InitStructure);
    //spi2_sck  pb13
    GPIO_InitStructure.GPIO_Pin  = SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);
    //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Pin  = SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
    //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Pin  = SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Modifiable parameter initialization SPI.
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @param  spi_baud_div:Specifies the Baud Rate prescaler value which will be.
/// @param  datawidth:data byte length.
/// @param  mode    :SPI mode.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void SPI2_Config(SPI_TypeDef* SPIx, u16 spi_baud_div)
{

    SPI_InitTypeDef SPI_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = (SPI_BaudRatePrescaler_TypeDef)spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
    SPI_Cmd(SPIx, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Modifiable parameter initialization SPI.
/// @note   None.
/// @param  SPIx:SPI1/SPI2.
/// @param  spi_baud_div:Specifies the Baud Rate prescaler value which will be.
/// @param  datawidth:data byte length.
/// @param  mode    :SPI mode.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_1_32bit_Init(SPI_TypeDef* SPIx, u16 spi_baud_div)
{
    SPI2_GPIO_Config();
    SPI2_Config(SPIx, spi_baud_div);
}


u32 Transform32Bit(u32 n)
{
    n = (n & 0x55555555) << 1 | (n & 0xAAAAAAAA) >> 1;
    n = (n & 0x33333333) << 2 | (n & 0xCCCCCCCC) >> 2;
    n = (n & 0x0F0F0F0F) << 4 | (n & 0xF0F0F0F0) >> 4;
    n = (n & 0x00FF00FF) << 8 | (n & 0xFF00FF00) >> 8;
    n = (n & 0x0000FFFF) << 16 | (n & 0xFFFF0000) >> 16;
    return n;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Transmits a Data through the SPIx peripheral.
/// @note   None.
/// @param  SPIx: SPI1/SPI2
/// @param  Data : Data to be transmitted.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_Send_1_32bits(SPI_TypeDef* SPIx, u32 SendData)
{
    u8 temp = 0;
    u8* puchars;

    puchars = (u8*) &SendData;
    // Write in the TXREG register the data to be sent
    temp = SPIx->ECR;
    temp = temp & 0x1F;
#ifndef SPIUSELSB
    if(temp == 0) {
        temp = 32;
    }
    else {
        SendData = SendData << (32 - temp);
    }

#ifdef USECORTEX_M3
    SendData = __rbit(SendData);
#else
    SendData = Transform32Bit(SendData);
#endif

#endif
    if(temp == 0) {
        temp = 32;
    }
    if(temp <= 8) {
        SPIx->TDR = puchars[0];
    }
    else if(temp <= 16) {
        SPIx->TDR = puchars[0];
        SPIx->TDR = puchars[1];
    }
    else if(temp <= 24) {
        SPIx->TDR = puchars[0];
        SPIx->TDR = puchars[1];
        SPIx->TDR = puchars[2];
    }
    else {
        SPIx->TDR = puchars[0];
        SPIx->TDR = puchars[1];
        SPIx->TDR = puchars[2];
        SPIx->TDR = puchars[3];
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief   Returns the most recent received data by the SPIx peripheral.
/// @note    None.
/// @param   SPIx: SPI1/SPI2
/// @retval  The value of the received data.
////////////////////////////////////////////////////////////////////////////////
u32 SPI_Recv_1_32bits(SPI_TypeDef* SPIx)
{
    u8 temp = 0;
    u32 recvdata = 0;
    u8* puchars;

    puchars = (u8*) &recvdata;

    temp = SPIx->ECR;
    if(temp == 0) {
        temp = 32;
    }
    // use LSB
    if(temp <= 8) {
        puchars[0] = (u8)SPIx->RDR;
    }
    else if(temp <= 16) {
        puchars[0] = (u8)SPIx->RDR;
        puchars[1] = (u8)SPIx->RDR;
    }
    else if(temp <= 24) {
        puchars[0] = (u8)SPIx->RDR;
        puchars[1] = (u8)SPIx->RDR;
        puchars[2] = (u8)SPIx->RDR;
    }
    else {
        puchars[0] = (u8)SPIx->RDR;
        puchars[1] = (u8)SPIx->RDR;
        puchars[2] = (u8)SPIx->RDR;
        puchars[3] = (u8)SPIx->RDR;
    }
#ifndef SPIUSELSB

#ifdef USECORTEX_M3
    SendData = __rbit(SendData);
#else
    recvdata = Transform32Bit(recvdata);
#endif
    recvdata = recvdata >> (32 - temp);
#endif
    return recvdata;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   Transmit and receive data through peripheral SPIx for full duplex mode.
/// @note    None.
/// @param   SPIx: SPI1/SPI2
/// @param   gTxData:send data.
/// @retval  None.
////////////////////////////////////////////////////////////////////////////////
static u32 SPI_WriteRead_1_32bits(SPI_TypeDef* SPIx, u32 gTxData)
{
    u32 gRxData;
    SPI_Send_1_32bits(SPIx, gTxData);
    while (1) {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT)) {
            break;
        }
    }
    while (1) {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)) {
            gRxData = SPI_Recv_1_32bits(SPIx);
            break;
        }
    }
    return gRxData;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   Transceiver/Receiver Test.
/// @note    None.
/// @param   None.
/// @retval  None.
////////////////////////////////////////////////////////////////////////////////
void SPI_1_32bit_Test(void)
{
    u32 t;

    W25xx_CS_Low();
    for(t = 0; t < 10; t++) {
        gRxData[t] = SPI_WriteRead_1_32bits(SPI, gTxData[t]);
        printf("rx[%d]=0x%x\r\n", t, gRxData[t]);
    }
    W25xx_CS_High();

    printf("SPI2 test over\r\n");
}


/// @}

/// @}

/// @}
