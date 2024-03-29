// EngineDev_GDD379.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "EngineDev_GDD379.h"
#include "Mush_Graphics.h"

#define MAX_LOADSTRING 100
Mush_Graphics Graph;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ENGINEDEV_GDD379, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

#ifndef NDEBUG
	AllocConsole();
	FILE* new_std_in_out;
	freopen_s(&new_std_in_out, "CONOUT$", "w", stdout);
	freopen_s(&new_std_in_out, "CONIN$", "r", stdin);
	std::cout << "Hello world!\n";
#endif

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ENGINEDEV_GDD379));
	
	Graph.SetPipeline();

	// Main application loop:
	while (true)
	{
		// Process all messages, stop on WM_QUIT
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// WM_QUIT does not need to be
			// translated or dispatched
			if (msg.message == WM_QUIT)
				break;
			// Translates messages and sends them
			// to WndProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// In the future, do per frame/tick updates here...
			Graph.Update();
			Graph.Render();
		}
	}

#ifndef NDEBUG
	FreeConsole();
#endif 

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENGINEDEV_GDD379));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   //////////////////////////////////////////////////////////////rrrrrr
   Graph.CreateDeviceSwapChain(hWnd);
   Graph.Init();
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/*int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;*/

	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case (WM_KEYDOWN) :
		switch (wParam)
		{
			case VK_A: Mush_Graphics::UpdateKeyboardInput(VK_A, true); break;
			case VK_W: Mush_Graphics::UpdateKeyboardInput(VK_W, true); break;
			case VK_S: Mush_Graphics::UpdateKeyboardInput(VK_S, true); break;
			case VK_D: Mush_Graphics::UpdateKeyboardInput(VK_D, true); break;
			case VK_E: Mush_Graphics::UpdateKeyboardInput(VK_E, true); break;
			case VK_Q: Mush_Graphics::UpdateKeyboardInput(VK_Q, true); break;
			//case VK_T: Mush_Graphics::UpdateKeyboardInput(VK_T, true, true); break;
			case VK_CONTROL: Mush_Graphics::UpdateKeyboardInput(VK_CONTROL, true, true); break;
			case VK_NUMPAD7: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD7, true, true); break;
			case VK_NUMPAD9: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD9, true, true); break;
			case VK_NUMPAD1: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD1, true, true); break;
			case VK_NUMPAD2: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD2, true); break;
			case VK_NUMPAD4: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD4, true); break;
			case VK_NUMPAD6: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD6, true); break;
			case VK_NUMPAD8: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD8, true); break;
			case VK_RIGHT: Mush_Graphics::UpdateKeyboardInput(VK_RIGHT, true); break;
			case VK_LEFT: Mush_Graphics::UpdateKeyboardInput(VK_LEFT, true); break;
			case VK_UP: Mush_Graphics::UpdateKeyboardInput(VK_UP, true); break;
			case VK_DOWN: Mush_Graphics::UpdateKeyboardInput(VK_DOWN, true); break;
			default:	break;
		}
		break;

	case (WM_KEYUP) :
		switch (wParam)
		{
			case VK_A: Mush_Graphics::UpdateKeyboardInput(VK_A, false); break;
			case VK_W: Mush_Graphics::UpdateKeyboardInput(VK_W, false); break;
			case VK_S: Mush_Graphics::UpdateKeyboardInput(VK_S, false); break;
			case VK_D: Mush_Graphics::UpdateKeyboardInput(VK_D, false); break;
			case VK_E: Mush_Graphics::UpdateKeyboardInput(VK_E, false); break;
			case VK_Q: Mush_Graphics::UpdateKeyboardInput(VK_Q, false); break;
			//case VK_T: Mush_Graphics::UpdateKeyboardInput(VK_T, false, true); break;
			case VK_CONTROL: Mush_Graphics::UpdateKeyboardInput(VK_CONTROL, false, true); break;
			case VK_NUMPAD7: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD7, false, true); break;
			case VK_NUMPAD9: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD9, false, true); break;
			case VK_NUMPAD1: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD1, false, true); break;
			case VK_NUMPAD2: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD2, false); break;
			case VK_NUMPAD4: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD4, false); break;
			case VK_NUMPAD6: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD6, false); break;
			case VK_NUMPAD8: Mush_Graphics::UpdateKeyboardInput(VK_NUMPAD8, false); break;
			case VK_RIGHT: Mush_Graphics::UpdateKeyboardInput(VK_RIGHT, false); break;
			case VK_LEFT: Mush_Graphics::UpdateKeyboardInput(VK_LEFT, false); break;
			case VK_UP: Mush_Graphics::UpdateKeyboardInput(VK_UP, false); break;
			case VK_DOWN: Mush_Graphics::UpdateKeyboardInput(VK_DOWN, false); break;
			default:		break;
		}
		break;
	case (WM_MOUSEMOVE) : 
		
		break;
		
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
