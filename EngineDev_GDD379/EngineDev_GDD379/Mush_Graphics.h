#pragma once
#include "stdafx.h"
#include "SharedDefines.h"
#include "XTime.h"

#define BACKBUFFER_WIDTH	1280.0f
#define BACKBUFFER_HEIGHT	720.0f

enum MouseAccess{ OPEN, CLOSED };
enum MouseStatus{ LOCKED, FREE };

class Mush_Graphics
{
private:

	float speed = 4;
	float turn;
	float xR, yR, zR;

	HWND m_window;	
	HWND m_desktop;
	RECT m_screen;
	tagPOINT m_screenCenter;

	static tagPOINTS CurrMouse;
	tagPOINTS PrevMouse;

	XTime m_timeX;
	static bool mahKeys[256];
	static std::vector<UINT> KeyStateON;
	static std::vector<UINT> KeyStateOFF;
	MouseStatus MStatus = MouseStatus::FREE;

	// Matrices
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_Projection;
	XMFLOAT4X4 m_CubeWorld;
	XMFLOAT4X4 m_Spinny;

	// Vectors
	XMFLOAT3 m_newCamOffset;
	XMFLOAT3 m_Tracker_Up;
	XMFLOAT3 m_Tracker_Pos;
	XMFLOAT3 m_Tracker_Tgt;

	// Buffers
	ID3D11Buffer *m_vb_Cube;
	ID3D11Buffer *m_ib_Cube;
	ID3D11Buffer *m_cBuff_perspective;
	ID3D11Buffer *m_cBuff_lighting;

	// Layouts
	ID3D11InputLayout *m_Lay_perspective;

	// Shaders
	ID3D11VertexShader *m_VS_perspective;
	ID3D11PixelShader *m_PS_perspective;

	// textures
	ID3D11Texture2D *m_tx_UVMap;

	// Views
	ID3D11RenderTargetView *m_iRenderTarget;
	ID3D11ShaderResourceView *m_iShaderView;
	ID3D11DepthStencilView *m_iDepthStencilView;

	// Pending...
	ID3D11SamplerState *m_SampleState;

	ID3D11Device *m_iDevice;
	ID3D11DeviceContext *m_iDeviceContext;
	D3D11_VIEWPORT m_viewPort;
	IDXGISwapChain *m_swapChain;

	cbMirror_Perspective toshader_Default;

public:
	static void UpdateKeyboardInput(UINT _key, bool _state, bool _toggle = false);
	static void UpdateMouseInput(tagPOINTS _point);
	static MouseAccess mAccess;

	Mush_Graphics();
	~Mush_Graphics();
	void Init();
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

	struct VERTEX_PosCol{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

	ID3D11Debug *Debuger;

	void CreateDeviceSwapChain(HWND &_window);

	void InitViewport(D3D11_VIEWPORT &_viewport, FLOAT _w, FLOAT _h,
		FLOAT TopLeftX = 0, FLOAT TopLeftY = 0,
		FLOAT _minDepth = 0, FLOAT _maxDepth = 1);

	void SetPipeline();
	void SetPipeline(pipeline_state_t *_pipe);
	void CreateDefaultCube(ID3D11Buffer **_vertBuffer);
	void SetDepthStencilBuffer(ID3D11Texture2D ** _buffer);
	void SetDepthStencilState(ID3D11DepthStencilState **_state);
	void SetDepthStencilView(ID3D11DepthStencilView **_view, ID3D11Texture2D *_resource);
	void SetRasterizerState(ID3D11RasterizerState **_rasterstate, ID3D11RenderTargetView **_RTV);
	void SetShaderInputlayout(ID3D11VertexShader **_vs, ID3D11PixelShader **_ps, ID3D11InputLayout **_input);
	BOOL LoadCompiledShaderData(char **byteCode, size_t &byteCodeSize, const char *fileName);
	void ReleasePipeline(pipeline_state_t *_pipe);

	bool Render();
	bool Update();

	private:
		void MushLookAt(const XMFLOAT4 &_view, const XMFLOAT4 &_target, XMMATRIX &_out);
		void MushTurnTo(const XMMATRIX &_view, const XMVECTOR _target, int _turn, XMMATRIX &_out);

};

