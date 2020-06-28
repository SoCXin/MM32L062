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

volatile u8 gRxFlag = 0;
volatile u8 gTxFlag = 0;
u8 gData;
u8 gTxBuff[16] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
u8 gRxBuff[16];

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clock and data bus configuration
/// @note   Keep the bus free which means SCK & SDA is high.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void I2C1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(I2C1_SDA_BUSCLK, ENABLE);
    RCC_AHBPeriphClockCmd(I2C1_SCL_BUSCLK, ENABLE);
    GPIO_PinAFConfig(I2C1_SCL_PORT, I2C1_SCL_AFSOURCE, I2C1_SCL_AFMODE);
    GPIO_PinAFConfig(I2C1_SDA_PORT, I2C1_SDA_AFSOURCE, I2C1_SDA_AFMODE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = I2C1_SCL_PIN;
    //Set GPIO spped
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    //Keep the bus free which means SCK & SDA is high
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = I2C1_SDA_PIN;
    GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC I2C1 Config
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void I2C1_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the I2Cx master mode
/// @note   None.
/// @param  : I2Cx (where x can be 1 or 2 to select the I2C peripheral)
/// @param  : iI2C_speed: I2C speed.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C_MasterModeInit(I2C_TypeDef* I2Cx, u32 uiI2C_speed)
{
    I2C_InitTypeDef I2C_InitStructure;
    I2C_StructInit(&I2C_InitStructure);
    //Enable I2C clock state
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    //Configure I2C as master mode
    I2C_InitStructure.Mode = I2C_CR_MASTER;
    I2C_InitStructure.OwnAddress = 0;
    I2C_InitStructure.Speed = I2C_CR_FAST;
    I2C_InitStructure.ClockSpeed = uiI2C_speed;
    //Initializes the I2Cx peripheral according to the specified
    I2C_Init(I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE);
    I2C_ITConfig( I2C1, I2C_IT_RX_FULL, ENABLE );//|I2C_IT_TX_EMPTY
}

void I2C1_IRQHandler(void)
{
    if(I2C_GetITStatus(I2C1, I2C_IT_TX_EMPTY)) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_TX_EMPTY);
        I2C_ITConfig(I2C1, I2C_IT_TX_EMPTY, DISABLE);
        gTxFlag = 1;
    }
    if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL)) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_RX_FULL);
        gData = I2C_ReceiveData(I2C1);
        gRxFlag = 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set the device address
/// @note   None.
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
static u8 EEPROM_ReadByte(I2C_TypeDef* I2Cx)
{
    u8 temp;
    gRxFlag = 0;
    I2C_ReadCmd(I2C1);
    while(gRxFlag == 0);
    temp = gData;
    return (u8)temp;
}
static void EEPROM_WriteByte(I2C_TypeDef* I2Cx, u8 data)
{
    I2C_SendData(I2Cx, data);
    I2C_ITConfig(I2Cx, I2C_IT_TX_EMPTY, ENABLE);
    while(1) {
        if( gTxFlag == 1) {
            gTxFlag = 0;
            break;
        }
    }
}
static void EEPROM_ReadBuff(I2C_TypeDef* I2Cx, u16 mem_byte_addr, u16 rx_count, u8* rx_data )
{
    u8 temp = 0;
    u16 i;
    temp = ((mem_byte_addr) & 0xff);
    EEPROM_WriteByte(I2Cx, temp);
    for(i = 0; i < rx_count; i++) {
        rx_data[i] = EEPROM_ReadByte(I2Cx);
    }
    I2C_GenerateSTOP(I2Cx, ENABLE);
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send a byte.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static void EEPROM_SendByte(u8 dat)
{
    //Send data
    I2C_SendData(I2C1, dat);
    //Checks whether transmit FIFO completely empty or not
    while(I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE) == 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send bytes
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
static u8 EEPROM_WriteBuff(u8 sub, u8* ptr, u16 cnt)
{
    //Send sub address
    EEPROM_SendByte(sub);
    while (cnt --) {
        //Send data
        EEPROM_SendByte(*ptr);
        //Point to the next data
        ptr++;
    }
    //Stop transmission
    I2C_GenerateSTOP(I2C1, ENABLE);
    //Checks whether stop condition has occurred or not.
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initial I2C
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C_NVIC_EepromInit()
{
    I2C1_NVIC_Init();
    I2C1_GPIO_Config();
    //Initializes the I2C master mode
    I2C_MasterModeInit(I2C1, 400000);
    //Set the EEPROM address
    I2C_SetDeviceAddr(I2C1, EEPROM_ADDR);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  I2C1 Master test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_NVIC_EepromTest(void)
{
    s32 i = 80000;
    //Write 16 bytes from gTxBuff[128] to 0x10 of EEPROM
    EEPROM_WriteBuff(0, gTxBuff, 0x10);
    while(i--);
    //Read 16 bytes from 0x10 of EEPROM to gRxBuff[128]
    EEPROM_ReadBuff( I2C1, 0, 0x10, gRxBuff );
}

/// @}

/// @}

/// @}
