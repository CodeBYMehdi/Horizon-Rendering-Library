#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

// VARIABLES 
//core
ID3D11Device* g_Device = nullptr;
ID3D11DeviceContext* g_DeviceContext = nullptr;
IDXGISwapChain* g_SwapChain = nullptr;
//render targets
ID3D11RenderTargetView* g_RenderTargetView = nullptr;
ID3D11DepthStencilView* g_DepthStencilView = nullptr;
//constant buffers (Matrices, lumieres ...)
ID3D11Buffer* g_ConstantBuffer = nullptr;


// INITIALISATION
void d3d11_InitContext(HRL_uint _width, HRL_uint _height, void* loader) {
    break;
}

void d3d11_BeginFrame() {
    break;
}

void d3d11_EndFrame() {
    break;
}

void d3d11_Shutdown() {
    break;
}
