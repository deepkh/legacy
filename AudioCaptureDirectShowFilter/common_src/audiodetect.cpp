#include "stdafx.h"
#include "audiodetect.h"
#include "math.h"


AudioDetect::AudioDetect(int nShift)
:pTmpBuf(0)
{
	pTmpBuf = new short[BUF_SIZE_CIRCULARBUFFER_INT16];
	CalcThresHold(nShift);
}

AudioDetect::~AudioDetect()
{
	delete [] pTmpBuf;
}

void AudioDetect::CalcThresHold(int nShift) //[0,100]
{
	nShift-=50;//[-50,50]
	bg_noise_T=(unsigned int)(BACKGROUND_NOISE+nShift*300);
	gtrace("background_noise_T:%d, %d\n", nShift, bg_noise_T);
}

/*
unsigned int AudioDetect::getVolume(short *buf, int size)
{
	int i;
	unsigned int volume=0;
	for(i=0; i<size; i++)
	{
//		unsigned int dB = (unsigned int)(10*log10(abs(buf[i])+1));
////		volume += dB*dB;
//		volume += dB;
//		volume += ((int)buf[i]*(int)buf[i])>>4;
		volume = volume + ((double)((int)buf[i]*(int)buf[i]));
	}
//	volume/=(unsigned int)(size*size);
	volume = volume / 16;
	volume = volume / (unsigned int)(size*size);
	return volume;
}
*/

unsigned int AudioDetect::getVolume(unsigned char *buf, int size)
{
	int i;
	unsigned int volume=0;
	for(i=0; i<size; i++)
	{
		volume += ((double)((int)buf[i]*(int)buf[i]));
	}
	volume = (int)(double)sqrt((double)(volume/(double)size));
//	gtrace("vol:%d\n", volume);
	return volume;
}
unsigned int AudioDetect::Detect(unsigned char *buf, int size)
{
	int i,j;
	unsigned int noise_t = 0;

	if ((noise_t = getVolume(buf, size)) > bg_noise_T)
	{
		return noise_t;
	}
	return noise_t;
}


/*
unsigned int AudioDetect::Detect(unsigned char *buf, int size)
{
	int i,j;
	unsigned int noise_t = 0;

	for (i=0; i<size/(BUF_SIZE_CIRCULARBUFFER_INT16*2); i++)
	{
		for (j=0; j<BUF_SIZE_CIRCULARBUFFER_INT16; j++)
		{
			pTmpBuf[j] = buf[i*BUF_SIZE_CIRCULARBUFFER_INT16*2+j*2+1] << 8;
			pTmpBuf[j] |= buf[i*BUF_SIZE_CIRCULARBUFFER_INT16*2+j*2];
		}

		if ((noise_t = getVolume(pTmpBuf, BUF_SIZE_CIRCULARBUFFER_INT16)) > bg_noise_T)
		{
			gtrace("********Detect voice(%d~%d): %d\n"
					, i*BUF_SIZE_CIRCULARBUFFER_INT16*2
					, i*BUF_SIZE_CIRCULARBUFFER_INT16*2+BUF_SIZE_CIRCULARBUFFER_INT16*2
					, noise_t);
			return 1;
		}
	}
	return 0;
}
*/
