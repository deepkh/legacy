
#ifndef _LINKED_LIST_BUFFER_
#define _LINKED_LIST_BUFFER_ 1

class CCriticalSection;

typedef struct _Buf_ {
    unsigned char *buf;
    unsigned int size;
	int nTimeStamp;							//ms
    struct _Buf_ *pNextBuf;
} Buffer;

void Buffer_Del_All(Buffer *pBuf);

Buffer *Buffer_New(unsigned char *buf, unsigned int size, int nTimeStamp);
void Buffer_Copy(Buffer *pDest, Buffer *pSrc);
void Buffer_Del(Buffer *pBuf);
Buffer *Buffer_GetNext(Buffer **ppFirst, CCriticalSection &g_cs);
void Buffer_AddNext(Buffer **ppFirst, CCriticalSection &g_cs, unsigned char *buf, unsigned int size, int nTimeStamp, int *index);

#endif
