#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <d3dcompiler.h>

#include <parrlibcore/matrix4f.h>

#include "common.h"
#include "../parrlib/impl/shader.h"

#include "constantbuffer.h"

namespace dx11 {
	using namespace prb;

	class Shader : public impl::Shader_base {
	private:
		void init(const void* vsByteCode, size_t vsByteSize, const void* psByteCode, size_t psByteSize, std::vector<D3D11_INPUT_ELEMENT_DESC> const& vdesc);
		void init(std::string fileNameVS, std::string filenamePS, std::vector<D3D11_INPUT_ELEMENT_DESC> const& vdesc);
	public:
		ID3D11InputLayout* layout = NULL;
		ID3D11VertexShader* vs = NULL;
		ID3D11PixelShader* ps = NULL;

		cbuf constBuf;
		std::vector<BYTE> uniformData;
		std::unordered_map<std::string, std::pair<size_t, size_t>> uniforms;

		//std::unordered_map<std::string, std::any> cbufs;
		//template<class T> T get(std::string id) { return std::any_cast<T>(cbufs[id]); }
		//template<class T> T operator[](std::string id) { return get<T>(id); }

		Shader();
		Shader(std::string fileNameVS, std::string fileNamePS, std::vector<D3D11_INPUT_ELEMENT_DESC> const& vdesc);
		Shader(std::string fileNameVS, std::string fileNamePS);
		Shader(std::string fileNameVS, std::string fileNamePS, std::vector<D3D11_INPUT_ELEMENT_DESC> vdesc, bool compileRuntime);

		void setUniform(std::string const& name, const void* val, size_t byteSize);

		void setUniform(std::string const& name, int i);
		void setUniform(std::string const& name, float f);
		void setUniform(std::string const& name, bool b);

		void setUniform(std::string const& name, vec2 const& v);
		void setUniform(std::string const& name, vec3 const& v);
		void setUniform(std::string const& name, vec4 const& v);

		void setUniform(std::string const& name, mat3 const& mat);
		void setUniform(std::string const& name, mat4 const& mat);

		void use() const;
		void release() const;

		void dispose();
	};

	extern "C" __declspec(dllexport) impl::Shader_base * createShader_1(std::string const& name, std::vector<std::pair<std::string, std::string>> const& attributes);

}
