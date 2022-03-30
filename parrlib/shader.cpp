#include "shader.h"

namespace prb {
	Shader::Shader() {}
	Shader::Shader(std::string const& name, std::vector<std::pair<std::string, std::string>> attributes) { ptr = impl::createShader_1(name, attributes); }
	//Shader::Shader(std::string const& vert, std::string const& frag) {}
	Shader::Shader(std::string const& name) : Shader(name, { {"POSITION", "r32g32_float"}, {"COLOR", "r32g32b32a32_float"} }) {}

	Shader::~Shader() {}

	void Shader::use() const { ptr->use(); }
	void Shader::release() const { ptr->release(); }

	void Shader::setUniform(std::string const& name, int i) { ptr->setUniform(name, i); }
	void Shader::setUniform(std::string const& name, float f) { ptr->setUniform(name, f); }
	void Shader::setUniform(std::string const& name, bool b) { ptr->setUniform(name, b); }

	void Shader::setUniform(std::string const& name, vec2 const& v) { ptr->setUniform(name, v); }
	void Shader::setUniform(std::string const& name, vec3 const& v) { ptr->setUniform(name, v); }
	void Shader::setUniform(std::string const& name, vec4 const& v) { ptr->setUniform(name, v); }

	void Shader::setUniform(std::string const& name, mat3 const& mat) { ptr->setUniform(name, mat); }
	void Shader::setUniform(std::string name, mat4 const& mat) { ptr->setUniform(name, mat); }
}
