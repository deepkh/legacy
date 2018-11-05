// GrayTransform.h: interface for the CGrayTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAYTRANSFORM_H__CB2BB9CE_C51C_4A14_93BC_42078D47963E__INCLUDED_)
#define AFX_GRAYTRANSFORM_H__CB2BB9CE_C51C_4A14_93BC_42078D47963E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


DEFINE_GUID(CLSID_GrayFilter, 
			0x1915c5c7, 0x2aa, 0x415f, 0x89, 0xf, 0x76, 0xd9, 0x4c, 0x85, 0xaa, 0xf1);

class CGrayTransform: public CTransformFilter
{
public:
	CGrayTransform();
	virtual ~CGrayTransform();
	
public:
	HRESULT CheckInputType(const CMediaType *mtIn);
	HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);
	HRESULT CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut);
	HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp);
	HRESULT Transform(IMediaSample * pIn, IMediaSample *pOut);
};

#endif // !defined(AFX_GRAYTRANSFORM_H__CB2BB9CE_C51C_4A14_93BC_42078D47963E__INCLUDED_)
