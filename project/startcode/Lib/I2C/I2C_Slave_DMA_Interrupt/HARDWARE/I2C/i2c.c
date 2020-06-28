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
/// @addtogroup BKP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP_Exported_Functions
/// @{

u8 gTxBuff[16] = {0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
u8 gRxBuff[16] ;
u32 gTxFlag = 1;
u32 gRxFlag = 1;
////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC Configure, Applicable to Cortex M0 or M3 processors.
/// @param  NVIC Channel, Priority or SubPriority.
/// @arg    ch: IRQChannel
/// @arg    pri: Priority, Use only Cortex-M3
/// @arg    sub: SubPriority
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void NVIC_Configure(void)
{
    exNVIC_Init_TypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = I2C1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStruct);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Clock and data bus configuration
/// @note   Keep the bus free which means SCK & SDA is high.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C1_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;             //Need extra plus pull
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;  //I2C1 remap IO port
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;  // clock input
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initial I2C
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C_SlaveMode()
{
    I2C_InitTypeDef I2C_InitStructure;
    I2C_StructInit(&I2C_InitStructure);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    I2C_InitStructure. Mode = (I2C_CR_MASTER >> 1); //Slave mode
    I2C_InitStructure. OwnAddress = 0;
    I2C_InitStructure. Speed = I2C_CR_STD;
    I2C_InitStructure. ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
    I2C_ITConfig( I2C1, I2C_IT_RD_REQ, ENABLE );//Read request
    I2C_ITConfig( I2C1, I2C_IT_RX_FULL, ENABLE );//Receive interrupt
    I2C_Cmd(I2C1, ENABLE);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port initialization configuration
/// @note
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_DMA_SlaveInit(void)
{
    I2C_DeInit(I2C1);
    NVIC_Configure();
    I2C1_GPIO_Config();
    I2C_SlaveMode();
    I2C_SendSlaveAddress(I2C1, 0xA8);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C Slave test
/// @note   Keep the bus free which means SCK & SDA is high.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C_DMA_SlaveTest(void)
{
    u32 i;
    while( gTxFlag & gRxFlag);
    for(i = 0; i < 16; i++) {
        printf("TX data%d is  : %x \r\n", i, gTxBuff[i]);
    }
    for(i = 0; i < 16; i++) {
        printf("RX data%d is  : %x \r\n", i, gRxBuff[i]);
    }
    gTxFlag = 1;
    gRxFlag = 1;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief    Use DMA receive data
/// @note     None.
/// @param  : size(Number of data)
/// @param  : rx_buf(receive data buff)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void I2C1_DMA_RX(u8* rx_buf, u16 size)
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
/// @brief    use DMA Send data
/// @note     None.
/// @param  : size(Number of data)
/// @param  : tx_buf(data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void I2C1_DMA_TX(u8* tx_buf, u16 size)
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
void I2C1_IRQHandler(void)
{
    u16 stop_flag, start_flag;
    if(I2C_GetITStatus(I2C1, I2C_IT_RD_REQ)) { //The master has sent a read request from the slave
        I2C1->RD_REQ;
        I2C1_DMA_TX(&gTxBuff[0], 16);
        while(!(I2C1->SR & 0x4));
        I2C_GenerateSTOP( I2C1, ENABLE );
        gTxFlag = 0;
    }
    // interrupt receive
    if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL)) { //Master sends slave receive
        I2C1_DMA_RX(&gRxBuff[0], 16);
        while(!(I2C1->SR & 0x4));
        I2C_GenerateSTOP( I2C1, ENABLE );
        gRxFlag = 0;
    }
    stop_flag = I2C1->STOP;
    start_flag = I2C1->START;
    if((stop_flag  & start_flag) != ((u32)RESET)) { //slave receive
        I2C1-> STOP ;
        I2C1-> START ;
    }
}


/// @}

/// @}

/// @}
