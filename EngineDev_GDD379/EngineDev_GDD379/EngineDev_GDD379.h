#pragma once

#include "resource.h"

// Stuff 
#if 0
BOOL LoadCompiledShaderData(char **byteCode, size_t &byteCodeSize, const char *fileName)
{
	std::ifstream load;
	load.open(fileName, std::ios_base::binary);
	if (!load.is_open()) return false;
	load.seekg(0, std::ios_base::end);
	byteCodeSize = size_t(load.tellg());
	*byteCode = new char[byteCodeSize];
	load.seekg(0, std::ios_base::beg);
	load.read(*byteCode, byteCodeSize);
	load.close();

	return true;
}

std::vector<UINT> DEMO_APP::KeyStateON;
std::vector<UINT> DEMO_APP::KeyStateOFF;
bool DEMO_APP::mahKeys[256] = {};

void DEMO_APP::UpdateKeyboardInput(UINT _key, bool _state, bool _toggle){
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

case (WM_KEYDOWN) :
	switch (wParam)
{
	case VK_A: DEMO_APP::UpdateKeyboardInput(VK_A, true); break;
	case VK_W: DEMO_APP::UpdateKeyboardInput(VK_W, true); break;
	case VK_S: DEMO_APP::UpdateKeyboardInput(VK_S, true); break;
	case VK_D: DEMO_APP::UpdateKeyboardInput(VK_D, true); break;
	case VK_E: DEMO_APP::UpdateKeyboardInput(VK_E, true); break;
	case VK_Q: DEMO_APP::UpdateKeyboardInput(VK_Q, true); break;
	case VK_T: DEMO_APP::UpdateKeyboardInput(VK_T, true, true); break;
	case VK_CONTROL: DEMO_APP::UpdateKeyboardInput(VK_CONTROL, true, true); break;
	case VK_NUMPAD2: DEMO_APP::UpdateKeyboardInput(VK_NUMPAD2, true); break;
	case VK_NUMPAD4: DEMO_APP::UpdateKeyboardInput(VK_NUMPAD4, true); break;
	case VK_NUMPAD6: DEMO_APP::UpdateKeyboardInput(VK_NUMPAD6, true); break;
	case VK_NUMPAD8: DEMO_APP::UpdateKeyboardInput(VK_NUMPAD8, true); break;
	default:	break;
}
				  break;

case (WM_KEYUP) :
	switch (wParam)
{
	case VK_A: DEMO_APP::UpdateKeyboardInput(VK_A, false); break;
	case VK_W: DEMO_APP::UpdateKeyboardInput(VK_W, false); break;
	case VK_S: DEMO_APP::UpdateKeyboardInput(VK_S, false); break;
	case VK_D: DEMO_APP::UpdateKeyboardInput(VK_D, false); break;
	case VK_E: DEMO_APP::UpdateKeyboardInput(VK_E, false); break;
	case VK_Q: DEMO_APP::UpdateKeyboardInput(VK_Q, false); break;
	case VK_T: DEMO_APP::UpdateKeyboardInput(VK_T, false, true); break;
	case VK_CONTROL: DEMO_APP::UpdateKeyboardInput(VK_CONTROL, false, true); break;
	case VK_NUMPAD2: DEMO_APP::UpdateKeyboardInput(VK_NUMPAD2, false); break;
	case VK_NUMPAD4: DEMO_APP::UpdateKeyboardInput(VK_NUMPAD4, false); break;
	case VK_NUMPAD6: DEMO_APP::UpdateKeyboardInput(VK_NUMPAD6, false); break;
	case VK_NUMPAD8: DEMO_APP::UpdateKeyboardInput(VK_NUMPAD8, false); break;
	default:		break;
}
				break;


#endif