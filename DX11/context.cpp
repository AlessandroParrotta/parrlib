#include "context.h"

#include <thread>
#include <chrono>

#include <parrlibcore/utils2d/axisalignedboundingbox2d.h>

#include "common.h"
#include "../parrlib/debug.h"
#include "../parrlib/context.h"

namespace dx11 {
	namespace DXContext {
		bool binitialized = false;

        HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) {

            vec2 pos = bb[0];
            vec2 size = bb.size();

            HWND hwnd = CreateWindowEx(NULL,
                L"WindowClass",
                title.c_str(),
                WS_OVERLAPPEDWINDOW,
                pos.x,
                pos.y,
                size.x,
                size.y,
                NULL,
                NULL,
                hInstance,
                NULL
            );

            return hwnd;
        }


		void init(HWND hWnd, int antiAlias, vec2 res) {

            //deb::debss << antiAlias; deb::msbinfo();

            cst::res(res);

            //deb::debss << cst::res(); deb::msbinfo();

            // create a struct to hold information about the swap chain
            DXGI_SWAP_CHAIN_DESC scd;

            // clear out the struct for use
            ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

            // fill the swap chain description struct
            scd.BufferCount = 1;                                    // one back buffer
            scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
            scd.BufferDesc.Width = cst::resx();                     // set the back buffer width
            scd.BufferDesc.Height = cst::resy();                    // set the back buffer height
            scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
            scd.OutputWindow = hWnd;                               // the window to be used
            scd.SampleDesc.Count = antiAlias;                              // how many multisamples
            scd.Windowed = TRUE;                                   // windowed/full-screen mode
            scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

            // create a device, device context and swap chain using the information in the scd struct
            ThrowIfFailed(D3D11CreateDeviceAndSwapChain(NULL,
                D3D_DRIVER_TYPE_HARDWARE,
                NULL,
                D3D11_CREATE_DEVICE_DEBUG,
                NULL,
                NULL,
                D3D11_SDK_VERSION,
                &scd,
                &swapchain,
                &dev,
                NULL,
                &devcon)
            );

            // get the address of the back buffer
            ID3D11Texture2D* pBackBuffer;
            ThrowIfFailed(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

            // use the back buffer address to create the render target
            ThrowIfFailed(dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer));
            pBackBuffer->Release();

            //depth stencil texture 
            D3D11_TEXTURE2D_DESC dptexDesc;
            ZeroMemory(&dptexDesc, sizeof(D3D11_TEXTURE2D_DESC));

            dptexDesc.Width = res.x;
            dptexDesc.Height = res.y;
            dptexDesc.MipLevels = 1;
            dptexDesc.ArraySize = 1;
            dptexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dptexDesc.SampleDesc.Count = prc::getAntiAliasing();
            dptexDesc.SampleDesc.Quality = 0;
            dptexDesc.Usage = D3D11_USAGE_DEFAULT;
            dptexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            dptexDesc.CPUAccessFlags = 0;
            dptexDesc.MiscFlags = 0;

            ThrowIfFailed(dev->CreateTexture2D(&dptexDesc, NULL, &depthStencilTex));
            ThrowIfFailed(dev->CreateDepthStencilView(depthStencilTex, NULL, &depthStencilView));


            D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

            // Depth test parameters
            depthStencilDesc.DepthEnable = true;
            depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

            // Stencil test parameters
            depthStencilDesc.StencilEnable = true;
            depthStencilDesc.StencilReadMask = 0xFF;
            depthStencilDesc.StencilWriteMask = 0xFF;

            // Stencil operations if pixel is front-facing
            depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
            depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            // Stencil operations if pixel is back-facing
            depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
            depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            ThrowIfFailed(dev->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

            // set the render target as the back buffer
            devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);
            devcon->OMSetDepthStencilState(depthStencilState, 1);

            // Set the viewport
            D3D11_VIEWPORT viewport;
            ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

            viewport.TopLeftX = 0;
            viewport.TopLeftY = 0;
            viewport.Width = cst::resx();
            viewport.Height = cst::resy();
            viewport.MinDepth = 0.f;
            viewport.MaxDepth = 1.f;

            devcon->RSSetViewports(1, &viewport);

            D3D11_BLEND_DESC BlendState;
            ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
            BlendState.RenderTarget[0].BlendEnable = TRUE;
            BlendState.AlphaToCoverageEnable = FALSE;
            BlendState.IndependentBlendEnable = FALSE;
            BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
            BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
            BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            ThrowIfFailed(dev->CreateBlendState(&BlendState, &g_pBlendStateNoBlend));

			binitialized = true;
		}
        bool initialized() { return binitialized; }

		void clear(vec4 color) {
			devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(color.x, color.y, color.z, color.w));// Clear the depth buffer.
            devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
            devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
		}

		void setBlend(bool enabled) {
			//if (enabled) {
				UINT sampleMask = 0xffffffff;
				devcon->OMSetBlendState(g_pBlendStateNoBlend, NULL, sampleMask);
			//}
		}

		void present(bool vSync) {
            //devcon->OMSetBlendState(NULL, NULL, 0xffffffff);
            devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);
            devcon->OMSetDepthStencilState(depthStencilState, 1);
			ThrowIfFailed(swapchain->Present(vSync, 0));
            //deb::msbinfo("present");
		}

		void setFullscreen(bool fullscreen) {
			swapchain->SetFullscreenState(fullscreen, NULL);
		}

		void dispose() {
			swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

			//close and release all existing COM objects
			//pVBuffer->Release();
			swapchain->Release();
			backbuffer->Release();
			dev->Release();
			devcon->Release();
		}

		void resize(vec2 res) {
            res = res.maxed(1.f);

			devcon->OMSetRenderTargets(0, 0, 0);

			backbuffer->Release();

			ThrowIfFailed(swapchain->ResizeBuffers(0, res.x, res.y, DXGI_FORMAT_UNKNOWN, D3D11_CREATE_DEVICE_DEBUG));

			// get the address of the back buffer
			ID3D11Texture2D* pBackBuffer;
			ThrowIfFailed(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

			// use the back buffer address to create the render target
			ThrowIfFailed(dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer));
			pBackBuffer->Release();

			// set the render target as the back buffer
			devcon->OMSetRenderTargets(1, &backbuffer, NULL);

            depthStencilState->Release();
            depthStencilTex->Release();
            depthStencilView->Release();

            //depth stencil texture 
            D3D11_TEXTURE2D_DESC dptexDesc;
            ZeroMemory(&dptexDesc, sizeof(D3D11_TEXTURE2D_DESC));

            dptexDesc.Width = res.x;
            dptexDesc.Height = res.y;
            dptexDesc.MipLevels = 1;
            dptexDesc.ArraySize = 1;
            dptexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dptexDesc.SampleDesc.Count = prc::getAntiAliasing();
            dptexDesc.SampleDesc.Quality = 0;
            dptexDesc.Usage = D3D11_USAGE_DEFAULT;
            dptexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            dptexDesc.CPUAccessFlags = 0;
            dptexDesc.MiscFlags = 0;

            ThrowIfFailed(dev->CreateTexture2D(&dptexDesc, NULL, &depthStencilTex));
            ThrowIfFailed(dev->CreateDepthStencilView(depthStencilTex, NULL, &depthStencilView));

            D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

            // Depth test parameters
            depthStencilDesc.DepthEnable = true;
            depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

            // Stencil test parameters
            depthStencilDesc.StencilEnable = true;
            depthStencilDesc.StencilReadMask = 0xFF;
            depthStencilDesc.StencilWriteMask = 0xFF;

            // Stencil operations if pixel is front-facing
            depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
            depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            // Stencil operations if pixel is back-facing
            depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
            depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            ThrowIfFailed(dev->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

            // set the render target as the back buffer
            devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);
            devcon->OMSetDepthStencilState(depthStencilState, 1);

			// Set the viewport
			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = res.x;
			viewport.Height = res.y;
            viewport.MinDepth = 0.f;
            viewport.MaxDepth = 1.f;

			devcon->RSSetViewports(1, &viewport);

            cst::res(res);
            //deb::debss << "resize " << res << " " << cst::res(); deb::msbinfo();
		}
	}

    extern "C" __declspec(dllexport) HWND createWindow(HINSTANCE hInstance, std::wstring title, aabb2 bb) { return DXContext::createWindow(hInstance, title, bb); }

    extern "C" __declspec(dllexport) void init(HWND hWnd, int antiAlias, vec2 res) { DXContext::init(hWnd, antiAlias, res); }
    extern "C" __declspec(dllexport) bool initialized() { return DXContext::initialized(); }

    extern "C" __declspec(dllexport) void setBlend(bool enabled) { DXContext::setBlend(enabled); }

    extern "C" __declspec(dllexport) void clear(vec4 color) { DXContext::clear(color); }

    extern "C" __declspec(dllexport) void present(bool vSync) { DXContext::present(vSync); }

    extern "C" __declspec(dllexport) void dispose() { DXContext::dispose(); }

    extern "C" __declspec(dllexport) void setFullscreen(bool fullscreen) { DXContext::setFullscreen(fullscreen); }

    extern "C" __declspec(dllexport) void resize(vec2 res) { DXContext::resize(res); }

}