#include "Shader.h"

#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>

#include "../parrlib/stringutils.h"

#include "../parrlib/debug.h"

namespace gl {
	std::string Shader::readShader(std::string const& fileName) {
		if (fileName.compare("") == 0) return "";

		std::string fname = strup::fallbackPath(fileName);

		std::string shaderCode;
		std::ifstream file(fname, std::ios::in);

		//if (!file.good()) {
		//	for (int i = 0; i < 5 && !file.good(); i++) {
		//		std::string fname = stru::getFileName(fileName);

		//		std::string backStr = "../";
		//		for (int j = 0; j < i; j++) backStr += "../";

		//		file = std::ifstream(backStr + "parrlibassets/shaders/" + fname, std::ios::in);
		//	}
		//}

		if (!file.good()) {
			deb::tss << "\ncan't read file " << stru::towstr(fileName); deb::mbe();
			terminate();
		}

		std::string line;
		while (std::getline(file, line)) {
			shaderCode += line + "\n";
		}

		return shaderCode;
	}

	GLuint Shader::createShader(GLenum shaderType, std::string source) {
		int compileResult = 0;

		GLuint shader = glCreateShader(shaderType);
		const char* shader_code_ptr = source.c_str();
		const int shader_code_size = source.size();

		glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

		//check for errors
		if (compileResult == GL_FALSE) {
			int info_log_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
			std::vector<char> shader_log(info_log_length);
			glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
			deb::tss << "\nERROR compiling shader " << stru::towstr(source) << "\n\n: " << "\n" << &shader_log[0]; deb::mbe();
			return 0;
		}

		return shader;
	}

	std::vector<std::string> Shader::getSourceCodes(std::vector<std::string> const& shaders) {
		std::vector<std::string> sources;
		for (int i = 0; i < shaders.size(); i++) {
			sources.push_back(readShader(shaders[i]));
		}
		return sources;
	}
	std::vector<GLuint> Shader::compileShaders(std::vector<std::string> const& sources) {
		std::vector<GLuint> cShaders; //compiled
		for (int i = 0; i < 4; i++) {
			if (sources[i].compare("") == 0) { cShaders.push_back(0); continue; }

			GLenum type = GL_VERTEX_SHADER;
		switch (i) { case 1: type = GL_TESS_CONTROL_SHADER; break; case 2: type = GL_GEOMETRY_SHADER; break; case 3: type = GL_FRAGMENT_SHADER; break; }

						   cShaders.push_back(createShader(type, sources[i]));
		}
		return cShaders;
	}

	GLuint Shader::createProgram(std::vector<GLuint> const& shaders) {
		this->shaders = shaders;

		GLuint program = glCreateProgram();

		//for loop read compile shaders
		for (int i = 0; i < 4; i++) {
			if (shaders[i] == 0) continue;

			glAttachShader(program, shaders[i]);
		}

		glLinkProgram(program);

		int link_result = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &link_result);

		//check for link errors
		if (link_result == GL_FALSE) {
			std::stringstream ss;
			for (int i = 0; i < 4; i++) {
				ss << "shader[" << i << "]\n";
				//ss << sources[i] << "\n";
				ss << "---------------------------------------\n";
			}

			int info_log_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
			std::vector<char> program_log(info_log_length);
			glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
			deb::tss << "\nShader Loader : LINK ERROR ()\n" << &program_log[0] << "\n" << stru::towstr(ss.str()); deb::mbe();
			return 0;
		}
		return program;
	}

	GLint Shader::getLocation(std::string const& str) {
		if (uniforms.find(str) == uniforms.end()) {
			uniforms[str] = glGetUniformLocation(program, str.c_str());
			if (uniforms[str] == -1) { deb::tss << "shader " << program << ": uniform '" << stru::towstr(str) << "' not found (or has been optimized away)!\n"; deb::mbe(); }
		}
		return uniforms[str];
	}

	GLint Shader::getLocation(std::string const& str) const {
		return uniforms.at(str);
	}

	void Shader::setUniform(std::string const& name, int v) { glUniform1i(getLocation(name), v); }
	void Shader::setUniform(std::string const& name, float v) { glUniform1f(getLocation(name), v); }
	void Shader::setUniform(std::string const& name, double v) { glUniform1d(getLocation(name), v); }
	void Shader::setUniform(std::string const& name, size_t v) { glUniform1i(getLocation(name), v); }
	void Shader::setUniform(std::string const& name, bool v) { glUniform1i(getLocation(name), v); }

	void Shader::setUniform(std::string const& name, vec2 const& v) { glUniform2f(getLocation(name), v.x, v.y); }
	void Shader::setUniform(std::string const& name, vec3 const& v) { glUniform3f(getLocation(name), v.x, v.y, v.z); }
	void Shader::setUniform(std::string const& name, vec4 const& v) { glUniform4f(getLocation(name), v.x, v.y, v.z, v.w); }

	void Shader::setUniform(std::string const& name, bool const& transpose, mat2 const& v) { glUniformMatrix2fv(getLocation(name), 1, transpose, *v.m); }
	void Shader::setUniform(std::string const& name, bool const& transpose, mat3 const& v) { glUniformMatrix3fv(getLocation(name), 1, transpose, *v.m); }
	void Shader::setUniform(std::string const& name, bool const& transpose, mat4 const& v) { glUniformMatrix4fv(getLocation(name), 1, transpose, *v.matrix); }

	void Shader::setUniform(std::string const& name, mat2 const& v) { setUniform(name, false, v); }
	void Shader::setUniform(std::string const& name, mat3 const& v) { setUniform(name, false, v); }
	void Shader::setUniform(std::string const& name, mat4 const& v) { setUniform(name, false, v); }

	void Shader::use() const {
		glUseProgram(program);
	}

	void Shader::release() const {
		glUseProgram(0);
	}

	void Shader::initFastVertFrag(std::string const& vertdeclsec, std::string const& vertbodysec, std::string const& fragdeclsec, std::string const& fragbodysec) {
		std::string vertSrc = "#version 330\n" + vertdeclsec + "\nvoid main(void){\n" + vertbodysec + "\n}\n";
		std::string fragSrc = "#version 330\n" + fragdeclsec + "\nvoid main(void){\n" + fragbodysec + "\n}\n";

		this->program = createProgram(compileShaders({ vertSrc, "", "", fragSrc }));
	}

	void Shader::initFastFrag(std::string const& declsec, std::string const& bodysec) {
		initFastVertFrag(
			"layout(location = 0) in vec2 pos;",
			"gl_Position = vec4(pos.xy, 0., 1.);",
			declsec, bodysec
		);
	}

	Shader::Shader() {

	}

	Shader::Shader(std::vector<std::string> const& shaders) {
		//this->program = createProgram(shaders);

		this->program = createProgram(compileShaders(getSourceCodes(shaders)));
	}

	Shader::Shader(std::string const& fileName) {
		std::vector<std::string> fl = stru::splitFilePath(fileName);

		std::vector<std::string> shaders;
		if (fl[2].compare(".sh") == 0) {
			std::string fback = strup::fallbackPath(fileName);
			std::fstream f(fback, std::ios::in);

			if (!f.good()) { deb::tss << "could not find shader '" << stru::towstr(fileName) << "'!\n"; deb::mbe(); return; }

			while (f.good()) {
				char str[1000];
				f.getline(str, 1000);
				shaders.push_back(fl[0] + str);
				deb::tss << "read " << stru::towstr(shaders.back()) << "\n"; deb::mbe();
			}

			f.close();

			this->program = createProgram(compileShaders(getSourceCodes(shaders)));
		}
		else if (fl[2].compare("") == 0) {
			this->program = createProgram(compileShaders(getSourceCodes({
				strup::fallbackPath(fileName + ".vert"),
				strup::fallbackPath(fileName + ".tes"),
				strup::fallbackPath(fileName + ".geom"),
				strup::fallbackPath(fileName + ".frag")
				})));
		}
	}

	Shader::Shader(const char* fileName) : Shader(std::string(fileName)) {}

	Shader::Shader(std::string const& vert, std::string const& frag) {
		this->program = createProgram(compileShaders(getSourceCodes({ vert, "", "", frag })));
	}

	Shader::~Shader() {

	}

	extern "C" __declspec(dllexport) impl::Shader_base * createShader_1(std::string const& name, std::vector<std::pair<std::string, std::string>> const& attributes) {
		std::string fileNameVS = strup::fallbackPath(outl::getExeFolder(), name + ".vert");
		std::string fileNamePS = strup::fallbackPath(outl::getExeFolder(), name + ".frag");

		return new Shader({ fileNameVS, fileNamePS });
	}

}