#pragma once

#include <vector>
#include <unordered_map>

#include <glad/glad.h>

#include "../parrlib/math/vector2f.h"
#include "../parrlib/math/vector4f.h"

#include "../parrlib/impl/framebuffer.h"

namespace gl {
	using namespace prb;
	class FrameBufferObject : public prb::impl::FrameBuffer_base {
	protected:
		vec2 oldres = 0.f;
		int sizeX, sizeY;

		GLuint fboID = 0;
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
		void compCheck() const;

		void bind();

		void unbind();

		void bindTexture();
		void unbindTexture();

		void resize(int sizeX, int sizeY);
		void resize(vec2 size);

		GLuint getID() const;
		//GLuint getDepID();
		GLuint getAttachment(GLenum attachmentType) const;
		GLuint getPrimaryColorAttachment() const;

		void setFiltering(int minFilter, int magFilter);
		void setFiltering(int filtering);

		GLenum getMinFiltering(GLenum att) const;
		GLenum getMagFiltering(GLenum att) const;
		GLenum getMinFiltering() const;
		GLenum getMagFiltering() const;

		void setWrapMode(int wrap_s, int wrap_t);
		void setWrapMode(int wrapMode);

		GLenum getWrap_s(GLenum att) const;
		GLenum getWrap_t(GLenum att) const;
		GLenum getWrap_s() const;
		GLenum getWrap_t() const;

		void setFormat(GLenum att, GLenum type, GLint internalFormat, GLenum format);
		void setFormat(GLenum type, GLint internalFormat, GLenum format);

		bool isNotRenderBuffer(GLenum att) const;

		GLenum getInternalFormat(GLenum att) const;
		GLenum getInternalFormat() const;

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

		//IMPL FUNCS

		void drawImmediate(mat3 const& mat);
	};

	extern "C" __declspec(dllexport) impl::FrameBuffer_base * createFrameBuffer_1();
	extern "C" __declspec(dllexport) impl::FrameBuffer_base * createFrameBuffer_2(vec2 size);

	typedef FrameBufferObject FBO;

}
