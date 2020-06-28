////////////////////////////////////////////////////////////////////////////////
/// @file     i2c.c
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#include "mm32_device.h"
#include "uart.h"
#define USING_EMINIBOARD

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup I2C_Exported_Constants
/// @{

volatile u8 rx_flag = 0;
volatile u8 tx_flag = 0;
extern u16 I2C_DMA_DIR;
u8 tx_buffer0[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
u8 rx_buffer0[16] ;
#define FLASH_DEVICE_ADDR 0xa0
////////////////////////////////////////////////////////////////////////////////
/// @brief    I2C master mode initialization setting
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void I2CInitMasterMode(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOA | RCC_AHBENR_GPIOB | RCC_AHBENR_GPIOC;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1;
#ifdef USING_EMINIBOARD
    GPIOB->CRL |= 0xff000000;   //set PB6,PB7 as AF Output Open-Drain
    GPIOB->ODR |= 0x3 << 6;     //set PB6,PB7 Output High
#else //USING_MINIBOARD
    GPIOB->CRH |= 0x00ff;       //set PB8,PB9 as AF Output Open-Drain
    GPIOB->ODR |= 0x3 << 0;     //set PB8,PB9 Output High
#endif
    I2C1->IC_CON = 0x163;
    I2C1->IC_TAR = FLASH_DEVICE_ADDR >> 1;
    I2C1->IC_SS_SCL_HCNT = 0x77;
    I2C1->IC_SS_SCL_LCNT = 0x55;
    I2C1->IC_RX_TL = 0x0; //Rx FIFO
    I2C1->IC_TX_TL = 0x0; //Tx FIFO
    I2C1->IC_INTR_MASK = 0; //disable all
    I2C1->IC_ENABLE |= 0x01;
#ifdef USING_EMINIBOARD
    GPIOB->AFRL &= (~(0xffUL << (6 * 4)));
    GPIOB->AFRL |= 0x11UL << (6 * 4); //set PB6,PB7 AF to I2C SCK, SDA
    //set PB6,PB7 as Output High
#else //USING_MINIBOARD
    GPIOB->AFRH &= ~0xff << 0;
    GPIOB->AFRH |= 0x11 << 0;   //set PB8,PB9 AF to I2C SCK, SDA
#endif

}
////////////////////////////////////////////////////////////////////////////////
/// @brief    check if the I2C transmission is empty
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void I2CTXEmptyCheck()
{
    while(1) {
        if((I2C1->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_TX_EMPTY))// send  buffer area null
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief    check if the I2C receive is full
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void I2CRXFullCheck()
{
    I2C1->IC_DATA_CMD = I2C_DATA_CMD_CMD;// read  data  instructions  ,Canregister be omitted
    while(1) {
        if((I2C1->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_RX_FULL))// receive  buffer area register  null
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    I2C byte transmission
/// @param    data
/// @retval   None
////////////////////////////////////////////////////////////////////////////////

void I2CTXByte(u8 data)
{
    I2C1->IC_DATA_CMD = data; // send  data
    I2CTXEmptyCheck();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    I2C byte receive
/// @param    None
/// @retval   temp
////////////////////////////////////////////////////////////////////////////////
u8 I2CRXByte()
{
    u8 temp;
    I2CRXFullCheck();
    temp = I2C1->IC_DATA_CMD;
    return temp;
}

ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cn, u32 it)
{
    return ((I2Cn->IC_RAW_INTR_STAT & it) ? SET : RESET);
}
#define I2C_IT_STOP_DET     ((u16)0x0200)

////////////////////////////////////////////////////////////////////////////////
/// @brief    Write in I2C master mode
/// @param    I2cx : x can be 1 or 2
/// @param    device_id  :which device you choice
/// @param    mem_byte_addr :where you want to write
/// @param    tx_count
/// @param    tx_data
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void I2CMasterWrite(I2C_TypeDef* I2Cx, u8 device_id, u16 mem_byte_addr, u16 tx_count, u8* tx_data )
{
    u16 temp;
    u16 i;
    u8* p;

    u16 overTime = 3000;
    p = tx_data;

    I2C1->IC_ENABLE &= 0xFFFE;
    I2C1->IC_TAR = 0xa0 >> 1;
    I2C1->IC_ENABLE |= 0x0001;
    temp = ((mem_byte_addr) & 0xff);
    I2CTXByte(temp);  //tx memory addr

    for(i = 0; i < tx_count; i++) {
        temp = *p;
        p++;
        if(i == (tx_count - 1))
            I2C1->IC_DATA_CMD = temp | 0x200; // //muaul set stop flag
        else
            I2CTXByte(temp); //tx data

    }

    I2C1->IC_ENABLE |= I2C_ENR_ABORT;

    while (I2C1->IC_ENABLE & I2C_ENR_ABORT) {
        if (overTime-- == 0)
            break;
    }
    I2C1->IC_CLR_TX_ABRT;

    while((I2C_GetITStatus(I2C1, ((u16)0x0200))) == 0);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief    read in I2C master mode
/// @param    I2cx : x can be 1 or 2
/// @param    device_id  :which device you choice
/// @param    mem_byte_addr :where you want to read
/// @param    tx_count
/// @param    tx_data
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void I2CMasterRead(I2C_TypeDef* I2Cx, u8 device_id, u16 mem_byte_addr, u16 rx_count, u8* rx_data )
{
    u8 temp = 0;
    u16 i;
    u16 overTime = 3000;

    I2Cx->IC_TAR = 0xa0 >> 1;

    I2C1->IC_ENABLE |= 0x0001;


    temp = ((mem_byte_addr) & 0xff);

    I2CTXByte(temp);  //tx memory addr


    for(i = 0; i < rx_count; i++) {
        rx_data[i] = I2CRXByte();

    }
    I2C1->IC_ENABLE |= I2C_ENR_ABORT;

    while (I2C1->IC_ENABLE & I2C_ENR_ABORT) {
        if (overTime-- == 0)
            break;
    }
    I2C1->IC_CLR_TX_ABRT;

    while((I2C_GetITStatus(I2C1, ((u16)0x0200))) == 0);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief    write and read data from flash device address
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void I2CMasterTest()
{
    u8 i;

    I2CInitMasterMode();


    I2CMasterWrite(I2C1, FLASH_DEVICE_ADDR, 16 * 1, 16, tx_buffer0 );

    for(i = 0; i < 16 ; i ++) {
        printf("TX data is: %x \r\n", tx_buffer0[i]);
    }

    I2CMasterRead(I2C1, FLASH_DEVICE_ADDR, 16 * 1, 16, rx_buffer0 );
}
/// @}

/// @}

/// @}
