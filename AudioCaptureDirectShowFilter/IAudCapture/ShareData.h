#ifndef _SHARE_DATA_
#define _SHARE_DATA_ 1


typedef struct {
  WORD  wFormatTag;
  WORD  nChannels;
  DWORD nSamplesPerSec;
  DWORD nAvgBytesPerSec;
  WORD  nBlockAlign;
  WORD  wBitsPerSample;
  WORD  cbSize;
}WAVEFORMATEX2;

enum RunStatus {
	STATUS_STOP =0,
	STATUS_RUN,
	STATUS_PAUSE
};

#endif