#include <xkill-utilities/Util.h>
#include <xkill-utilities/SkinnedData.h>

#include "ManagementD3D.h"
#include "ManagementFX.h"
#include "ManagementCB.h"
#include "ManagementLight.h"
#include "ManagementViewport.h"
#include "ManagementModel.h"
#include "ManagementTex.h"
#include "ManagementSS.h"
#include "ManagementRS.h"
#include "ManagementGBuffer.h"
#include "ManagementDebug.h"
#include "ManagementMath.h"
#include "ManagementInstance.h"
#include "ManagementSprites.h"
#include "ManagementAnimation.h"

#include "Winfo.h"
#include "ModelD3D.h"
#include "gBuffer.h"
#include "SubsetD3D.h"
#include "DebugShapeD3D.h"
#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"
#include "TypeFX.h"
#include "Renderer.h"
#include "ViewportData.h"

//temp
#include "AnimatedMesh.h"
#include "M3DLoader.h"
//#include "SkinnedData.h"

ATTRIBUTES_DECLARE_ALL;

Renderer::Renderer(HWND windowHandle)
{
	windowHandle_	= windowHandle;
	
	winfo_ = nullptr;

	managementD3D_		 = nullptr;
	managementFX_		 = nullptr;
	managementCB_		 = nullptr;
	managementLight_	 = nullptr;
	managementViewport_	 = nullptr;
	managementModel_	 = nullptr;
	managementTex_		 = nullptr;
	managementSS_		 = nullptr;
	managementRS_		 = nullptr;
	managementGBuffer_	 = nullptr;
	managementDebug_	 = nullptr;
	managementMath_		 = nullptr;
	managementInstance_  = nullptr;
	managementSprites_   = nullptr;
	managementAnimation_ = nullptr;

	//attributesRenderOwner_	= nullptr;

	ATTRIBUTES_INIT_ALL;

	debugLinesVertexBuffer_ = nullptr;

	//temp
	m3dLoader_		= nullptr;
	animatedMesh_	= nullptr;
}
Renderer::~Renderer()	
{
	SAFE_DELETE(winfo_);

	SAFE_DELETE(managementD3D_);
	SAFE_DELETE(managementFX_);	
	SAFE_DELETE(managementCB_);
	SAFE_DELETE(managementLight_);
	SAFE_DELETE(managementViewport_);
	SAFE_DELETE(managementModel_);
	SAFE_DELETE(managementTex_);
	SAFE_DELETE(managementSS_);
	SAFE_DELETE(managementRS_);
	SAFE_DELETE(managementGBuffer_);
	SAFE_DELETE(managementMath_);
	SAFE_DELETE(managementInstance_);
	SAFE_DELETE(managementSprites_);
	SAFE_DELETE(managementAnimation_);

	//d3dDebug_->reportLiveDeviceObjects();
	SAFE_DELETE(managementDebug_);

	SAFE_RELEASE(debugLinesVertexBuffer_);

	//temp
//	SAFE_DELETE(m3dLoader_);
//	SAFE_DELETE(animatedMesh_);
}

void Renderer::reset()
{
	SAFE_RESET(managementD3D_);
	SAFE_RESET(managementD3D_);
	SAFE_RESET(managementFX_);	
	SAFE_RESET(managementCB_);
	SAFE_RESET(managementLight_);
	SAFE_RESET(managementViewport_);
	SAFE_RESET(managementSS_);
	SAFE_RESET(managementRS_);
	SAFE_RESET(managementGBuffer_);
	SAFE_RESET(managementSprites_);
	SAFE_RESET(managementAnimation_);
}

void Renderer::unloadModels()
{
	managementModel_->unloadModels();
}

HRESULT Renderer::resize(unsigned int screenWidth, unsigned int screenHeight)
{
	//Get number of split-screens
	unsigned int numSS = itrSplitScreen.size();
	
	//Initialize new windo-type object.
	HRESULT hr = S_OK;
	unsigned int numViewports, csDispatchX, csDispatchY;
	numViewports	= numSS;
	csDispatchX		= screenWidth	/ CS_TILE_SIZE;
	csDispatchY		= screenHeight	/ CS_TILE_SIZE;
	winfo_->init(
		screenWidth, 
		screenHeight, 
		numViewports, 
		csDispatchX, 
		csDispatchY);

	hr = managementD3D_->resize();
	if(SUCCEEDED(hr))
		hr = managementGBuffer_->resize(managementD3D_->getDevice());
	if(SUCCEEDED(hr))
		hr = managementViewport_->resize();

	//Update per-instance constant-buffer.
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	managementCB_->setCB(
		CB_TYPE_INSTANCE,
		TypeFX_CS, 
		CB_REGISTER_INSTANCE, 
		devcon);
	managementCB_->updateCBInstance(
		devcon, 
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight());

	return hr;
}

HRESULT Renderer::init()
{
	HRESULT hr = S_OK;

	initWinfo();
	if(SUCCEEDED(hr))
		hr = initManagementD3D();
	if(SUCCEEDED(hr))
		hr = initManagementFX();
	if(SUCCEEDED(hr))
		hr = initManagementCB();
	if(SUCCEEDED(hr))
		hr = initManagementLight();
	if(SUCCEEDED(hr))
		hr = initManagementModel();
	if(SUCCEEDED(hr))
		hr = initManagementTex();
	if(SUCCEEDED(hr))
		hr = initManagementViewport();
	if(SUCCEEDED(hr))
		hr = initManagementSS();
	if(SUCCEEDED(hr))
		hr = initManagementRS();
//	if(SUCCEEDED(hr))
//		hr = initManagementDebug();
	initManagementMath();
	if(SUCCEEDED(hr))
		hr = initManagementGBuffer();
	if(SUCCEEDED(hr))
		hr = initManagementSprites();
	initManagementInstance();
	initManagementAnimation();

	//temp
	/*
	m3dLoader_ = new M3DLoader();
	animatedMesh_ = nullptr;
	animatedMesh_ = new AnimatedMesh();
	m3dLoader_->loadM3D("../../xkill-resources/xkill-models/soldier.m3d",
					   animatedMesh_->getVertices(),
					   animatedMesh_->getIndices(),
					   animatedMesh_->getSubsets(),
					   animatedMesh_->getMaterials(),
					   animatedMesh_->getSkinInfo());
	animatedMesh_->init(managementD3D_->getDevice());
	*/
	initDebugAnimation();

	return hr;
}
void Renderer::initWinfo()
{
	Event_GetWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);

	unsigned int screenWidth, screenHeight, numViewports, csDispatchX, csDispatchY;
	screenWidth		= windowResolution.width;
	screenHeight	= windowResolution.height;
	numViewports	= itrCamera.size();
	csDispatchX		= screenWidth	/ CS_TILE_SIZE;
	csDispatchY		= screenHeight	/ CS_TILE_SIZE;

	winfo_ = new Winfo();
	winfo_->init(
		screenWidth, 
		screenHeight, 
		numViewports, 
		csDispatchX, 
		csDispatchY);
}
HRESULT Renderer::initManagementD3D()
{
	HRESULT hr;

	managementD3D_ = new ManagementD3D(windowHandle_, winfo_);
	hr = managementD3D_->init();

	return hr;
}
HRESULT Renderer::initManagementFX()
{
	HRESULT hr = S_OK;

	bool debugShaders = false;
#if defined (_DEBUG) || defined (DEBUG)
	debugShaders = true;
#endif //_DEBUG || DEBUG

	managementFX_ = new ManagementFX(debugShaders);
	hr = managementFX_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementCB()
{
	HRESULT hr = S_OK;

	managementCB_ = new ManagementCB();
	hr = managementCB_->init(managementD3D_->getDevice());

	if(SUCCEEDED(hr))
	{
		ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
		managementCB_->setCB(
			CB_TYPE_INSTANCE,
			TypeFX_CS, 
			CB_REGISTER_INSTANCE, 
			devcon);
		managementCB_->updateCBInstance(
			devcon, 
			winfo_->getScreenWidth(), 
			winfo_->getScreenHeight());
	}

	return hr;
}
HRESULT Renderer::initManagementLight()
{
	HRESULT hr = S_OK;

	managementLight_ = new ManagementLight();
	managementLight_->init();

	return hr;
}
HRESULT Renderer::initManagementModel()
{
	HRESULT hr = S_OK;

	managementModel_ = new ManagementModel();
	hr = managementModel_->init();

	return hr;
}
HRESULT Renderer::initManagementTex()
{
	HRESULT hr = S_OK;

	managementTex_ = new ManagementTex();
	hr = managementTex_->init();

	return hr;
}
HRESULT Renderer::initManagementViewport()
{
	HRESULT hr = S_OK;

	managementViewport_ = new ManagementViewport(winfo_);
	hr = managementViewport_->init();

	return hr;
}
HRESULT Renderer::initManagementSS()
{
	HRESULT hr = S_OK;

	managementSS_ = new ManagementSS();
	hr = managementSS_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementRS()
{
	HRESULT hr = S_OK;

	managementRS_ = new ManagementRS();
	managementRS_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementGBuffer()
{
	HRESULT hr = S_OK;

	managementGBuffer_ = new ManagementGBuffer(winfo_);
	hr = managementGBuffer_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementDebug()
{
	HRESULT hr = S_OK;

	managementDebug_ = new ManagementDebug();
	hr = managementDebug_->init(managementD3D_->getDevice());

	return hr;
}
void Renderer::initManagementMath()
{
	managementMath_ = new ManagementMath();
}
void Renderer::initManagementInstance()
{
	managementInstance_ = new ManagementInstance();
}
HRESULT Renderer::initManagementSprites()
{
	HRESULT hr = S_OK;
	managementSprites_ = new ManagementSprites();
	hr = managementSprites_->init(managementD3D_->getDevice());
	return hr;
}
void Renderer::initManagementAnimation()
{
	managementAnimation_ = new ManagementAnimation();
}

void Renderer::update()
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Update lights.
	managementLight_->update(device, devcon);

	//Update instances.
	managementInstance_->update(device, devcon);
}
void Renderer::render()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	//Clear g-buffers and depth buffer.
	managementGBuffer_->clearGBuffers(devcon);
	managementD3D_->clearDepthBuffer();
	managementD3D_->clearBackBuffer();

	//Update per-frame constant buffer.
	managementCB_->setCB(
		CB_TYPE_FRAME, 
		TypeFX_VS, 
		CB_REGISTER_FRAME, 
		devcon);
	managementCB_->updateCBFrame(
		devcon,
		managementLight_->getLightDirCurCount(),
		managementLight_->getLightPointCurCount(),
		managementLight_->getLightSpotCurCount());

	AttributePtr<Attribute_SplitScreen>	ptr_splitScreen;
	AttributePtr<Attribute_Camera>		ptr_camera; 
	AttributePtr<Attribute_Spatial>		ptr_spatial;
	AttributePtr<Attribute_Position>	ptr_position;

	ViewportData vpData;

	//Render each split-screen separately
	std::vector<SplitScreenViewport>* ssViewports = managementViewport_->getSplitScreenViewports();
	std::vector<ViewportData> vpDatas(ssViewports->size());
	for(unsigned int i = 0; i < ssViewports->size(); i++)
	{
		ptr_splitScreen		= ssViewports->at(i).ptr_splitScreen;
		ptr_camera			= ptr_splitScreen->ptr_camera;
		ptr_spatial			= ptr_camera->ptr_spatial;
		ptr_position		= ptr_spatial->ptr_position;

		managementViewport_->setViewport(devcon, i);

		//Store all the viewport-specific data for the backbuffer-rendering.
		vpData.camIndex			= ptr_camera.index();
		vpData.view				= DirectX::XMFLOAT4X4(((float*)&ptr_camera->mat_view));
		vpData.proj				= DirectX::XMFLOAT4X4(((float*)&ptr_camera->mat_projection));
		vpData.viewInv			= managementMath_->calculateMatrixInverse(vpData.view);
		vpData.projInv			= managementMath_->calculateMatrixInverse(vpData.proj);
		vpData.eyePos			= *(DirectX::XMFLOAT3*)&ptr_position->position;
		vpData.viewportTopX		= static_cast<unsigned int>(ptr_splitScreen->ssTopLeftX);
		vpData.viewportTopY		= static_cast<unsigned int>(ptr_splitScreen->ssTopLeftY);
		vpData.zNear			= ptr_camera->zNear;
		vpData.zFar				= ptr_camera->zFar;
		vpData.viewportWidth	= (float)ptr_splitScreen->ssWidth;
		vpData.viewportHeight	= (float)ptr_splitScreen->ssHeight;
		vpDatas[i]				= vpData;

		renderViewportToGBuffer(vpData);
	}

	//Render everything to backbuffer.
	for(unsigned int i = 0; i < vpDatas.size(); i++)
		renderViewportToBackBuffer(vpDatas[i]);

	for(unsigned int i=0; i< vpDatas.size(); i++)
		renderHudElements(i);

	managementD3D_->present();
}
void Renderer::renderViewportToGBuffer(ViewportData& vpData)									
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	if(animatedMesh_)
		renderAnimatedMesh(vpData.view, vpData.proj);

	managementFX_->setShader(devcon, SHADERID_VS_DEFAULT);
	managementFX_->setShader(devcon, SHADERID_PS_DEFAULT);


	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	managementGBuffer_->setGBuffersAndDepthBuffer(devcon, managementD3D_->getDepthBuffer());

	//Update per-viewport constant buffer.
	managementCB_->setCB(CB_TYPE_CAMERA, TypeFX_VS, CB_REGISTER_CAMERA, managementD3D_->getDeviceContext());
	managementCB_->updateCBCamera(managementD3D_->getDeviceContext(),
		vpData.view,
		vpData.viewInv,
		vpData.proj,
		vpData.projInv,
		vpData.eyePos,
		vpData.viewportTopX,
		vpData.viewportTopY,
		vpData.zNear,
		vpData.zFar,
		vpData.viewportWidth,
		vpData.viewportHeight);

	std::map<unsigned int, InstancedData*> instancesMap = managementInstance_->getInstancesMap();
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap.begin(); i != instancesMap.end(); i++)
	{
		//if(i->first == 12)
		//	renderAnimation(i->first, vpData.view, vpData.proj);
		//else
			renderInstance(i->first, i->second);
	}

	//Make me use iterators!
	AttributePtr<Attribute_DebugShape> ptr_debugShape;
	while(itrDebugShape.hasNext())
	{
		ptr_debugShape = itrDebugShape.getNext();
		if(ptr_debugShape->render)
		{
			renderDebugShape(
				ptr_debugShape,
				itrDebugShape.storageIndex(),
				vpData.view, 
				vpData.proj);
		}
	}

	if(settings->showDebugPhysics)
	{
		drawBulletPhysicsDebugLines(vpData.view, vpData.proj);
	}
	
	//Unset and clean.
	managementGBuffer_->unsetGBuffersAndDepthBufferAsRenderTargets(devcon);
	managementFX_->unsetAll(devcon);
	managementFX_->unsetLayout(devcon);
	//managementSS_->unsetSS(devcon, TypeFX_PS, 0);
	devcon->RSSetState(nullptr);
}
void Renderer::renderViewportToBackBuffer(ViewportData& vpData)
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	//Set backbuffer.
	managementD3D_->setDepthBufferSRV(GBUFFER_SHADER_REGISTER_DEPTH);
	managementD3D_->setUAVBackBufferCS();

	//Set shader.
	managementFX_->setShader(devcon, SHADERID_CS_DEFAULT);

	//Set constant buffers.
	managementCB_->setCB(CB_TYPE_FRAME,		TypeFX_CS, CB_REGISTER_FRAME,	devcon);
	managementCB_->setCB(CB_TYPE_CAMERA,	TypeFX_CS, CB_REGISTER_CAMERA,	devcon);
	managementCB_->updateCBCamera(managementD3D_->getDeviceContext(),
		vpData.view,
		vpData.viewInv,
		vpData.proj,
		vpData.projInv,
		vpData.eyePos,
		vpData.viewportTopX,
		vpData.viewportTopY,
		vpData.zNear,
		vpData.zFar,
		vpData.viewportWidth,
		vpData.viewportHeight);

	//Connect g-buffers to shader.
	managementGBuffer_->setGBuffersAsCSShaderResources(devcon);

	//Set lights.
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_DIR,		LIGHT_SRV_REGISTER_DIR);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_POINT,		LIGHT_SRV_REGISTER_POINT);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_SPOT,		LIGHT_SRV_REGISTER_SPOT);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_POS_VIEW,	LIGHT_SRV_REGISTER_POS);
	
	//Set default samplerstate.
	managementSS_->setSS(devcon, TypeFX_CS, 0, SS_ID_DEFAULT);

	//Call compute shader kernel.
	unsigned int dispatchX = winfo_->getCSDispathX() / managementViewport_->getNumViewportsX();
	unsigned int dispatchY = winfo_->getCSDispathY() / managementViewport_->getNumViewportsY();
	devcon->Dispatch(dispatchX, dispatchY, 1);

	//Unset and clean.
	managementLight_->unsetLightSRVCS(devcon, LIGHTBUFFERTYPE_DIR,		LIGHT_SRV_REGISTER_DIR);
	managementLight_->unsetLightSRVCS(devcon, LIGHTBUFFERTYPE_POINT,	LIGHT_SRV_REGISTER_POINT);
	managementLight_->unsetLightSRVCS(devcon, LIGHTBUFFERTYPE_SPOT,		LIGHT_SRV_REGISTER_SPOT);
	managementLight_->unsetLightSRVCS(devcon, LIGHTBUFFERTYPE_POS_VIEW,	LIGHT_SRV_REGISTER_POS);

	managementFX_->unsetShader(devcon, SHADERID_CS_DEFAULT);

	managementD3D_->unsetUAVBackBufferCS();
	managementD3D_->unsetDepthBufferSRV(GBUFFER_SHADER_REGISTER_DEPTH);
	managementGBuffer_->unsetGBuffersAsCSShaderResources(devcon);

	//devcon->CSSetSamplers(0, 0, nullptr); //move me into managementSS
}

void Renderer::renderInstance(unsigned int meshID, InstancedData* instance)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Fetch renderer representation of model.
	ModelD3D* modelD3D	= managementModel_->getModelD3D(meshID, device);

	ShadingDesc shadingDesc = deriveShadingDesc(modelD3D->getVertexType());
	setShadingDesc(shadingDesc);

	//Set vertex buffer.
	UINT offset[2] = { 0, 0 };
	ID3D11Buffer* vbs[2] = 
	{ 
		modelD3D->getVertexBuffer(), 
		instance->getDataBuffer()
	};
	devcon->IASetVertexBuffers(0, 2, vbs, shadingDesc.stride_, offset);
	
	std::vector<SubsetD3D*>		subsetD3Ds	= modelD3D->getSubsetD3Ds();
	std::vector<MaterialDesc>	materials	= modelD3D->getMaterials();
	for(unsigned int i = 0; i < subsetD3Ds.size(); i++)
	{
		unsigned int materialIndex	= subsetD3Ds[i]->getMaterialIndex();

		renderSubset(
			subsetD3Ds[i],
			materials[materialIndex],
			instance->getDataCountCur());
	}

	//Unset vertex buffers. (In case of the instance buffer needing to be mapped to in ManagementInstance)
	ID3D11Buffer* nullVBs[2] = { NULL, NULL };
	devcon->IASetVertexBuffers(
		0, 
		2, 
		nullVBs, 
		offset, 
		offset);
}

ShadingDesc Renderer::deriveShadingDesc(VertexType vertexType)
{
	ShadingDesc shadingDesc;
	switch(vertexType)
	{
	case VERTEX_TYPE_POS_NORM_TEX_TAN:
		{
			shadingDesc.vsID_ = SHADERID_VS_POS_NORM_TEX_TAN_INSTANCE;
			shadingDesc.psID_ = SHADERID_PS_NORMALMAP;

			shadingDesc.layoutID_ = LAYOUTID_POS_NORM_TEX_TAN_INSTANCED;

			shadingDesc.stride_[0] = sizeof(VertexPosNormTexTan);
			shadingDesc.stride_[1] = sizeof(VertexInstanced);
			break;
		}
	default:
		{
			shadingDesc.vsID_ = SHADERID_VS_DEFAULT;
			shadingDesc.psID_ = SHADERID_PS_DEFAULT;

			shadingDesc.layoutID_ = LAYOUTID_POS_NORM_TEX_INSTANCED;

			shadingDesc.stride_[0] = sizeof(VertexPosNormTex);
			shadingDesc.stride_[1] = sizeof(VertexInstanced);
			break;
		}
	}

	return shadingDesc;
}
void Renderer::setShadingDesc(ShadingDesc shadingDesc)
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	//Set shaders
	managementFX_->setShader(devcon, shadingDesc.vsID_);
	managementFX_->setShader(devcon, shadingDesc.psID_);

	//Set layout
	managementFX_->setLayout(devcon, shadingDesc.layoutID_);
}

void Renderer::renderSubset(SubsetD3D* subset, MaterialDesc& material, unsigned int numInstances)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Set textures.
	ID3D11ShaderResourceView* texAlbedo = managementTex_->getTexSrv(material.idAlbedoTex_);
	ID3D11ShaderResourceView* texNormal = managementTex_->getTexSrv(material.idNormalTex_);
	devcon->PSSetShaderResources(0, 1, &texAlbedo);
	devcon->PSSetShaderResources(1, 1, &texNormal);

	//Set per-subset constant buffer.
	managementCB_->setCB(
		CB_TYPE_SUBSET, 
		TypeFX_PS, 
		CB_REGISTER_SUBSET, 
		devcon);
	DirectX::XMFLOAT3 dxSpec(
		material.specularTerm_.x, 
		material.specularTerm_.y, 
		material.specularTerm_.z);
	managementCB_->updateCBSubset(
		devcon,
		dxSpec,
		material.specularPower_);

	//Set index-buffer.
	UINT offset = 0;
	devcon->IASetIndexBuffer(
		subset->getIndexBuffer(), 
		DXGI_FORMAT_R32_UINT, 
		offset);

	//Set topology. Where to put this?
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw subset.
	devcon->DrawIndexedInstanced(
		subset->getNumIndices(),
		numInstances,
		0, 0, 0);
}
void Renderer::renderDebugShape(
	AttributePtr<Attribute_DebugShape>	ptr_debugShape, 
	unsigned int			shapeIndex,
	DirectX::XMFLOAT4X4		viewMatrix, 
	DirectX::XMFLOAT4X4		projectionMatrix)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();
	
	// Get transform matrices.
	AttributePtr<Attribute_Spatial>	ptr_spatial		= ptr_debugShape->ptr_spatial;
	AttributePtr<Attribute_Position> ptr_position = ptr_spatial->ptr_position;
	DirectX::XMFLOAT4X4 worldMatrix			= managementMath_->calculateWorldMatrix(ptr_spatial, ptr_position);
	DirectX::XMFLOAT4X4 worldMatrixInverse	= managementMath_->calculateMatrixInverse(worldMatrix);
	DirectX::XMFLOAT4X4 finalMatrix			= managementMath_->calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
	
	managementFX_->setShader(devcon, SHADERID_VS_COLOR);
	managementFX_->setShader(devcon, SHADERID_PS_COLOR);

	//Update per-object constant buffer.
	managementCB_->setCB(CB_TYPE_OBJECT, TypeFX_VS, CB_REGISTER_OBJECT, devcon);
	managementCB_->updateCBObject(
		devcon, 
		finalMatrix, 
		worldMatrix, 
		worldMatrixInverse);
	
	//Fetch renderer representation of shape.
	DebugShapeD3D* shapeD3D = managementModel_->getDebugShapeD3D(shapeIndex, device);
	
	//Set vertex buffer.
	ID3D11Buffer* vertexBuffer	= shapeD3D->getVB()->getVB();
	unsigned int numVertices	= shapeD3D->getVB()->getNumVertices();

	UINT stride = sizeof(VertexPosColor);
	UINT offset = 0;
	devcon->IASetVertexBuffers(
		0, 
		1, 
		&vertexBuffer, 
		&stride, 
		&offset);

	//Set input layout
	managementFX_->setLayout(devcon, LAYOUTID_POS_COLOR);

	//Set topology. Where to put this?
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//Draw subset.
	devcon->Draw(numVertices, 0);
}

void Renderer::drawBulletPhysicsDebugLines(
	DirectX::XMFLOAT4X4		viewMatrix, 
	DirectX::XMFLOAT4X4		projectionMatrix)
{
	//Bullet Physics debug lines are stored as a pointer to an std::vector containing "VertexPosColor" in an event (refer to debugDrawDispatcher::drawLine).
	//The event is fetched and handled from here. Afterwards the event queue is flushed using FLUSH_QUEUED_EVENTS.
	static unsigned int nrOfDebugLines = 0;

	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Update (recreate) vertex buffer
	std::vector<Event*>* debugDrawEvents = GET_POINTER_TO_QUEUED_EVENTS(EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES);
	if(debugDrawEvents->size() > 0)
	{
		SAFE_RELEASE(debugLinesVertexBuffer_);
		
		//Assume that debugDrawEvents contains one event, i.e. handle only one event. All events, including this one, should be flushed correctly below using FLUSH_QUEUED_EVENTS.
		Event* e = debugDrawEvents->at(0);
		EventType type = e->getType();
		if(type == EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES)
		{
			Event_DrawBulletPhysicsDebugLines* debugDraw = static_cast<Event_DrawBulletPhysicsDebugLines*>(e);
			std::vector<VertexPosColor>* debugLines = debugDraw->debugLineVertices; //Vertex vector extracted from event
			nrOfDebugLines = debugDraw->debugLineVertices->size(); //Vertex vector extracted from event

			D3D11_BUFFER_DESC vbd;
			vbd.Usage			= D3D11_USAGE_DYNAMIC;
			vbd.ByteWidth		= sizeof(VertexPosColor) * nrOfDebugLines;
			vbd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
			vbd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
			vbd.MiscFlags		= 0;

			D3D11_SUBRESOURCE_DATA vinitData;
			vinitData.pSysMem = debugLines->data();

			//Create vertex buffer
			HRESULT hr;
			hr = device->CreateBuffer(&vbd, &vinitData, &debugLinesVertexBuffer_);
		}
	}
	FLUSH_QUEUED_EVENTS(EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES);

	//Draw Bullet Physics debug lines
	if(debugLinesVertexBuffer_)
	{
		DirectX::XMFLOAT4X4 identityMatrix
		(
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		);

		DirectX::XMFLOAT4X4 worldMatrix			= identityMatrix;
		DirectX::XMFLOAT4X4 worldMatrixInverse	= identityMatrix;
		DirectX::XMFLOAT4X4 finalMatrix			= managementMath_->calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
					
		managementFX_->setShader(devcon, SHADERID_VS_COLOR);
		managementFX_->setShader(devcon, SHADERID_PS_COLOR);

		//Update per-object constant buffer.
		managementCB_->setCB(CB_TYPE_OBJECT, TypeFX_VS, CB_REGISTER_OBJECT, devcon);
		managementCB_->updateCBObject(
			devcon, 
			finalMatrix, 
			worldMatrix, 
			worldMatrixInverse);

		UINT stride = sizeof(VertexPosColor);
		UINT offset = 0;
		devcon->IASetVertexBuffers(
			0, 
			1, 
			&debugLinesVertexBuffer_, 
			&stride, 
			&offset);

		managementFX_->setLayout(devcon, LAYOUTID_POS_COLOR);

		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		devcon->Draw(nrOfDebugLines, 0);
	}
}

void Renderer::renderHudElements(int viewportIndex)
{
	//Since drawn directly in screen space hud elements are not affected by the camera's aspect ratio,
	//Therefor when there are two viewports the hud elements needs to be scaled by 0.5 along the x-axis
	//to keep thier proportions.
	float scaleModifierX = 1.0f;
	int numViewports = managementViewport_->getNumViewportsX() * managementViewport_->getNumViewportsY();
	if(numViewports == 2)
		scaleModifierX = 0.5f;

	renderHudElementCrossHair(viewportIndex, scaleModifierX);
}
void Renderer::renderHudElementCrossHair(int viewportIndex, float scaleModifierX)
{
	DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(0.0f);
	DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(0.0f);
	DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(0.0f);
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(0.2f * scaleModifierX, 0.2f, 1.0f);

	DirectX::XMMATRIX spriteMatrix = rotationX * rotationY * rotationZ * scaling * translation;
	DirectX::XMFLOAT4X4 transformationMatrix;
	DirectX::XMStoreFloat4x4(&transformationMatrix, spriteMatrix);

	drawHudElement(viewportIndex, TEXTURE_ID_CROSS_HAIR, transformationMatrix);
}
void Renderer::drawHudElement(int viewportIndex, unsigned int textureId, DirectX::XMFLOAT4X4 transformationMatrix)
{
	ID3D11Device*		 device = managementD3D_->getDevice();
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	managementCB_->updateCBSprite(devcon, transformationMatrix);
	managementCB_->setCB(CB_TYPE_SPRITE, TypeFX_VS, CB_REGISTER_SPRITE, devcon);

	managementViewport_->setViewport(devcon, viewportIndex);

	managementFX_->setShader(devcon, SHADERID_VS_SPRITE);
	managementFX_->setShader(devcon, SHADERID_PS_SPRITE);
	
	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_SPRITE);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	ID3D11ShaderResourceView* tex = managementTex_->getTexSrv(textureId);
	devcon->PSSetShaderResources(0, 1, &tex);

	managementD3D_->setRenderTargetViewBackBuffer();
	//6managementD3D_->setUAVBackBufferCS();

	managementFX_->setLayout(devcon, LAYOUTID_POS_NORM_TEX);
	
	ID3D11Buffer* vertexBuffer = managementSprites_->getVertexBuffer();
	UINT stride = sizeof(VertexPosNormTex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(
				0, 
				1, 
				&vertexBuffer, 
				&stride, 
				&offset);
	devcon->IASetIndexBuffer(managementSprites_->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(NUM_SPRITE_INDICES, 0, 0);

	managementD3D_->unsetRenderTargetViewBackBufer();
	//managementD3D_->unsetUAVBackBufferCS();

	managementFX_->unsetAll(devcon);
	//devcon->PSSetSamplers(0, 0, nullptr);
	devcon->IASetInputLayout(nullptr);
	devcon->RSSetState(nullptr);
}

void Renderer::renderAnimation(unsigned int meshID, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	ModelD3D* modelD3D	= managementModel_->getModelD3D(meshID, device);

	DirectX::XMFLOAT4X4 worldMatrix(1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 3.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4X4 worldMatrixInverse	= worldMatrix;
	DirectX::XMFLOAT4X4 finalMatrix			= managementMath_->calculateFinalMatrix(worldMatrix, view, projection);
	
	managementCB_->setCB(CB_TYPE_OBJECT, TypeFX_VS, CB_REGISTER_OBJECT, devcon);
	managementCB_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);

	std::vector<DirectX::XMFLOAT4X4> finalTransforms;
	managementAnimation_->getAnimation(0)->getFinalTransforms("ArmatureAction", managementAnimation_->time, &finalTransforms);

	managementCB_->setCB(CB_TYPE_BONE, TypeFX_VS, CB_REGISTER_BONE, devcon);
	managementCB_->updateCBBone(devcon, finalTransforms);

	managementFX_->setShader(devcon, SHADERID_VS_ANIMATION);
	managementFX_->setShader(devcon, SHADERID_PS_ANIMATION);
	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	managementGBuffer_->setGBuffersAndDepthBuffer(devcon, managementD3D_->getDepthBuffer());

	ID3D11Buffer* vertexBuffer = modelD3D->getVertexBuffer();
	UINT stride = sizeof(VertexPosNormTexTanSkinned);
	UINT offset = 0;
	devcon->IASetVertexBuffers(
				0, 
				1, 
				&vertexBuffer, 
				&stride, 
				&offset);
	devcon->IASetIndexBuffer(modelD3D->getSubsetD3Ds().at(0)->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
	managementFX_->setLayout(devcon, LAYOUTID_POS_NORM_TEX_TAN_SKINNED);
	
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(modelD3D->getSubsetD3Ds().at(0)->getNumIndices(), 0, 0);

	managementFX_->setShader(devcon, SHADERID_VS_DEFAULT);
	managementFX_->setShader(devcon, SHADERID_PS_DEFAULT);
}

void Renderer::renderAnimatedMesh(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	DirectX::XMFLOAT4X4 worldMatrix(0.01f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.01f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.01f, 0.0f,
									0.0f, 2.3f, 0.0f, 1.0f);
	DirectX::XMFLOAT4X4 worldMatrixInverse	= worldMatrix;
	DirectX::XMFLOAT4X4 finalMatrix			= managementMath_->calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
	
	managementCB_->setCB(CB_TYPE_OBJECT, TypeFX_VS, CB_REGISTER_OBJECT, devcon);
	managementCB_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);
	
	animatedMesh_->update(0.002f);
	std::vector<DirectX::XMFLOAT4X4> finalTransforms;
	animatedMesh_->getSkinInfo()->getFinalTransforms("Take1", animatedMesh_->getTimePosition(), &finalTransforms);

	managementCB_->setCB(CB_TYPE_BONE, TypeFX_VS, CB_REGISTER_BONE, devcon);
	managementCB_->updateCBBone(devcon, finalTransforms);

	managementFX_->setShader(devcon, SHADERID_VS_ANIMATION);
	managementFX_->setShader(devcon, SHADERID_PS_ANIMATION);

	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	managementGBuffer_->setGBuffersAndDepthBuffer(devcon, managementD3D_->getDepthBuffer());

	managementD3D_->clearDepthBuffer();

	ID3D11Buffer* vertexBuffer = animatedMesh_->getVertexBuffer();
	UINT stride = sizeof(VertexPosNormTexTanSkinned);
	UINT offset = 0;
	devcon->IASetVertexBuffers(
				0, 
				1, 
				&vertexBuffer, 
				&stride, 
				&offset);
	devcon->IASetIndexBuffer(animatedMesh_->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
	managementFX_->setLayout(devcon, LAYOUTID_POS_NORM_TEX_TAN_SKINNED);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(animatedMesh_->getNumIndices(), 0, 0);

	managementGBuffer_->unsetGBuffersAndDepthBufferAsRenderTargets(devcon);

	managementFX_->unsetAll(devcon);

	devcon->PSSetSamplers(0, 0, nullptr);
	devcon->IASetInputLayout(nullptr);
	devcon->RSSetState(nullptr);
}

void Renderer::loadTextures(TexDesc* texdesc)
{
	managementTex_->handleTexDesc(texdesc, managementD3D_->getDevice());
}
void Renderer::addAnimation(SkinnedData* skinnedData)
{
	managementAnimation_->addAnimation(skinnedData);
}


void Renderer::initDebugAnimation()
{
	m3dLoader_ = new M3DLoader();

	animatedMesh_ = nullptr;
	animatedMesh_ = new AnimatedMesh();
	
	m3dLoader_->loadM3D("../../xkill-resources/xkill-models/soldier.m3d",
	animatedMesh_->getVertices(),
	animatedMesh_->getIndices(),
	animatedMesh_->getSubsets(),
	animatedMesh_->getMaterials(),
	animatedMesh_->getSkinInfo());
	
	animatedMesh_->init(managementD3D_->getDevice());
}