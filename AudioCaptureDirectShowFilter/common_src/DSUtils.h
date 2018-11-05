
#ifndef _DSUTILS_
#define _DSUTILS_

#include <dshow.h>


HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
void RemoveFromRot(DWORD pdwRegister);

IPin *GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
HRESULT GetUnConnectPin(IBaseFilter *pFilter,
						PIN_DIRECTION PinDirect,
						IPin **pPin);
HRESULT ConnectFilters(IGraphBuilder *pGraph,
					IPin *pOutPin,
					IBaseFilter *pInFilter);
HRESULT ConnectFilters(IGraphBuilder *pGraph,
					IBaseFilter *pOutFilter,
					IBaseFilter *pInFilter);

void RemoveDownstream(IGraphBuilder *pGraph, IBaseFilter *pFilter);

#endif