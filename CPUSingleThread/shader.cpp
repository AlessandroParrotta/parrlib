#include "shader.h"

namespace softst {
	Shader::Shader() {}
	Shader::Shader(std::string const& name, std::vector<std::pair<std::string, std::string>> const& attributes) {
		this->attributes = attributes;
	}

	void Shader::setUniform(std::string const& name, void const* data, size_t byteSize) {
		if (uniforms.find(name) == uniforms.end()) {
			uniforms[name].first = byteMap.size();
			uniforms[name].second = byteSize;
			byteMap.resize(byteMap.size() + byteSize);
		}

		for (int i = 0; i < byteSize; i++) { byteMap[uniforms[name].first + i] = ((unsigned char*)data)[i]; }
	}

	void Shader::setUniform(std::string const& name, int i) { setUniform(name, &i, sizeof(int)); }
	void Shader::setUniform(std::string const& name, float f) { setUniform(name, &f, sizeof(float)); }
	void Shader::setUniform(std::string const& name, bool b) { setUniform(name, &b, sizeof(bool)); }

	void Shader::setUniform(std::string const& name, vec2 const& v) { setUniform(name, &v, sizeof(vec2)); }
	void Shader::setUniform(std::string const& name, vec3 const& v) { setUniform(name, &v, sizeof(vec3)); }
	void Shader::setUniform(std::string const& name, vec4 const& v) { setUniform(name, &v, sizeof(vec4)); }

	void Shader::setUniform(std::string const& name, mat3 const& mat) { setUniform(name, &mat, sizeof(mat3)); }
	void Shader::setUniform(std::string const& name, mat4 const& mat) { setUniform(name, &mat, sizeof(mat4)); }

	void Shader::use() const {

	}
	void Shader::release() const {

	}

	void Shader::vertexShaderSolve(std::vector<float>& result, std::vector<float> const& data, int offset, std::vector<unsigned int> const& dataTypes) const {
		int curIdx = 0;
		for (int i = 0; i < dataTypes.size(); i++) {
			for (int j = 0; j < dataTypes[i]; j++) {
				result[curIdx] = data[offset + curIdx];
				curIdx++;
			}
		}
	}

	void Shader::fragmentShaderSolve(vec4& result, std::vector<float> const& data) const {
		result = vc4::white;
	}

	extern "C" __declspec(dllexport) impl::Shader_base * createShader_1(std::string const& name, std::vector<std::pair<std::string, std::string>> const& attributes) { return new Shader(name, attributes); }
}
