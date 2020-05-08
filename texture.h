#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>

#include "Matrix3f.h"
#include "Vector4f.h"

class Texture{
	public:
		GLuint texID=0;
		vec2 size;

		GLint internalFormat = GL_RGBA;
		GLint border = 0;
		GLenum format = GL_RGBA;
		GLenum type = GL_UNSIGNED_BYTE;

		GLint minfilter = GL_LINEAR, magfilter = GL_LINEAR;
		GLint wraps = GL_REPEAT, wrapt = GL_REPEAT;

		void bind();
		void unbind();

		void fillRegion(vec2 pos, vec2 size, GLenum format, const void* data);
		void fillRegion(vec2 pos, vec2 size, const void* data);
		void fillRegion(vec2 pos, vec2 size, vec4 col);
		void setPixel(vec2 pos, vec4 col);

		void setData(const void* data);
		template<typename T> void setDataT(std::vector<T> val);
		void setData(float val);
		void setData(unsigned char val);
		void setData(int val);
		void setData(vec2 val);
		void setData(vec3 val);
		void setData(vec4 val);
		
		int getChannelNumber(GLenum format);
		int getChannelNumber();
		template<typename T> void* getDataT(GLenum format);
		//this function allocates a new pointer, make sure to delete[] it
		void* getData(GLenum format);
		void* getData();
		
		void setFiltering(GLint min, GLint mag);
		void setFiltering(GLint filtering);
		void setWrap(GLint s, GLint t);

		void resize(vec2 size);

		void setUnpackAligment(GLint val); //for textrenderer

		bool null();

		std::string name;
		void draw(mat3 transform);
		void draw(vec2 pos, vec2 size);

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

		~Texture();

		void dispose();
};

//get size of texture
//int w, h;
//int miplevel = 0;
//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

//void draw(float x, float y, float sizeX, float sizeY, float rotation);
