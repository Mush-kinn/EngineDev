#include "..\\SharedDefines.h"

struct VERTEX_PosCol{
	float3 pos : SV_POSITION;
	float4 col : COLOR;
};

struct PS_DefaultInput{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};