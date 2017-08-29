#pragma once
#include "stdafx.h"
#include "SharedDefines.h"


class Mush_Graphics
{
private:
	// Matrices
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_Projection;
	XMFLOAT4X4 m_CubeWorld;

	XMFLOAT4X4 Spinny;

	// Vectors
	XMFLOAT3 newCamOffset;
	XMFLOAT3 Tracker_Up;
	XMFLOAT3 Tracker_Pos;
	XMFLOAT3 Tracker_Tgt;

	// Buffers
	ID3D11Buffer *vb_Cube;
	ID3D11Buffer *ib_Cube;
	ID3D11Buffer *cBuff_perspective;
	ID3D11Buffer *cBuff_lighting;

	// Layouts
	ID3D11InputLayout *iLay_perspective;

	// Shaders
	ID3D11VertexShader *VertSha_perspective;
	ID3D11PixelShader *PixSha_perspective;

	// textures
	ID3D11Texture2D *tx_UVMap;

	// Views
	ID3D11RenderTargetView *iRenderTarget;
	ID3D11ShaderResourceView *iShaderView;
	ID3D11DepthStencilView *iDepthStencilView;

	// Pending...
	ID3D11SamplerState *SampleState;

	ID3D11Device *iDevice;
	ID3D11DeviceContext *iDeviceContext;
	D3D11_VIEWPORT viewPort;
	IDXGISwapChain *swapChain;

public:
	Mush_Graphics();
	~Mush_Graphics();

	struct pipeline_state_t
	{
		ID3D11InputLayout *input_layout;
		ID3D11VertexShader *vertex_shader;
		ID3D11PixelShader * pixel_shader;
		ID3D11RenderTargetView *render_target;
		ID3D11Texture2D *depthStencilBuffer;
		ID3D11DepthStencilState *depthStencilState;
		ID3D11DepthStencilView *depthStencilView;
		ID3D11RasterizerState *rasterState;
	}default_pipeline;

	void InitViewport(D3D11_VIEWPORT &_viewport, FLOAT _w, FLOAT _h,
		FLOAT TopLeftX = 0, FLOAT TopLeftY = 0,
		FLOAT _minDepth = 0, FLOAT MaxDepth = 1);

	void SetPipeline(pipeline_state_t *_pipe);
	void CreateDefaultCube(ID3D11Buffer **_vertBuffer);

	struct VERTEX_Pos_Col{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

};

