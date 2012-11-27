#include "renderingUtilities.h"

#include "d3dDebug.h"

D3DDebug::D3DDebug()
{
	#if defined(DEBUG) || defined(_DEBUG)
	dxgiFactory_		= nullptr;
	d3d11Debug_		= nullptr;
	dxgiDebug_		= nullptr;
	dxgiInfoQueue_	= nullptr;
	#endif //DEBUG || _DEBUG
}
D3DDebug::~D3DDebug()
{
	#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(dxgiFactory_);
	SAFE_RELEASE(d3d11Debug_);
	SAFE_RELEASE(dxgiDebug_);
	SAFE_RELEASE(dxgiInfoQueue_);
	#endif //DEBUG || _DEBUG
}

void D3DDebug::reset()
{
	#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(dxgiFactory_);
	SAFE_RELEASE(d3d11Debug_);
	SAFE_RELEASE(dxgiDebug_);
	SAFE_RELEASE(dxgiInfoQueue_);
	#endif //DEBUG || _DEBUG
}
HRESULT D3DDebug::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	#if defined(DEBUG) || defined(_DEBUG)
	
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&dxgiFactory_));
	if(hr == S_OK)
	{
		typedef HRESULT(__stdcall *fPtr)(const IID&, void**); 
		HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll"); 
		fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");
		
		DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&dxgiDebug_);
		DXGIGetDebugInterface(__uuidof(IDXGIInfoQueue), (void**)&dxgiInfoQueue_);

		hr = device->QueryInterface(__uuidof(ID3D11Debug), (void**)(&d3d11Debug_));
	}
	#endif //DEBUG || _DEBUG

	return hr;
}

HRESULT D3DDebug::reportLiveDeviceObjects()
{
	HRESULT hr = S_OK;

	#if defined(DEBUG) || defined(_DEBUG)
	hr = d3d11Debug_->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	#endif //DEBUG || _DEBUG

	return hr;
}
HRESULT D3DDebug::reportLiveObjects()
{
	HRESULT hr = S_OK;

	#if defined(DEBUG) || defined(_DEBUG)
	hr = dxgiDebug_->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	#endif //DEBUG || _DEBUG

	return hr;
}