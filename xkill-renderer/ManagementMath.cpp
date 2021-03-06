
#include <DirectXMath.h>

#include <xkill-utilities/AttributeType.h>

#include "ManagementMath.h"

ManagementMath::ManagementMath()
{
	identityMatrix_ = DirectX::XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
ManagementMath::~ManagementMath()
{
	//Do nothing.
}

DirectX::XMFLOAT4X4 ManagementMath::calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix)
{
	DirectX::CXMMATRIX	cxmMatrix		= DirectX::XMLoadFloat4x4(&matrix);
	DirectX::XMVECTOR	vDeterminant	= DirectX::XMMatrixDeterminant(cxmMatrix);
	DirectX::XMMATRIX	xmMatrixInverse = DirectX::XMMatrixInverse(&vDeterminant, cxmMatrix);
	
	DirectX::XMFLOAT4X4 matrixInverse;
	DirectX::XMStoreFloat4x4(&matrixInverse, xmMatrixInverse);
	
	return matrixInverse;
}
DirectX::XMFLOAT4X4 ManagementMath::calculateWorldMatrix(
	AttributePtr<Attribute_Spatial> ptr_spatial, 
	AttributePtr<Attribute_Position> ptr_position)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(ptr_position->position.x,
																 ptr_position->position.y,
																 ptr_position->position.z);

	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(ptr_spatial->scale.x,
														 ptr_spatial->scale.y,
														 ptr_spatial->scale.z);

	DirectX::XMFLOAT4 fRotation = DirectX::XMFLOAT4(ptr_spatial->rotation.x,
													ptr_spatial->rotation.y,
													ptr_spatial->rotation.z,
													ptr_spatial->rotation.w);

	DirectX::XMVECTOR qRotation = DirectX::XMLoadFloat4(&fRotation);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(qRotation);

	DirectX::XMMATRIX mWorldMatrix = scaling * rotation * translation;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMStoreFloat4x4(&worldMatrix, mWorldMatrix);

	return worldMatrix;
}
DirectX::XMFLOAT4X4 ManagementMath::calculateFinalMatrix(
	DirectX::XMFLOAT4X4 worldMatrix, 
	DirectX::XMFLOAT4X4 viewMatrix, 
	DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::CXMMATRIX mView		= DirectX::XMLoadFloat4x4(&viewMatrix);
	DirectX::CXMMATRIX mProjection	= DirectX::XMLoadFloat4x4(&projectionMatrix);
	DirectX::CXMMATRIX mWorld		= DirectX::XMLoadFloat4x4(&worldMatrix);

	DirectX::XMMATRIX mFinalMatrix = mWorld * mView * mProjection;
	
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMStoreFloat4x4(&finalMatrix, mFinalMatrix);

	return finalMatrix;
}

float ManagementMath::getRandom()
{
	return (float)(rand()) / (float)RAND_MAX;
}
float ManagementMath::getRandom(float min, float max)
{
	return min + getRandom() * (max - min);
}