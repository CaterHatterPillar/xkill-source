#ifndef XKILL_RENDERER_UTILNORMALMAP_HLSL
#define XKILL_RENDERER_UTILNORMALMAP_HLSL

float3 NormalSampleToWorldSpace(
	float3 normalMapSample,
	float3 unitNormalW,
	float3 tangentW)
{
	//Uncompress each component from [0, 1] to [-1, 1].
	float3 normalT = normalMapSample * 2.0f - 1.0f;
	
	//Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	//Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);
	return bumpedNormalW;
}

#endif //XKILL_RENDERER_UTILNORMALMAP_HLSL