// GrayTransform.cpp: implementation of the CGrayTransform class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GrayTransform.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrayTransform::CGrayTransform()
:CTransformFilter(NAME("Gray Transform"), 0, CLSID_GrayFilter)
{

}

CGrayTransform::~CGrayTransform()
{

}

HRESULT CGrayTransform::CheckInputType(const CMediaType *mtIn)
{
	if (mtIn->majortype != MEDIATYPE_Video &&
		mtIn->subtype != MEDIASUBTYPE_RGB24 &&
		mtIn->formattype != FORMAT_VideoInfo &&
		mtIn->cbFormat != sizeof(VIDEOINFOHEADER))
	{
		return VFW_E_TYPE_NOT_ACCEPTED;
	}

//	VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER*) mtIn->pbFormat;
//	if (pvi->bmiHeader.biWidth != 352 && pvi->bmiHeader.biHeight != 288)
//		return VFW_E_TYPE_NOT_ACCEPTED;

	return S_OK;
}

HRESULT CGrayTransform::GetMediaType(int iPosition, CMediaType *pMediaType)
{
	ASSERT(m_pInput);
	if (iPosition < 0)
	{
		return E_INVALIDARG;
	}

	if (iPosition == 0)
	{
		return m_pInput->ConnectionMediaType(pMediaType);
	}

	return VFW_S_NO_MORE_ITEMS;
}

HRESULT CGrayTransform::CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut)
{
	BITMAPINFOHEADER *phin = HEADER(mtIn);
	BITMAPINFOHEADER *phout = HEADER(mtOut);
	if (mtIn->subtype != mtOut->subtype &&
		phin->biWidth != phout->biWidth &&
		phin->biHeight != phout->biHeight)
		return VFW_E_TYPE_NOT_ACCEPTED;
	return S_OK;
}

HRESULT CGrayTransform::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp)
{
    AM_MEDIA_TYPE mt;
    HRESULT hr = m_pOutput->ConnectionMediaType(&mt);
    if (FAILED(hr))
    {
        return hr;
    }
	
    ASSERT(mt.formattype == FORMAT_VideoInfo);
    BITMAPINFOHEADER *pbmi = HEADER(mt.pbFormat);
    pProp->cbBuffer = DIBSIZE(*pbmi) * 2; 
    if (pProp->cbAlign == 0)
    {
        pProp->cbAlign = 1;
    }
    if (pProp->cBuffers == 0)
    {
        pProp->cBuffers = 1;
    }
    // Release the format block.
    FreeMediaType(mt);
	
    // Set allocator properties.
    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProp, &Actual);
    if (FAILED(hr)) 
    {
        return hr;
    }
    // Even when it succeeds, check the actual result.
    if (pProp->cbBuffer > Actual.cbBuffer) 
    {
        return E_FAIL;
    }
    return S_OK;

}

double GetY(BYTE *pRGB)
{
	double R = ((double)(unsigned int) pRGB[2]);//255.0f;
	double G = ((double)(unsigned int) pRGB[1]);//255.0f;
	double B = ((double)(unsigned int) pRGB[0]);//255.0f;
	return 0.299f*R + 0.587f*G + 0.114f*B;
}

#define CHECK_BOUNDARY(x) (BYTE)(x > 255 ? 255 : (x < 0 ? 0: x));
#define CHECK_BOUNDARY_INVERSE(x) (BYTE)(x > 255 ? 0 : (x < 0 ? 255: 255-x));
#define BOUNDARY_COLOR 255
#define NOT_BOUNDARY_COLOR 0

int SobelX[9] = {1,  0,  -1, 2,  0,  -2, 1,  0,  -1};
int SobelY[9] = {1, 2, 1, 0,   0,  0, -1,   -2,  -1};


void SobelXY(BYTE *pSrc, BYTE *pDest, int x, int y, int nw, int nh, void *pParam)
{
	int nBin = (int) pParam;
	int pad = 1;
	
	int nxr = 0, nyr = 0;
	int nxg = 0, nyg = 0;
	int nxb = 0, nyb = 0;
	for (int my=-pad;my<=pad; my++)
	{
		for (int mx=-pad;mx<=pad;mx++)
		{
			int nOffsetX = x+mx;
			int nOffsetY = y+my;
			if (nOffsetX<0 || nOffsetX>=nw || nOffsetY<0 || nOffsetY>=nh) continue;

			int r = (int) pSrc[(nOffsetY*nw+nOffsetX)*3+2];
			int g = (int) pSrc[(nOffsetY*nw+nOffsetX)*3+1];
			int b = (int) pSrc[(nOffsetY*nw+nOffsetX)*3+0];
			nxr += r * SobelX[(my+1)*3+(mx+1)];
			nyr += r * SobelY[(my+1)*3+(mx+1)];
			nxg += g * SobelX[(my+1)*3+(mx+1)];
			nyg += g * SobelY[(my+1)*3+(mx+1)];
			nxb += b * SobelX[(my+1)*3+(mx+1)];
			nyb += b * SobelY[(my+1)*3+(mx+1)];
		}
	}
/*	BYTE bVal = CHECK_BOUNDARY(sqrt(nAccuX*nAccuX+nAccuY*nAccuY));
	if (nBin)
		bVal = bVal < 128 ?  NOT_BOUNDARY_COLOR: BOUNDARY_COLOR;
	if (x == 0 || y == 0 ||
		x == nw-1 || y == nh-1)
		bVal = NOT_BOUNDARY_COLOR;*/

	pDest[(y*nw+x)*3+2] = CHECK_BOUNDARY(sqrt(nxr*nxr+nyr*nyr));
	pDest[(y*nw+x)*3+1] = CHECK_BOUNDARY(sqrt(nxg*nxg+nyg*nyg));
	pDest[(y*nw+x)*3+0] = CHECK_BOUNDARY(sqrt(nxb*nxb+nyb*nyb));
}

void EdgeSobelProcess(BYTE *pIn, BYTE *pOut, int nw, int nh)
{
	for (int y=0; y<nh; y++)
	{
		for (int x=0; x<nw; x++)
		{
			SobelXY(pIn, pOut, x, y, nw, nh, (void*) 0);
		}
	}
	
}

typedef struct _RGB24_ {
	BYTE r;
	BYTE g;
	BYTE b;
} RGB24;

HRESULT CGrayTransform::Transform(IMediaSample * pSrc, IMediaSample *pDest)
{
	static bool bFirst = false;
	CMediaType pType = m_pInput->CurrentMediaType();
	VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pType.pbFormat;
	BITMAPINFOHEADER pbi = pvi->bmiHeader;
	int nw = pbi.biWidth;
	int nh = pbi.biHeight;
	int nlen = nw * nh;
	BYTE *pData1 = 0, *pData2 = 0;
	pSrc->GetPointer(&pData1);
	pDest->GetPointer(&pData2);
	RGB24 *prgb1 = (RGB24*) pData1;
	RGB24 *prgb2 = (RGB24*) pData2;

	EdgeSobelProcess(pData1, pData2, nw, nh);

/*	for (int i=0; i<nlen; i++)
	{
		prgb2[i].r = prgb1[i].r;
		prgb2[i].g = prgb1[i].g;
		prgb2[i].b = prgb1[i].b;
	}*/


	if (!bFirst)
	{
//		mbox(_T("nw:%d, nh:%d, bits: %d"), pbi.biWidth, pbi.biHeight, pbi.biBitCount);
//		mbox(_T("nw*nh: %d\nGetActualDataLength: %d,%d\n, bmiHeader.biSizeImage:%d \nGetSize():%d,%d\n"),
//			nlen, pSrc->GetActualDataLength(), pDest->GetActualDataLength(), pbi.biSizeImage, pSrc->GetSize(), pDest->GetSize());
//		bFirst = true;
	}


	
    pDest->SetActualDataLength(pbi.biSizeImage);

	return NOERROR;
}