#ifndef XKILL_RENDERER_D3DMANAGEMENT_H
#define XKILL_RENDERER_D3DMANAGEMENT_H

#include <d3d11.h>

#include "d3dInterface.h"

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4X4;
}

struct VertexPosNormTex;

static const unsigned int MULTISAMPLES_GBUFFERS		= 1;
static const unsigned int MULTISAMPLES_BACKBUFFER	= 1;
static const unsigned int MULTISAMPLES_DEPTHBUFFER	= 1;

//! Class for maintaining DirectX core objects.
/*!
\ingroup xkill-renderer
*/
class D3DManagement : public D3DInterface
{
public:
	//! Sets D3DManagement to its default state.
	/*!
	\param windowHandle A handle to a WIN-API window.
	\param screenWidth Width of the screen.
	\param screenHeight Height of the screen.
	*/
	D3DManagement(HWND windowHandle, unsigned int screenWidth, unsigned int screenHeight);
	//!Releases all memory and resets D3DManagement to its default state.
	virtual ~D3DManagement();
	//!Releases all memory and resets D3DManagement to its default state.
	virtual void reset();
	
	HRESULT resize(unsigned int screenWidth, unsigned int screenHeight);

	//! Set the variable uavBackBuffer to the compute shader stage.
	void setUAVBackBufferCS();
	//! Presents the back buffer.
	void present();
	//! Clears the depth buffer in preparation for rendering. 
	void clearDepthBuffer();
	//! Initializes D3DManagement's members.
	/*!	\return First encountered error.
		\sa initDeviceAndSwapChain
		\sa initDepthBuffer
		\sa initBackBuffer
		\sa initGBuffers
		\sa initViewport
		\sa initRSDefault
		\sa initSSDefault
		\sa initFXManagement
		\sa initDebug
		\sa initConstantBuffers
		\sa initVertexBuffer
	*/
	HRESULT init();

	ID3D11Device*			getDevice()			const;
	ID3D11DeviceContext*	getDeviceContext()	const;
	ID3D11DepthStencilView* getDepthBuffer()	const;
private:
	//! Initializes struct describing swapchain using values passed in constructor.
	/*!
	\return Any error encountered during initialization.
	\sa createDeviceAndSwapChain
	*/
	HRESULT initDeviceAndSwapChain();
	//! Creates device and swap chain using correct feature-level based on hardware.
	/*!
	\param swapChainDesc Description of d3dswapchain.
	\return Any error encountered during initialization.
	*/
	HRESULT createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC swapChainDesc);
	//! Gets texture from swapchain and creates corresponding render target view and UAV.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initBackBuffer();
	//! Creates depth-buffer texture with it's corresponding depth stencil view.
	/*!
	\return Any error encountered during initialization.
	*/
	HRESULT initDepthBuffer();
	//! Translates the initiated feature-level to string which may be presented in window.
	/*!
	\return The feature-level if known or "Default" otherwize.
	\param featureLevel The initiated feature-level.
	*/
	LPCWSTR featureLevelToString(const D3D_FEATURE_LEVEL featureLevel);

	HWND						windowHandle_;						//!< WINAPI-handle to window.
	unsigned int				screenWidth_;						//!< Width of screen.
	unsigned int				screenHeight_;						//!< Height of screen.

	ID3D11Device*				device_;							//!< DirectX device pointer.
	ID3D11DeviceContext*		devcon_;							//!< DirectX device context pointer.
	IDXGISwapChain*				swapChain_;							//!< DirectX swap chain.
	ID3D11RenderTargetView*		rtvBackBuffer_;						//!< Used to render to texBackBuffer.
	ID3D11UnorderedAccessView*	uavBackBuffer_;						//!< Used to render to texBackBuffer using DirectCompute.
	ID3D11DepthStencilView*		dsvDepthBuffer_;					//!< Used to render to texDepthBuffer.
	
	ID3D11Texture2D*			texBackBuffer_;						//!< Contains the final image.
	ID3D11Texture2D*			texDepthBuffer_;					//!< Saves the depth of each rendered pixel.
};

#endif //XKILL_RENDERER_D3DMANAGEMENT_H