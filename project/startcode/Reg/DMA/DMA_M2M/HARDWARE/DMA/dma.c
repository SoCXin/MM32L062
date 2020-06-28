////////////////////////////////////////////////////////////////////////////////
/// @file     dma.c
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
#define _DMA_C_

// Files includes
#include "dma.h"
#include "delay.h"
#include "uart.h"
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DMA
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup DMA_Exported_Constants
/// @{

vu8 dma1Flag = 0x0;
vu8 dma2Flag = 0x0;

u8 R_Buffer[64 * 4] = {0}; // receive  address
u8 T_Buffer[64 * 4] = {0}; // send  address


/////////////////////////////////////////////////////////////////////////////////
/// @brief   setup M0 core NVIC Initialize
/// @param   NVIC_IRQChannelPriority,
/// @param   NVIC_IRQChannel,
/// @param   NVIC_IRQChannelCmd
/// @retval  None
/////////////////////////////////////////////////////////////////////////////////
void M0_NVIC_Init(u32 NVIC_IRQChannelPriority, IRQn_Type NVIC_IRQChannel, FunctionalState NVIC_IRQChannelCmd)
{
    if (NVIC_IRQChannelCmd != DISABLE) {
        NVIC->IP[NVIC_IRQChannel >> 0x02] =
            (NVIC->IP[NVIC_IRQChannel >> 0x02] &
             (~(((u32)0xFF) << ((NVIC_IRQChannel & 0x03) * 8)))) |
            ((((u32)NVIC_IRQChannelPriority << 6) & 0xFF) << ((NVIC_IRQChannel & 0x03) * 8));

        NVIC->ISER[0] = 0x01 << (NVIC_IRQChannel & 0x1F);
    }
    else {
        NVIC->ICER[0] = 0x01 << (NVIC_IRQChannel & 0x1F);
    }
}

void DMA1_Channel1_IRQHandler()
{
    if(DMA1->ISR & DMA_ISR_TCIF1) {
        DMA1->IFCR = DMA_IFCR_CTCIF1;
        dma1Flag = 0x1;
    }
}

void DMA1_Channel2_3_IRQHandler()
{
    if(DMA1->ISR & DMA_ISR_TCIF2) {
        DMA1->IFCR = DMA_IFCR_CTCIF2;
        dma2Flag = 0x1;
    }
}


void DMAcheckStatus(u32 DMA_FLAG)
{
    while(1) {
        if(DMA1->ISR & DMA_FLAG) {
            DMA1->IFCR = DMA_FLAG;
            break;
        }
    }
}
void DMAdisable(DMA_Channel_TypeDef* DMAy_Channelx)
{
    //disable DMA_EN
    DMAy_Channelx->CCR &= 0xFFFFFFFE;
}

void dma_m8tom8_test()
{
    u32 i;
    u32 temp;
    for(i = 0; i < 64; i++) {
        *((volatile u8*)((u32)R_Buffer + i)) = i + 1;
    }
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DELAY_Ms(5);                //wait DMAclock  stabilization
    DMA1_Channel2->CPAR = (u32)T_Buffer;    //DMA1 external  address
    DMA1_Channel2->CMAR = (u32)R_Buffer; //DMA1,memory  device address
    DMA1_Channel2->CCR |= DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR = 64;
    DMA1_Channel2->CCR |= DMA_CCR1_PINC;
    DMA1_Channel2->CCR |= DMA_CCR1_MINC;
    DMA1_Channel2->CCR &= ~DMA_CCR1_PSIZE_0;    //external  data the width is 8 bits
    DMA1_Channel2->CCR &= ~DMA_CCR1_MSIZE_0;    //memory  device data the width is 8 bits
    DMA1_Channel2->CCR |= DMA_CCR1_PL_0;    //Medium priority
    DMA1_Channel2->CCR |= DMA_CCR1_MEM2MEM;     //register memory  device to memory  device mode


    M0_NVIC_Init(2, DMA1_Channel2_3_IRQn, ENABLE);
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;

    DMA1_Channel2->CCR |= DMA_CCR1_EN;        //start DMA transmission
    while(1) {
        if(dma2Flag) {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR &= ~(DMA_CCR1_EN);     // close DMA transmission
    for(i = 0; i < 64; i++) {

        temp = *((volatile u8*)((u32)T_Buffer + i));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

void dma_m8tom16_test()
{
    u32 i;
    u32 temp;
    for(i = 0; i < 64; i++) {
        *((volatile u8*)((u32)R_Buffer + i)) = i + 1;
    }
    RCC->AHBENR |= RCC_AHBENR_DMA1EN ;
    DELAY_Ms(5);                //wait DMAclock  stabilization
    DMA1_Channel2->CPAR = (u32)T_Buffer;    //DMA1 external  address
    DMA1_Channel2->CMAR = (u32)R_Buffer; //DMA1,memory  device address
    DMA1_Channel2->CCR |= DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR = 64;
    DMA1_Channel2->CCR |= DMA_CCR1_PINC;
    DMA1_Channel2->CCR |= DMA_CCR1_MINC;
    DMA1_Channel2->CCR |= DMA_CCR1_PSIZE_0;     //external  data 16bit
    DMA1_Channel2->CCR &= ~DMA_CCR1_MSIZE_0;    //memory  device data the width is 8 bits
    DMA1_Channel2->CCR |= DMA_CCR1_PL_0;    //Medium priority
    DMA1_Channel2->CCR |= DMA_CCR1_MEM2MEM;     //register memory  device to memory  device mode

    M0_NVIC_Init(2, DMA1_Channel2_3_IRQn, ENABLE);
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;

    DMA1_Channel2->CCR |= DMA_CCR1_EN;        //start DMA transmission
    while(1) {
        if(dma2Flag) {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR &= ~(DMA_CCR1_EN);     // close DMA transmission
    for(i = 0; i < 128; i++) {

        temp = *((volatile u8*)((u32)T_Buffer + i));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

void dma_m8tom32_test()
{
    u32 i;
    u32 temp;
    for(i = 0; i < 64; i++) {
        *((volatile u8*)((u32)R_Buffer + i)) = i + 1;
    }
    RCC->AHBENR |= RCC_AHBENR_DMA1EN ;
    DELAY_Ms(5);                //wait DMAclock  stabilization
    DMA1_Channel2->CPAR = (u32)T_Buffer;    //DMA1 external  address
    DMA1_Channel2->CMAR = (u32)R_Buffer; //DMA1,memory  device address
    DMA1_Channel2->CCR |= DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR = 64;
    DMA1_Channel2->CCR |= DMA_CCR1_PINC;
    DMA1_Channel2->CCR |= DMA_CCR1_MINC;
    DMA1_Channel2->CCR |= DMA_CCR1_PSIZE_1;     //external  data 32bit
    DMA1_Channel2->CCR &= ~DMA_CCR1_MSIZE_0;    //memory  device data the width is 8 bits
    DMA1_Channel2->CCR |= DMA_CCR1_PL_0;    //Medium priority
    DMA1_Channel2->CCR |= DMA_CCR1_MEM2MEM;     //register memory  device to memory  device mode

    M0_NVIC_Init(2, DMA1_Channel2_3_IRQn, ENABLE);
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;

    DMA1_Channel2->CCR |= DMA_CCR1_EN;        //start DMA transmission
    while(1) {
        if(dma2Flag) {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR &= ~(DMA_CCR1_EN);     // close DMA transmission
    for(i = 0; i < 256; i++) {

        temp = *((volatile u8*)((u32)T_Buffer + i));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}


void DMA_m16tom8_test()
{
    u32 i;
    u32 temp;
    for(i = 0; i < 64; i++) {
        *((volatile u16*)((u32)R_Buffer + (i * 2))) = 0xb1b0 + i;

    }
    RCC->AHBENR |= RCC_AHBENR_DMA1EN ;
    DELAY_Ms(5);                //wait DMAclock  stabilization
    DMA1_Channel2->CPAR = (u32)T_Buffer;    //DMA1 external  address
    DMA1_Channel2->CMAR = (u32)R_Buffer; //DMA1,memory  device address
    DMA1_Channel2->CCR |= DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR = 64;
    DMA1_Channel2->CCR |= DMA_CCR1_PINC;
    DMA1_Channel2->CCR |= DMA_CCR1_MINC;
    DMA1_Channel2->CCR &= ~DMA_CCR1_PSIZE_0;    //external  data the width is 8 bits
    DMA1_Channel2->CCR |= DMA_CCR1_MSIZE_0;     //memory  device data 16bit
    DMA1_Channel2->CCR |= DMA_CCR1_PL_0;    //Medium priority
    DMA1_Channel2->CCR |= DMA_CCR1_MEM2MEM;     //register memory  device to memory  device mode

    M0_NVIC_Init(2, DMA1_Channel2_3_IRQn, ENABLE);
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;

    DMA1_Channel2->CCR |= DMA_CCR1_EN;        //start DMA transmission
    while(1) {
        if(dma2Flag) {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR &= ~(DMA_CCR1_EN);     // close DMA transmission
    for(i = 0; i < 32; i++) {

        temp = *((volatile u8*)((u32)T_Buffer + i));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

void DMA_m16tom16_test()
{
    u32 i;
    u32 temp;
    for(i = 0; i < 64; i++) {
        *((volatile u16*)((u32)R_Buffer + (i * 2))) = 0xa1a0 + i;

    }
    RCC->AHBENR |= RCC_AHBENR_DMA1EN ;
    DELAY_Ms(5);                //wait DMAclock  stabilization
    DMA1_Channel2->CPAR = (u32)T_Buffer;    //DMA1 external  address
    DMA1_Channel2->CMAR = (u32)R_Buffer; //DMA1,memory  device address
    DMA1_Channel2->CCR |= DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR = 64;
    DMA1_Channel2->CCR |= DMA_CCR1_PINC;
    DMA1_Channel2->CCR |= DMA_CCR1_MINC;
    DMA1_Channel2->CCR |= DMA_CCR1_PSIZE_0;     //external  data 16bit
    DMA1_Channel2->CCR |= DMA_CCR1_MSIZE_0;     //memory  device data 16bit
    DMA1_Channel2->CCR |= DMA_CCR1_PL_0;    //Medium priority
    DMA1_Channel2->CCR |= DMA_CCR1_MEM2MEM;     //register memory  device to memory  device mode

    M0_NVIC_Init(2, DMA1_Channel2_3_IRQn, ENABLE);
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;

    DMA1_Channel2->CCR |= DMA_CCR1_EN;        //start DMA transmission
    while(1) {
        if(dma2Flag) {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR &= ~(DMA_CCR1_EN);     // close DMA transmission
    for(i = 0; i < 64; i++) {

        temp = *((volatile u16*)((u32)T_Buffer + (i * 2)));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}



void DMA_m16tom32_test()
{
    u32 i;
    u32 temp;
    for(i = 0; i < 64; i++) {
        *((volatile u16*)((u32)R_Buffer + (i * 2))) = 0xc1c0 + i;

    }
    RCC->AHBENR |= RCC_AHBENR_DMA1EN ;
    DELAY_Ms(5);                //wait DMAclock  stabilization
    DMA1_Channel2->CPAR = (u32)T_Buffer;    //DMA1 external  address
    DMA1_Channel2->CMAR = (u32)R_Buffer; //DMA1,memory  device address
    DMA1_Channel2->CCR |= DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR = 64;
    DMA1_Channel2->CCR |= DMA_CCR1_PINC;
    DMA1_Channel2->CCR |= DMA_CCR1_MINC;
    DMA1_Channel2->CCR |= DMA_CCR1_PSIZE_1;     //external  data 16bit
    DMA1_Channel2->CCR |= DMA_CCR1_MSIZE_0;     //memory  device data 16bit
    DMA1_Channel2->CCR |= DMA_CCR1_PL_0;    //Medium priority
    DMA1_Channel2->CCR |= DMA_CCR1_MEM2MEM;     //register memory  device to memory  device mode


    M0_NVIC_Init(2, DMA1_Channel2_3_IRQn, ENABLE);
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;

    DMA1_Channel2->CCR |= DMA_CCR1_EN;        //start DMA transmission
    while(1) {
        if(dma2Flag) {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR &= ~(DMA_CCR1_EN);     // close DMA transmission
    for(i = 0; i < 256; i++) {

        temp = *((volatile u16*)((u32)T_Buffer + (i * 2)));

        printf("temp%d=0x%x\r\n", i, temp);
    }
}

/// @}


/// @}

/// @}























