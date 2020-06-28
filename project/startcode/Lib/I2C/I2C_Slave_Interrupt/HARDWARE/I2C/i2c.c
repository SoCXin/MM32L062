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
volatile s32 gTxCnt = 0;
volatile s32 gRxCnt = 0;
u32 gTxFlag = 1;
u32 gRxFlag = 1;

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART send byte.
/// @note   None.
/// @param  dat(A byte data).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void I2C_TX_EmptyCheck(I2C_TypeDef* I2Cx)
{
    while(1) {
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TX_EMPTY)) {
            break;
        }
    }
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
void I2C_NVIC_SlaveInit(void)
{
    I2C_DeInit(I2C1);
    I2C1_NVIC_Init();
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
void I2C_NVIC_SlaveTest()
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

void I2C1_IRQHandler(void)
{
    u16 stop_flag, start_flag;
    if(I2C_GetITStatus(I2C1, I2C_IT_RD_REQ)) { //The master has sent a read request from the slave
        I2C1->RD_REQ;
        I2C1->DR = (u8)gTxBuff[gTxCnt];
        I2C_TX_EmptyCheck(I2C1);
        while(!(I2C1->SR & 0x4));
        I2C_GenerateSTOP( I2C1, ENABLE );

        gTxCnt ++;
        if(gTxCnt == 16) {
            gTxCnt = 0;
        }
        gTxFlag = 0;
    }
    // interrupt receive
    if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL)) { //Master sends slave receive
        gRxBuff[gRxCnt++] = I2C_ReceiveData(I2C1);
        while(!(I2C1->SR & 0x4));
        I2C_GenerateSTOP( I2C1, ENABLE );
        if(gRxCnt == 16) {
            gRxCnt = 0;
        }
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
