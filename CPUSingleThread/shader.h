#pragma once

#include <unordered_map>

#include "../parrlib/impl/shader.h"

namespace softst {
	using namespace prb;
	class Shader : public impl::Shader_base {
	public:
		std::vector<unsigned char> byteMap;
		std::unordered_map<std::string, std::pair<size_t, size_t>> uniforms;
		int type = 0; //0 = vertex, 1 = fragment

		std::vector<std::pair<std::string, std::string>> attributes;

		Shader();
		Shader(std::string const& name, std::vector<std::pair<std::string, std::string>> const& attributes);

		void setUniform(std::string const& name, void const* data, size_t byteSize);
		//template<typename T> void setUniform(std::string const& name, T data) { setUniform(name, &data, sizeof(T)); }

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

		void vertexShaderSolve(std::vector<float>& result, std::vector<float> const& data, int offset, std::vector<unsigned int> const& dataTypes) const;
		void fragmentShaderSolve(vec4& result, std::vector<float> const& data) const;
	};

	extern "C" __declspec(dllexport) impl::Shader_base * createShader_1(std::string const& name, std::vector<std::pair<std::string, std::string>> const& attributes);
}