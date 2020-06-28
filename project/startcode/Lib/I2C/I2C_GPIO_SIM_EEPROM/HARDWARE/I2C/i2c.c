////////////////////////////////////////////////////////////////////////////////
/// @file    i2c.c
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
#define _I2C_C_

// Files includes
#include "delay.h"
#include "led.h"
#include "i2c.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP_Exported_Functions
/// @{

#define EEPROM_ADDR 0xA0
#define FALSE    0
#define TRUE    1


u8 gTxBuff[128] = {0xab, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
u8 gRxBuff[128];
u16 gDataCnt;

#define EEPROM_PageSize 16//8

////////////////////////////////////////////////////////////////////////////////
/// @brief  Pin initialization configuration
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //Configure I2C1 Pins:SCL and SDA
    RCC_AHBPeriphClockCmd(I2C1_SCL_BUSCLK, ENABLE);
    RCC_AHBPeriphClockCmd(I2C1_SDA_BUSCLK, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
    GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Pin delay.
/// @note   Setting according to the selected clock.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_Delay(void)
{
    //set I2C SCL speed
    u8 i = 13;
    while(i) {
        i--;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  IIC start flag.
/// @note   Pay attention to start and end sequence diagrams when problems arise.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
bool EEPROM_Start(void)
{
    EEPROM_Delay();
    SDA_H;
    SCL_H;
    EEPROM_Delay();
    //If the SDA line is low level, the bus will fail and quit.
    if(!SDA_read) return FALSE;
    SDA_L;
    EEPROM_Delay();
    //If the SDA line is high level, the bus will fail and quit.
    if(SDA_read) return FALSE;
    SDA_L;
    EEPROM_Delay();
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  IIC stop flag.
/// @note   Pay attention to start and end sequence diagrams when problems arise.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Stop(void)
{
    SCL_L;
    EEPROM_Delay();
    SDA_L;
    EEPROM_Delay();
    SCL_H;
    EEPROM_Delay();
    SDA_H;
    EEPROM_Delay();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  IIC ACK flag.
/// @note   Pay attention to diagrams when problems arise.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Ack(void)
{
    SCL_L;
    EEPROM_Delay();
    SDA_L;
    EEPROM_Delay();
    SCL_H;
    EEPROM_Delay();
    SCL_L;
    EEPROM_Delay();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  IIC ACK flag.
/// @note   Pay attention to diagrams when problems arise.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_NoAck(void)
{
    SCL_L;
    EEPROM_Delay();
    SDA_H;
    EEPROM_Delay();
    SCL_H;
    EEPROM_Delay();
    SCL_L;
    EEPROM_Delay();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Wait IIC ACK flag.
/// @note   Pay attention to diagrams when problems arise.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
bool EEPROM_WaitAck(void)
{
    bool bstatus;
    SCL_L;
    EEPROM_Delay();
    SDA_H;
    EEPROM_Delay();
    SCL_H;
    EEPROM_Delay();
    if(SDA_read) {
        //  No ack
        bstatus = FALSE;
    }
    else {
        //  ack
        bstatus = TRUE;
    }
    SCL_L;
    return bstatus;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send data.
/// @note   Data from high to low.
/// @param  SendByte(send a byte data ).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_SendByte(u8 SendByte)
{
    u8 i = 8;
    while(i--) {
        SCL_L;
        EEPROM_Delay();
        if(SendByte & 0x80)
            SDA_H;
        else
            SDA_L;
        SendByte <<= 1;
        EEPROM_Delay();
        SCL_H;
        EEPROM_Delay();
    }
    SCL_L;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive data.
/// @note   Data from high to low.
/// @param  None.
/// @retval ReceiveByte(receive a byte data ).
////////////////////////////////////////////////////////////////////////////////
u8 EEPROM_ReceiveByte(void)
{
    u8 i = 8;
    u8 ReceiveByte = 0;
    SDA_H;
    while(i--) {
        ReceiveByte <<= 1;
        SCL_L;
        EEPROM_Delay();
        SCL_H;
        EEPROM_Delay();
        if(SDA_read) {
            ReceiveByte |= 0x01;
        }
    }
    SCL_L;
    return ReceiveByte;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write data to EEPROM.
/// @note   None.
/// @param  SendByte(send a byte data ).
/// @param  WriteAddress(Starting address ).
/// @param  DeviceAddress(Equipment Address ).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
bool EEPROM_ByteWrite(u8 SendByte, u16 WriteAddress, u8 DeviceAddress)
{
    if(!EEPROM_Start()) return FALSE;
    //Setting High Start Address + Device Address.
    EEPROM_SendByte(((WriteAddress & 0x0700) >> 7) | (DeviceAddress & 0xFE));
    if(!EEPROM_WaitAck()) {
        EEPROM_Stop();
        return FALSE;
    }
    //Setting Low Start Address
    EEPROM_SendByte((u8)(WriteAddress & 0x00FF));
    EEPROM_WaitAck();
    EEPROM_SendByte(SendByte);
    EEPROM_WaitAck();
    EEPROM_Stop();
    //Because you are waiting for the EERPOM write to complete, you can use
    //query or latency£¨10ms£©
    DELAY_Ms(10);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write data to EEPROM.
/// @note   None.
/// @param  SendByte(send a byte data ).
/// @param  WriteAddress(Starting address ).
/// @param  DeviceAddress(Equipment Address ).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
bool EEPROM_PageWrite(u8* pBuffer, u8 length, u16 WriteAddress, u8 DeviceAddress)
{
    if((length + WriteAddress % EEPROM_PageSize) > EEPROM_PageSize) return FALSE;
    if(!EEPROM_Start()) return FALSE;
    //Setting High Start Address + Device Address.
    EEPROM_SendByte(((WriteAddress & 0x0700) >> 7) | (DeviceAddress & 0xFE));

    if(!EEPROM_WaitAck()) {
        EEPROM_Stop();
        return FALSE;
    }
    //Setting Low Start Address
    EEPROM_SendByte((u8)(WriteAddress & 0x00FF));
    EEPROM_WaitAck();

    while(length--) {
        EEPROM_SendByte(*pBuffer);
        EEPROM_WaitAck();
        pBuffer++;
    }
    EEPROM_Stop();
    //Because you are waiting for the EERPOM write to complete, you can use
    //query or latency£¨10ms£©
    DELAY_Ms(10);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write data to EEPROM.
/// @note   None.
/// @param  SendByte(send a byte data ).
/// @param  WriteAddress(Starting address ).
/// @param  DeviceAddress(Equipment Address ).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_BufferWrite(u8* pBuffer, u16 length, u16 WriteAddress, u8 DeviceAddress )
{
    u16 i;
    u16 Addr = 0, count = 0;
    //Write address is the first page of the start page.
    Addr = WriteAddress % EEPROM_PageSize;
    //Number of entries to be written on the start page
    count = EEPROM_PageSize - Addr;
    if(length <= count) {
        //Write only one page of data
        EEPROM_PageWrite(pBuffer, length, WriteAddress, DeviceAddress);
    }
    else {
        //Write the data on the first page first.
        EEPROM_PageWrite(pBuffer, count, WriteAddress, DeviceAddress);
        if((length - count) <= EEPROM_PageSize) {
            //Write one more page at the end of the data
            EEPROM_PageWrite(pBuffer + count, length - count, WriteAddress + count, DeviceAddress);
        }
        else {
            for(i = 0; i < ((length - count) / EEPROM_PageSize); i++) {
                EEPROM_PageWrite(pBuffer + count + i * EEPROM_PageSize, EEPROM_PageSize, WriteAddress + count + i * EEPROM_PageSize, DeviceAddress);
            }
            if( ((length - count) % EEPROM_PageSize) != 0 ) {
                EEPROM_PageWrite(pBuffer + count + i * EEPROM_PageSize, ((length - count) % EEPROM_PageSize), WriteAddress + count + i * EEPROM_PageSize, DeviceAddress);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  read data from EEPROM.
/// @note   None.
/// @param  ReadByte(receive a byte data ).
/// @param  WriteAddress(Starting address ).
/// @param  DeviceAddress(Equipment Address ).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
bool EEPROM_RandomRead(u8* pByte, u16 ReadAddress, u8 DeviceAddress)
{
    if(!EEPROM_Start()) return FALSE;
    //Setting High Start Address + Device Address.
    EEPROM_SendByte(((ReadAddress & 0x0700) >> 7) | (DeviceAddress & 0xFE));
    if(!EEPROM_WaitAck()) {
        EEPROM_Stop();
        return FALSE;
    }
    EEPROM_SendByte((u8)(ReadAddress & 0x00FF));
    EEPROM_WaitAck();
    EEPROM_Start();
    EEPROM_SendByte(((ReadAddress & 0x0700) >> 7) | (DeviceAddress | 0x0001));
    EEPROM_WaitAck();
    *pByte = EEPROM_ReceiveByte();
    EEPROM_NoAck();
    EEPROM_Stop();
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  read data from EEPROM.
/// @note   None.
/// @param  ReadByte(receive a byte data ).
/// @param  WriteAddress(Starting address ).
/// @param  DeviceAddress(Equipment Address ).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
bool EEPROM_SequentialRead(u8* pBuffer, u8 length, u16 ReadAddress, u8 DeviceAddress)
{
    if(!EEPROM_Start()) return FALSE;
    //Setting High Start Address + Device Address.
    EEPROM_SendByte(((ReadAddress & 0x0700) >> 7) | (DeviceAddress & 0xFE));
    if(!EEPROM_WaitAck()) {
        EEPROM_Stop();
        return FALSE;
    }
    EEPROM_SendByte((u8)(ReadAddress & 0x00FF));
    EEPROM_WaitAck();
    EEPROM_Start();
    EEPROM_SendByte(((ReadAddress & 0x0700) >> 7) | (DeviceAddress | 0x0001));
    EEPROM_WaitAck();
    while(length) {
        *pBuffer = EEPROM_ReceiveByte();
        if(length == 1) EEPROM_NoAck();
        else EEPROM_Ack();
        pBuffer++;
        length--;
    }
    EEPROM_Stop();
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write address.
/// @note   None.
/// @param  ReadByte(receive a byte data ).
/// @param  WriteAddress(Starting address ).
/// @param  DeviceAddress(Equipment Address ).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_Writeany(u16 addr, u8* ptr, u16 cnt)
{
    EEPROM_BufferWrite(ptr, cnt, addr, 0xA0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write a data packet.
/// @note   None.
/// @param  addr (Sub address of EEPROM)
/// @param  ptr (Data in the buffer)
/// @param  cnt (Number of data)
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_Write(u16 addr, u8* ptr, u16 cnt)
{
    u16 i;
    for(i = 0; i < cnt; i++) {
        EEPROM_ByteWrite(*ptr++, addr + i, 0xA0);
        DELAY_Ms(1);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive a data packet.
/// @note   None.
/// @param  addr (Sub address of EEPROM)
/// @param  ptr (Data in the buffer)
/// @param  cnt (Number of data)
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_Read(u16 addr, u8* ptr, u16 cnt)
{
    EEPROM_SequentialRead(ptr, cnt, addr, 0xA0);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port initialization configuration
/// @note
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_SIM_I2C_Init()
{
    //Use I2C1 Port to connect external I2C interface type EEPROM 24C02;
    //run Write and Read series bytes Data;
    //Initial I2C
    I2C_GPIO_Config();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIOsim_I2C_WR_EEPROM
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void GPIO_SIM_I2C_Test(void)
{
    u16 i;

    //Write 16 bytes from gTxBuff[128] to 0x10 of EEPROM
    EEPROM_Write(0x10, gTxBuff, 0x10);
    //Read 16 bytes from 0x10 of EEPROM to gRxBuff[128]
    EEPROM_Read(0x10, gRxBuff, 0x10);
    for(i = 0; i < 0x10; i++) {
        gTxBuff[i] = i;
    }
    EEPROM_Writeany(0x20, gTxBuff, 0x8);
    EEPROM_Read(0x20, gRxBuff, 0x08);
    EEPROM_Writeany(0x20 + 0x8, gTxBuff, 0x8 + 0x8);
    EEPROM_Read(0x20 + 0x8, gRxBuff, 0x8 + 0x8);
    gDataCnt = 0;
    for(i = 0; i < 0x10; i++) {
        if((gTxBuff[i]) == (gRxBuff[i])) {
            gDataCnt++;
        }
    }
}




/// @}

/// @}

/// @}
