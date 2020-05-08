#pragma once

#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Vector2f.h"
#include "util.h"

class FrameBufferObject{
protected:
	vec2 oldres = 0.f;
	int sizeX, sizeY;

	GLuint fboID=0;
	//three types of attachments: color, depth, stencil
	//MAX_COLOR_ATTACHMENTS to know the max number of color attachments possible, minimum guaranteed is 8
	//types of attachments:
	/*
		GL_COLOR_ATTACHMENTi
		GL_DEPTH_ATTACHMENT
		GL_STENCIL_ATTACHMENT
		GL_DEPTH_STENCIL_ATTACHMENT
	*/
	std::unordered_map<GLenum, GLuint> attachments;

	//int internalFormat = GL_RGBA;
	//int format = GL_RGBA;
	//int type = GL_UNSIGNED_BYTE;

	//int minFilter = GL_LINEAR;
	//int magFilter = GL_LINEAR;

	//int wrap_s = GL_REPEAT;
	//int wrap_t = GL_REPEAT;

public:
	struct ColorAttachment {
		GLenum internalFormat = GL_RGBA;
		GLenum format = GL_RGBA;
		GLenum type = GL_UNSIGNED_BYTE;

		GLenum minFilter = GL_LINEAR;
		GLenum magFilter = GL_LINEAR;

		GLenum wrap_s = GL_REPEAT;
		GLenum wrap_t = GL_REPEAT;
	};
protected:
	std::unordered_map<GLenum, ColorAttachment> formats; //only for color attachments

public:
	void init();
	void compCheck();

	void bind();

	void unbind();

	void resize(int sizeX, int sizeY);
	void resize(vec2 size);

	GLuint getID() const;
	//GLuint getDepID();
	GLuint getAttachment(GLenum attachmentType) const;
	GLuint getPrimaryColorAttachment() const;

	void setFiltering(int minFilter, int magFilter);
	void setFiltering(int filtering);

	GLenum getMinFiltering(GLenum att);
	GLenum getMagFiltering(GLenum att);
	GLenum getMinFiltering();
	GLenum getMagFiltering();

	void setWrapMode(int wrap_s, int wrap_t);
	void setWrapMode(int wrapMode);

	GLenum getWrap_s(GLenum att);
	GLenum getWrap_t(GLenum att);
	GLenum getWrap_s();
	GLenum getWrap_t();

	void setFormat(GLenum att, GLenum type, GLint internalFormat, GLenum format);
	void setFormat(GLenum type, GLint internalFormat, GLenum format);

	bool isNotRenderBuffer(GLenum att);

	GLenum getInternalFormat(GLenum att);
	GLenum getInternalFormat();
	
	void addAttachment(GLenum att, ColorAttachment format);
	void addAttachment(GLenum att);
	void removeAttachment(GLenum i);

	int sizex() const;
	int sizey() const;
	vec2 size() const;

	void clear(vec4 color);
	void clear();

	FrameBufferObject();
	FrameBufferObject(int sizeX, int sizeY);
	FrameBufferObject(int sizeX, int sizeY, int magFilter, int minFilter);
	FrameBufferObject(int sizeX, int sizeY, int minFilter, int magFilter, int internalFormat, int format, int type);
	
	FrameBufferObject(vec2 size, std::vector<GLenum> attachments, ColorAttachment format);
	FrameBufferObject(vec2 size, std::vector<GLenum> attachments);

	~FrameBufferObject();

	void dispose();
};

typedef FrameBufferObject FBO;

