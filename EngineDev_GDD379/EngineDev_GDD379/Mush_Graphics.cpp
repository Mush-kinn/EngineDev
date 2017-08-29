#include "stdafx.h"
#include "Mush_Graphics.h"


Mush_Graphics::Mush_Graphics()
{

}


Mush_Graphics::~Mush_Graphics()
{
}

void Mush_Graphics::CreateDefaultCube(ID3D11Buffer **_vertBuffer){

	XMFLOAT3 verts[8] = {
			{ XMFLOAT3(-0.5f, 0.5f, -0.5f) }, { XMFLOAT3(0.5f, 0.5f, -0.5f) },
			{ XMFLOAT3(-0.5f, -0.5f, -0.5f) }, { XMFLOAT3(0.5f, -0.5f, -0.5f) },
			{ XMFLOAT3(-0.5f, 0.5f, 0.5f) }, { XMFLOAT3(0.5f, 0.5f, 0.5f) },
			{ XMFLOAT3(-0.5f, -0.5f, 0.5f) }, { XMFLOAT3(0.5f, -0.5f, 0.5f) }
	};

	XMFLOAT4 colors[6] = {
			{ XMFLOAT4(1, 0, 0, 1) }, { XMFLOAT4(0, 1, 0, 1) }, { XMFLOAT4(0, 0, 1, 1) },
			{ XMFLOAT4(1, 0, 1, 1) }, { XMFLOAT4(1, 1, 0, 1) }, { XMFLOAT4(0, 1, 1, 1) }
	};


	VERTEX_Pos_Col newCube[36];
	// ft 0-1-2  1-3-2,  [2]
	newCube[0] = { verts[0], colors[2] };
	newCube[1] = { verts[1], colors[2] };
	newCube[2] = { verts[2], colors[2] };

	newCube[3] = { verts[1], colors[2] };
	newCube[4] = { verts[3], colors[2] };
	newCube[5] = { verts[2], colors[2] };

	// top  4-5-0 5-1-0, [1]
	newCube[6] = { verts[4], colors[1] };
	newCube[7] = { verts[5], colors[1] };
	newCube[8] = { verts[0], colors[1] };

	newCube[9] = { verts[5], colors[1] };
	newCube[10] = { verts[1], colors[1] };
	newCube[11] = { verts[0], colors[1] };

	//rt 1-5-3  5-7-3  [0]
	newCube[12] = { verts[1], colors[0] };
	newCube[13] = { verts[5], colors[0] };
	newCube[14] = { verts[3], colors[0] };

	newCube[15] = { verts[5], colors[0] };
	newCube[16] = { verts[7], colors[0] };
	newCube[17] = { verts[3], colors[0] };

	//lf 4-0-6  0-2-6 [6]
	newCube[18] = { verts[4], colors[6] };
	newCube[19] = { verts[0], colors[6] };
	newCube[20] = { verts[6], colors[6] };

	newCube[21] = { verts[0], colors[6] };
	newCube[22] = { verts[2], colors[6] };
	newCube[23] = { verts[6], colors[6] };

	//bk 5-4-7  4-6-7  [5]
	newCube[24] = { verts[5], colors[5] };
	newCube[25] = { verts[4], colors[5] };
	newCube[26] = { verts[7], colors[5] };

	newCube[27] = { verts[4], colors[5] };
	newCube[28] = { verts[6], colors[5] };
	newCube[29] = { verts[7], colors[5] };

	//dn 2-3-6 3-7-6  [4]
	newCube[30] = { verts[2], colors[4] };
	newCube[31] = { verts[3], colors[4] };
	newCube[32] = { verts[6], colors[4] };

	newCube[33] = { verts[3], colors[4] };
	newCube[34] = { verts[7], colors[4] };
	newCube[35] = { verts[6], colors[4] };



	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = sizeof(VERTEX_Pos_Col) * 36;
	
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = newCube;

	iDevice->CreateBuffer(&desc, &data, _vertBuffer);

}