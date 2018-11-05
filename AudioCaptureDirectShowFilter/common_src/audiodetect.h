#ifndef _AUDIO_DETECT_
#define _AUDIO_DETECT_

#define BUF_SIZE_CIRCULARBUFFER_INT16 64
#define BACKGROUND_NOISE	15000

class AudioDetect {
public:
	AudioDetect(int nShift);
	~AudioDetect();
private:
	void CalcThresHold(int nShift);
	unsigned int getVolume(unsigned char *buf, int size);
public:
	unsigned int Detect(unsigned char *buf, int size);

public:
	unsigned int bg_noise_T;
	short *pTmpBuf;
};



#endif
