#include <DirectXMath.h>

#include "BoneAnimation.h"

BoneAnimation::BoneAnimation()
{
}

BoneAnimation::~BoneAnimation()
{
}

void BoneAnimation::interpolate(float time, DirectX::XMFLOAT4X4& matrix) const
{
	if(time <= keyframes_.front().timePosition)
		keyframeFirst(matrix);
	else if(time >= keyframes_.back().timePosition)
		keyframeLast(matrix);
	else
		keyframeInterpolate(time, matrix);
}

void BoneAnimation::keyframeFirst(DirectX::XMFLOAT4X4& matrix) const
{
	DirectX::XMVECTOR scale					= DirectX::XMLoadFloat3(&keyframes_.front().scale);
	DirectX::XMVECTOR translation			= DirectX::XMLoadFloat3(&keyframes_.front().translation);
	DirectX::XMVECTOR rotationQuaternion	= DirectX::XMLoadFloat4(&keyframes_.front().rotationQuaternion);

	DirectX::XMVECTOR zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(scale, zero, rotationQuaternion, translation));
}

void BoneAnimation::keyframeLast(DirectX::XMFLOAT4X4& matrix) const
{
	DirectX::XMVECTOR scale					= DirectX::XMLoadFloat3(&keyframes_.back().scale);
	DirectX::XMVECTOR translation			= DirectX::XMLoadFloat3(&keyframes_.back().translation);
	DirectX::XMVECTOR rotationQuaternion	= DirectX::XMLoadFloat4(&keyframes_.back().rotationQuaternion);

	DirectX::XMVECTOR zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(scale, zero, rotationQuaternion, translation));
}

void BoneAnimation::keyframeInterpolate(float time, DirectX::XMFLOAT4X4& matrix) const
{
	bool done			= false;
	unsigned int index	= 0;

	while(index < keyframes_.size() && !done)
	{
		if(time >= keyframes_[index].timePosition && time <= keyframes_[index+1].timePosition)
		{
			float numerator		= (time - keyframes_[index].timePosition);
			float denominator	= (keyframes_[index+1].timePosition - keyframes_[index].timePosition);
			float lerpPercent	= numerator/denominator; 

			DirectX::XMVECTOR scale0 = DirectX::XMLoadFloat3(&keyframes_[index].scale);
			DirectX::XMVECTOR scale1 = DirectX::XMLoadFloat3(&keyframes_[index+1].scale);

			DirectX::XMVECTOR translation0 = DirectX::XMLoadFloat3(&keyframes_[index].translation);
			DirectX::XMVECTOR translation1 = DirectX::XMLoadFloat3(&keyframes_[index+1].translation);
			
			DirectX::XMVECTOR rotationQuaternion0 = DirectX::XMLoadFloat4(&keyframes_[index].rotationQuaternion);
			DirectX::XMVECTOR rotationQuaternion1 = DirectX::XMLoadFloat4(&keyframes_[index+1].rotationQuaternion);

			DirectX::XMVECTOR scale				 = DirectX::XMVectorLerp(scale0, scale1, lerpPercent);
			DirectX::XMVECTOR translation		 = DirectX::XMVectorLerp(translation0, translation1, lerpPercent);
			DirectX::XMVECTOR rotationQuaternion = DirectX::XMVectorLerp(rotationQuaternion0, rotationQuaternion1, lerpPercent);

			DirectX::XMVECTOR zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(scale, zero, rotationQuaternion, translation));

			done = true;
		}
		index++;
	}
}

float BoneAnimation::getStartTime() const
{
	return keyframes_.front().timePosition;
}

float BoneAnimation::getEndTime() const
{
	return keyframes_.back().timePosition;
}