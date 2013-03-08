#include <d3d11.h>
#include <DirectXMath.h>

#include <xkill-utilities/Util.h>

#include "ManagementLight.h"
#include "renderingUtilities.h"

ATTRIBUTES_DECLARE_ALL;

ManagementLight::ManagementLight()
{
	ATTRIBUTES_INIT_ALL;

	streamDirLight_		= nullptr;
	streamPointLight_	= nullptr;
	streamPosLight_		= nullptr;
}
ManagementLight::~ManagementLight()
{
	SAFE_DELETE(streamDirLight_);
	SAFE_DELETE(streamPointLight_);
	SAFE_DELETE(streamPosLight_);
}

void ManagementLight::init()
{
	streamDirLight_ = new DataStreamSRV<LightDescDir>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
	streamPointLight_ = new DataStreamSRV<LightDescPoint>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
	streamPosLight_ = new DataStreamSRV<Float3>(
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
}

void ManagementLight::reset()
{
	SAFE_DELETE(streamDirLight_);
	SAFE_DELETE(streamPointLight_);
	SAFE_DELETE(streamPosLight_);

	streamDirLight_		= nullptr;
	streamPointLight_	= nullptr;
	streamPosLight_		= nullptr;
}

void ManagementLight::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	//Reset counters.
	streamDirLight_		->resetStream();
	streamPointLight_	->resetStream();
	streamPosLight_		->resetStream();

	//Send new data into streams.
	updateStreamDirLight();
	updateStreamPointLight();
	//updateStreamPosLight();

	//Map updated streams onto buffers.
	streamDirLight_		->updateDataStream(device, devcon);
	streamPointLight_	->updateDataStream(device, devcon);
	streamPosLight_		->updateDataStream(device, devcon);
}

void ManagementLight::setLightSRVCS(
	ID3D11DeviceContext*	devcon, 
	LightBufferType			bufferType, 
	unsigned int			shaderRegister)
{
	switch (bufferType)
	{
	case LIGHTBUFFERTYPE_DIR:
		streamDirLight_->setStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_POINT:
		streamPointLight_->setStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_POS_VIEW:
		streamPosLight_->setStreamSRV(devcon, shaderRegister);
		break;
	}
}
void ManagementLight::unsetLightSRVCS(
	ID3D11DeviceContext*	devcon, 
	LightBufferType			bufferType, 
	unsigned int			shaderRegister)
{
	switch (bufferType)
	{
	case LIGHTBUFFERTYPE_DIR:
		streamDirLight_->unsetStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_POINT:
		streamPointLight_->unsetStreamSRV(devcon, shaderRegister);
		break;
	case LIGHTBUFFERTYPE_POS_VIEW:
		streamPosLight_->unsetStreamSRV(devcon, shaderRegister);
		break;
	}
}

void ManagementLight::updateStreamDirLight()
{
	AttributePtr<Attribute_Light_Dir> ptr_lightDir;
	while(itrLightDir.hasNext())
	{
		ptr_lightDir = itrLightDir.getNext();
		streamDirLight_->pushData(ptr_lightDir->lightDir);
	}
}
void ManagementLight::updateStreamPointLight()
{
	AttributePtr<Attribute_Light_Point>	ptr_lightPoint;
	AttributePtr<Attribute_Position>	ptr_position;
	while(itrLightPoint.hasNext())
	{
		ptr_lightPoint = itrLightPoint.getNext();
		ptr_position = ptr_lightPoint->ptr_position;

		streamPointLight_->pushData(ptr_lightPoint->lightPoint);
		streamPosLight_->pushData(ptr_position->position);
	}
}

unsigned int ManagementLight::getLightDirCurCount()
{
	return streamDirLight_->getDataCountCur();
}
unsigned int ManagementLight::getLightPointCurCount()
{
	return streamPointLight_->getDataCountCur();
}