#include "DXTexture.h"
#include <string>
#include <algorithm>

#define USEDSHOW

// for avi playback
#pragma comment(lib, "winmm.lib")
//#ifdef USEVFW
#pragma comment(lib, "vfw32.lib")
//#endif
#ifdef USEDSHOW
#pragma comment(lib, "strmiids.lib")
#endif

//
bool ends_with(std::wstring const &a, std::wstring const &b) {
    auto len = b.length();
    auto pos = a.length() - len;
    if (pos < 0)
        return false;
    auto pos_a = &a[pos];
    auto pos_b = &b[0];
    while (*pos_a)
        if (*pos_a++ != *pos_b++)
            return false;
    return true;
}

TCHAR easytolower(TCHAR in){
  if(in<=L'Z' && in>=L'A')
    return in-(L'Z'-L'z');
  return in;
}

#define TOLOWERW(x) (std::transform((x).begin(), (x).end(), (x).begin(), easytolower))

#ifdef USEDSHOW
template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

// Query whether a pin has a specified direction (input / output)
HRESULT IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BOOL *pResult)
{
    PIN_DIRECTION pinDir;
    HRESULT hr = pPin->QueryDirection(&pinDir);
    if (SUCCEEDED(hr))
    {
        *pResult = (pinDir == dir);
    }
    return hr;
}


HRESULT IsPinConnected(IPin *pPin, BOOL *pResult)
{
    IPin *pTmp = NULL;
    HRESULT hr = pPin->ConnectedTo(&pTmp);
    if (SUCCEEDED(hr))
    {
        *pResult = TRUE;
    }
    else if (hr == VFW_E_NOT_CONNECTED)
    {
        // The pin is not connected. This is not an error for our purposes.
        *pResult = FALSE;
        hr = S_OK;
    }

    SafeRelease(&pTmp);
    return hr;
}

// Match a pin by pin direction and connection state.
HRESULT MatchPin(IPin *pPin, PIN_DIRECTION direction, BOOL bShouldBeConnected, BOOL *pResult)
{
    if (pResult == NULL)
		return S_FALSE;	// ERROR

    BOOL bMatch = FALSE;
    BOOL bIsConnected = FALSE;

    HRESULT hr = IsPinConnected(pPin, &bIsConnected);
    if (SUCCEEDED(hr))
    {
        if (bIsConnected == bShouldBeConnected)
        {
            hr = IsPinDirection(pPin, direction, &bMatch);
        }
    }

    if (SUCCEEDED(hr))
    {
        *pResult = bMatch;
    }
    return hr;
}

// Return the first unconnected input pin or output pin.
HRESULT FindUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    IEnumPins *pEnum = NULL;
    IPin *pPin = NULL;
    BOOL bFound = FALSE;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        goto done;
    }

    while (S_OK == pEnum->Next(1, &pPin, NULL))
    {
        hr = MatchPin(pPin, PinDir, FALSE, &bFound);
        if (FAILED(hr))
        {
            goto done;
        }
        if (bFound)
        {
            *ppPin = pPin;
            (*ppPin)->AddRef();
            break;
        }
        SafeRelease(&pPin);
    }

    if (!bFound)
    {
        hr = VFW_E_NOT_FOUND;
    }

done:
    SafeRelease(&pPin);
    SafeRelease(&pEnum);
    return hr;
}

HRESULT ConnectFilters(
    IGraphBuilder *pGraph, // Filter Graph Manager.
    IPin *pOut,            // Output pin on the upstream filter.
    IBaseFilter *pDest)    // Downstream filter.
{
    IPin *pIn = NULL;
        
    // Find an input pin on the downstream filter.
    HRESULT hr = FindUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
    if (SUCCEEDED(hr))
    {
        // Try to connect them.
        hr = pGraph->Connect(pOut, pIn);
        pIn->Release();
    }
    return hr;
}

HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest)
{
    IPin *pOut = NULL;

    // Find an output pin on the first filter.
    HRESULT hr = FindUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
    if (SUCCEEDED(hr))
    {
        hr = ConnectFilters(pGraph, pOut, pDest);
        pOut->Release();
    }
    return hr;
}

// redirecting for grab graphic data
HRESULT ( __stdcall * Receive_ ) ( void* inst, IMediaSample *smp ) ; 
HRESULT   __stdcall   Receive    ( void* inst, IMediaSample *smp ) {     
    BYTE*     buf;    smp->GetPointer(&buf); DWORD len = smp->GetActualDataLength();
    //AM_MEDIA_TYPE* info;
    //smp->GetMediaType(&info);
    HRESULT   ret  =  Receive_   ( inst, smp );   
    return    ret; 
}
#define DsHook(a,b,c) if (!c##_) { INT_PTR* p=b+*(INT_PTR**)a;   VirtualProtect(&c##_,4,PAGE_EXECUTE_READWRITE,&no);\
                                          *(INT_PTR*)&c##_=*p;   VirtualProtect(p,    4,PAGE_EXECUTE_READWRITE,&no);   *p=(INT_PTR)c; }
#endif

BOOL DXTexture::LoadTexture(const TCHAR *path, IDirect3DDevice9* pd3dDevice)
{
	// check is file avi?
	std::wstring data(path);
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	if (ends_with(data, L".avi")) {
#ifdef USEVFW
		// in case of avi
		// http://www.gamedev.net/page/resources/_/technical/game-programming/working-with-avi-files-r840
		AVIFileInit();
		if (AVIFileOpen(&pAvi, path, OF_READ, 0)) {
			// error
			return FALSE;
		}

		// get avi info
		// to get width, height.
		AVIFileInfo(pAvi, &aviInfo, sizeof(aviInfo));

		// get video stream
		/*PAVISTREAM pVideo[100];
		int nNumAviStream = 0;
		do {
			if (AVIFileGetStream(pAvi, &pVideo[nNumAviStream],
				streamtypeVIDEO, nNumAviStream))
				break;
		} while (++nNumAviStream && nNumAviStream < 100);
		if (!nNumAviStream)
			return FALSE;
		PAVISTREAM pStream = pVideo[0];	// first one is default stream*/
		if (AVIFileGetStream(pAvi, &pStream, streamtypeVIDEO, 0))
			return FALSE;

		// get end time
		aviTime = AVIStreamEndTime(pStream);

		// processing video stream
		frame =	AVIStreamGetFrameOpen(pStream, 0);
		if (!frame)
			return FALSE;

		// create empty texture for future drawing
		if (FAILED(D3DXCreateTexture(pd3dDevice, aviInfo.dwWidth, aviInfo.dwHeight,
			1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTexture)))
			return FALSE;
#endif
#ifdef USEDSHOW
		// refer http://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/ac877e2d-80a7-47b6-b315-5e3160b8b219/alternative-for-isamplegrabber?forum=windowsdirectshowdevelopment
		// http://msdn.microsoft.com/en-us/library/windows/desktop/dd407288(v=vs.85).aspx

		CoInitialize(NULL);
		if (FAILED(CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER, 
			IID_IGraphBuilder,(void **)&g)))
			return FALSE;
		if (FAILED(CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC,
			IID_IBaseFilter, (void**)&pVmr)))
			return FALSE;

		g->AddFilter(pVmr, L"Grabber");
		if (FAILED(pVmr->QueryInterface(IID_ISampleGrabber, (void **)&pGrabber)))
			return FALSE;

		// set grabber format
		AM_MEDIA_TYPE mt;
		ZeroMemory(&mt, sizeof(mt));
		mt.majortype = MEDIATYPE_Video;
		mt.subtype = MEDIASUBTYPE_RGB24;
		mt.formattype = FORMAT_VideoInfo;
		pGrabber->SetMediaType(&mt);

		// connect pins
		if (FAILED(g->AddSourceFilter(path, L"Video", &sourceFilter)))
			return FALSE;
		IEnumPins *pEnum = NULL;
		IPin *pPin = NULL;
		sourceFilter->EnumPins(&pEnum);
		while (S_OK == pEnum->Next(1, &pPin, 0)) {
			HRESULT _hr = ConnectFilters(g, pPin, pVmr);
			SafeRelease(&pPin);
			if (SUCCEEDED(_hr))
				break;
		}

		// set null renderer
		IBaseFilter *pNullF = NULL;
		if (FAILED(CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, 
			IID_PPV_ARGS(&pNullF))))
			return FALSE;
		g->AddFilter(pNullF, L"Null Filter");
		ConnectFilters(g, pVmr, pNullF);

		// get video info
		bool mediaConnected = false;
		HRESULT hr = pGrabber->GetConnectedMediaType(&connectedType);
		if (SUCCEEDED(hr)) {
			if (connectedType.formattype == FORMAT_VideoInfo) {
				infoHeader = (VIDEOINFOHEADER*)connectedType.pbFormat;
				width = infoHeader->bmiHeader.biWidth;
				height = infoHeader->bmiHeader.biHeight;
				mediaConnected = true;
			}
			// it will use when preparing bitmap ..
			//CoTaskMemFree(connectedType.pbFormat);
		}
		
		// create empty texture for future drawing
		if (FAILED(D3DXCreateTexture(pd3dDevice, width, height,
			1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTexture)))
			return FALSE;

		// buffer samples for bitmap
		//pGrabber->SetOneShot(TRUE);
		pGrabber->SetBufferSamples(TRUE);

		g->QueryInterface(IID_IMediaControl, (void **)&m);
		m->Run();
#endif
	} else {
		// in case of image
		// check if texture is already exists ...
		if (pTexture != 0)
			pTexture->Release();

		D3DXIMAGE_INFO pImgInf;
		if (FAILED(D3DXCreateTextureFromFileEx(pd3dDevice, path,
			D3DX_DEFAULT, D3DX_DEFAULT, 
			1,	// Mip level
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 0x00000001, 0x00000001,
			0x00000000,	//
			&pImgInf,
			NULL,
			&pTexture))) return FALSE;

		width = txtRect.right = pImgInf.Width;
		height = txtRect.bottom = pImgInf.Height;
	}

	return TRUE;
}

RECT* DXTexture::GetRect() {
	return &txtRect;
}

int DXTexture::getMovieTime() {
	if (!aviTime)
		return 0;
	
	DWORD ntime = GetTickCount();
	int r = ntime - aviTime;
	aviTime = ntime;
	return r;
}

VOID DXTexture::updateMovieTime() {
	aviTime = GetTickCount();
}

LPDIRECT3DTEXTURE9 DXTexture::GetTexture() {
	// you should not store texture
	// because texture will be created every time
	// in case of movie file.
#ifdef USEVFW
	if (pAvi) {
		// get frame
		int frameTime = AVIStreamTimeToSample(pStream, getMovieTime());

		LPBITMAPINFOHEADER bitmap;
		bitmap = (LPBITMAPINFOHEADER)AVIStreamGetFrame(frame, frameTime);

		// For 16, 24, or 32bit image formats
		//LPBYTE pData = (LPBYTE)bitmap + bitmap->biSize;

		// For 8bit image formats
		//LPBYTE pData = lpbi + lpbi->biSize + 256 * sizeof(RGBQUAD);

		// create surface
		LPDIRECT3DSURFACE9 pSurface;
		HDC hdc;
		pTexture->GetSurfaceLevel(0, &pSurface);
		if (SUCCEEDED(pSurface->GetDC(&hdc))) {
			// prepare
			LPBYTE pGDIBits;
			HBITMAP hBmp = CreateDIBSection(hdc, (LPBITMAPINFO)bitmap, DIB_RGB_COLORS, (LPVOID*)&pGDIBits, NULL, 0);

			// copy bitmap data
			HDC hBmpDC = CreateCompatibleDC(hdc);
			HGDIOBJ pOldObj = SelectObject(hBmpDC, hBmp);
			BitBlt(hdc, 0, 0, aviInfo.dwWidth, aviInfo.dwHeight,
				hBmpDC, 0, 0, SRCCOPY);

			DeleteObject(hBmp);
			DeleteDC(hBmpDC);
			pSurface->ReleaseDC(hdc);
		}

		pSurface->Release();

		// For 16, 24, or 32bit image formats
		//LPBYTE pData = (LPBYTE)bitmap + bitmap->biSize;

		// For 8bit image formats
		//LPBYTE pData = lpbi + lpbi->biSize + 256 * sizeof(RGBQUAD);
	}
#endif
#ifdef USEDSHOW
	if (g) {
		long bufSize;
		BYTE *bitmap;
		HRESULT hr = pGrabber->GetCurrentBuffer(&bufSize, 0);
		if (FAILED(pGrabber->GetCurrentBuffer(&bufSize, 0)))
			return 0;
		bitmap = (BYTE*)malloc(bufSize);
		if (FAILED(pGrabber->GetCurrentBuffer(&bufSize, (long*)bitmap)))
			return 0;

		// 3byte per pixel ...?
		D3DLOCKED_RECT lockRect;
		pTexture->LockRect(0, &lockRect, 0, 0);
		DWORD *pTexData = (DWORD*)lockRect.pBits;
		for (int idx=0; idx<(width*height*3); idx+=3) {
			pTexData[idx/3] = D3DCOLOR_XRGB(bitmap[idx], bitmap[idx+1], bitmap[idx+2]);
		}
		pTexture->UnlockRect(0);
		free(bitmap);
	}
#endif

	return pTexture;
}

BOOL DXTexture::isTextureLoaded() {
	return (pTexture != 0);
}

BOOL DXTexture::Release() {
	// texture release
	if (pTexture) {
		pTexture->Release();
		pTexture = 0;
	}

	// stream & avi file release
#if USEVFW
	if (pAvi) {
		AVIStreamRelease(pStream);
		AVIFileRelease(pAvi);
		AVIFileExit();
		pAvi = 0;
	}
#endif

	if (g) {
		CoTaskMemFree(connectedType.pbFormat);
		SafeRelease(&pGrabber);
		SafeRelease(&sourceFilter);
		SafeRelease(&pVmr);
		SafeRelease(&m);
		SafeRelease(&g);
		g = 0;
	}

	return TRUE;
}