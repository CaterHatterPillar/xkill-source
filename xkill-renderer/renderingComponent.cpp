#include "renderingComponent.h"

#include <xkill-utilities/AttributeType.h>
#include <DirectXMath.h>

#include "fxManagement.h"
#include "ViewportManagement.h"
#include "gBuffer.h"
#include "renderingUtilities.h"
#include "d3dDebug.h"
#include "CBManagement.h"
#include "LightManagement.h"
#include "objLoaderBasic.h"
#include "mathBasic.h"
#include "vertices.h"

#include <xkill-utilities/EventManager.h>

RenderingComponent::RenderingComponent(
		HWND windowHandle)
{
	GET_ATTRIBUTES(renderAttributes_, RenderAttribute, ATTRIBUTE_RENDER);
	GET_ATTRIBUTES(cameraAttributes_, CameraAttribute, ATTRIBUTE_CAMERA);
	
	Event_getWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);

	windowHandle_	= windowHandle;
	screenWidth_	= windowResolution.width;
	screenHeight_	= windowResolution.height;
	numViewports_	= cameraAttributes_->size();

	fxManagement_		= nullptr;
	cbManagement_		= nullptr; 
	viewportManagement_ = nullptr;
	lightManagement_	= nullptr;
	
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers_[i] = nullptr;
	
	d3dDebug_	= nullptr;

	device_		= nullptr;
	devcon_		= nullptr;
	swapChain_	= nullptr;

	rtvBackBuffer_	= nullptr;
	uavBackBuffer_	= nullptr;
	dsvDepthBuffer_	= nullptr;
	rsDefault_		= nullptr;
	ssDefault_		= nullptr;
	
	texBackBuffer_	= nullptr;
	texDepthBuffer_	= nullptr;
}
RenderingComponent::~RenderingComponent()
{
	if(swapChain_)
	{
		swapChain_->SetFullscreenState(false, nullptr);
		SAFE_RELEASE(swapChain_);
	}
	
	SAFE_RELEASE(device_);
	SAFE_RELEASE(devcon_);
	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);
	SAFE_RELEASE(dsvDepthBuffer_);
	SAFE_RELEASE(rsDefault_);
	SAFE_RELEASE(ssDefault_);
	SAFE_RELEASE(texBackBuffer_);
	SAFE_RELEASE(texDepthBuffer_);

	SAFE_DELETE(cbManagement_);
	SAFE_DELETE(fxManagement_);
	
	SAFE_DELETE(viewportManagement_);
	SAFE_DELETE(lightManagement_);
	
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers_[i]);
	
	//d3dDebug_->reportLiveDeviceObjects();
	SAFE_DELETE(d3dDebug_);
}
HRESULT RenderingComponent::init()
{
	HRESULT hr = S_OK;

	if(SUCCEEDED(hr))
		hr = initDeviceAndSwapChain();
	if(SUCCEEDED(hr))
		hr = initDepthBuffer();
	if(SUCCEEDED(hr))
		hr = initBackBuffer();
	if(SUCCEEDED(hr))
		hr = initGBuffers();
	if(SUCCEEDED(hr))
		hr = initViewport();
	if(SUCCEEDED(hr))
		hr = initRSDefault();
	if(SUCCEEDED(hr))
		hr = initSSDefault();
	if(SUCCEEDED(hr))
		hr = initFXManagement();
	if(SUCCEEDED(hr))
		hr = initCBManagement();
	if(SUCCEEDED(hr))
		hr = initLightManagement();
//	if(SUCCEEDED(hr))
//		hr = initDebug();

	return hr;
}
void RenderingComponent::reset()
{
	if(swapChain_)
	{
		swapChain_->SetFullscreenState(false, nullptr);
		SAFE_RELEASE(swapChain_);
	}
	
	if(fxManagement_)
		fxManagement_->reset();
	if(viewportManagement_)
		viewportManagement_->reset();
	if(cbManagement_)
		cbManagement_->reset();
	if(lightManagement_)
		lightManagement_->reset();

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		if(gBuffers_[i])
			gBuffers_[i]->reset();
	
	SAFE_RELEASE(device_);
	SAFE_RELEASE(devcon_);
	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);
	SAFE_RELEASE(dsvDepthBuffer_);
	SAFE_RELEASE(rsDefault_);
	SAFE_RELEASE(ssDefault_);
	SAFE_RELEASE(texBackBuffer_);
	SAFE_RELEASE(texDepthBuffer_);
	//SAFE_RELEASE(cbPerFrame_);

	EventManager::getInstance();
}

void RenderingComponent::onUpdate(float delta)
{
	clearGBuffers();
	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		DirectX::XMFLOAT4X4 view((float*)&cameraAttributes_->at(i).mat_view);
		DirectX::XMFLOAT4X4 projection((float*)&cameraAttributes_->at(i).mat_projection);

		DirectX::XMFLOAT4X4 viewInverse			= calculateMatrixInverse(view);
		DirectX::XMFLOAT4X4 projectionInverse	= calculateMatrixInverse(projection);
		
		SpatialAttribute*	spatialAttribute = static_cast<SpatialAttribute*>(cameraAttributes_->at(i).spatialAttribute.host);
		PositionAttribute*	positionAttribute = static_cast<PositionAttribute*>(spatialAttribute->positionAttribute.host);
		DirectX::XMFLOAT3	eyePosition = *(DirectX::XMFLOAT3*)&positionAttribute->position;
		//DirectX::XMFLOAT3 eyePosition(0.0f, 0.0f, -50.0f);

		setViewport(i);
		renderToGBuffer(view, viewInverse, projection, projectionInverse, eyePosition);
	}
	
	renderToBackBuffer();
}

void RenderingComponent::render(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection)
{
	//renderToGBuffer(view, projection);
	renderToBackBuffer();
}
void RenderingComponent::renderToGBuffer(DirectX::XMFLOAT4X4 view,
										 DirectX::XMFLOAT4X4 viewInverse,
										 DirectX::XMFLOAT4X4 projection,
										 DirectX::XMFLOAT4X4 projectionInverse,
										 DirectX::XMFLOAT3	eyePosition)
{

	devcon_->VSSetShader(fxManagement_->getDefaultVS()->getVertexShader(), nullptr, 0);
	devcon_->PSSetShader(fxManagement_->getDefaultPS()->getPixelShader(), nullptr, 0);
	devcon_->PSSetSamplers(0, 1, &ssDefault_);
	devcon_->RSSetState(rsDefault_);
	
	UINT stride = sizeof(VertexPosNormTex);
	UINT offset = 0;
	devcon_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	devcon_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	devcon_->IASetInputLayout(fxManagement_->getILDefaultVSPosNormTex());
	devcon_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon_->DrawIndexed(tempIndicesSize, 0, 0);
	//devcon_->Draw(tempVerticesSize, 0);

	gBufferRenderSetRenderTargets();
	
	devcon_->ClearDepthStencilView(dsvDepthBuffer_, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Fetch attributes
	std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
	std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
	std::vector<PositionAttribute>* allPosition;	GET_ATTRIBUTES(allPosition, PositionAttribute, ATTRIBUTE_POSITION);
	
	for(unsigned int i=0; i<renderAttributes_->size(); i++)
	{
		DirectX::XMFLOAT4X4 finalMatrix = calculateFinalMatrix(view, projection, allSpatial->at(i), allPosition->at(i),  i);
		gBufferRenderUpdateConstantBuffers(finalMatrix, view, viewInverse, projection, projectionInverse, eyePosition);

		UINT stride = sizeof(VertexPosNormTex);
		UINT offset = 0;
		devcon_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
		devcon_->IASetInputLayout(fxManagement_->getILDefaultVSPosNormTex());
		devcon_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devcon_->Draw(vertices_->size(), 0);
	}

	gBufferRenderClean();
}
void RenderingComponent::renderToBackBuffer()
{
	FLOAT black[]	= {0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT red[]		= {1.0f, 0.0f, 0.0f, 1.0f };
	FLOAT green[]	= {0.0f, 1.0f, 0.0f, 1.0f };
	FLOAT blue[]	= {0.0f, 0.0f, 1.0f, 1.0f };

	ID3D11UnorderedAccessView* uav[] = { uavBackBuffer_ };
	devcon_->CSSetUnorderedAccessViews(0, 1, uav, nullptr);

	cbManagement_->csSet(CB_FRAME_INDEX, 0, devcon_);
	cbManagement_->csSet(CB_INSTANCE_INDEX, 1, devcon_);
	cbManagement_->updateCBInstance(devcon_, screenWidth_, screenHeight_);

	lightManagement_->setLightSRVCS(devcon_, 2);

	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers_[i]->getSRV();
	devcon_->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	devcon_->CSSetSamplers(0, 1, &ssDefault_);

	fxManagement_->getDefaultCS()->set(devcon_);
	devcon_->Dispatch(25, 25, 1);
	fxManagement_->getDefaultCS()->unset(devcon_);

	ID3D11UnorderedAccessView* uav2[] = { nullptr };
	devcon_->CSSetUnorderedAccessViews(0, 1, uav2, NULL);
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = nullptr;
	devcon_->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	devcon_->CSSetSamplers(0, 0, nullptr);

	swapChain_->Present(0, 0);
}
void RenderingComponent::setViewport(unsigned int index)
{
	viewportManagement_->setViewport(devcon_, index);
}
void RenderingComponent::clearGBuffers()
{
	FLOAT black[]	= {0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT red[]		= {1.0f, 0.0f, 0.0f, 1.0f };
	FLOAT green[]	= {0.0f, 1.0f, 0.0f, 1.0f };
	FLOAT blue[]	= {0.0f, 0.0f, 1.0f, 1.0f };

	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	
	devcon_->ClearRenderTargetView(renderTargets[GBUFFERID_ALBEDO], black);
	devcon_->ClearRenderTargetView(renderTargets[GBUFFERID_NORMAL], black);
}

void RenderingComponent::gBufferRenderUpdateConstantBuffers(DirectX::XMFLOAT4X4 finalMatrix,
															DirectX::XMFLOAT4X4 viewMatrix,
															DirectX::XMFLOAT4X4 viewInverseMatrix,
															DirectX::XMFLOAT4X4 projectionMatrix,
															DirectX::XMFLOAT4X4 projectionInverseMatrix,
															DirectX::XMFLOAT3	eyePosition)
{
	cbManagement_->vsSet(CB_FRAME_INDEX, 0, devcon_);
	cbManagement_->updateCBFrame(devcon_,
								 finalMatrix,
								 viewMatrix,
								 viewInverseMatrix,
								 projectionMatrix,
								 projectionInverseMatrix,
								 eyePosition, 
								 lightManagement_->getNumLights());
}
void RenderingComponent::gBufferRenderClean()
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();

	devcon_->VSSetShader(NULL, NULL, 0);
	devcon_->PSSetShader(NULL, NULL, 0);
	
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = nullptr;
	
	devcon_->OMSetRenderTargets(GBUFFERID_NUM_BUFFERS, renderTargets, nullptr);
	devcon_->PSSetSamplers(0, 0, nullptr);
	devcon_->IASetInputLayout(nullptr);
	devcon_->RSSetState(nullptr);
}
void RenderingComponent::gBufferRenderSetRenderTargets()
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	devcon_->OMSetRenderTargets(GBUFFERID_NUM_BUFFERS, renderTargets, dsvDepthBuffer_);
}

DirectX::XMFLOAT4X4 RenderingComponent::calculateFinalMatrix(DirectX::XMFLOAT4X4 viewMatrix,
															 DirectX::XMFLOAT4X4 projectionMatrix,
															 SpatialAttribute spatialAttribute,
															 PositionAttribute positionAttribute,
															 unsigned int attributeIndex)
{
	DirectX::CXMMATRIX mView		= DirectX::XMLoadFloat4x4(&viewMatrix);
	DirectX::CXMMATRIX mProjection	= DirectX::XMLoadFloat4x4(&projectionMatrix);


	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(positionAttribute.position.x,
																 positionAttribute.position.y,
																 positionAttribute.position.z);

	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(spatialAttribute.scale.x,
														 spatialAttribute.scale.y,
														 spatialAttribute.scale.z);

	DirectX::XMFLOAT4 fRotation = DirectX::XMFLOAT4(spatialAttribute.rotation.x,
													spatialAttribute.rotation.y,
													spatialAttribute.rotation.z,
													spatialAttribute.rotation.w);

	DirectX::XMVECTOR qRotation = DirectX::XMLoadFloat4(&fRotation);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(qRotation);

	DirectX::XMMATRIX mWorldMatrix = scaling * rotation * translation;
	DirectX::XMMATRIX mFinalMatrix = mWorldMatrix * mView * mProjection;
	
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMStoreFloat4x4(&finalMatrix, mFinalMatrix);

	return finalMatrix;
}
DirectX::XMFLOAT4X4 RenderingComponent::calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix)
{
	DirectX::CXMMATRIX	cxmMatrix		= DirectX::XMLoadFloat4x4(&matrix);
	DirectX::XMVECTOR	vDeterminant	= DirectX::XMMatrixDeterminant(cxmMatrix);
	DirectX::XMMATRIX	xmMatrixInverse = DirectX::XMMatrixInverse(&vDeterminant, cxmMatrix);
	
	DirectX::XMFLOAT4X4 matrixInverse;
	DirectX::XMStoreFloat4x4(&matrixInverse, xmMatrixInverse);
	
	return matrixInverse;
}

LPCWSTR RenderingComponent::featureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
	LPCWSTR featureString = L"Default";
	if(featureLevel == D3D_FEATURE_LEVEL_11_0)
		featureString = L"XKILL | Direct3D 11.0 device initiated with Direct3D 11.0 feature level";
	else if(featureLevel == D3D_FEATURE_LEVEL_10_1)
		featureString = L"XKILL | Direct3D 11.0 device initiated with Direct3D 10.1 feature level";
	else if(featureLevel == D3D_FEATURE_LEVEL_10_0)
		featureString = L"XKILL | Direct3D 11.0 device initiated with Direct3D 10.0 feature level";
	
	return featureString;
}
HRESULT RenderingComponent::initDeviceAndSwapChain()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount		= 1;
	swapChainDesc.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width	= screenWidth_;
	swapChainDesc.BufferDesc.Height	= screenHeight_;
	swapChainDesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.OutputWindow		= windowHandle_;
	swapChainDesc.SampleDesc.Count	= MULTISAMPLES_BACKBUFFER;
	swapChainDesc.Windowed			= true;
	swapChainDesc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = createDeviceAndSwapChain(swapChainDesc);

	return hr;
}
HRESULT RenderingComponent::createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC swapChainDesc)
{
	HRESULT hr = E_FAIL;

	UINT numFeatureLevels = 3;
	D3D_FEATURE_LEVEL initiatedFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0,
										 D3D_FEATURE_LEVEL_10_1,
										 D3D_FEATURE_LEVEL_10_0};

	UINT numDriverTypes = 2;
	D3D_DRIVER_TYPE driverTypes[] = {	D3D_DRIVER_TYPE_HARDWARE,
										D3D_DRIVER_TYPE_REFERENCE};

	UINT flags = 0;
#if defined (_DEBUG) || defined (DEBUG)
	flags = D3D11_CREATE_DEVICE_DEBUG; //Enables shader debugging with PIX
#endif //_DEBUG || DEBUG

	unsigned int index = 0;
	while(index < numDriverTypes && hr != S_OK)
	{	
		D3D_DRIVER_TYPE driverType = driverTypes[index];

		hr = D3D11CreateDeviceAndSwapChain(0,
											driverType,
											0,
											flags,
											featureLevels,
											numFeatureLevels,
											D3D11_SDK_VERSION, 
											&swapChainDesc,
											&swapChain_,
											&device_,
											&initiatedFeatureLevel,
											&devcon_);
		if(SUCCEEDED(hr))
			SetWindowText(windowHandle_, featureLevelToString(initiatedFeatureLevel));

		index++;
	}
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::createDeviceAndSwapChain D3D11CreateDeviceAndSwapChain failed");

	return hr;
}
HRESULT RenderingComponent::initDepthBuffer()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));
	texd.Width		= screenWidth_;
	texd.Height		= screenHeight_;
	texd.ArraySize	= 1;
	texd.MipLevels	= 1;
	texd.SampleDesc.Count = MULTISAMPLES_DEPTHBUFFER;
	texd.Format		= DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags	= D3D11_BIND_DEPTH_STENCIL;

	hr = device_->CreateTexture2D(&texd, NULL, &texDepthBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initDepthBuffer CreateTexture2D failed");

	if(hr == S_OK)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format			= DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2DMS;

		hr = device_->CreateDepthStencilView(texDepthBuffer_, &dsvd, &dsvDepthBuffer_);
		if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initDepthBuffer CreateDepthStencilView failed");
	}

	return hr;
}
HRESULT RenderingComponent::initBackBuffer()
{
	HRESULT hr = S_OK;

	swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texBackBuffer_);
	hr = device_->CreateRenderTargetView(texBackBuffer_, NULL, &rtvBackBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initBackBuffer CreateRenderTargetView failed");

	hr = device_->CreateUnorderedAccessView(texBackBuffer_, NULL, &uavBackBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initBackBuffer CreateUnorderedAccessView failed");

	return hr;
}
HRESULT RenderingComponent::initGBuffers()
{
	HRESULT hr = S_OK;

	GBuffer* gBuffer = nullptr;

	/*Albedo*/
	gBuffer = new GBuffer(screenWidth_, screenHeight_, MULTISAMPLES_GBUFFERS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	hr = gBuffer->init(device_);
	gBuffers_[GBUFFERID_ALBEDO] = gBuffer;

	/*Normals*/
	if(hr == S_OK)
	{
		gBuffer = new GBuffer(screenWidth_, screenHeight_, MULTISAMPLES_GBUFFERS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		hr = gBuffer->init(device_);
		gBuffers_[GBUFFERID_NORMAL] = gBuffer;
	}

	return hr;
}
HRESULT RenderingComponent::initViewport()
{
	HRESULT hr = S_OK;
	viewportManagement_ = new ViewportManagement(numViewports_,
												 screenWidth_,
												 screenHeight_);
	hr = viewportManagement_->init();

	return hr;
}
HRESULT RenderingComponent::initRSDefault()
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rsd;
	rsd.CullMode				= D3D11_CULL_NONE;
	rsd.FillMode				= D3D11_FILL_SOLID;
	rsd.FrontCounterClockwise	= false;
	rsd.DepthBias				= false;
	rsd.DepthBiasClamp			= 0;
	rsd.SlopeScaledDepthBias	= 0;
	rsd.DepthClipEnable			= true;
	rsd.ScissorEnable			= false;
	rsd.MultisampleEnable		= true;
	rsd.AntialiasedLineEnable	= true;

	hr = device_->CreateRasterizerState(&rsd, &rsDefault_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initRSDefault CreateRasterizerState failed");
	
	return hr;
}
HRESULT RenderingComponent::initSSDefault()
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter		= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD		= 0;
    sampDesc.MaxLOD		= D3D11_FLOAT32_MAX;

	hr = device_->CreateSamplerState(&sampDesc, &ssDefault_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initSSDefault CreateSamplerState failed");

	return hr;
}
HRESULT RenderingComponent::initFXManagement()
{
	HRESULT hr = S_OK;

	fxManagement_ = new FXManagement();
	hr = fxManagement_->init(device_);

	return hr;
}
HRESULT RenderingComponent::initCBManagement()
{
	HRESULT hr = S_OK;

	cbManagement_ = new CBManagement();
	cbManagement_->init(device_);

	return hr;
}
HRESULT RenderingComponent::initLightManagement()
{
	HRESULT hr = S_OK;

	lightManagement_ = new LightManagement();
	hr = lightManagement_->init(device_);

	return hr;
}
HRESULT RenderingComponent::initDebug()
{
	HRESULT hr = S_OK;

	d3dDebug_ = new D3DDebug();
	hr = d3dDebug_->init(device_);

	return hr;
}

void RenderingComponent::onEvent( Event* e )
{

}
