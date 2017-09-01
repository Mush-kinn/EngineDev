#include "ShaderDefines.hlsli"

PS_DefaultInput main(VERTEX_PosCol input)
{
	PS_DefaultInput output;
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	
	output.pos = pos;
	output.col = input.col;
	return output;
}