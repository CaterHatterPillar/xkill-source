#ifndef XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL
#define XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL

#include "CS_Blur_Bilateral.hlsl"

[numthreads(N, 1, 1)]
void CS_Blur_Bilateral_Horz(
	int3	threadIDDispatch	: SV_DispatchThreadID,
	int3	threadIDBlock		: SV_GroupThreadID,
	uint	threadIDBlockIndex	: SV_GroupIndex)
{
	if(threadIDBlockIndex == 0)
	{
		blurKernel = ExtractBlurKernel();
	}

	const int2 dispatch = threadIDDispatch.xy + int2(viewportTopX, viewportTopY);
	
	//Load texel, depth and view-space normal into shared memory to reduce memory bandwidth:
	int2 xy = min(dispatch, toBlur.Length.xy - 1); //Clamp out of bound samples that occur at image borders.
	CacheData(
		threadIDBlock.x + blurRadius,
		xy);
	
	//Have some pixels read an additional texel into shared memory to make up for blurRadius:
	if(threadIDBlock.x < blurRadius)
	{
		xy = int2(
			max(dispatch.x - blurRadius, 0), //Clamp out of bound samples that occur at image borders. 
			dispatch.y);
		CacheData(
			threadIDBlock.x,
			xy);
	}
	if(threadIDBlock.x >= N - blurRadius)
	{
		xy = int2(
			min(dispatch.x + blurRadius, toBlur.Length.x - 1), //Clamp out of bound samples that occur at image borders.
			dispatch.y);
		CacheData(
			threadIDBlock.x + 2 * blurRadius,
			xy);
	}
	GroupMemoryBarrierWithGroupSync();

	const int		center			= threadIDBlock.x + blurRadius;
	const float3	centerNormal	= sharedNormV[center];
	const float		centerDepth		= sharedDepth[center];

	//Center value always contributes to the sum.
	float	totalWeight	= blurKernel._[5];
	float4	blur		= blurKernel._[5] * sharedCache[center];
	for(int i = -blurRadius; i <= blurRadius; i++)
	{
		if(i == 0)
			continue; //ugly

		int	n = threadIDBlock.x + blurRadius + i;
		if(
			dot(sharedNormV[n], centerNormal)	>= 0.8f && 
			abs(sharedDepth[n] - centerDepth)	<= 0.2f)
		{
			float weight = blurKernel._[i + blurRadius];
			blur		+= weight * sharedCache[n];
			totalWeight	+= weight;
		}
	}

	//Compensate discarded samples by making total weights sum to 1.
	blur /= totalWeight;
	blurred[uint2(
		threadIDDispatch.x + viewportTopX,
		threadIDDispatch.y + viewportTopY)] = blur;
}

#endif //XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL