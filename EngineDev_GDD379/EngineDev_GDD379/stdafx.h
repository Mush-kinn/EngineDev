// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <fstream>
#include <d3d11.h>


// TODO: reference additional headers your program requires here
#include <iostream>
#pragma comment(lib,"d3d11.lib")
#include "AlphaDefines.h"
#include <DirectXMath.h>
using namespace DirectX;
