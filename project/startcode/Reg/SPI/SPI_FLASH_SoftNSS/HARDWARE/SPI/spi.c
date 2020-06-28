////////////////////////////////////////////////////////////////////////////////
/// @file     spi.c
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
#define _SPI_C_

// Files includes
#include "mm32_device.h"
#include "spi.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SPI_Exported_Constants
/// @{

u8 tmpdata[256];
u8 tmpdata1[256];
u8 rxtmpdata[256];
volatile u8 spi0_rx_flag;
volatile u8 spi0_tx_flag;
extern void DataCompare(u8* p1, u8* p2, u32 count);
////////////////////////////////////////////////////////////////////////////////
/// @brief   choose SPI  soft NSS
/// @param   None
/// @param   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_CSLow()
{
    //Spi cs assign to this pin,select
    GPIOB->ODR &= ~(0x1000);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   choose SPI  soft  configure NSS
/// @param   None
/// @param   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_CSHigh()
{
    //Spi cs release
    GPIOB->ODR |= (0x1 << 12);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Transmit Enable bit TXEN
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_TXEn()
{
    //Transmit Enable bit TXEN
    SPI2->GCTL |= 0x0008;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    disable TXEN
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_TXDisable()
{
    //disable TXEN
    SPI2->GCTL |= 0xfff7;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   enable RXEN
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
void SPIM_RXEn()
{
    //enable RXEN
    SPI2->GCTL |= 0x0010;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   RXEN disable
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
void SPIM_RXDisable()
{
    //disable RXEN
    SPI2->GCTL |= 0xffef;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief    through external  SPIx  receive and sent  data  ,full duplex ( receive and sent )
/// @param    tx_data :which data yo want to sent
/// @retval   data in RXREG
////////////////////////////////////////////////////////////////////////////////
u32 SPIMReadWriteByte(u8 tx_data)
{
    SPI2->TXREG = tx_data;
    while (1) {
        //if(SPI_GetFlagStatus(SPI2, SPI_FLAG_RXAVL))
        if((SPI2->CSTAT & 0x0002) == 0x0002) {
            return SPI2->RXREG;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    initial SPI
/// @param    spi_baud_div
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_Init(u16 spi_baud_div)
{
    RCC->APB1ENR |= 1 << 14;
    SPIM_CSHigh();

    //  RCC->APB2ENR|=0x00000008|0x00000004;
    RCC->AHBENR |= 0x7 << 17;

    GPIOB->AFRH &= ~0xffff0000;

    GPIOB->CRH &= 0x0000FFFF; //set PB12,PB13,PB14,PB15default value

    GPIOB->CRH |= 0xB0B00000; //PB15,PB13push-pull output
    GPIOB->CRH |= 0x08000000; //PB14pull-down input
    GPIOB->ODR |= GPIO_ODR_ODR14;
    GPIOB->CRH |= 0x00030000; //PB12push-pull output


    SPI2->GCTL |= SPI_GCTL_MM; //master  mode
    SPI2->CCTL |= SPI_CCTL_SPILEN;

    SPI2->CCTL &= ~SPI_CCTL_CPOL; //SPI_CPOL_Low
    SPI2->CCTL |= SPI_CCTL_CPHA; //SPI_CPHA_1Edge

    SPI2->GCTL &= ~SPI_GCTL_NSS_SEL; // soft control NSS
    SPI2->CCTL |= 0x10; //start high speed  mode
    SPI2->SPBRG = 0x2; //2 fractional frequency,baud  should 72M/2=36M
    SPI2->CCTL &= ~SPI_CCTL_LSBFE; //MSB
    SPI2->GCTL |= SPI_GCTL_SPIEN; //enableSPI

    SPIM_TXEn();
    SPIM_RXEn();
    SPI2->GCTL |= 0x01;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     read ID
/// @param     None
/// @retval    None
////////////////////////////////////////////////////////////////////////////////
void SPIM_ReadID()
{
    u8 temp;
    u32 i;

    SPIM_CSLow();                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(RDID);

    for(i = 0; i < 3; i++) {
        temp = SPIMReadWriteByte(0x01);
        printf("temp=0x%x\r\n", temp);
    }
    SPIM_CSHigh();                              //Spi cs release
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   write  data enable
/// @param   None
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
void SPIM_WriteEnable()
{
    SPIM_CSLow();                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(WREN);
    SPIM_CSHigh();                              //Spi cs release
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    check  data Whether sending is correct
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_checkStatus()
{
    u8 temp;
    SPIM_CSLow();                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(RDSR);
    while(1) {
        temp = SPIMReadWriteByte(0x00);
        if((temp & 0x01) == 0x0)
            break;
    }
    SPIM_CSHigh();                              //Spi cs release
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    write  data disable
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_WriteDisable()
{
    SPIM_CSLow();
    SPIMReadWriteByte(WRDI);
    SPIM_CSHigh();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Read 256 pages data
/// @param    address; *p;  number
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_PageRead(u32 address, u8* p, u32 number) //page = 256 bytes
{
    u8 addr0, addr1, addr2;
    u32 i;
    address = address & 0xffffff00;                 //page address
    addr0 = (u8)(address >> 16);
    addr1 = (u8)(address >> 8);
    addr2 = (u8)address;

    SPIM_CSLow();                               //Spi cs assign to this pin,select

    SPIMReadWriteByte(READ);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);

    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = SPIMReadWriteByte(0x00);
    }


    SPIM_CSHigh();                              //Spi cs release
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   send 256page  data
/// @param   address;*p;number
/// @retval  None
////////////////////////////////////////////////////////////////////////////////
void SPIM_PageProgram(u32 address, u8* p, u32 number)
{
    u32 j;
    u8 addr0, addr1, addr2;
    address = address & 0xffffff00;                 //page address
    addr0 = (u8)(address >> 16);
    addr1 = (u8)(address >> 8);
    addr2 = (u8)address;

    SPIM_WriteEnable();
    SPIM_CSLow();                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(PP);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    for(j = 0; j < number; j++) {
        SPIMReadWriteByte(*(p++));
    }

    SPIM_CSHigh();                              //Spi cs release

    SPIM_checkStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    ereas  data in sector
/// @param    address:delete sector address  each sector = 64Kbytes
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_SectorErase(u32 address)
{
    u8 addr0, addr1, addr2;
    address = address & 0xffff0000;
    addr0 = ((u8)(address >> 16)) & 0xff;
    addr1 = ((u8)(address >> 8)) & 0xff;
    addr2 = ((u8)address) & 0xff;

    SPIM_WriteEnable();

    SPIM_CSLow();                               //Spi cs assign to this pin,select

    SPIMReadWriteByte(SE);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    SPIM_CSHigh();                              //Spi cs release

    SPIM_checkStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   ereas data in block
/// @param   None
/// @retvsl  None
////////////////////////////////////////////////////////////////////////////////
void SPIM_BlockErase()
{
    SPIM_WriteEnable();

    SPIM_CSLow();                               //Spi cs assign to this pin,select

    SPIMReadWriteByte(BE);

    SPIM_CSHigh();                              //Spi cs release

    SPIM_checkStatus();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief    CS the signal goes up,stop communication
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_Close()
{
    SPIM_CSHigh();                              //Spi cs release
    SPIMReadWriteByte(0x01);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    test procedure ,use uart  print 256 page  data
/// @param    None
/// @retval   None
////////////////////////////////////////////////////////////////////////////////
void SPIM_Test(void)
{
    u32 i;
    for(i = 0; i < 256; i++) {
        tmpdata[i] = i * 2;
    }

    printf("SPI2 test\r\n");
    SPIM_Init(0x4);//baud  72M/2=36M

    SPIM_ReadID();

    SPIM_SectorErase(0);

    SPIM_PageProgram(0, tmpdata, 256);

    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0, rxtmpdata, 256);

    for(i = 0; i < 10; i++) {
        printf("rx[%d]=0x%x\r\n", i, rxtmpdata[i]);
    }

    printf("SPI2 test over\r\n");



}
/// @}

/// @}

/// @}
