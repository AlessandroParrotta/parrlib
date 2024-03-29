#pragma once

#include <unordered_map>
#include <iostream>

#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include <parrlibcore/matrix4f.h>
#include <parrlibcore/matrix3f.h>
#include <parrlibcore/matrix2f.h>

#include <parrlibcore/vector2f.h>
#include <parrlibcore/vector3f.h>
#include <parrlibcore/otherutil.h>
#include <parrlibcore/constants.h>
#include <parrlibcore/tick.h>

#include "../parrlib/Input.h"

#include "../parrlib/impl/shader.h"

namespace gl {
	using namespace prb;
	class Shader : public impl::Shader_base{
	public:
		std::vector<GLuint> shaders;
		GLuint program;

		std::unordered_map<std::string, GLint> uniforms;

		std::string readShader(std::string const& fileName);
		GLuint createShader(GLenum shaderType, std::string source);
		std::vector<std::string> getSourceCodes(std::vector<std::string> const& shaders);
		std::vector<GLuint> compileShaders(std::vector<std::string> const& sources);
		GLuint createProgram(std::vector<GLuint> const& shaders);

		GLint getLocation(std::string const& str) const;
		GLint getLocation(std::string const& str);

		void setUniform(std::string const& name, int v);
		void setUniform(std::string const& name, float v);
		void setUniform(std::string const& name, double v);
		void setUniform(std::string const& name, size_t v);
		void setUniform(std::string const& name, bool v);

		void setUniform(std::string const& name, vec2 const& v);
		void setUniform(std::string const& name, vec3 const& v);
		void setUniform(std::string const& name, vec4 const& v);

		void setUniform(std::string const& name, bool const& transpose, mat2 const& v);
		void setUniform(std::string const& name, bool const& transpose, mat3 const& v);
		void setUniform(std::string const& name, bool const& transpose, mat4 const& v);

		void setUniform(std::string const& name, mat2 const& v);
		void setUniform(std::string const& name, mat3 const& v);
		void setUniform(std::string const& name, mat4 const& v);

		void use() const;
		void release() const;

		void initFastVertFrag(std::string const& vertdeclsec, std::string const& vertbodysec, std::string const& fragdeclsec, std::string const& fragbodysec);
		void initFastFrag(std::string const& declsec, std::string const& bodysec);

		Shader();
		Shader(std::vector<std::string> const& shaders);
		Shader(std::string const& fileName);
		Shader(const char* fileName);
		Shader(std::string const& vert, std::string const& frag);
		~Shader();
	};

	extern "C" __declspec(dllexport) impl::Shader_base * createShader_1(std::string const& name, std::vector<std::pair<std::string, std::string>> const& attributes);
}
