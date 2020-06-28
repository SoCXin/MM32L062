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
#include "uart.h"
#include "i2c.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C_Exported_Functions
/// @{
u8 gTxBuff[16] = {0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
u8 gRxBuff[16] ;

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clock and data bus configuration
/// @note   Keep the bus free which means SCK & SDA is high.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(I2C1_SDA_BUSCLK, ENABLE);
    RCC_AHBPeriphClockCmd(I2C1_SCL_BUSCLK, ENABLE);
    GPIO_PinAFConfig(I2C1_SCL_PORT, I2C1_SCL_AFSOURCE, I2C1_SCL_AFMODE);
    GPIO_PinAFConfig(I2C1_SDA_PORT, I2C1_SDA_AFSOURCE, I2C1_SDA_AFMODE);
    GPIO_InitStructure.GPIO_Pin  = I2C1_SCL_PIN;
    //Set GPIO spped
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    //Keep the bus free which means SCK & SDA is high
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = I2C1_SDA_PIN;
    GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief    Set the device address
/// @note     None.
/// @param  : I2Cx(where x can be 1 or 2 to select the I2C peripheral)
/// @param  : deviceaddr(device address).
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C_SetDeviceAddr(I2C_TypeDef* I2Cx, u8 deviceaddr)
{
    //Disable I2C
    I2C_Cmd(I2Cx, DISABLE);
    //Set the device address
    I2C_Send7bitAddress(I2Cx, deviceaddr, I2C_Direction_Transmitter);
    //Enable I2C
    I2C_Cmd(I2Cx, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port initialization configuration
/// @note
/// @param  Speed: I2C_CR_STD/I2C_CR_FAST
///         ClockSpeed:0-100k/400k
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_MasterModeInit(u16 Speed, u32 ClockSpeed, u8 deviceaddr)
{
    I2C_InitTypeDef I2C_InitStructure;
    I2C_StructInit(&I2C_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    I2C_InitStructure.I2C_Mode = I2C_CR_MASTER;
    I2C_InitStructure.I2C_OwnAddress = deviceaddr;
    I2C_InitStructure.I2C_Speed = Speed;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port initialization configuration
/// @note
/// @param  : deviceaddr(device address).
///           Speed: I2C_CR_STD/I2C_CR_FAST
///           ClockSpeed:0-100k/400k
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_DMA_Init(void)
{
    I2C_DeInit(I2C1);
    I2C1_GPIO_Config();
    I2C_MasterModeInit(I2C_CR_FAST, 400000, EEPROM_ADDR);
    I2C_SetDeviceAddr(I2C1, EEPROM_ADDR);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART Trasmit by DMA mode initialize
/// @note   first note input memory address and receive bytes counter.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_DMA_TxInit(u8* tx_buf, u16 size)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel2);
    DMA_StructInit(&DMA_InitStructure);
    // Initialize the DMA_PeripheralBaseAddr member
    DMA_InitStructure.DMA_PeripheralBaseAddr = I2C1_BASE + 0x10;
    // Initialize the DMA_MemoryBaseAddr member
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)tx_buf;
    // Initialize the DMA_DIR member
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // Initialize the DMA_BufferSize member
    DMA_InitStructure.DMA_BufferSize = size;
    // Initialize the DMA_PeripheralInc member
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // Initialize the DMA_MemoryInc member
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable ;
    // Initialize the DMA_PeripheralDataSize member
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    // Initialize the DMA_MemoryDataSize member
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // Initialize the DMA_Mode member
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    // Initialize the DMA_Priority member
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    // Initialize the DMA_M2M member
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel2, ENABLE);
    // Enable DMA transmission
    I2C1->DMA |= TDMAE_SET;
    //Waiting for the completion of DMA transmission
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));
    DMA_Cmd(DMA1_Channel2, DISABLE);
    // Disable DMA transmission
    I2C1->DMA &= ~TDMAE_SET;

}
////////////////////////////////////////////////////////////////////////////////
/// @brief    Use DMA receive data
/// @note     None.
/// @param  : size(Number of data)
/// @param  : rx_buf(receive data buff)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_DMA_RxInit(u8* rx_buf, u16 size)
{
    u32 ui_Tdata = 0x100;
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    //Disable DMA transmission
    I2C1->DMA |= RDMAE_SET | TDMAE_SET;
    DMA_DeInit(DMA1_Channel3);
    DMA_StructInit(&DMA_InitStructure);
    // Initialize the DMA_PeripheralBaseAddr member
    DMA_InitStructure.DMA_PeripheralBaseAddr = I2C1_BASE + 0x10;
    // Initialize the DMA_MemoryBaseAddr member
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)rx_buf;
    // Initialize the DMA_DIR member
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    // Initialize the DMA_BufferSize member
    DMA_InitStructure.DMA_BufferSize = size;
    // Initialize the DMA_PeripheralInc member
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // Initialize the DMA_MemoryInc member
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable ;
    // Initialize the DMA_PeripheralDataSize member
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    // Initialize the DMA_MemoryDataSize member
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // Initialize the DMA_Mode member
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    // Initialize the DMA_Priority member
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    // Initialize the DMA_M2M member
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    DMA_DeInit(DMA1_Channel2);
    // Initialize the DMA_PeripheralBaseAddr member
    DMA_InitStructure.DMA_PeripheralBaseAddr = I2C1_BASE + 0x10;
    // Initialize the DMA_MemoryBaseAddr member
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ui_Tdata;
    // Initialize the DMA_DIR member
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // Initialize the DMA_BufferSize member
    DMA_InitStructure.DMA_BufferSize = size;
    // Initialize the DMA_PeripheralInc member
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // Initialize the DMA_MemoryInc member
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable  ;
    // Initialize the DMA_PeripheralDataSize member
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    // Initialize the DMA_MemoryDataSize member
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    // Initialize the DMA_Mode member
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    // Initialize the DMA_Priority member
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    // Initialize the DMA_M2M member
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel2, ENABLE);
    // Wait for DMA transfer to complete
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)) {
        //I2C1->IC_DATA_CMD = 0x100;
    }
    DMA_Cmd(DMA1_Channel3, DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    I2C1->DMA &= ~(RDMAE_SET | TDMAE_SET);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief    Send data
/// @note     None.
/// @param  : device_id( Slave address )
/// @param  : mem_byte_addr(Write data)
/// @param  : tx_count(Number of data)
/// @param  : tx_data(data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_DMA_Write(u16 tx_count, u8* tx_data )
{
    u8 byte = 0;
    EEPROM_DMA_TxInit((u8*)&byte, 1);
    EEPROM_DMA_TxInit(tx_data, tx_count);
    //  while(!I2C_GetFlagStatus(I2C1,I2C_STATUS_FLAG_TFE));
    while(!(I2C1->SR & 0x4));
    I2C_GenerateSTOP( I2C1, ENABLE );
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive data
/// @note   None.
/// @param  : device_id( Slave address )
/// @param  : mem_byte_addr(Write data)
/// @param  : rx_count(Number of data)
/// @param  : rx_data(data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_DMA_Read(u16 rx_count, u8* rx_data )
{
    u8 byte = 0;
    EEPROM_DMA_TxInit((u8*)&byte, 1);
    EEPROM_DMA_RxInit(rx_data, rx_count);
    while(!(I2C1->SR & 0x4));
    I2C_GenerateSTOP( I2C1, ENABLE );
}
////////////////////////////////////////////////////////////////////////////////
/// @brief    Write and read data from EEPROM.
/// @note     None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C_DMA_Test(void)
{
    u32 i = 100000;
    EEPROM_DMA_Write(16, gTxBuff );
    while(i--);
    EEPROM_DMA_Read( 16, gRxBuff );
    for(i = 0; i < 16; i++) {
        printf("RX data%d is  : %x \r\n", i, gRxBuff[i]);
    }
    printf("i2c1 dma test over\r\n");
}

/// @}

/// @}

/// @}
