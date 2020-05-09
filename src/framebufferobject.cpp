#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <parrlib/framebufferobject.h>
#include <parrlib/timer.h>
#include <parrlib/context.h>

#include <iostream>

void FrameBufferObject::addAttachment(GLenum att, FrameBufferObject::ColorAttachment format) {
	if (attachments.find(att) != attachments.end()) { std::cout << "FBO: " << fboID << ": error: attachment " << att << " already created\n"; return; }

	bind();

		if (isNotRenderBuffer(att)) {
			//attachments.push_back(0);
			GLuint at = 0;
			glGenTextures(1, &at);
			glBindTexture(GL_TEXTURE_2D, at);

				glTexImage2D(GL_TEXTURE_2D, 0, format.internalFormat, sizeX, sizeY, 0, format.format, format.type, NULL);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, format.minFilter);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, format.magFilter);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format.wrap_s);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format.wrap_t);

				//if (att == GL_DEPTH_ATTACHMENT) glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

				glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, att, GL_TEXTURE_2D, at, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
		
			attachments[att] = at;
			//formats[att] = { internalFormat, format, type, minFilter, magFilter, wrap_s, wrap_t };
			formats[att] = format;
		}
		else if (att == GL_DEPTH_ATTACHMENT) {
			GLuint at = 0;
			glGenRenderbuffers(1, &at);
			glBindRenderbuffer(GL_RENDERBUFFER, at);

				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, sizeX, sizeY);

			glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, at);
			
			attachments[att] = at;
		}
		else if (att == GL_STENCIL_ATTACHMENT) {
			GLuint at = 0;
			glGenRenderbuffers(1, &at);
			glBindRenderbuffer(GL_RENDERBUFFER, at);

				glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_COMPONENTS/*probably wrong*/, sizeX, sizeY);

			glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, at);
			
			attachments[att] = at;
		}
		else if (att == GL_DEPTH_STENCIL_ATTACHMENT) {
			GLuint at = 0;
			glGenRenderbuffers(1, &at);
			glBindRenderbuffer(GL_RENDERBUFFER, at);

				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY);

			glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, at);
		
			attachments[att] = at;
		}

		compCheck();

	unbind();
}
void FrameBufferObject::addAttachment(GLenum att) { 
	if (formats.empty() || formats.find(GL_COLOR_ATTACHMENT0) == formats.end()) 
			addAttachment(att, { GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR,GL_LINEAR, GL_REPEAT, GL_REPEAT });
	else	addAttachment(att, formats[GL_COLOR_ATTACHMENT0]);
	
}

//default initialization
void FrameBufferObject::init() {
	glGenFramebuffers(1, &fboID);
	//glGenRenderbuffers(1, &depID);

	//render buffer is like a texture but you can access directly without weird color conversions
	//so its faster and that's why it's better to use it for the depth attachment (it's not really a texture)
	//in a nutshell: use render buffers if you're never going to use them as a texture
	//you cannot access a renderbuffer from a shared in any way

	//glBindFramebuffer(GL_FRAMEBUFFER, fboID);

		//GLint drawFboId = 0, readFboId = 0;
		//glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
		//glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);
		//std::cout << "FBO IDs: " << drawFboId << " " << readFboId << " " << fboID << "\n";

		addAttachment(GL_COLOR_ATTACHMENT0);
		addAttachment(GL_DEPTH_STENCIL_ATTACHMENT);

		//glBindRenderbuffer(GL_RENDERBUFFER, depID);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, sizeX, sizeY);

		//glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depID);

		compCheck();

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//std::cout << "Framebuffer initialized.";
}

void FrameBufferObject::bind() {
	oldres = cst::res();
	cst::res(vec2(sizeX, sizeY));

	util::bindFramebuffer(fboID);

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, sizeX, sizeY);
}

void FrameBufferObject::unbind() {
	glPopAttrib();
	
	util::unbindFramebuffer();
	//std::cout << "unbind oldbuf " << oldBufferID << "\n";

	cst::res(oldres);
}

void FrameBufferObject::resize(int sizeX, int sizeY) {
	if (sizeX <= 0 || sizeY <= 0) return;

	if (!(this->sizeX == sizeX && this->sizeY == sizeY)) {
		this->sizeX = sizeX;
		this->sizeY = sizeY;

		for (auto& it : attachments) {
			if (isNotRenderBuffer(it.first)) {
				if (formats.find(it.first) == formats.end()) { std::cout << "fbo " << fboID << " error: could not find format for attachment " << it.first << "\n"; return; }
				ColorAttachment fm = formats[it.first];
				glBindTexture(GL_TEXTURE_2D, it.second);

					glTexImage2D(GL_TEXTURE_2D, 0, fm.internalFormat, sizeX, sizeY, 0, fm.format, fm.type, NULL);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, fm.minFilter);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, fm.magFilter);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, fm.wrap_s);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, fm.wrap_t);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		//std::cout << "check\n";
		if (getAttachment(GL_DEPTH_ATTACHMENT) != 0) {
			glBindRenderbuffer(GL_RENDERBUFFER, attachments[GL_DEPTH_ATTACHMENT]);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, sizeX, sizeY);
		}
		if (getAttachment(GL_STENCIL_ATTACHMENT) != 0) {
			glBindRenderbuffer(GL_RENDERBUFFER, attachments[GL_STENCIL_ATTACHMENT]);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_COMPONENTS/*probably wrong*/, sizeX, sizeY);
		}
		if (getAttachment(GL_DEPTH_STENCIL_ATTACHMENT) != 0) {
			glBindRenderbuffer(GL_RENDERBUFFER, attachments[GL_DEPTH_STENCIL_ATTACHMENT]);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY);
		}

		compCheck();
	}
}
void FrameBufferObject::resize(vec2 size) { resize(size.x, size.y); }

GLuint FrameBufferObject::getID() const {
	return fboID;
}

//GLuint FrameBufferObject::getDepID() { return depID; }

GLuint FrameBufferObject::getAttachment(GLenum att) const {
	auto it = attachments.find(att);
	if (it == attachments.end()) { /*std::cout << "FBO " << fboID << ": warning: could not find attachment " << util::fboAttachmentToString(att)  << "\n";*/ return 0; }
	//return attachments[att];

	return it->second;
}
GLuint FrameBufferObject::getPrimaryColorAttachment() const { return getAttachment(GL_COLOR_ATTACHMENT0);  }

int FrameBufferObject::sizex() const { return sizeX; }
int FrameBufferObject::sizey() const { return sizeY; }
vec2 FrameBufferObject::size() const { return { (float)sizeX, (float)sizeY }; }

void FrameBufferObject::clear(vec4 color){
	if (sizeX <= 0 || sizeY <= 0) return;

	GLint oldb = prc::fboid;
	//glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldb);

	if(oldb != fboID) bind();
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	if (oldb != fboID) unbind();
}

void FrameBufferObject::clear() { clear(vc4::black); }

void FrameBufferObject::setFiltering(int minFilter, int magFilter) {
	//this->minFilter = minFilter;
	//this->magFilter = magFilter;
	for (auto& it : attachments) {
		if (isNotRenderBuffer(it.first)) {
			if (formats.find(it.first) == formats.end()) { std::cout << "fbo " << fboID << " error: could not find format for attachment " << it.first << "\n"; return; }
			ColorAttachment fm = formats[it.first];
			glBindTexture(GL_TEXTURE_2D, it.second);

				glTexImage2D(GL_TEXTURE_2D, 0, fm.internalFormat, sizeX, sizeY, 0, fm.format, fm.type, NULL);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

			glBindTexture(GL_TEXTURE_2D, 0);

			formats[it.first].minFilter = minFilter;
			formats[it.first].magFilter = magFilter;
		}
	}
}

void FrameBufferObject::setFiltering(int filtering) {
	setFiltering(filtering, filtering);
}

GLenum FrameBufferObject::getMinFiltering(GLenum att) {
	if (attachments.find(att) == attachments.end()) { std::cout << "FBO: " << fboID << ": error: attachment " << att << " doesn't exist\n"; return 0; }

	return formats[att].minFilter;
}
GLenum FrameBufferObject::getMinFiltering() {return getMinFiltering(GL_COLOR_ATTACHMENT0);}

GLenum FrameBufferObject::getMagFiltering(GLenum att) {
	if (attachments.find(att) == attachments.end()) { std::cout << "FBO: " << fboID << ": error: attachment " << att << " doesn't exist\n"; return 0; }

	return formats[att].magFilter;
}
GLenum FrameBufferObject::getMagFiltering() {return getMagFiltering(GL_COLOR_ATTACHMENT0);}

void FrameBufferObject::setWrapMode(int wrap_s, int wrap_t) {
	//this->wrap_s = wrap_s;
	//this->wrap_t = wrap_t;
	for (auto& it : attachments) {
		if (isNotRenderBuffer(it.first)) {
			if (formats.find(it.first) == formats.end()) { std::cout << "fbo " << fboID << " error: could not find format for attachment " << it.first << "\n"; return; }
			ColorAttachment fm = formats[it.first];
			glBindTexture(GL_TEXTURE_2D, it.second);

				glTexImage2D(GL_TEXTURE_2D, 0, fm.internalFormat, sizeX, sizeY, 0, fm.format,fm. type, NULL);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

			glBindTexture(GL_TEXTURE_2D, 0);

			formats[it.first].wrap_s = wrap_s;
			formats[it.first].wrap_t = wrap_t;
		}
	}
}

void FrameBufferObject::setWrapMode(int wrapMode) {
	setWrapMode(wrapMode, wrapMode);
}

bool FrameBufferObject::isNotRenderBuffer(GLenum att) {
	return  att != GL_DEPTH_ATTACHMENT &&
		att != GL_STENCIL_ATTACHMENT &&
		att != GL_DEPTH_STENCIL_ATTACHMENT;
}

GLenum FrameBufferObject::getWrap_s(GLenum att) {
	if (attachments.find(att) == attachments.end()) { std::cout << "FBO: " << fboID << ": error: attachment " << att << " doesn't exist\n"; return 0; }
	return formats[att].wrap_s;
}
GLenum FrameBufferObject::getWrap_s() {return getWrap_s(GL_COLOR_ATTACHMENT0);}

GLenum FrameBufferObject::getWrap_t(GLenum att) {
	if (attachments.find(att) == attachments.end()) { std::cout << "FBO: " << fboID << ": error: attachment " << att << " doesn't exist\n"; return 0; }
	return formats[att].wrap_t;
}
GLenum FrameBufferObject::getWrap_t() {return getWrap_t(GL_COLOR_ATTACHMENT0);}

void FrameBufferObject::setFormat(GLenum att, GLenum type, GLint internalFormat, GLenum format) {
	if (attachments.find(att) == attachments.end()) { std::cout << "FBO: " << fboID << ": error: attachment " << att << " doesn't exist\n"; return; }

	formats[att].type = type;
	formats[att].internalFormat = internalFormat;
	formats[att].format = format;

	glBindTexture(GL_TEXTURE_2D, attachments[att]);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, sizeX, sizeY, 0, format, type, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, formats[att].minFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, formats[att].magFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, formats[att].wrap_s);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, formats[att].wrap_t);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBufferObject::setFormat(GLenum type, GLint internalFormat, GLenum format) { 
	for (auto& a : attachments) 
		if (isNotRenderBuffer(a.second)) {
			setFormat(a.second, type, internalFormat, format); 
		}
}

GLenum FrameBufferObject::getInternalFormat(GLenum att) {
	if (attachments.find(att) == attachments.end()) { std::cout << "FBO: " << fboID << ": error: attachment " << att << " doesn't exist\n"; return 0; }
	return formats[att].internalFormat;
}
GLenum FrameBufferObject::getInternalFormat() {return getInternalFormat(GL_COLOR_ATTACHMENT0);}

void FrameBufferObject::removeAttachment(GLenum att) {
	if (att == GL_DEPTH_ATTACHMENT)				glDeleteRenderbuffers(1, &attachments[GL_DEPTH_ATTACHMENT]);
	else if (att == GL_STENCIL_ATTACHMENT)		glDeleteRenderbuffers(1, &attachments[GL_STENCIL_ATTACHMENT]);
	else if (att == GL_DEPTH_STENCIL_ATTACHMENT) glDeleteRenderbuffers(1, &attachments[GL_DEPTH_STENCIL_ATTACHMENT]);
	else { glDeleteTextures(1, &attachments[att]); formats.erase(att); }
	
	attachments.erase(att);
}

void FrameBufferObject::dispose() {
	for (auto& it : attachments) {
		if (isNotRenderBuffer(it.first)) glDeleteTextures(1, &it.second);
	}

	if(getAttachment(attachments[GL_DEPTH_ATTACHMENT]) != 0) glDeleteRenderbuffers(1, &attachments[GL_DEPTH_ATTACHMENT]);
	if(getAttachment(attachments[GL_STENCIL_ATTACHMENT]) != 0) glDeleteRenderbuffers(1, &attachments[GL_STENCIL_ATTACHMENT]);
	if(getAttachment(attachments[GL_DEPTH_STENCIL_ATTACHMENT]) != 0) glDeleteRenderbuffers(1, &attachments[GL_DEPTH_STENCIL_ATTACHMENT]);
	
	glDeleteFramebuffers(1, &fboID);
}

void FrameBufferObject::compCheck() {
	GLenum check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (check) {
		case GL_FRAMEBUFFER_COMPLETE:

		break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "FrameBuffer: " << fboID << ", has caused a GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT exception\n";
			glfwTerminate();
		break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "FrameBuffer: " << fboID << ", has caused a GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT exception\n";
			glfwTerminate();
		break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout << "FrameBuffer: " << fboID << ", has caused a GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER exception\n";
		break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout << "FrameBuffer: " << fboID << ", has caused a GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER exception\n";
			glfwTerminate();
		break;
		default:
			std::cout << "Unexpected reply from glCheckFramebufferStatus: " << check << ".\n";
			glfwTerminate();
		break;
	}
}

FrameBufferObject::FrameBufferObject(){
	
}

FrameBufferObject::FrameBufferObject(int sizeX, int sizeY) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	init();
}

FrameBufferObject::FrameBufferObject(int sizeX, int sizeY, int minFilter, int magFilter) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	//this->minFilter = minFilter;
	//this->magFilter = magFilter;
	init();
}

FrameBufferObject::FrameBufferObject(int sizeX, int sizeY, int minFilter, int magFilter, int internalFormat, int format, int type) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	//this->minFilter = minFilter;
	//this->magFilter = magFilter;
	//this->internalFormat = internalFormat;
	//this->format = format;
	//this->type = type;
	init();
}

//attachments must not contain duplicates
FrameBufferObject::FrameBufferObject(vec2 size, std::vector<GLenum> attachments, FrameBufferObject::ColorAttachment format) {
	this->sizeX = (int)size.x;
	this->sizeY = (int)size.y;

	glGenFramebuffers(1, &fboID);

	for (GLuint a : attachments) {
		//if (a == GL_DEPTH_ATTACHMENT) {
		//	std::cout << "adding depth\n";
		//	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		//		
		//		glGenRenderbuffers(1, &depID);
		//		glBindRenderbuffer(GL_RENDERBUFFER, depID);

		//			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, sizeX, sizeY);

		//		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depID);

		//		compCheck();

		//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//	//addAttachment(a);
		//}
		//else addAttachment(a);
		addAttachment(a, format);
	}
}

FrameBufferObject::FrameBufferObject(vec2 size, std::vector<GLenum> attachments) : 
	FrameBufferObject(size, attachments, FrameBufferObject::ColorAttachment{ GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR,GL_LINEAR, GL_REPEAT, GL_REPEAT }) {}

FrameBufferObject::~FrameBufferObject(){
	//dispose();
}
