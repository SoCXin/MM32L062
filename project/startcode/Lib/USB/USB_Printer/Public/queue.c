////////////////////////////////////////////////////////////////////////////////
/// @file     queue.c
/// @author   AE TEAM
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE ADC
///           FIRMWARE LIBRARY.
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
#define _QUENE_C_

// Files includes
#include <stdio.h>
#include <string.h>

#include "queue.h"
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief    Queue establishment
/// @param    pQ8:    Queue
/// @param    pBuf:   Queue buffer address
/// @param    bufSize:Queue buffer size
/// @retval   0
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_PacketCreate(QUEUE8_t* pQ8, u8* pBuf, u32 bufSize)
{
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pBuf);
    ASSERT_PARAM(bufSize);

    pQ8->bufSize = bufSize;
    pQ8->pBuf = pBuf;
    pQ8->pStart = pBuf;
    pQ8->pEnd = pBuf;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     Data Load queue
/// @param     pQ8:    Queue
/// @param     pData:  Data to queue
/// @param     len:    Data length
/// @retval    index
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_PacketIn(QUEUE8_t* pQ8, u8* pData, u32 len)
{
    volatile u8* pEnd = NULL;

    volatile u32 index = 0;

    ASSERT_PARAM(pData);       //Total buffer size      Data
    ASSERT_PARAM(pQ8);         //Enter the queue          Queue
    ASSERT_PARAM(pQ8->pBuf);   //Buffer first address
    ASSERT_PARAM(pQ8->pStart); //Start address Delete operation at the front of the queue front
    ASSERT_PARAM(pQ8->pEnd);   //End address queue back end insert operation rear

    pEnd = pQ8->pEnd;

    for (index = 0; index < len; index++) {
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize) {
            pEnd = pQ8->pBuf;
        }

        if (pEnd == pQ8->pStart) {
            break;
        }

        *pQ8->pEnd = *pData++;

        pQ8->pEnd = pEnd;
    }

    return index;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     Data in the queue
/// @param     pQ8:    Queue
/// @param     pData:  Buffer
/// @param     dataLen:Buffer size
/// @retval    index
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_PacketOut(QUEUE8_t* pQ8, u8* pData, u32 dataLen)
{
    volatile u8* pStart = NULL;
    volatile u32 index = 0;

    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart = pQ8->pStart;

    while ((pStart != pQ8->pEnd) && (index < dataLen) && (index < pQ8->bufSize)) {
        pData[index++] = *pStart++;
        if (pStart >= pQ8->pBuf + pQ8->bufSize) {
            pStart = pQ8->pBuf;
        }
    }

    pQ8->pStart = pStart;

    return index;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Data length in Queue
/// @param    pQ8:    Queue
/// @param    pData:  Buffer
/// @param   dataLen:Buffer size
/// @retval  index
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_PacketLengthGet(QUEUE8_t* pQ8)
{

    volatile u8* pStart = NULL;
    u32 index = 0;

    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart = pQ8->pStart;

    while ((pStart != pQ8->pEnd) && (index < pQ8->bufSize)) {
        index++;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) {
            pStart = pQ8->pBuf;
        }
    }

    return index;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Take the data in the queue with the start and end characters
///           (the removed data includes the start and separator characters)
/// @param    pQ8:        Queue
/// @param    startChar:  Starter
/// @param    endChar:    Terminator
/// @param    pData:      Buffer
/// @param    dataLen:    Buffer size
/// @retval   index
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_PacketStartEndDifferentCharSplit(QUEUE8_t* pQ8, u8 startChar, u8 endChar, u8* pData, u32 dataLen)
{
    s32 count;
    s32 index;
    volatile u8* pStart;
    volatile u8* pEnd;

    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart = pQ8->pStart;
    count = pQ8->bufSize;

    while ((pStart != pQ8->pEnd) && count--) { //Find the starting character
        if (startChar == *pStart)
            break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize)
            pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd)
        return 0; //No start character found
    if (count == -1)
        return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
        pEnd = pQ8->pBuf;

    while ((pEnd != pQ8->pEnd) && count--) { //Find end character
        if (endChar == *pEnd)
            break;
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
            pEnd = pQ8->pBuf;
    }

    if (pEnd == pQ8->pEnd)
        return 0; //No terminator found
    if (count == -1)
        return 0;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
        pEnd = pQ8->pBuf;

    count = pQ8->bufSize - count;
    index = 0;
    //Get Data from start character to end character
    while ((pStart != pEnd) && (index < dataLen) && (index < pQ8->bufSize) && count--) {
        pData[index++] = *pStart++;
        if (pStart >= pQ8->pBuf + pQ8->bufSize)
            pStart = pQ8->pBuf;
    }

    pQ8->pStart = pEnd;
    return index;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Extract the data (including delimiters) in the first and last delimiters
/// @param    pQ8:        Queue
/// @param    startChar:  Starter
/// @param    endChar:    Terminator
/// @param    pData:      Buffer
/// @param    dataLen:    Buffer size
/// @retval   index
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_PacketStartEndCharSplit(QUEUE8_t* pQ8, u8 splitChar, u8* pData, u32 dataLen)
{
    s32 count;
    s32 index;
    volatile u8* pStart;
    volatile u8* pEnd;

    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart = pQ8->pStart;
    count = pQ8->bufSize;

    while ((pStart != pQ8->pEnd) && count--) { //Find the starting character
        if (splitChar == *pStart)
            break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize)
            pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd)
        return 0; //No start character found
    if (count == -1)
        return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
        pEnd = pQ8->pBuf;

    while ((pEnd != pQ8->pEnd) && count--) { //Find end character
        if (splitChar == *pEnd)
            break;
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
            pEnd = pQ8->pBuf;
    }

    if (pEnd == pQ8->pEnd)
        return 0; //No terminator found
    if (count == -1)
        return 0;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
        pEnd = pQ8->pBuf;

    count = pQ8->bufSize - count;
    index = 0;
    //Get Data from start character to end character
    while ((pStart != pEnd) && (index < dataLen) && (index < pQ8->bufSize) && count--) {
        pData[index++] = *pStart++;
        if (pStart >= pQ8->pBuf + pQ8->bufSize)
            pStart = pQ8->pBuf;
    }

    //If the fetched Data includes only the separator, it may be the last terminator and the next start character, so the last terminator is discarded.
    if (index <= 2) {
        index = 0;
        if (--pStart < pQ8->pBuf)
            pStart = pQ8->pBuf + pQ8->bufSize - 1;
    }

    pQ8->pStart = pStart;
    return index;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     Extracts single-ended separator data (including separators)
/// @param     pQ8
/// @param     splitChar
/// @param     dataLen
/// retval     index
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_PacketCharSplit(QUEUE8_t* pQ8, u8 splitChar, u8* pData, u32 dataLen)
{
    s32 count;
    s32 index;
    volatile u8* pStart;
    volatile u8* pEnd;

    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart = pQ8->pStart;
    count = pQ8->bufSize;

    while ((pStart != pQ8->pEnd) && count--) { //Find the starting character
        if (splitChar == *pStart)
            break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize)
            pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd)
        return 0; //No start character found
    if (count == -1)
        return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
        pEnd = pQ8->pBuf;

    pStart = pQ8->pStart;
    count = pQ8->bufSize;
    index = 0;
    while ((pStart != pEnd) && (index < dataLen) && count--) { //Find the starting character
        pData[index++] = *pStart;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize)
            pStart = pQ8->pBuf;
    }

    pQ8->pStart = pStart;
    return index;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Extract the data for the double-ended separator (including the separator)
/// @param    QUEUE8_t * pQ8
/// @param    u8 splitChar1
/// @param    u8 splitChar2
/// @param    u8 * pData
/// @param    u32 dataLen
/// @param    u32
/// @retval   index
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_PacketDoubleEndCharSplit(QUEUE8_t* pQ8, u8 splitChar1, u8 splitChar2, u8* pData, u32 dataLen)
{
    s32 count;
    s32 index;
    volatile u8* pStart;
    volatile u8* pEnd;
    u8 lastChar = 0;

    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart = pQ8->pStart;
    count = pQ8->bufSize;

    while ((pStart != pQ8->pEnd) && count--) { //Find the starting character
        if ((splitChar1 == lastChar) && (splitChar2 == *pStart))
            break;

        lastChar = *pStart;

        if (++pStart >= pQ8->pBuf + pQ8->bufSize)
            pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd)
        return 0; //No start character found
    if (count == -1)
        return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
        pEnd = pQ8->pBuf;

    pStart = pQ8->pStart;
    count = pQ8->bufSize;
    index = 0;
    while ((pStart != pEnd) && (index < dataLen) && count--) { //Find the starting character
        pData[index++] = *pStart;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize)
            pStart = pQ8->pBuf;
    }

    pQ8->pStart = pStart;
    return index;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     Structure queue
/// @param     pQueue:     Queue name
/// @param     pBuf:       Queue Buffer
/// @param     bufSize:    Swap area size (bytes)
/// @param     blkSize:    Single structure size (bytes)
/// @retval    0
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_StructCreate(QUEUE_STRUCT_t* pQueue, void* pBuf, u32 bufSize, u16 blkSize)
{
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pBuf);
    ASSERT_PARAM(bufSize);
    ASSERT_PARAM(blkSize);

    pQueue->elemSize = blkSize;
    pQueue->sumCount = bufSize / blkSize;
    pQueue->pBuf = pBuf;
    pQueue->start = 0;
    pQueue->end = 0;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    The structure Queue is pushed onto the stack and the buffer is not loaded if it is full
/// @param    pQueue:     Queue name
/// @param    pData:      Data to be pushed onto the stack
/// @param    blkCount:   Number of structures to be stacked (unit: number of structures)
/// @retval   i
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_StructIn(QUEUE_STRUCT_t* pQueue, void* pData, u32 blkCount)
{
    u32 i = blkCount;
    u32 end = 0;

    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);
    ASSERT_PARAM(pData);

    end = pQueue->end;
    for (i = 0; i < blkCount; i++) {
        //After loading another set of Data, whether the pointer points to the end of the stack
        if (++end >= pQueue->sumCount) {
            end = 0;
        }

        //Fill out the buffer and exit directly
        if (end == pQueue->start) {
            break;
        }

        memcpy((u8*)pQueue->pBuf + pQueue->end * pQueue->elemSize, pData, pQueue->elemSize);

        pData = (u8*)pData + pQueue->elemSize;
        pQueue->end = end;
    }

    return i;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief     Structure Queue popped
/// @param     pQueue:     Queue name
/// @param     pData:     Data Buffer ready to pop
/// @param     blkCount:   Maximum number of stored stack structures (unit: number of structures)
/// @retval    index
////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_StructOut(QUEUE_STRUCT_t* pQueue, void* pData, u32 blkCount)
{
    u32 index = 0;

    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);
    ASSERT_PARAM(pData);

    while ((pQueue->start != pQueue->end) && (index < pQueue->sumCount) && (index < blkCount)) {
        memcpy(pData, (u8*)pQueue->pBuf + pQueue->start * pQueue->elemSize, pQueue->elemSize);

        pData = (u8*)pData + pQueue->elemSize;
        index++;
        if (++pQueue->start >= pQueue->sumCount) {
            pQueue->start = 0;
        }
    }

    return index;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief    Get the number of structure queues (the number of structures)
/// @param    pQueue
/// @retval   index
/////////////////////////////////////////////////////////////////////////////////
u32 QUEUE_StructCountGet(QUEUE_STRUCT_t* pQueue)
{
    u32 index = 0;
    u32 start = 0;

    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);

    start = pQueue->start;
    while ((start != pQueue->end) && (index < pQueue->sumCount)) {
        index++;
        if (++start >= pQueue->sumCount) {
            start = 0;
        }
    }

    return index;
}

#if DEBUG_FULL_ASSERT

////////////////////////////////////////////////////////////////////////////////
/// @brief     abnormal
/// @param     file
/// @param     line
/// @retval    None
////////////////////////////////////////////////////////////////////////////////
void ASSERT_FAILED(u8* file, u32 line)
{
    u8 flg = 1;

    printf("Error message file:%s Line% d \r\n", file, line);
    while (flg)
        ;
}

#endif
