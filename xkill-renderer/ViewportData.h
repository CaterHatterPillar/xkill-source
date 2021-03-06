#ifndef XKILL_RENDERER_VIEWPORTDATA_H
#define XKILL_RENDERER_VIEWPORTDATA_H

#include <DirectXMath.h>

struct ViewportData
{
	unsigned int camIndex; 

	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 viewInv;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMFLOAT4X4 projInv;
	DirectX::XMFLOAT3	eyePos;
	unsigned int		viewportTopX;
	unsigned int		viewportTopY;
	float		zNear;
	float		zFar;
	float		viewportWidth;
	float		viewportHeight;
};

#endif //XKILL_RENDERER_VIEWPORTDATA_H