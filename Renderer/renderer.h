#pragma once
#include <dxgi.h>

namespace Base::Renderer
{
	bool InitRenderer(IDXGISwapChain* pSwapChain);
	void ShutdownRenderer();
	void RenderImGui();
}


