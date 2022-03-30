#include "shader.h"

#include <filesystem>

#include "../parrlib/math/stringutils.h"
#include "../parrlib/math/otherutil.h"
#include "../parrlib/debug.h"


namespace dx11 {

    void Shader::init(const void* vsByteCode, size_t vsByteSize, const void* psByteCode, size_t psByteSize, std::vector<D3D11_INPUT_ELEMENT_DESC> const& vdesc) {
        ThrowIfFailed(
            dev->CreateVertexShader(
                vsByteCode,
                vsByteSize,
                nullptr,
                &vs
            )
        );

        ThrowIfFailed(
            dev->CreateInputLayout(
                &vdesc[0],
                vdesc.size(),
                vsByteCode,
                vsByteSize,
                &layout
            )
        );

        ThrowIfFailed(
            dev->CreatePixelShader(
                psByteCode,
                psByteSize,
                nullptr,
                &ps
            )
        );
    }

    void Shader::init(std::string fileNameVS, std::string filenamePS, std::vector<D3D11_INPUT_ELEMENT_DESC> const& vdesc) {
        std::vector<byte> vshader = readFile(fileNameVS);
        std::vector<byte> pshader = readFile(filenamePS);
        init((const void*)&vshader[0], vshader.size(), (const void*)&pshader[0], pshader.size(), vdesc);
    }

    Shader::Shader() {}
    Shader::Shader(std::string fileNameVS, std::string fileNamePS, std::vector<D3D11_INPUT_ELEMENT_DESC> const& vdesc) { init(fileNameVS, fileNamePS, vdesc); }
    Shader::Shader(std::string fileNameVS, std::string fileNamePS) {
        std::vector<D3D11_INPUT_ELEMENT_DESC> vdesc =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        init(fileNameVS, fileNamePS, vdesc);
    }

    void manageCompileError(HRESULT hr, ID3DBlob** errorBlob) {
        if (FAILED(hr))
        {
            deb::tss << "shader compilation error:\n";
            if (*errorBlob)
            {
                deb::tss << ((char*)(*errorBlob)->GetBufferPointer());
                (*errorBlob)->Release();
            }
            else deb::tss << "unknown error (maybe file not found?)";

            deb::mbe();

            std::terminate();
        }
    }

    Shader::Shader(std::string fileNameVS, std::string fileNamePS, std::vector<D3D11_INPUT_ELEMENT_DESC> vdesc, bool compileRuntime) {
        if (compileRuntime) {
            const D3D_SHADER_MACRO defines[] =
            {
                "EXAMPLE_DEFINE", "1",
                NULL, NULL
            };
            
            //for(int i=0; i<vdesc.size(); i++) deb::debss << vdesc[i].SemanticName << " "; 
            //deb::msbinfo();

            ID3DBlob* vsBlob = 0;
            ID3DBlob* errBlob = 0;
            HRESULT pHResult;
            manageCompileError(D3DX11CompileFromFile(stru::towstr(fileNameVS).c_str(), NULL, NULL, "main", "vs_4_0_level_9_3", 0, 0, NULL, &vsBlob, &errBlob, NULL), &errBlob);

            ID3DBlob* psBlob;
            manageCompileError(D3DX11CompileFromFile(stru::towstr(fileNamePS).c_str(), NULL, NULL, "main", "ps_4_0_level_9_3", 0, 0, NULL, &psBlob, &errBlob, NULL), &errBlob);

            init(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), psBlob->GetBufferPointer(), psBlob->GetBufferSize(), vdesc);
        }
        else init(fileNameVS, fileNamePS, vdesc);
    }

    void Shader::setUniform(std::string const& name, const void* val, size_t byteSize) {
        if (uniforms.find(name) == uniforms.end()) {
            uniforms[name] = { uniformData.size(), byteSize };

            uniformData.reserve(uniformData.size() + byteSize);
            for (int i = 0; i < byteSize; i++) { uniformData.push_back(((BYTE*)val)[i]); }

            //align to 16 bytes
            if (byteSize % 16 != 0) {
                int align = 16 - byteSize % 16; std::vector<BYTE> zeros(align, 0);
                uniformData.insert(uniformData.end(), zeros.begin(), zeros.end());

                //deb::debss << align << " " << uniformData.size() << " " << byteSize << "\n"; deb::outStr();
            }

            //recreate the constant buffer with the updated byte map
            constBuf.dispose();
            constBuf = cbuf(&uniformData[0], uniformData.size());

            //deb::debss << "new uniform set '" << name << "' " << uniforms[name].first << " " << uniforms[name].second;  deb::msbinfo();
        }

        int offset = uniforms[name].first;
        int uniformSize = uniforms[name].second;
        for (int i = 0; i < uniformSize; i++) uniformData[offset + i] = ((BYTE*)val)[i];

        //deb::debss << "BYTEMAP: ";
        //for (int i = 0; i < byteSize; i++) deb::debss << (int)uniformData[offset + i] << " ";
        //deb::debss << " REAL: ";
        //for (int i = 0; i < byteSize; i++) deb::debss << (int)((byte*)val)[i] << " ";
        //deb::debss << "\n"; deb::outStr();

        //constBuf.setData(((byte*)&uniformData[0]) + offset, offset, uniformSize);
        constBuf.setData(&uniformData[0], uniformData.size());

        //deb::debss << offset << " " << uniforms[name].second << "\n"; deb::outStr();
    }

    void Shader::setUniform(std::string const& name, int i) { setUniform(name, (void*)&i, sizeof(int)); }
    void Shader::setUniform(std::string const& name, float f) { setUniform(name, (void*)&f, sizeof(float)); }
    void Shader::setUniform(std::string const& name, bool b) { setUniform(name, (void*)&b, sizeof(bool)); }

    void Shader::setUniform(std::string const& name, vec2 const& v) { setUniform(name, (void*)&v, sizeof(vec2)); }
    void Shader::setUniform(std::string const& name, vec3 const& v) { setUniform(name, (void*)&v, sizeof(vec3)); }
    void Shader::setUniform(std::string const& name, vec4 const& v) { setUniform(name, (void*)&v, sizeof(vec4)); }

    void Shader::setUniform(std::string const& name, mat3 const& mat) { setUniform(name, (void*)&mat, sizeof(mat3)); }
    void Shader::setUniform(std::string const& name, mat4 const& mat) { setUniform(name, (void*)&mat, sizeof(mat4)); }

    void Shader::use() const {
        devcon->VSSetShader(vs, 0, 0);
        devcon->PSSetShader(ps, 0, 0);
        devcon->IASetInputLayout(layout);

        if (constBuf.cptr) { constBuf.use(); /*deb::msbinfo("use constbuf");*/ }
    }
    void Shader::release() const {}

    void Shader::dispose() {
        layout->Release();
        vs->Release();
        ps->Release();

        constBuf.dispose();
    }

    extern "C" __declspec(dllexport) impl::Shader_base * createShader_1(std::string const& name, std::vector<std::pair<std::string, std::string>> const& attributes) {

        //std::string fileNameVS = name + "v.cso";
        //std::string fileNamePS = name + "p.cso";

        std::vector<D3D11_INPUT_ELEMENT_DESC> shaderDesc;
        for (int i = 0; i < attributes.size(); i++) {
            
            LPCSTR name = attributes[i].first.c_str();
            DXGI_FORMAT fmt = DXGI_FORMAT_R32G32B32A32_FLOAT;
            
            if (attributes[i].second.compare("r32g32b32a32_float") == 0)        fmt = DXGI_FORMAT_R32G32B32A32_FLOAT;
            else if (attributes[i].second.compare("r32g32b32_float") == 0)      fmt = DXGI_FORMAT_R32G32B32_FLOAT;
            else if (attributes[i].second.compare("r32g32_float") == 0)         fmt = DXGI_FORMAT_R32G32_FLOAT;
            else if (attributes[i].second.compare("r8g8_uchar") == 0)           fmt = DXGI_FORMAT_R8G8_UNORM;
            else if (attributes[i].second.compare("r8g8b8a8_uchar") == 0)       fmt = DXGI_FORMAT_R8G8B8A8_UNORM;

            D3D11_INPUT_ELEMENT_DESC desc = { name, 0, fmt, 0, (i > 0 ? D3D11_APPEND_ALIGNED_ELEMENT : 0), D3D11_INPUT_PER_VERTEX_DATA, 0 };
            shaderDesc.push_back(desc);
        }

        std::string fileNameVS = stru::fallbackPath(outl::getExeFolder(), name + "v.cso");
        std::string fileNamePS = stru::fallbackPath(outl::getExeFolder(), name + "p.cso");
        
        if (!std::filesystem::exists(fileNameVS) || !std::filesystem::exists(fileNamePS)) {
            fileNameVS = stru::fallbackPath(outl::getExeFolder(), name + "v.hlsl");
            fileNamePS = stru::fallbackPath(outl::getExeFolder(), name + "p.hlsl");

            return new Shader(fileNameVS, fileNamePS, shaderDesc, true);
        }
        else return new Shader(fileNameVS, fileNamePS, shaderDesc);
    }

}
