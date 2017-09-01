#include "ShaderDefines.hlsli"
float4 main(PS_DefaultInput input) : SV_TARGET
{
	float4 temp;
	temp = input.col;
	return temp;
}