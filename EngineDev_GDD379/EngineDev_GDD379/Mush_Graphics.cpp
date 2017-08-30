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


	VERTEX_PosCol newCube[36];
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
	desc.ByteWidth = sizeof(VERTEX_PosCol) * 36;
	
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = newCube;

	m_iDevice->CreateBuffer(&desc, &data, _vertBuffer);

}

void Mush_Graphics::CreateDeviceSwapChain(ID3D11Device **_device, IDXGISwapChain **_chain, ID3D11DeviceContext **_cntx, HWND &_window){

	DXGI_SWAP_CHAIN_DESC chainDesc;
	ZeroMemory(&chainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	chainDesc.BufferCount = 1;
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	chainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferDesc.Height = (UINT)BACKBUFFER_HEIGHT;
	chainDesc.BufferDesc.Width = (UINT)BACKBUFFER_WIDTH;
	chainDesc.OutputWindow = _window;
	chainDesc.SampleDesc.Count = 1;
	chainDesc.SampleDesc.Quality = 0;
	chainDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_10_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

#if _DEBUG
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
		&FeatureLevelsRequested, numLevelsRequested, D3D11_SDK_VERSION, &chainDesc, &m_swapChain,
		&m_iDevice, &FeatureLevelsSupported, &m_iDeviceContext);
#else
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL,
		&FeatureLevelsRequested, numLevelsRequested, D3D11_SDK_VERSION, &chainDesc, &swapChain,
		&iDevice, &FeatureLevelsSupported, &iDeviceContext);
#endif

	ID3D11Resource *iResource;
	ZeroMemory(&iResource, sizeof(ID3D11Resource));
	m_swapChain->GetBuffer(0, __uuidof(iResource), reinterpret_cast<void**>(&iResource));
	m_iDevice->CreateRenderTargetView(iResource, NULL, &m_iRenderTarget);
	iResource->Release();

}

void Mush_Graphics::SetDepthStencilBuffer(ID3D11Texture2D **_buffer){

	D3D11_TEXTURE2D_DESC Stencil_Resource_desc;
	ZeroMemory(&Stencil_Resource_desc, sizeof(D3D11_TEXTURE2D_DESC));
	Stencil_Resource_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	Stencil_Resource_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	Stencil_Resource_desc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	Stencil_Resource_desc.Height = (UINT)BACKBUFFER_HEIGHT;
	Stencil_Resource_desc.Width = (UINT)BACKBUFFER_WIDTH;
	Stencil_Resource_desc.ArraySize = 1;
	Stencil_Resource_desc.SampleDesc.Count = 1;

	m_iDevice->CreateTexture2D(&Stencil_Resource_desc, NULL, _buffer);
}

void Mush_Graphics::SetDepthStencilState(ID3D11DepthStencilState **_state){

	D3D11_DEPTH_STENCIL_DESC state_desc;
	ZeroMemory(&state_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Depth test parameters
	state_desc.DepthEnable = true;
	state_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	state_desc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	state_desc.StencilEnable = true;
	state_desc.StencilReadMask = 0xFF;
	state_desc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	state_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	state_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	state_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	state_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	state_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	state_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	state_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	state_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	m_iDevice->CreateDepthStencilState(&state_desc, _state);

}

void Mush_Graphics::SetDepthStencilView(ID3D11DepthStencilView **_view, ID3D11Texture2D *_resource){

	D3D11_DEPTH_STENCIL_VIEW_DESC STENCIL_DESC;
	ZeroMemory(&STENCIL_DESC, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	STENCIL_DESC.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	STENCIL_DESC.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	m_iDevice->CreateDepthStencilView(_resource, &STENCIL_DESC, _view);

}

void Mush_Graphics::SetRasterizerState(ID3D11RasterizerState **_rasterstate){

	D3D11_RASTERIZER_DESC raster_desc;
	ZeroMemory(&raster_desc, sizeof(D3D11_RASTERIZER_DESC));
	raster_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	raster_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	raster_desc.DepthClipEnable = true;

	m_iDevice->CreateRasterizerState(&raster_desc, _rasterstate);
}