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
/// @addtogroup BKP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP_Exported_Functions
/// @{

gEepromTypeDef gEeprom;
u8 gTxData[16] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99};
u8 gRxData[16];
////////////////////////////////////////////////////////////////////////////////
/// @brief  EEPROM_Write
/// @note   Write a data packet.
/// @param  : sub (Sub address of EEPROM)
/// @param  : ptr (Data in the buffer)
/// @param  : len (Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Write(u8 sub, u8* ptr, u16 len)
{
    do {
        //write data
        EEPROM_SendPacket(sub, ptr, len);
        //till I2C is not work
        while(gEeprom.busy);
    } while(!gEeprom.ack);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  EEPROM_Read
/// @note   Receive a data packet.
/// @param  : sub (Sub address of EEPROM)
/// @param  : ptr (Buffer to storage data)
/// @param  : len (Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Read(u8 sub, u8* ptr, u16 len)
{
    do {
        //read data
        EEPROM_ReadPacket(sub, ptr, len);
        //till I2C is not work
        while(gEeprom.busy);
    } while(!gEeprom.ack);
}

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
/// @brief  Initializes the I2Cx master mode
/// @note   None.
/// @param  : I2Cx (where x can be 1 or 2 to select the I2C peripheral)
/// @param  : iI2C_speed: I2C speed.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void I2C_MasterModeInit(I2C_TypeDef* I2Cx, u32 uiI2C_speed)
{
    I2C_InitTypeDef I2C_InitStructure;

    //Enable I2C clock state
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    I2C_StructInit(&I2C_InitStructure);
    //Configure I2C as master mode
    I2C_InitStructure.Mode = I2C_CR_MASTER;
    I2C_InitStructure.OwnAddress = 0;
    I2C_InitStructure.Speed = I2C_CR_FAST;
    I2C_InitStructure.ClockSpeed = uiI2C_speed;

    //Initializes the I2Cx peripheral according to the specified
    I2C_Init(I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE);
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

////////////////////////////////////////////////////////////////////////////////
/// @brief  Wait for EEPROM getting ready.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_WaitEEready(void)
{
    //eeprom operation interval delay
    u32 i = 10000;
    while(i--);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send a byte.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_WriteByte(u8 dat)
{
    //Send data
    I2C_SendData(I2C1, dat);
    //Checks whether transmit FIFO completely empty or not
    while(I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE) == 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive a byte.
/// @note   None.
/// @param  : None.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_ReadBuff(void)
{
    u8 i, flag = 0, _cnt = 0;
    for (i = 0; i < gEeprom.cnt; i++) {
        while(1) {
            //Write command is sent when RX FIFO is not full
            if ((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (flag == 0)) {
                //Configure to read
                I2C_ReadCmd(I2C1);
                _cnt++;
                //When flag is set, receive complete
                if (_cnt == gEeprom.cnt)
                    flag = 1;
            }
            //Check receive FIFO not empty
            if (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE)) {
                //read data to gEeprom.ptr
                gEeprom.ptr[i] = I2C_ReceiveData(I2C1);
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send bytes
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
u8 EEPROM_WriteBuff(u8 sub, u8* ptr, u16 cnt)
{
    //Send sub address
    EEPROM_WriteByte(sub);
    while (cnt --) {
        //Send data
        EEPROM_WriteByte(*ptr);
        //Point to the next data
        ptr++;
    }
    //Stop transmission
    I2C_GenerateSTOP(I2C1, ENABLE);
    //Checks whether stop condition has occurred or not.
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);
    gEeprom.ack = true;
    //I2C operation stops
    gEeprom.busy = false;
    //Wait for EEPROM getting ready.
    EEPROM_WaitEEready();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send a data packet
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : The state of this transmission.
////////////////////////////////////////////////////////////////////////////////
u8 EEPROM_SendPacket(u8 sub, u8* ptr, u16 cnt)
{
    u8 i;
    //i2c option flag set to write
    gEeprom.opt = WR;
    //number to send
    gEeprom.cnt = cnt;
    //sub address
    gEeprom.sub = sub;
    //I2C operation starts
    gEeprom.busy = true;
    gEeprom.ack = false;

    if ((sub % PAGESIZE) > 0) {
        //Need temp number of data, just right to the page address
        u8 temp = MIN((PAGESIZE - sub % PAGESIZE), gEeprom.cnt);
        //If WRITE successful
        if(EEPROM_WriteBuff(sub, ptr, temp)) {
            //Point to the next page
            ptr +=  temp;
            gEeprom.cnt -=  temp;
            sub += temp;
        }
        //gEeprom.cnt = 0 means transmition complete
        if (gEeprom.cnt == 0) return true;
    }
    for (i = 0; i < (gEeprom.cnt / PAGESIZE); i++) {
        //Full page write
        if (EEPROM_WriteBuff(sub, ptr, PAGESIZE)) {
            //Point to the next page
            ptr += PAGESIZE;
            sub += PAGESIZE;
            gEeprom.cnt -= PAGESIZE;
        }
        if (gEeprom.cnt == 0) return true;
    }
    if (gEeprom.cnt > 0) {
        if (EEPROM_WriteBuff(sub, ptr, gEeprom.cnt)) return true;
    }
    //I2C operation ends
    gEeprom.busy = false;
    gEeprom.ack = true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive a data packet
/// @note   None.
/// @param  : sub(Sub address of EEPROM)
/// @param  : ptr(Data in the buffer)
/// @param  : cnt(Number of data)
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void EEPROM_ReadPacket(u8 sub, u8* ptr, u16 cnt)
{
    //I2C operation starts
    gEeprom.busy = true;
    gEeprom.ack = false;
    gEeprom.sub = sub;
    gEeprom.ptr = ptr;
    gEeprom.cnt = cnt;

    //Send sub address
    EEPROM_WriteByte(gEeprom.sub);
    //receive bytes
    EEPROM_ReadBuff();
    //Stop transmission
    I2C_GenerateSTOP(I2C1, ENABLE);
    //Checks whether stop condition has occurred or not.
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);

    //I2C operation ends
    gEeprom.busy = false;
    gEeprom.ack = true;
    EEPROM_WaitEEready();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port initialization configuration
/// @note
/// @param  : deviceaddr(device address).
///           Speed: I2C_CR_STD/I2C_CR_FAST
///           ClockSpeed:0-100k/400k
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_WR_EepromInit(void)
{
    //Initial value of i2c struct
    memset(&gEeprom, 0x00, sizeof(gEeprom));
    I2C1_GPIO_Config();
    //Initializes the I2C master mode
    I2C_MasterModeInit(I2C1, 400000);
    //Set the EEPROM address
    I2C_SetDeviceAddr(I2C1, EEPROM_ADDR);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize LED GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as LED, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_WR_EepromTest(void)
{
    //Write 16 bytes from buffer0[128] to 0x10 of EEPROM
    EEPROM_Write(0x00, gTxData, 0x10);
    DELAY_Ms(1);
    //Read 16 bytes from 0x10 of EEPROM to buffer1[128]
    EEPROM_Read(0x00, gRxData, 0x10);
}




/// @}

/// @}

/// @}
