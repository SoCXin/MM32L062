////////////////////////////////////////////////////////////////////////////////
/// @file    can.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _CAN_C_

// Files includes

#include "stdio.h"
#include "can.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CAN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup CAN_Exported_Constants
/// @{

u8 gRxFlag;
////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize CAN GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as CAN, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// @brief   CAN initialize
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_CAN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    //CAN RX PB8
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //CAN TX PB9
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_4);

    NVIC_InitStructure.NVIC_IRQChannel = CAN_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
CAN_FLT_GROUP* GetCAN_FilterNum(u8 Num )
{
    CAN_FLT_GROUP* GroupN;
    if(Num == 0) {
        GroupN = (CAN_FLT_GROUP*)(&(CAN_Peli_FLT->GROUP0));
    }
    else {
        GroupN = (CAN_FLT_GROUP*)((u32)(&(CAN_Peli_FLT->GROUP1)) + ((Num - 1) << 5));
    }
    return GroupN;
}
void CAN_Filter0_19(u8 Num, u8 type, u32* idcode, u32* idmask)
{
    u32 idCodeTemp1, idMaskTemp1, idCodeTemp2, idMaskTemp2;
    u8 regTemp, regTemp1, filFlag = 0 ;
    u32 tempAdd;
    CAN_FLT_GROUP* Group_n;

    Group_n = GetCAN_FilterNum(Num );
    if(type & (1 << 1)) {
        filFlag = 1;//
    }

    if(Num != 0) {
        regTemp = Num / 8;
        regTemp1 = Num % 8;

        tempAdd = ((u32) & (CAN_Peli_FLT->AFM0) + (regTemp << 2) );
        if(filFlag) {
            *(u32*)tempAdd |= 1 << regTemp1;
        }
        else {
            *(u32*)tempAdd &= ~(1 << regTemp1);
        }

        tempAdd = ((u32) & (CAN_Peli_FLT->FGA0) + (regTemp << 2) );
        *(u32*)tempAdd |= 1 << regTemp1;
    }
    else {
        if(filFlag) {
            CAN1_PELI->MOD |= 1 << 3;
        }
        else {
            CAN1_PELI->MOD &= ~(1 << 3);
        }
    }

    if(filFlag == 1) {
        if(type & (1 << 0)) { // Extension
            idCodeTemp1 = (*idcode) << (32 - 29 );
            idMaskTemp1 = (*idmask) << (3 );
            idMaskTemp1 |= 0x7;//Other shielding positions 1
        }
        else { //Standard frame
            idCodeTemp1 = (*idcode) << (32 - 11);
            idMaskTemp1 = (*idmask) << (32 - 11 );
            idMaskTemp1 |= 0x1FFFFF;//Other shielding positions 1
        }
        Group_n->ACR0 = ( idCodeTemp1 >> 24 ) & 0xff;
        Group_n->ACR1 = ( idCodeTemp1 >> 16 ) & 0xff;
        Group_n->ACR2 = ( idCodeTemp1 >> 8 ) & 0xff;
        Group_n->ACR3 = ( idCodeTemp1 >> 0 ) & 0xff;

        Group_n->AMR0 = (idMaskTemp1 >> 24 ) & 0xff;
        Group_n->AMR1 = (idMaskTemp1 >> 16 ) & 0xff;
        Group_n->AMR2 = (idMaskTemp1 >> 8 ) & 0xff;
        Group_n->AMR3 = (idMaskTemp1 >> 0 ) & 0xff;
    }
    else {
        idCodeTemp1 = (*idcode + 0);
        idMaskTemp1 = (*idmask + 0);
        idCodeTemp2 = (*idcode + 1);
        idMaskTemp2 = (*idmask + 1);
        if(type & (1 << 0)) {
            idCodeTemp1 = idCodeTemp1 << (32 - 29);
            idMaskTemp1 = idMaskTemp1 << (32 - 29 );
            idCodeTemp2 = idCodeTemp2 << (32 - 29);
            idMaskTemp2 = idMaskTemp2 << (32 - 29 );
            idMaskTemp1 |= 0xFFFF;
            idMaskTemp2 |= 0xFFFF;
        }
        else {
            idCodeTemp1 = idCodeTemp1 << (32 - 11);
            idMaskTemp1 = idMaskTemp1 << (32 - 11 );
            idCodeTemp2 = idCodeTemp2 << (32 - 11);
            idMaskTemp2 = idMaskTemp2 << (32 - 11 );
            idMaskTemp1 |= 0x1FFFFF;
            idMaskTemp2 |= 0x1FFFFF;
        }
        Group_n->ACR0 = ( idCodeTemp1 >> 24 ) & 0xff;
        Group_n->ACR1 = ( idCodeTemp1 >> 16 ) & 0xff;
        Group_n->AMR0 = (idMaskTemp1 >> 24 ) & 0xff;
        Group_n->AMR1 = (idMaskTemp1 >> 16 ) & 0xff;

        Group_n->ACR2 = ( idCodeTemp2 >> 24 ) & 0xff;
        Group_n->ACR3 = ( idCodeTemp2 >> 16 ) & 0xff;
        Group_n->AMR2 = (idMaskTemp2 >> 24) & 0xff;
        Group_n->AMR3 = (idMaskTemp2 >> 16) & 0xff;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief   parameter configuration
/// @note    It must be careful of the Chip Version.
/// @param  CAN_Pre: CAN_250K  CAN_500K ( CAN traffic rate.)
/// @param  CAN_Mode:
///                  StandardFrame_SingleFilter=0
///                  StandardFrame_SingleFilter=1
///                  ExtendedFrame_SingleFilter=2
///                  ExtendedFrame_DoubleFilter=3
/// @param  idCode1:receiving address1
/// @param  idCode2:receiving address2
/// @param  mask1  : Mask frame Settings
/// @param  mask2  : Mask frame Settings
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CAN_Filter_20GroupInit(u32 CAN_Pre)
{
    CAN_Peli_InitTypeDef          CAN_Peli_InitStructure;
    RCC_ClocksTypeDef             RCC_Clocks;
    u32 idcode[2], idmask[2];
    CAN_GPIO_Config();
    CAN_ResetMode_Cmd(CAN1, ENABLE);                                                //CAN0_enter_resetmode();// Enter reset mode
    CAN_Mode_Cmd(CAN1, CAN_PELIMode);                                               //CAN enters Peli mode

    RCC_GetClocksFreq(&RCC_Clocks);

    CAN_Peli_StructInit(&CAN_Peli_InitStructure);

    CAN_AutoCfg_BaudParam( &CAN_Peli_InitStructure, RCC_Clocks.PCLK1_Frequency, CAN_Pre);

    CAN_Peli_InitStructure.SAM = RESET;
    CAN_Peli_InitStructure.STM = DISABLE;
    CAN_Peli_InitStructure.LOM = DISABLE;
    //  CAN_Peli_InitStructure.EWLR = 0x96;
    CAN_Peli_Init(&CAN_Peli_InitStructure);

    idcode[0] = 0x188;
    idmask[0] = 0x000;//

    CAN_Filter0_19(0, 2, idcode, idmask);

    idcode[0] = 0x136;
    idmask[0] = 0x000;//

    CAN_Filter0_19(1, 2, idcode, idmask); //

    idcode[0] = 0x168;
    idmask[0] = 0x000;//

    CAN_Filter0_19(2, 3, idcode, idmask); //

    idcode[0] = 0x13f;
    idmask[0] = 0x000;//

    CAN_Filter0_19(3, 3, idcode, idmask); //

    idcode[0] = 0x17c;
    idmask[0] = 0x000;//

    CAN_Filter0_19(4, 2, idcode, idmask); //

    idcode[0] = 0x324;
    idmask[0] = 0x000;//

    CAN_Filter0_19(5, 2, idcode, idmask); //

    idcode[0] = 0x13a;
    idmask[0] = 0x000;//

    CAN_Filter0_19(6, 3, idcode, idmask); //

    idcode[0] = 0x158;
    idmask[0] = 0x000;//

    CAN_Filter0_19(7, 3, idcode, idmask); //

    idcode[0] = 0x100;
    idmask[0] = 0x000;//

    CAN_Filter0_19(8, 2, idcode, idmask); //


    idcode[0] = 0x101;
    idmask[0] = 0x000;//

    CAN_Filter0_19(9, 3, idcode, idmask); //


    idcode[0] = 0x102;
    idmask[0] = 0x000;//

    CAN_Filter0_19(10, 3, idcode, idmask); //

    idcode[0] = 0x103;
    idmask[0] = 0x000;//

    CAN_Filter0_19(11, 3, idcode, idmask); //

    idcode[0] = 0x104;
    idmask[0] = 0x000;//

    CAN_Filter0_19(12, 3, idcode, idmask); //

    idcode[0] = 0x105;
    idmask[0] = 0x000;//

    CAN_Filter0_19(13, 3, idcode, idmask); //

    idcode[0] = 0x106;
    idmask[0] = 0x000;//

    CAN_Filter0_19(14, 3, idcode, idmask); //

    idcode[0] = 0x107;
    idmask[0] = 0x000;//

    CAN_Filter0_19(15, 3, idcode, idmask); //

    idcode[0] = 0x108;
    idmask[0] = 0x000;//

    CAN_Filter0_19(16, 3, idcode, idmask); //

    idcode[0] = 0x109;
    idmask[0] = 0x000;//

    CAN_Filter0_19(17, 3, idcode, idmask); //

    idcode[0] = 0x110;
    idmask[0] = 0x000;//

    CAN_Filter0_19(18, 3, idcode, idmask); //

    idcode[0] = 0x111;
    idmask[0] = 0x000;//

    CAN_Filter0_19(19, 3, idcode, idmask); //


    CAN_Peli_ITConfig(CAN_IT_RI, ENABLE);
    CAN_Peli_ITConfig(CAN_IT_BEI, ENABLE);
    CAN_Peli_ITConfig(CAN_IT_DOI, ENABLE);

    //CAN_Peli_ITConfig(CAN_IT_WUI,ENABLE);
    CAN_Peli_ITConfig(CAN_IT_ALI, ENABLE);

    CAN_ResetMode_Cmd(CAN1, DISABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send function
/// @note   the register configuration if there is no return value.
/// @param  TxMessage:Sent message.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Send_CANFrame(CanTxMsg* TxMessage)
{
    CanPeliTxMsg     CanPeliTxMsgStructure;
    u32 ID = 0;
    u32 i;

    if(TxMessage->CANIDtype) {
        ID = TxMessage->CANID << 3;

        CanPeliTxMsgStructure.FF   = 0x01;
        CanPeliTxMsgStructure.IDLL = (ID >> 0) & 0xff;
        CanPeliTxMsgStructure.IDLH = (ID >> 8) & 0xff;
        CanPeliTxMsgStructure.IDHL = (ID >> 16) & 0xff;
        CanPeliTxMsgStructure.IDHH = (ID >> 24) & 0xff;
    }
    else {
        ID = TxMessage->CANID << 21;

        CanPeliTxMsgStructure.FF   = 0x00;
        CanPeliTxMsgStructure.IDHL = (ID >> 16) & 0xff;
        CanPeliTxMsgStructure.IDHH = (ID >> 24) & 0xff;
    }

    CanPeliTxMsgStructure.DLC  = TxMessage->DLC;
    CanPeliTxMsgStructure.RTR  = TxMessage->RTR;

    for(i = 0; i < 8; i ++ ) {
        CanPeliTxMsgStructure.Data[i] = *(TxMessage->Data + i);
    }

    CAN_Peli_Transmit(&CanPeliTxMsgStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Interrupt service function
/// @note   Note the arbitration register when receiving an exception.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void CAN_IRQHandler(void)
{
    u32 CAN_IR_STA;
    u32 CAN_SR_STA;
    CAN_IR_STA = CAN1_PELI->IR;
    CAN_SR_STA = CAN1_PELI->SR;
    if(CAN_IR_STA & CAN_IT_RI) {
        //Message received successfully
        CAN_Peli_Receive(&gCanPeliRxBuff);
        gRxFlag = 1;
    }
    //error
    if(CAN_IR_STA & CAN_IT_BEI) {
    }
    if(CAN_SR_STA & CAN_STATUS_DOS  ) {
        CAN1_PELI->CMR  |= 0x01 << 5;
        CAN1_PELI->CMR  |= 0x08;
    }
    //Arbitration is missing
    if(CAN_IR_STA & CAN_IT_ALI  ) {
    }
}


/// @}

/// @}

/// @}
