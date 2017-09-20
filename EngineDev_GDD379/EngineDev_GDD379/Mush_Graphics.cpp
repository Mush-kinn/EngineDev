#include "stdafx.h"
#include "Mush_Graphics.h"

std::vector<UINT> Mush_Graphics::KeyStateON;
std::vector<UINT> Mush_Graphics::KeyStateOFF;
MouseAccess Mush_Graphics::mAccess;
bool Mush_Graphics::mahKeys[256] = {};
tagPOINTS Mush_Graphics::CurrMouse;

void Mush_Graphics::UpdateKeyboardInput(UINT _key, bool _state, bool _toggle){
	if (_state && _toggle){
		return;
	}
	if (!_toggle){
		if (_state)
			KeyStateON.push_back(_key);
		else
			KeyStateOFF.push_back(_key);
	}
	// Toggle
	else{
		mahKeys[_key] ? KeyStateOFF.push_back(_key) : KeyStateON.push_back(_key);
	}
}

Mush_Graphics::Mush_Graphics()
{ 
	XMStoreFloat4x4(&m_CubeWorld, XMMatrixIdentity());
}


void Mush_Graphics::Init(){

	testing = new Debug_Renderer(m_iDevice, m_iDeviceContext);
	m_Tracker_Up = XMFLOAT3(0, 1, 0);
	m_Tracker_Pos = XMFLOAT3(0, 0, 0);
	m_Tracker_Tgt = XMFLOAT3(0, 0, 0);
	
	m_view = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.5f, 1.0f);

	XMMATRIX rotation_trix, projection;
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	view = XMMatrixTranslation(0, 1, -5);
	view = XMMatrixInverse(nullptr, view);
	toshader_Default.view = XMMatrixTranspose(view);
	XMStoreFloat4x4(&m_view, view);

	// zNear = 0.1;
	// zFar = 10
	// vFOV = 90
	float aspect = BACKBUFFER_WIDTH / BACKBUFFER_HEIGHT;
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), aspect, 0.1f, 100.0f);
	toshader_Default.projection = XMMatrixTranspose(projection);
	XMStoreFloat4x4(&m_Projection, projection);

	D3D11_BUFFER_DESC cb_3d;
	ZeroMemory(&cb_3d, sizeof(D3D11_BUFFER_DESC));
	cb_3d.Usage = D3D11_USAGE_DYNAMIC;
	cb_3d.ByteWidth = sizeof(cbMirror_Perspective);
	cb_3d.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_3d.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	m_iDevice->CreateBuffer(&cb_3d, NULL, &m_cBuff_perspective);

	m_iDevice->QueryInterface(IID_PPV_ARGS(&Debuger));

	// Get a handle to the desktop window
	m_desktop = GetDesktopWindow();
	GetWindowRect(m_desktop, &m_screen);	

	//D3D11_SAMPLER_DESC sampler_desc;
	//ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));
	//sampler_desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampler_desc.MinLOD = 0;
	//sampler_desc.MaxLOD = Test_UV_Map_numlevels;
	//sampler_desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER_EQUAL;

	//iDevice->CreateSamplerState(&sampler_desc, &m_SampleState);

	CreateDefaultCube(&m_vb_Cube);

	turn = 0.04f;
	xR = 1;
	yR = 2;
	zR = 3;
	m_timeX.Throttle(60);
}

void Mush_Graphics::ReleasePipeline(pipeline_state_t *_pipe){
	_pipe->depthStencilBuffer->Release();
	_pipe->depthStencilState->Release();
	_pipe->depthStencilView->Release();
	_pipe->input_layout->Release();
	_pipe->pixel_shader->Release();
	_pipe->rasterState->Release();
	_pipe->render_target->Release();
	_pipe->vertex_shader->Release();
}

Mush_Graphics::~Mush_Graphics()
{
	delete testing;
	OutputDebugStringW(L"\n\n\n <Detailed Dump> \n\n");

	m_iDeviceContext->ClearState();
	m_iDevice->Release();
	m_swapChain->Release();
	m_iDeviceContext->Release();
	
	m_vb_Cube->Release();
	m_cBuff_perspective->Release();

	ReleasePipeline(&default_pipeline);

	Debuger->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	Debuger->Release();
	OutputDebugStringW(L"\n </Detailed Dump> \n\n\n");
}

void Mush_Graphics::CreateDefaultCube(ID3D11Buffer **_vertBuffer){

	XMFLOAT3 verts[8] = {
			{ XMFLOAT3(-0.5f, 0.5f, -0.5f) }, { XMFLOAT3(0.5f, 0.5f, -0.5f) },
			{ XMFLOAT3(-0.5f, -0.5f, -0.5f) }, { XMFLOAT3(0.5f, -0.5f, -0.5f) },
			{ XMFLOAT3(-0.5f, 0.5f, 0.5f) }, { XMFLOAT3(0.5f, 0.5f, 0.5f) },
			{ XMFLOAT3(-0.5f, -0.5f, 0.5f) }, { XMFLOAT3(0.5f, -0.5f, 0.5f) }
	};

	XMFLOAT4 colors[6] = {
			{ XMFLOAT4(1, 0, 0, 1) }, { XMFLOAT4(0, 1, 0, 1) }, { XMFLOAT4(1, 1, 0, 1) },
			{ XMFLOAT4(1, 0, 1, 1) }, { XMFLOAT4(0, 0, 1, 1) }, { XMFLOAT4(0, 1, 1, 1) }
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
	newCube[18] = { verts[4], colors[5] };
	newCube[19] = { verts[0], colors[5] };
	newCube[20] = { verts[6], colors[5] };
									 
	newCube[21] = { verts[0], colors[5] };
	newCube[22] = { verts[2], colors[5] };
	newCube[23] = { verts[6], colors[5] };

	//bk 5-4-7  4-6-7  [5]
	newCube[24] = { verts[5], colors[4] };
	newCube[25] = { verts[4], colors[4] };
	newCube[26] = { verts[7], colors[4] };
									 
	newCube[27] = { verts[4], colors[4] };
	newCube[28] = { verts[6], colors[4] };
	newCube[29] = { verts[7], colors[4] };

	//dn 2-3-6 3-7-6  [4]
	newCube[30] = { verts[2], colors[3] };
	newCube[31] = { verts[3], colors[3] };
	newCube[32] = { verts[6], colors[3] };
									 
	newCube[33] = { verts[3], colors[3] };
	newCube[34] = { verts[7], colors[3] };
	newCube[35] = { verts[6], colors[3] };



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

void Mush_Graphics::CreateDeviceSwapChain(HWND &_window){

	m_window = _window;

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

void Mush_Graphics::SetRasterizerState(ID3D11RasterizerState **_rasterstate, ID3D11RenderTargetView **_RTV){

	D3D11_RASTERIZER_DESC raster_desc;
	ZeroMemory(&raster_desc, sizeof(D3D11_RASTERIZER_DESC));
	raster_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	raster_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	raster_desc.DepthClipEnable = true;

	m_iDevice->CreateRasterizerState(&raster_desc, _rasterstate);

	ID3D11Resource *iResource;
	ZeroMemory(&iResource, sizeof(ID3D11Resource));
	m_swapChain->GetBuffer(0, __uuidof(iResource), reinterpret_cast<void**>(&iResource));
	m_iDevice->CreateRenderTargetView(iResource, NULL, _RTV);
	iResource->Release();

}

void Mush_Graphics::SetShaderInputlayout(ID3D11VertexShader **_vs, ID3D11PixelShader **_ps, ID3D11InputLayout **_input){
	char *vsByte, *psByte;
	size_t vs_t, ps_t;
	
	LoadCompiledShaderData(&vsByte, vs_t, "..\\Debug\\VS_Default.cso");
	LoadCompiledShaderData(&psByte, ps_t, "..\\Debug\\PS_Default.cso");

	m_iDevice->CreateVertexShader(vsByte, vs_t, NULL, _vs);
	m_iDevice->CreatePixelShader(psByte, ps_t, NULL, _ps);

	D3D11_INPUT_ELEMENT_DESC layout[2];
	layout[0] = { "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	layout[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	m_iDevice->CreateInputLayout(layout, 2, vsByte, vs_t, _input);
}

BOOL Mush_Graphics::LoadCompiledShaderData(char **byteCode, size_t &byteCodeSize, const char *fileName){
	std::ifstream load;
	load.open(fileName, std::ios_base::binary);
	if (!load.is_open()) 
		return false;
	load.seekg(0, std::ios_base::end);
	byteCodeSize = size_t(load.tellg());
	*byteCode = new char[byteCodeSize];
	load.seekg(0, std::ios_base::beg);
	load.read(*byteCode, byteCodeSize);
	load.close();
	return true;
}

void Mush_Graphics::SetPipeline(pipeline_state_t *_pipe){
	
	SetDepthStencilBuffer(&_pipe->depthStencilBuffer);
	SetDepthStencilState(&_pipe->depthStencilState);
	SetDepthStencilView(&_pipe->depthStencilView, _pipe->depthStencilBuffer);
	SetShaderInputlayout(&_pipe->vertex_shader, &_pipe->pixel_shader, &_pipe->input_layout);
	SetRasterizerState(&_pipe->rasterState, &_pipe->render_target);
}

void Mush_Graphics::SetPipeline(){

	SetDepthStencilBuffer(&default_pipeline.depthStencilBuffer);
	SetDepthStencilState(&default_pipeline.depthStencilState);
	SetDepthStencilView(&default_pipeline.depthStencilView, default_pipeline.depthStencilBuffer);
	SetShaderInputlayout(&default_pipeline.vertex_shader, &default_pipeline.pixel_shader, &default_pipeline.input_layout);
	SetRasterizerState(&default_pipeline.rasterState, &default_pipeline.render_target);
	InitViewport(m_viewPort, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);
}

void Mush_Graphics::InitViewport(D3D11_VIEWPORT &_viewport, FLOAT _w, FLOAT _h, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT _minDepth, FLOAT _maxDepth) {
	ZeroMemory(&_viewport, sizeof(_viewport));
	_viewport.Height = _h;
	_viewport.Width = _w;
	_viewport.TopLeftX = TopLeftX;
	_viewport.TopLeftY = TopLeftY;
	_viewport.MinDepth = _minDepth;
	_viewport.MaxDepth = _maxDepth;
}

bool Mush_Graphics::Render(){
	
	m_timeX.Throttle(60);

	turn += 0.001f;

	XMMATRIX cube_matrix;
	cube_matrix = XMMatrixRotationRollPitchYaw(turn*xR, turn*yR, turn*zR);;
	toshader_Default.model = cube_matrix;
	toshader_Default.view = XMMatrixTranspose( XMLoadFloat4x4(&m_view));

	UINT _startSlot = 0;
	UINT _numBuffs = 1;
	UINT _strides = 0;
	UINT _offSets = 0;
	_strides = static_cast<UINT>(sizeof(VERTEX_PosCol));
	D3D11_MAPPED_SUBRESOURCE map_cube;
	ZeroMemory(&map_cube, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_iDeviceContext->Map(m_cBuff_perspective, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &map_cube);
	memcpy(map_cube.pData, &toshader_Default, sizeof(toshader_Default));
	m_iDeviceContext->Unmap(m_cBuff_perspective, 0);

	m_iDeviceContext->ClearDepthStencilView(default_pipeline.depthStencilView, D3D11_CLEAR_DEPTH, 1, NULL);


	// IA Stage
	m_iDeviceContext->IASetVertexBuffers(0, 1, &m_vb_Cube, &_strides, &_offSets);
	//m_iDeviceContext->IASetIndexBuffer(ib_Box, DXGI_FORMAT_R32_UINT, 0);
	m_iDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_iDeviceContext->IASetInputLayout(default_pipeline.input_layout);


	// VS Stage
	m_iDeviceContext->VSSetConstantBuffers(0, 1, &m_cBuff_perspective);
	m_iDeviceContext->VSSetShader(default_pipeline.vertex_shader, NULL, NULL);

#if 0
	// GS Stage
	m_iDeviceContext->GSSetShader(GeoSha_Prototype, NULL, NULL);
	m_iDeviceContext->GSSetConstantBuffers(0, 1, &cBuff_perspective);
#endif

	// PS Stage
	//m_iDeviceContext->PSSetConstantBuffers(0, 1, &m_cBuff_perspective);
	//m_iDeviceContext->PSSetConstantBuffers(1, 1, &cBuff_lighting);
	//m_iDeviceContext->PSSetSamplers(0, 1, &SampleState);
	m_iDeviceContext->PSSetShader(default_pipeline.pixel_shader, NULL, NULL);
	//m_iDeviceContext->PSSetShaderResources(0, 1, &ShaderView);
	//m_iDeviceContext->PSSetShaderResources(1, 1, &SkyboxView);

	// RS Stage
	m_iDeviceContext->RSSetState(default_pipeline.rasterState);
	m_iDeviceContext->RSSetViewports(1, &m_viewPort);

	// OM Stage
	m_iDeviceContext->OMSetRenderTargets(1, &default_pipeline.render_target, default_pipeline.depthStencilView );

	// Draw Stage
	FLOAT DarkBlue[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	FLOAT Black[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	m_iDeviceContext->ClearRenderTargetView(default_pipeline.render_target, DarkBlue);

	m_iDeviceContext->Draw(36, 0);

	toshader_Default.model = XMMatrixTranspose( XMLoadFloat4x4(&m_CubeWorld));
	
	ZeroMemory(&map_cube, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_iDeviceContext->Map(m_cBuff_perspective, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &map_cube);
	memcpy(map_cube.pData, &toshader_Default, sizeof(toshader_Default));
	m_iDeviceContext->Unmap(m_cBuff_perspective, 0);
	m_iDeviceContext->Draw(36, 0);


	m_swapChain->Present(0, 0);
	return false;
}

bool Mush_Graphics::Update(){

	m_timeX.Signal();
	mAccess = CLOSED;

	while (!KeyStateON.empty())
	{
		mahKeys[KeyStateON.back()] = true;
		KeyStateON.pop_back();
	}
	while (!KeyStateOFF.empty()){
		mahKeys[KeyStateOFF.back()] = false;
		KeyStateOFF.pop_back();
	}

	// Hide mouse, enable mouse controlled camera movement
	if (MStatus == MouseStatus::FREE && mahKeys[VK_CONTROL]){
		//ShowCursor(false);
		MStatus = MouseStatus::LOCKED;

		tagPOINT temp;
		temp.x = BACKBUFFER_WIDTH * 0.5f;
		temp.y = BACKBUFFER_HEIGHT * 0.5f;
		std::cout << temp.x << " [] Init - Center []" << temp.y << "\n";

		ClientToScreen(m_window, &temp);
		std::cout << temp.x << " [] Init - from Client " << temp.y << "\n";

		SetCursorPos(temp.x, temp.y);
		std::cout << temp.x << " [] Init - set []" << temp.y << "\n";

		PrevMouse.x = temp.x;
		PrevMouse.y = temp.y;
		CurrMouse = PrevMouse;

		std::cout << PrevMouse.x << " [] Init - PrevMouse[] " << PrevMouse.y << "\n";

	}
	// Show mouse, disable mouse controlled camera movement
	if (MStatus == MouseStatus::LOCKED && !mahKeys[VK_CONTROL]){
		//ShowCursor(true);
		MStatus = MouseStatus::FREE;
	}

	float sDelt = (float)m_timeX.SmoothDelta();
	if (mahKeys[VK_A])
		m_newCamOffset.x -= speed * sDelt;
	if (mahKeys[VK_D])
		m_newCamOffset.x += speed * sDelt;
	if (mahKeys[VK_W])
		m_newCamOffset.z += speed * sDelt;
	if (mahKeys[VK_S])
		m_newCamOffset.z -= speed * sDelt;
	if (mahKeys[VK_Q])
		m_newCamOffset.y -= speed * sDelt;
	if (mahKeys[VK_E])
		m_newCamOffset.y += speed * sDelt;

	XMMATRIX temp, INverted;
	XMVECTOR Scale, Rot, Trans;
	
	if (!mahKeys[VK_T]){
		temp = XMMatrixIdentity();

		if (MStatus == LOCKED){

			int dx, dy;

			dx = CurrMouse.x - PrevMouse.x;
			dy = CurrMouse.y - PrevMouse.y;

			std::cout << dx << " []Update - yRotation[] " << dy << "\n";

			temp = XMMatrixRotationY(XMConvertToRadians(dx * sDelt)) *temp;
		}
		else{
			if (mahKeys[VK_NUMPAD4])
				temp = XMMatrixRotationY(XMConvertToRadians(-90 * sDelt)) *temp;
			if (mahKeys[VK_NUMPAD6])
				temp = XMMatrixRotationY(XMConvertToRadians(90 * sDelt)) *temp;
		}

		INverted = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_view));
		XMMatrixDecompose(&Scale, &Rot, &Trans, INverted);
		temp = XMMatrixScalingFromVector(Scale) * XMMatrixRotationQuaternion(Rot) * temp;
		temp = temp * XMMatrixTranslation(0, m_newCamOffset.y, 0) * XMMatrixTranslationFromVector(Trans);

		if (MStatus == LOCKED){
			int dx, dy;
			dx = CurrMouse.x - PrevMouse.x;
			dy = CurrMouse.y - PrevMouse.y;

			std::cout << dx << " []Update - xRotation[] " << dy << "\n";

			temp = XMMatrixRotationX(XMConvertToRadians(dy * sDelt)) *temp;

		}
		else{
			if (mahKeys[VK_NUMPAD8])
				temp = XMMatrixRotationX(XMConvertToRadians(-90 * sDelt)) * temp;
			if (mahKeys[VK_NUMPAD2])
				temp = XMMatrixRotationX(XMConvertToRadians(90 * sDelt)) * temp;
		}
		XMFLOAT4 MAX(1, 1, 1, 1), MIN(-1, 0, -1, 0);
		temp.r[1] = XMVectorClamp(temp.r[1], XMLoadFloat4(&MIN), XMLoadFloat4(&MAX));
		XMStoreFloat4x4(&m_Spinny, temp);

		temp = XMMatrixIdentity();
		temp = temp * XMMatrixTranslation(m_newCamOffset.x, 0, m_newCamOffset.z);
		temp = temp * XMLoadFloat4x4(&m_Spinny);
		XMStoreFloat4x4(&m_view, XMMatrixInverse(NULL, temp));
		//XMStoreFloat4x4(&m_BoxWorld, XMMatrixTranslationFromVector(Trans));
	}
	else{
		XMMATRIX temp = XMMatrixIdentity();
		INverted = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_view));
		XMMatrixDecompose(&Scale, &Rot, &Trans, INverted);
		XMVECTOR aVector = XMVector4Transform(Trans, (temp*XMMatrixTranslation(0, m_newCamOffset.y, 0)));
		XMMatrixDecompose(&Scale, &Rot, &Trans, XMLoadFloat4x4(&m_CubeWorld));
		XMStoreFloat3(&m_Tracker_Tgt, Trans);
		temp = XMMatrixLookAtLH(aVector, XMLoadFloat3(&m_Tracker_Tgt), XMLoadFloat3(&m_Tracker_Up));
		temp = temp * XMMatrixInverse(NULL, XMMatrixTranslation(m_newCamOffset.x, 0, m_newCamOffset.z));
		XMStoreFloat4x4(&m_view, temp);
		//XMStoreFloat4x4(&m_BoxWorld, XMMatrixTranslationFromVector(aVector));

	}


	ZeroMemory(&m_newCamOffset, sizeof(m_newCamOffset));
	// CUBE
	if (!mahKeys[VK_T]){
		temp = XMMatrixIdentity();

		if (mahKeys[VK_LEFT])
			m_newCamOffset.x -= speed * sDelt;
		if (mahKeys[VK_RIGHT])
			m_newCamOffset.x += speed * sDelt;
		if (mahKeys[VK_UP])
			m_newCamOffset.z += speed * sDelt;
		if (mahKeys[VK_DOWN])
			m_newCamOffset.z -= speed * sDelt;
		if (mahKeys[NULL])
			m_newCamOffset.y -= speed * sDelt;
		if (mahKeys[NULL])
			m_newCamOffset.y += speed * sDelt;

		if (MStatus == LOCKED & false){
			int dx, dy;

			dx = CurrMouse.x - PrevMouse.x;
			dy = CurrMouse.y - PrevMouse.y;
			temp = XMMatrixRotationY(XMConvertToRadians(dx * sDelt)) *temp;
			//temp = XMMatrixRotationX(XMConvertToRadians(dy * sDelt)) *temp;
			//int tempx = BACKBUFFER_WIDTH*0.5f;
			//int tempy = BACKBUFFER_HEIGHT*0.5f;
			//SetCursorPos(tempx, tempy);
			//PrevMouse.x = tempx;
			//PrevMouse.y = tempy;
			//CurrMouse = PrevMouse;
		}
		else{
			if (mahKeys[NULL])
				temp = XMMatrixRotationY(XMConvertToRadians(-90 * sDelt)) *temp;
			if (mahKeys[NULL])
				temp = XMMatrixRotationY(XMConvertToRadians(90 * sDelt)) *temp;
		}

		INverted = XMLoadFloat4x4(&m_CubeWorld);
		XMMatrixDecompose(&Scale, &Rot, &Trans, INverted);
		temp = XMMatrixScalingFromVector(Scale) * XMMatrixRotationQuaternion(Rot) * temp;
		temp = temp * XMMatrixTranslation(0, m_newCamOffset.y, 0) * XMMatrixTranslationFromVector(Trans);

		if (MStatus == LOCKED && false){
			float dx, dy;
			dx = CurrMouse.x - PrevMouse.x;
			dy = CurrMouse.y - PrevMouse.y;
			//temp = XMMatrixRotationY(XMConvertToRadians(dx * sDelt)) *temp;
			temp = XMMatrixRotationX(XMConvertToRadians(dy * sDelt)) *temp;
			//int tempx = BACKBUFFER_WIDTH*0.5f;
			//int tempy = BACKBUFFER_HEIGHT*0.5f;
			//SetCursorPos(tempx, tempy);
			//PrevMouse.x = tempx;
			//PrevMouse.y = tempy;
			//CurrMouse = PrevMouse;
		}
		else{
			if (mahKeys[NULL])
				temp = XMMatrixRotationX(XMConvertToRadians(-90 * sDelt)) * temp;
			if (mahKeys[NULL])
				temp = XMMatrixRotationX(XMConvertToRadians(90 * sDelt)) * temp;
		}
		XMFLOAT4 MAX(1, 1, 1, 1), MIN(-1, 0, -1, 0);
		temp.r[1] = XMVectorClamp(temp.r[1], XMLoadFloat4(&MIN), XMLoadFloat4(&MAX));
		XMStoreFloat4x4(&m_Spinny, temp);

		temp = XMMatrixIdentity();
		temp = temp * XMMatrixTranslation(m_newCamOffset.x, 0, m_newCamOffset.z);
		// LOOK-AT
		if (mahKeys[VK_NUMPAD7]){
			XMMATRIX look;
			XMFLOAT4 pos;
			XMStoreFloat4(&pos, XMLoadFloat4x4(&m_Spinny).r[3]);
			MushLookAt(pos, XMFLOAT4(0, 0, 0, 1), look);
			temp = temp * look;
			XMStoreFloat4x4(&m_CubeWorld, temp);
		}
		// TURN-TO
		else if (mahKeys[VK_NUMPAD9]){
			temp = temp * XMLoadFloat4x4(&m_Spinny);
			MushTurnTo(temp, XMLoadFloat4(&XMFLOAT4(0, 0, 0, 1)), sDelt * 0.000001f, temp);
			XMStoreFloat4x4(&m_CubeWorld, temp);
		}
		// Inactive: Free Camera
		else{
			temp = temp * XMLoadFloat4x4(&m_Spinny);
			XMStoreFloat4x4(&m_CubeWorld, temp);
		}
		//XMStoreFloat4x4(&m_BoxWorld, XMMatrixTranslationFromVector(Trans));
	}
	

	if (MStatus == LOCKED){

		tagPOINT temp;
		temp.x = BACKBUFFER_WIDTH *0.5f;
		temp.y = BACKBUFFER_HEIGHT * 0.5f;
		std::cout << temp.x << " [] Reset - Center []" << temp.y << "\n";

		ClientToScreen(m_window, &temp);
		std::cout << temp.x << " [] Reset - from Client " << temp.y << "\n";

		SetCursorPos(temp.x, temp.y);
		std::cout << temp.x << " [] Reset - set []" << temp.y << "\n";

		PrevMouse.x = temp.x;
		PrevMouse.y = temp.y;
		CurrMouse = PrevMouse;
		
		std::cout << PrevMouse.x << " [] Reset - PrevMouse[] " << PrevMouse.y << "\n";
	}

	if (testing != NULL && false){
		VERTEX_PosCol tmp,tmp2;
	}

	mAccess = OPEN;
	ZeroMemory(&m_newCamOffset, sizeof(m_newCamOffset));
	return true;
	
}

void Mush_Graphics::UpdateMouseInput(tagPOINTS _points){
	if (mAccess == OPEN && (CurrMouse.x != _points.x || CurrMouse.y != _points.y)){
		CurrMouse = _points;
		std::cout << _points.x << " []UpdateMouseInput[] " << _points.y << "\n";
	}
}

void Mush_Graphics::MushLookAt(const XMFLOAT4 &_view, const XMFLOAT4 &_target, XMMATRIX &_out){
	XMVECTOR newZ, newX, up;
	newZ = XMLoadFloat3(&XMFLOAT3(_target.x - _view.x, _target.y - _view.y, _target.z - _view.z));
	newZ = XMVector3Normalize(newZ);

	up = XMLoadFloat3(&XMFLOAT3(0, 1, 0));

	newX = XMVector3Cross(up, newZ);
	newX = XMVector3Normalize(newX);

	up = XMVector3Cross(newZ, newX);
	up = XMVector3Normalize(up);

	_out.r[0] = newX;
	_out.r[1] = up;
	_out.r[2] = newZ;
	_out.r[3] = XMLoadFloat4(&_view);

}

void Mush_Graphics::MushTurnTo(const XMMATRIX &_view, const XMVECTOR _target, int _turn, XMMATRIX &_out){
	XMFLOAT4 V, T, W,W2;

	XMVECTOR tempo = XMVectorSubtract(_target, _view.r[3]);
	tempo = XMVector3Normalize(tempo);

	XMStoreFloat4(&V, XMVector3Dot(tempo, _view.r[0]));

	if (V.w > 0.1f){
		_out = XMMatrixRotationY(XMConvertToRadians(turn)) * _view;
	}
	else if (V.w < -0.1f){
		_out = XMMatrixRotationY(XMConvertToRadians(-turn)) * _view;
	}
	else {
		_out = XMMatrixIdentity() * _view;
	}
}

Mush_Graphics::Debug_Renderer::Debug_Renderer(ID3D11Device *_device, ID3D11DeviceContext * _context) 
		: Max_verts(40), 
		debug_device(_device), 
		debug_context(_context) {
	vert_count = 0;
	cpu_buffer = new VERTEX_PosCol[Max_verts];
	
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(VERTEX_PosCol) * Max_verts;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	
	debug_device->CreateBuffer(&desc, NULL, &gpu_buffer);
}

Mush_Graphics::Debug_Renderer::~Debug_Renderer(){
	gpu_buffer->Release();
	delete[] cpu_buffer;
}

void Mush_Graphics::Debug_Renderer::add_line(VERTEX_PosCol a, VERTEX_PosCol b){
	if (vert_count+2 >= Max_verts)
		return;
	cpu_buffer[vert_count++] = a;
	cpu_buffer[vert_count++] = b;
}

void Mush_Graphics::Debug_Renderer::add_line(VERTEX_PosCol a, VERTEX_PosCol b, XMFLOAT4 _color){
	if (vert_count + 2 >= Max_verts)
		return;
	a.color = b.color = _color;
	cpu_buffer[vert_count++] = a;
	cpu_buffer[vert_count++] = b;
}

unsigned int Mush_Graphics::Debug_Renderer::flush(){
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
	debug_context->Map(gpu_buffer, 1, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &map);
	memcpy(&map, &cpu_buffer, sizeof(cpu_buffer));
	debug_context->Unmap(gpu_buffer, NULL);
	auto temp = vert_count;
	vert_count = 0;
	return temp;
}