#include "common.h"

#include "shader.h"
#include "../parrlib/debug.h"
#include "../parrlib/math/stringutils.h"

namespace dx11 {

    void ThrowIfFailed(HRESULT hr) { if (FAILED(hr)) std::terminate(); }

    // global declarations
    HWND windowHwnd;

    IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
    ID3D11Device* dev;                     // the pointer to our Direct3D device interface
    ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
    ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer
    ID3D11Texture2D* depthStencilTex = NULL;
    ID3D11DepthStencilState* depthStencilState = NULL;
    ID3D11DepthStencilView* depthStencilView = NULL;
    //ID3D11InputLayout* pLayout;            // the pointer to the input layout
    //ID3D11VertexShader* pVS;               // the pointer to the vertex shader
    //ID3D11PixelShader* pPS;                // the pointer to the pixel shader
    Shader defTexShader;
    Shader defShader;
    ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer
    ID3D11BlendState* g_pBlendStateNoBlend;

    double deltaTime = 0.;
    double curtime = 0.;

    std::wstring reverse(std::wstring str) {
        std::wstring res;
        for (int i = 0; i < str.length(); i++) res += str[str.length() - 1 - i];
        return res;
    }

    std::wstring getExt(std::wstring str) {
        if (str.find('.') == std::wstring::npos) return str;

        std::wstring rev = reverse(str);
        return reverse(rev.substr(0, rev.find(L'.')));
    }

    std::wstring getFileName(std::wstring str) {
        if (str.find(L'\\') == std::wstring::npos) return str;

        std::wstring rev = reverse(str);
        return reverse(rev.substr(0, rev.find(L'\\')));
    }

    std::string reverse(std::string str) {
        std::string res;
        for (int i = 0; i < str.length(); i++) res += str[str.length() - 1 - i];
        return res;
    }

    std::string getFolder(std::string path) {
        if (path.find('.') == std::string::npos || path.find('\\') == std::string::npos) return path;

        std::string rev = reverse(path);
        return reverse(rev.substr(rev.find('\\'), rev.length()));
    }

    std::string getExeLocation() {
        LPSTR exeloc = new CHAR[1000];
        DWORD nsize = 1000;
        GetModuleFileNameA(NULL, exeloc, nsize);

        std::string res(exeloc);
        delete[] exeloc;

        return res;
    }
    std::string getExeFolder() { return getFolder(getExeLocation()); }

    std::vector<byte> readFile(std::string name) {
        std::ifstream f(name, std::ios::binary);

        if (!f.is_open() || !f.good()) {
            //throw std::runtime_error("file could not be opened\n"); 
            deb::tss << "could not open file '" << name << "'"; deb::mbe();
            std::terminate();
        }

        if (f.fail()) std::terminate();

        int length = 0;
        f.seekg(0, f.end);
        length = f.tellg();
        f.seekg(0, f.beg);

        std::vector<byte> bytes;

        char* buf = new char[length];

        f.read(buf, length);
        for (int i = 0; i < length; i++) bytes.push_back((unsigned char)buf[i]);

        delete[] buf;

        if (!f) std::terminate();

        f.close();

        return bytes;
    }

    ID3D11VertexShader* getVertexShaderFromFile(std::string const& path) {
        ID3D11VertexShader* ptr = NULL;
        std::vector<byte> vshader = readFile(path);
        ThrowIfFailed(
            dev->CreateVertexShader(
                &vshader[0],
                vshader.size(),
                nullptr,
                &ptr
            )
        );
        return ptr;
    }
    ID3D11PixelShader* getPixelShaderFromFile(std::string const& path) {
        ID3D11PixelShader* ptr = NULL;
        std::vector<byte> pshader = readFile(path);
        ThrowIfFailed(
            dev->CreatePixelShader(
                &pshader[0],
                pshader.size(),
                nullptr,
                &ptr
            )
        );
        return ptr;
    }

}
