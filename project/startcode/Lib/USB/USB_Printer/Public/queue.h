/////////////////////////////////////////////////////////////////////////////////////////
/// @file   queue.h
/// @author AE TEAM
/// @brief  Queue Operation Header
/////////////////////////////////////////////////////////////////////////////////////////
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

#ifndef _QUEUE_H_
#define _QUEUE_H_
// Files includes
#include "types.h"

#ifdef  __cplusplus
extern "C"
{
#endif





#define DEBUG_FULL_ASSERT       0

#if     DEBUG_FULL_ASSERT
#define ASSERT_PARAM(a)          ((a) ? (void)0 : ASSERT_FAILED((u8 *)__FILE__, __LINE__))
void ASSERT_FAILED(u8* file, u32 line);
#else
#define ASSERT_PARAM(a)
#endif





typedef volatile struct {
    volatile u32   bufSize;        //Total buffer size
    volatile u8*    pStart;        //starting address
    volatile u8*    pEnd;          //End address
    volatile u8*    pBuf;          //Buffer first address
} QUEUE8_t;


typedef volatile struct {
    volatile u32   elemSize;       //Structure unit size
    volatile u32   sumCount;       //Maximum number of structure units
    volatile u32   start;          //End structure address
    volatile u32   end;            //Buffer first address
    volatile void*       pBuf;          //Starting structure address
} QUEUE_STRUCT_t;



u32 QUEUE_PacketCreate(QUEUE8_t* pQ8, u8* pBuf, u32 bufSize);
u32 QUEUE_PacketIn(QUEUE8_t* pQ8, u8* pData, u32 len);
u32 QUEUE_PacketOut(QUEUE8_t* pQ8, u8* pData, u32 dataLen);
u32 QUEUE_PacketLengthGet(QUEUE8_t* pQ8);

u32 QUEUE_PacketCharSplit(QUEUE8_t* pQ8, u8 splitChar, u8* pData, u32 dataLen);
u32 QUEUE_PacketStartEndCharSplit(QUEUE8_t* pQ8, u8 splitChar, u8* pData, u32 dataLen);
u32 QUEUE_PacketStartEndDifferentCharSplit(QUEUE8_t* pQ8, u8 startChar, u8 endChar, u8* pData, u32 dataLen);
u32 QUEUE_PacketDoubleEndCharSplit(QUEUE8_t* pQ8, u8 splitChar1, u8 splitChar2, u8* pData, u32 dataLen);


u32 QUEUE_StructCreate(QUEUE_STRUCT_t* pQueue, void* pBuf, u32 bufSize, u16 blkSize);
u32 QUEUE_StructIn(QUEUE_STRUCT_t* pQueue, void* pData, u32 blkCount);
u32 QUEUE_StructOut(QUEUE_STRUCT_t* pQueue, void* pData, u32 blkCount);
u32 QUEUE_StructCountGet(QUEUE_STRUCT_t* pQueue);


#ifdef  __cplusplus
}
#endif

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
