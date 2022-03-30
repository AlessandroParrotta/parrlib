#pragma once

#include <iostream>
#include <vector>
#include <fstream>

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

namespace dx11 {
	class Shader;

	void ThrowIfFailed(HRESULT hr);

	// global declarations
	extern HWND windowHwnd;

	extern IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	extern ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	extern ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
	extern ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer
	extern ID3D11Texture2D* depthStencilTex;
	extern ID3D11DepthStencilState* depthStencilState;
	extern ID3D11DepthStencilView* depthStencilView;
	//extern ID3D11InputLayout* pLayout;            // the pointer to the input layout
	//extern ID3D11VertexShader* pVS;               // the pointer to the vertex shader
	//extern ID3D11PixelShader* pPS;                // the pointer to the pixel shader
	extern Shader defShader;
	extern Shader defTexShader;
	extern ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer
	extern ID3D11BlendState* g_pBlendStateNoBlend;

	extern double deltaTime;
	extern double curtime;

	std::wstring reverse(std::wstring str);
	std::wstring getExt(std::wstring str);
	std::wstring getFileName(std::wstring str);

	std::string reverse(std::string str);

	std::string getFolder(std::string path);
	std::string getExeFolder();

	std::vector<byte> readFile(std::string name);

	ID3D11VertexShader* getVertexShaderFromFile(std::string const& path);
	ID3D11PixelShader* getPixelShaderFromFile(std::string const& path);

}
