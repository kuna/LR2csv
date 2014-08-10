#pragma once
#include "Stdafx.h"

/* ISampleGrabber definition */
interface ISampleGrabberCB : public IUnknown
{
	virtual STDMETHODIMP SampleCB( double SampleTime, IMediaSample *pSample ) = 0;
	virtual STDMETHODIMP BufferCB( double SampleTime, BYTE *pBuffer, long BufferLen ) = 0;
};

interface ISampleGrabber : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE SetOneShot( BOOL OneShot ) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetMediaType( const AM_MEDIA_TYPE *pType ) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType( AM_MEDIA_TYPE *pType ) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples( BOOL BufferThem ) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer( long *pBufferSize, long *pBuffer ) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample( IMediaSample **ppSample ) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetCallback( ISampleGrabberCB *pCallback, long WhichMethodToCallback ) = 0;
};


static const IID IID_ISampleGrabber = { 0x6B652FFF, 0x11FE, 0x4fce, { 0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F } };
static const IID IID_ISampleGrabberCB = { 0x0579154A, 0x2B53, 0x4994, { 0xB0, 0xD0, 0xE7, 0x73, 0x14, 0x8E, 0xFF, 0x85 } };
static const CLSID CLSID_SampleGrabber = { 0xC1F400A0, 0x3F08, 0x11d3, { 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };
/* ISampleGrabber definition end */

EXTERN_C const CLSID CLSID_NullRenderer;

class DXTexture {
private:
#ifdef USEVFW
	PAVIFILE pAvi;
	AVIFILEINFO aviInfo;
	PAVISTREAM pStream;
	PGETFRAME frame;
	int aviTime;
#endif
	IGraphBuilder* g;
	IBaseFilter* pVmr, *sourceFilter;
	IMediaControl* m;
	IMediaEvent* e;
	ISampleGrabber *pGrabber;
	AM_MEDIA_TYPE connectedType;
	VIDEOINFOHEADER *infoHeader;

	RECT txtRect;
	LPDIRECT3DTEXTURE9 pTexture;
public:
	BOOL LoadTexture(const TCHAR *path, IDirect3DDevice9* pd3dDevice, D3DCOLOR colorKey=0);
	BOOL isTextureLoaded();
	RECT* GetRect();
	LPDIRECT3DTEXTURE9 GetTexture();
	BOOL Release();

	bool loop;
	int width;
	int height;
};
