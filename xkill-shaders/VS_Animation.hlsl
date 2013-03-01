
#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

DefaultVSOut VS_Animation(VSInPosNormTexTanSkinned vsIn)
{
	DefaultVSOut output;

	float weights[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	weights[0] = vsIn.weights.x;
	weights[1] = vsIn.weights.y;
	weights[2] = vsIn.weights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 position = float3(0.0f, 0.0f, 0.0f);
	float3 normal	= float3(0.0f, 0.0f, 0.0f);

	float4x4 boneTransform = 0;

	for(int i=0; i<4; i++)
	{
		boneTransform += boneTransforms[vsIn.boneIndices[i]] * weights[i];

		//position	+= weights[i] * mul(float4(vsIn.position, 1.0f), boneTransforms[vsIn.boneIndices[i]]).xyz;
		//normal	+= weights[i] * mul(vsIn.normal, (float3x3)boneTransforms[vsIn.boneIndices[i]]);
	}

	position = vsIn.position;
	normal = vsIn.normal;
	
	position = mul(position, boneTransform).xyz;

	output.position		= mul(float4(position, 1.0f), finalMatrix);
	output.positionW	= mul(float4(position, 1.0f), worldMatrix).xyz;
	output.normalW		= mul(float4(normal, 0.0f), worldMatrix).xyz;
	output.texcoord		= vsIn.texcoord;
	return output;
}