#include "stdafx.h"
#include "LinkedListBuffer.h"
#include <afxmt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************************
  Linked List Buffer
 **************************************************/

void Buffer_Del_All(Buffer *pBuf)
{
    if (pBuf == 0) return;

    if (pBuf->pNextBuf)
    {
		Buffer_Del_All(pBuf->pNextBuf);
		pBuf->pNextBuf = 0;
    }
    if (pBuf->buf && pBuf->size)
		free(pBuf->buf);
    free(pBuf);
    //trace("[BUF_DEL2] %d\n", count);
}

void Buffer_Copy(Buffer *pDest, Buffer *pSrc)
{
	if (pDest->buf)
	{
		free(pDest->buf);
		pDest->buf = 0;
	}

    memset(pDest, 0, sizeof(Buffer));
	pDest->size = pSrc->size;
	pDest->nTimeStamp = pSrc->nTimeStamp;

    if (pSrc->buf && pSrc->size)
    {
		pDest->buf = (unsigned char*) malloc(pSrc->size);
		memcpy(pDest->buf, pSrc->buf, pSrc->size);
    }
}
// no circular version
// malloc & free version
Buffer *Buffer_New(unsigned char *buf, unsigned int size, int nTimeStamp)
{
    Buffer *pBuf = (Buffer *) malloc(sizeof(Buffer));
    memset(pBuf, 0, sizeof(Buffer));
	pBuf->size = size;
	pBuf->nTimeStamp = nTimeStamp;

    if (buf && size)
    {
		pBuf->buf = (unsigned char*) malloc(size);
		memcpy(pBuf->buf, buf, size);
    }
    return pBuf;
}

void Buffer_Del(Buffer *pBuf)
{
    if (pBuf->buf && pBuf->size)
		free(pBuf->buf);
    free(pBuf);
}

Buffer *Buffer_GetNext(Buffer **ppFirst, CCriticalSection &g_cs)
{
    Buffer *pNext = 0;
	g_cs.Lock();
	pNext = ppFirst[0]->pNextBuf;
	if (pNext)
	{
		Buffer_Del(ppFirst[0]);
	   	ppFirst[0] = pNext;
	}
	g_cs.Unlock();
    return pNext;
}

void Buffer_AddNext(Buffer **ppFirst
				, CCriticalSection &g_cs
				, unsigned char *buf
				, unsigned int size
				, int nTimeStamp
				, int *index)
{
    int count = 1;
    Buffer *pNew;
    Buffer *pNext;
	g_cs.Lock();
    pNew = Buffer_New(buf, size, nTimeStamp);
    pNext = 0;
    pNext = ppFirst[0];
    while(pNext->pNextBuf != 0)
    {
		pNext = pNext->pNextBuf;
		count++;
    }
    pNext->pNextBuf = pNew;
    *index = count;
    g_cs.Unlock();
}

