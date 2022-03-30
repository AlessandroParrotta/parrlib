#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>

#include "../parrlib/math/Matrix3f.h"
#include "../parrlib/math/vector2f.h"
#include "../parrlib/math/vector4f.h"

#include "../parrlib/impl/texture.h"

namespace gl {
	using namespace prb;
	class Texture : public impl::Texture_base{
	public:
		GLuint id = 0;
		vec2 size;

		GLint internalFormat = GL_RGBA;
		GLint border = 0;
		GLenum format = GL_RGBA; //only used for functions, DOES NOT REPRESENT REAL FORMAT (internalFormat is)
		int channels = 0;
		GLenum type = GL_UNSIGNED_BYTE;

		GLint minfilter = GL_LINEAR, magfilter = GL_LINEAR;
		GLint wraps = GL_REPEAT, wrapt = GL_REPEAT;

		void* data = NULL;

		size_t getByteSize();

		void bind() const;
		void unbind() const;

		void fillRegion(vec2 pos, vec2 size, GLenum format, const void* data);
		void fillRegion(vec2 pos, vec2 size, const void* data);
		void fillRegion(vec2 pos, vec2 size, vec4 col);
		void setPixel(vec2 pos, vec4 col);

		void setFiltering(GLint min, GLint mag);
		void setFiltering(GLint filtering);
		void setWrap(GLint s, GLint t);

		void resize(vec2 size);

		void setUnpackAligment(GLint val); //for textrenderer

		bool null() const;

		std::string name;
		void draw(mat3 transform) const;
		void draw(vec2 pos, vec2 size) const;

		void loadTex(std::string const& fileName);

		Texture();
		Texture(GLuint texID, vec2 res);
		Texture(GLuint texID);

		Texture(std::string const& fileName);
		Texture(const char* fileName);
		Texture(const unsigned char* data, vec2 size);

		Texture(vec4 initCol, vec2 size);

		Texture(vec2 size);
		Texture(vec2 size, GLint internalFormat, GLenum format, GLenum type);
		Texture(const void* data, vec2 size, GLint internalFormat, GLenum format, GLenum type);

		~Texture();

		void dispose();

		void setData(void* data);
		void* getDataRaw() const;

		//IMPL FUNCS
		void setData(unsigned char* data, prb::vec2 size);
		void setData(unsigned char* data, prb::vec2 size, int linesize);
		std::vector<unsigned char> getData() const;
		void fillRegion(unsigned char* subData, prb::vec2 start, prb::vec2 end);
		prb::vec2 getSize() const;
		void drawImmediate(std::vector<float> const& verts) const;
		void drawImmediate(prb::vec2 pos, prb::vec2 size) const;
	};

	//get size of texture
	//int w, h;
	//int miplevel = 0;
	//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
	//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

	//void draw(float x, float y, float sizeX, float sizeY, float rotation);

	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_1();
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_2(std::wstring const& path);
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_3(unsigned char* data, vec2 size);
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_4(unsigned char* data, vec2 size, int linesize);
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_5(vec2 size);
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_6(vec2 size, int linesize);

}
