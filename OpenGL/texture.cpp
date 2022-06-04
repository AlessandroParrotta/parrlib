#include "Texture.h"

#include "../parrlib/stringutils.h"

#include "../parrlib/util.h"
#include "../parrlib/debug.h"
#include "common.h"

#include "vertexarrayobject.h"


namespace gl {
	size_t getSizeOf(GLenum type) {
		switch (type) {
		case GL_UNSIGNED_BYTE:	return sizeof(unsigned char);
		case GL_UNSIGNED_INT:	return sizeof(unsigned int);
		case GL_INT:			return sizeof(int);
		case GL_FLOAT:			return sizeof(float);
		}
		return 0;
	}

	size_t Texture::getByteSize() {
		return size.x * size.y * channels * getSizeOf(type);
	}

	void* cpy(const void* src, size_t size) { void* res = new unsigned char[size]; memcpy(res, src, size); return res; }

	int getChannels(GLenum format) {
		if (format == GL_RGBA)		return 4;
		else if (format == GL_RGB)		return 3;
		else if (format == GL_RG)		return 2;
		else if (format == GL_RED ||
			format == GL_GREEN ||
			format == GL_BLUE)		return 1;
		else if (format == GL_ALPHA)		return 1;
		else								return -1;
	}


	Texture::Texture() {

	}

	Texture::Texture(GLuint texID, vec2 size) {
		this->id = texID;
		this->size = size;

		//TODO
	}

	Texture::Texture(GLuint texID) {
		this->id = texID;

		int w = 0, h = 0;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glBindTexture(GL_TEXTURE_2D, 0);

		size = vec2(w, h);

		//TODO format channels
	}

	Texture::Texture(std::string const& fileName) {
		loadTex(fileName);
	}

	Texture::Texture(const char* fileName) {
		loadTex(fileName);
	}

	Texture::Texture(vec4 initCol, vec2 size) {
		internalFormat = GL_RGBA;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		channels = getChannels(format);

		unsigned char* data = new unsigned char[size.x * size.y * 4];
		for (int y = 0; y < size.y; y++) {
			for (int x = 0; x < size.x; x++) {
				int idx = (y * size.x + x) * 4;

				data[idx + 0] = (unsigned char)(initCol.x * 255.f);
				data[idx + 1] = (unsigned char)(initCol.y * 255.f);
				data[idx + 2] = (unsigned char)(initCol.z * 255.f);
				data[idx + 3] = (unsigned char)(initCol.w * 255.f);
			}
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		this->data = data;
		//delete[] data;

		this->size = size;
	}

	Texture::Texture(vec2 size, GLint internalFormat, GLenum format, GLenum type) {
		this->size = size;
		this->internalFormat = internalFormat;
		this->format = format;
		this->type = type;
		channels = getChannels(format);
		//deb::debss << "initializing texture " << size << " " << getByteSize() << "\n"; deb::outStr();

		data = new unsigned char[getByteSize()]; memset(data, 0, getByteSize());

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Texture::Texture(vec2 size) : Texture(size, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE) {}

	Texture::Texture(const void* data, vec2 size, GLint internalFormat, GLenum format, GLenum type) {
		this->size = size;
		this->internalFormat = internalFormat;
		this->format = format;
		this->type = type;
		channels = getChannels(format);

		//deb::debss << "initializing texture " << size << " " << getByteSize() << " " << data << " " << this->data <<"\n"; deb::outStr();

		this->data = cpy(data, getByteSize());
		//for (int i = 0; i < getByteSize(); i++) {
		//	deb::debss << (int)((unsigned char*)this->data)[i] << " == " << (int)((unsigned char*)data)[i] << "\n"; deb::outStr();
		//}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, this->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		fillRegion((unsigned char*)data, 0.f, size);

		if (com::checkForGLErrors()) std::terminate();
	}
	Texture::Texture(const unsigned char* data, vec2 size) : Texture(data, size, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE){}

	Texture::~Texture() {

	}

	void Texture::fillRegion(vec2 pos, vec2 size, GLenum format, const void* data) {
		pos = pos.minned(getSize()).maxed(0.f);
		vec2 end = (pos + size).minned(getSize()).maxed(0.f);
		vec2 oldSize = size;
		size = (end - pos).minned(oldSize);

		//deb::debss << pos << " " << size << " " << this->size << "\n"; deb::outStr();

		for (int y = (int)pos.y; y < (int)end.y; y++) {
			int srcIdx = (y - (int)pos.y) * size.x + 0;
			int dstIdx = y * this->size.x + (int)pos.x;
			memcpy(&(((unsigned char*)this->data)[dstIdx * 4]), &(((unsigned char*)data)[srcIdx * 4]), (size.x * 4));
		}

		//for (int y = (int)start.y; y < (int)end.y; y++) {
		//	int srcIdx = (y - (int)start.y) * width + 0;
		//	int dstIdx = y * this->width + (int)start.x;
		//	memcpy(&(data[dstIdx * 4]), &(subData[srcIdx * 4]), (width * 4));
		//}

		//std::cout << (GLint)pos.x << " " << (GLint)pos.y << " " << (GLint)size.x << " " << (GLint)size.y << "\n";
		bind();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)pos.x, (GLint)pos.y, (GLint)size.x, (GLint)size.y, format, type, data);
		unbind();

		if (com::checkForGLErrors()) std::terminate();
	}
	void Texture::fillRegion(vec2 pos, vec2 size, const void* data) { fillRegion(pos, size, internalFormat, data); }

	void Texture::fillRegion(vec2 pos, vec2 size, vec4 col) {
		const unsigned char px[] = {
			(unsigned char)(col.x * 255.f),
			(unsigned char)(col.y * 255.f),
			(unsigned char)(col.z * 255.f),
			(unsigned char)(col.w * 255.f)
		};

		unsigned char* data = new unsigned char[size.x * size.y * 4];
		for (int i = 0; i < size.x * size.y; i++) {
			data[i * 4 + 0] = px[0];
			data[i * 4 + 1] = px[1];
			data[i * 4 + 2] = px[2];
			data[i * 4 + 3] = px[3];
		}

		fillRegion(pos, size, data);

		delete[] data;
	}

	void Texture::fillRegion(unsigned char* subData, vec2 start, vec2 size) {
		fillRegion(start, size, subData);
		if (com::checkForGLErrors()) std::terminate();
	}

	void Texture::setPixel(vec2 pos, vec4 col) {
		if (pos.x < 0.f || pos.y < 0.f || pos.x > size.x || pos.y > size.y) return;

		const unsigned char data[] = {
			(unsigned char)(col.x * 255.f),
			(unsigned char)(col.y * 255.f),
			(unsigned char)(col.z * 255.f),
			(unsigned char)(col.w * 255.f)
		};

		bind();
		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)pos.x, (GLint)pos.y, 1, 1, format, type, data);
		unbind();
	}

	vec2 Texture::getSize() const {
		return size;
	}

	void Texture::setFiltering(GLint min, GLint mag) {
		minfilter = min;
		magfilter = mag;
		bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		unbind();
	}

	void Texture::setFiltering(GLint filtering) { setFiltering(filtering, filtering); }

	void Texture::setWrap(GLint s, GLint t) {
		bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
		unbind();
	}

	void Texture::setUnpackAligment(GLint val) {
		bind();
		glPixelStorei(GL_UNPACK_ALIGNMENT, val);
		unbind();
	}

	bool Texture::null() const { return id == 0; }

	void Texture::draw(mat3 transform) const {
		//util::drawTexture(*this, transform);
		//TODO
	}
	void Texture::draw(vec2 pos, vec2 size) const { draw(pmat3::translate(pos) * pmat3::scale(size)); }

	void Texture::drawImmediate(std::vector<float> const& verts) const {
		VAO vao = { verts, { 2,4,2 } };
		vao.setData(verts, {2,4,2});

		vao.bind();
			vao.draw();
		vao.unbind();

		vao.dispose();
	}

	void Texture::drawImmediate(vec2 pos, vec2 size) const {
		drawImmediate({
			pos.x, pos.y,  
		});
	}

	void Texture::bind() const {
		glBindTexture(GL_TEXTURE_2D, id);
		glEnable(GL_TEXTURE_2D);
	}

	void Texture::unbind() const {
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::loadTex(std::string const& fileName) {
		if (id != 0) dispose();

		//int width, height;
		//unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		//if (!image) { std::cout << "could not load image '" + fileName + "'\n"; return; }

		////i 'force' the formats to adhere to the image i'm loading
		//internalFormat = GL_RGBA;
		//format = GL_RGBA;
		//type = GL_UNSIGNED_BYTE;

		//GLuint id = 0;
		//glGenTextures(1, &id);
		//glBindTexture(GL_TEXTURE_2D, id);
		//	//probably format is the format of the input pointer (that you give to GL), then the texture is converted to internalFormat inside GL
		//	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, image);
		//	//you HAVE to set the parameters otherwise the texture just doesn't show up (that's because of mipmapping)
		//	//by default GL_TEXTURE_MIN_FILTER is set to MIPMAP, so in order to draw the texture you need to change it to a non-MIPMAP type (GL_LINEAR or GL_NEAREST)
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //this is the only one you need to explicitly set if you're not using mipmaps
		//glBindTexture(GL_TEXTURE_2D, 0);

		//SOIL_free_image_data(image);

		////std::cout << width << " " << height << "\n";

		//size = vec2(width, height);
		//texID = id;

		std::string tfileName = strup::fallbackPath(fileName);

		id = SOIL_load_OGL_texture(tfileName.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (id == 0) deb::outStr("OpenGL BACKEND ERROR: texture ", fileName.c_str(), " could not be created (maybe file does not exist?)\n");
		if (id == 0) return;

		int w = 0, h = 0;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glBindTexture(GL_TEXTURE_2D, 0);

		channels = 4; //soil load rgba

		size = vec2(w, h);
	}

	void Texture::resize(vec2 size) {
		std::vector<std::byte> nData((int64_t)size.x * (int64_t)size.y * (int64_t)channels * (int64_t)getSizeOf(type), (std::byte)0);

		//deb::debss << "resize " << size << " " << nData.size() << " (" << (size.x * size.y * channels * getSizeOf(type)) << " " << channels << " " << getSizeOf(format) << ")\n"; deb::outStr();

		Texture t(&nData[0], size, internalFormat, format, type);
		//if (size.x > this->size.x || size.y > this->size.y) ;
		t.fillRegion(0.f, this->size, data);
		dispose();
		*this = t;
	}

	void* Texture::getDataRaw() const {
		if (!data) return NULL;

		switch (type) {
		case GL_UNSIGNED_BYTE:	{ unsigned char* res = new unsigned char[size.x * size.y * channels];	memcpy(res, data, size.x * size.y * channels * sizeof(unsigned char));	return res; }break;
		case GL_UNSIGNED_INT:	{ unsigned int* res = new unsigned int[size.x * size.y * channels];		memcpy(res, data, size.x * size.y * channels * sizeof(unsigned int));	return res; }break;
		case GL_INT:			{ int* res = new int[size.x * size.y * channels];						memcpy(res, data, size.x * size.y * channels * sizeof(int));			return res; }break;
		case GL_FLOAT:			{ float* res = new float[size.x * size.y * channels];					memcpy(res, data, size.x * size.y * channels * sizeof(float));			return res; }break;
		}
		return NULL;
	}

	std::vector<unsigned char> Texture::getData() const {	
		if (!data) return {};

		std::vector<unsigned char> res; res.resize(size.x * size.y * channels);
		
		switch (type) {
		case GL_UNSIGNED_BYTE:	{	unsigned char* px = (unsigned char*)data;	for (int i = 0; i < size.x * size.y * channels; i++) res[i] = (unsigned char)px[i]; }break;
		case GL_UNSIGNED_INT:	{	unsigned int* px = (unsigned int*)data;		for (int i = 0; i < size.x * size.y * channels; i++) res[i] = (unsigned char)px[i]; }break;
		case GL_INT:			{	int* px = (int*)data;						for (int i = 0; i < size.x * size.y * channels; i++) res[i] = (unsigned char)px[i]; }break;
		case GL_FLOAT:			{	float* px = (float*)data;					for (int i = 0; i < size.x * size.y * channels; i++) res[i] = (unsigned char)px[i]; }break;
		}

		return res;
	}

	void Texture::setData(void* data) {
		if (null()) return;

		bind();
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, border, format, type, data);
		unbind();
	}

	void Texture::setData(unsigned char* data, vec2 size) {
		resize(size); setData(data);
	}

	void Texture::setData(unsigned char* data, vec2 size, int linesize) {
		setData(data, size);
	}

	void Texture::dispose() {
		if (!id) return;
		glDeleteTextures(1, &id);
		id = 0;
		size = 0.f;
		delete[] data;
	}

	//void Texture::draw(float x, float y, float sizeX, float sizeY, float rotation) {
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, texID);
	//	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//	glBegin(GL_QUADS);
	//		glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
	//		glTexCoord2f(1.0f, 0.0f); glVertex2f(x + sizeX, y);
	//		glTexCoord2f(1.0f, 1.0f); glVertex2f(x + sizeX, y + sizeY);
	//		glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + sizeY);
	//	glEnd();
	//
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}


	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_1() { return new Texture(); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_2(std::wstring const& path) { return new Texture(stru::tostr(path)); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_3(unsigned char* data, vec2 size) { return new Texture(data, size); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_4(unsigned char* data, vec2 size, int linesize) { return new Texture(data, size); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_5(vec2 size) { return new Texture(size); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_6(vec2 size, int linesize) { return new Texture(size); }

}
