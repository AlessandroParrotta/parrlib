#include "Texture.h"

#include "Util.h"

#include "stringutils.h"

void Texture::fillRegion(vec2 pos, vec2 size, GLenum format, const void* data){
	//std::cout << (GLint)pos.x << " " << (GLint)pos.y << " " << (GLint)size.x << " " << (GLint)size.y << "\n";
	bind();
		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)pos.x, (GLint)pos.y, (GLint)size.x, (GLint)size.y, format, type, data);
	unbind();
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

void Texture::setData(const void* data) {
	bind();
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, border, format, type, data);
	unbind();
}

template<typename T> void Texture::setDataT(std::vector<T> val) {
	int chnum = getChannelNumber(internalFormat);
	if (val.size() < chnum) return;

	size_t s = size.x * size.y * chnum;
	T* vals = new T[s];
	for (size_t i = 0; i < size.x * size.y; i++) {
		for (int j = 0; j < chnum; j++) { vals[i + j] = val[j]; }
	}

	bind();
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, border, format, type, vals);
	unbind();
	
	delete[] vals;
}
template void Texture::setDataT<unsigned char>	(std::vector<unsigned char> val);
template void Texture::setDataT<int>			(std::vector<int> val);
template void Texture::setDataT<float>			(std::vector<float> val);
void Texture::setData(float val)			{ setDataT<float>({val}); }
void Texture::setData(unsigned char val)	{ setDataT<unsigned char>({ val }); }
void Texture::setData(int val)				{ setDataT<int>({ val }); }
void Texture::setData(vec2 val) { 
	switch (type) {
	case GL_UNSIGNED_BYTE:	setDataT<unsigned char>	({ (unsigned char)val.x, (unsigned char)val.y });
	case GL_FLOAT:			setDataT<float>			({ (float)val.x, (float)val.y });
	case GL_INT:			setDataT<int>			({ (int)val.x, (int)val.y });
	}
}
void Texture::setData(vec3 val) { 
	switch (type) {
	case GL_UNSIGNED_BYTE:	setDataT<unsigned char>	({ (unsigned char)val.x, (unsigned char)val.y, (unsigned char)val.z });
	case GL_FLOAT:			setDataT<float>			({ (float)val.x, (float)val.y, (float)val.z });
	case GL_INT:			setDataT<int>			({ (int)val.x, (int)val.y, (int)val.z });
	}
}
void Texture::setData(vec4 val) { 
	switch (type) {
	case GL_UNSIGNED_BYTE:	setDataT<unsigned char>	({ (unsigned char)val.x, (unsigned char)val.y,(unsigned char)val.z, (unsigned char)val.w });
	case GL_FLOAT:			setDataT<float>			({ (float)val.x, (float)val.y, (float)val.z, (float)val.w });
	case GL_INT:			setDataT<int>			({ (int)val.x, (int)val.y, (int)val.z, (int)val.w });
	}
}

void Texture::resize(vec2 size) {
	if (null() || size.x < 0.f || size.y < 0.f) return;

	//alignment means find the next multiple of ALIGNMENT that is higher than the width (in pixels) of the row
	//so GL_UNSIGNED_BYTE, GL_RGB and 9 width, 27 bytes in all, with 8 alignment the next row is at 32 
	//cause 32 is the closest multiple of 8 higher than 27
	bind();
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	unbind();

	int chnum = getChannelNumber(internalFormat);
	void* restore = nullptr; // new unsigned char[size.x * size.y * chnum];
	int sizet = size.x * size.y * chnum;

	switch (type) {
	case GL_UNSIGNED_BYTE:	restore = new unsigned char[sizet]; break;
	case GL_FLOAT:			restore = new float[sizet]; break;
	case GL_INT:			restore = new int[sizet]; break;
	case GL_UNSIGNED_INT:	restore = new unsigned int[sizet]; break;
	default: std::cout << "error resize: unknown type\n"; return;
	}

	void* data = getData();
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			int idx = (y * size.x + x) * chnum;

			for (int i = 0; i < chnum; i++) {
				switch (type) {
				case GL_UNSIGNED_BYTE:	((unsigned char*)restore)	[idx + i] = (unsigned char)0;	break;
				case GL_FLOAT:			((float*)restore)			[idx + i] = 0.f;				break;
				case GL_INT:			((int*)restore)				[idx + i] = 0;					break;
				case GL_UNSIGNED_INT:	((unsigned int*)restore)	[idx + i] = 0;					break;
				default: std::cout << "error resize: unknown type\n";	return;
				}
			}
		}
	}
	for (int y = 0; y < this->size.y; y++) {
		for (int x = 0; x < this->size.x; x++) {
			if (x < size.x && y < size.y) {
				int idx = (y * size.x + x) * chnum;
				int dataIdx = (y * this->size.x + x) * chnum;

				for (int i = 0; i < chnum; i++) {
					switch (type) {
					case GL_UNSIGNED_BYTE:	((unsigned char*)restore)	[idx + i] = ((unsigned char*)data)	[dataIdx + i];	break;
					case GL_FLOAT:			((float*)restore)			[idx + i] = ((float*)data)			[dataIdx + i];	break;
					case GL_INT:			((int*)restore)				[idx + i] = ((int*)data)			[dataIdx + i];	break;
					case GL_UNSIGNED_INT:	((unsigned int*)restore)	[idx + i] = ((unsigned int*)data)	[dataIdx + i];	break;
					default: std::cout << "error resize: unknown type\n";	return;
					}
				}
			}
		}
	}

	//unsigned char* data = (unsigned char*)getData();
	//unsigned char* restore = new unsigned char[size.x * size.y * 4];
	//for (int y = 0; y < size.y; y++) {
	//	for (int x = 0; x < size.x; x++) {
	//		int idx = (y * size.x + x) * 4;
	//		restore[idx + 0] = (unsigned char)0;
	//		restore[idx + 1] = (unsigned char)0;
	//		restore[idx + 2] = (unsigned char)0;
	//		restore[idx + 3] = (unsigned char)0;
	//	}
	//}
	//for (int y = 0; y < this->size.y; y++) {
	//	for (int x = 0; x < this->size.x; x++) {
	//		if (x < size.x && y < size.y) {
	//			int idx = (y * size.x + x) * 4;
	//			int dataIdx = (y * this->size.x + x) * 4;

	//			restore[idx + 0] = data[dataIdx + 0];
	//			restore[idx + 1] = data[dataIdx + 1];
	//			restore[idx + 2] = data[dataIdx + 2];
	//			restore[idx + 3] = data[dataIdx + 3];
	//		}
	//	}
	//}

	bind();
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, NULL);
	unbind();
	fillRegion(0.f, size, restore);

	delete[] restore;
	delete[] data;

	this->size = size;
}

int Texture::getChannelNumber(GLenum format) {
	if (		format == GL_RGBA)		return 4;
	else if (	format == GL_RGB)		return 3;
	else if (	format == GL_RG)		return 2;
	else if (	format == GL_RED	|| 
				format == GL_GREEN	|| 
				format == GL_BLUE)		return 1;
	else if (	format == GL_ALPHA)		return 1;
	else								return -1;
}

int Texture::getChannelNumber() { return getChannelNumber(internalFormat); }

template<typename T>
void* Texture::getDataT(GLenum format){
	if (null()) return nullptr;
	void* buf = nullptr;

	buf = new T[size.x * size.y * getChannelNumber(format)];
	//std::cout << "created void* buf of type " << util::glCodeToString(type) << " " << size.x << "x" << size.y << " * " << getChannelNumber(format) << "\n";
	
	bind();
		glGetTexImage(GL_TEXTURE_2D, 0, format, type, buf);
	unbind();

	return buf;
}
template void* Texture::getDataT<unsigned char*>(GLenum format);
template void* Texture::getDataT<float*>(GLenum format);
template void* Texture::getDataT<int*>(GLenum format);

void* Texture::getData(GLenum format) {
	//std::cout << "getData called with format " << util::textureFormatToString(format) << "\n";
	if (type == GL_UNSIGNED_BYTE)	return getDataT<unsigned char*>(format);
	else if (type == GL_FLOAT)		return getDataT<float*>(format);
	else if (type == GL_INT)		return getDataT<int*>(format);
	else							return nullptr;
}
void* Texture::getData() { return getData(internalFormat); }

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

bool Texture::null() { return texID == 0; }

void Texture::draw(mat3 transform) {
	util::pushMatrix(transform);
		util::drawTexture(texID, -1.f, 2.f, 0.f);
	util::popMatrix();
}
void Texture::draw(vec2 pos, vec2 size) { draw(pmat3::translate(pos) * pmat3::scale(size)); }

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadTex(std::string const& fileName) {
	if (texID != 0) dispose();

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

	std::string tfileName = stru::fallbackPath(fileName);

	texID = SOIL_load_OGL_texture(tfileName.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texID == 0) return;

	int w = 0, h = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	glBindTexture(GL_TEXTURE_2D, 0);

	size = vec2(w, h);
}

Texture::Texture(){

}

Texture::Texture(GLuint texID, vec2 size) {
	this->texID = texID;
	this->size = size;
}

Texture::Texture(GLuint texID) {
	this->texID = texID;

	int w = 0, h=0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	glBindTexture(GL_TEXTURE_2D, 0);

	size = vec2(w, h);
}

Texture::Texture(std::string const& fileName) {
	loadTex(fileName);
}

Texture::Texture(const char* fileName) {
	loadTex(fileName);
}
Texture::Texture(const unsigned char* data, vec2 size) {
	this->size = size;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(vec4 initCol, vec2 size) {
	internalFormat = GL_RGBA;
	format = GL_RGBA;
	type = GL_UNSIGNED_BYTE;

	unsigned char* data = new unsigned char[size.x * size.y * 4];
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			int idx = (y * size.x + x) * 4;

			data[idx + 0] = (unsigned char)(initCol.x*255.f);
			data[idx + 1] = (unsigned char)(initCol.y*255.f);
			data[idx + 2] = (unsigned char)(initCol.z*255.f);
			data[idx + 3] = (unsigned char)(initCol.w*255.f);
		}
	}

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] data;

	this->size = size;
}

Texture::Texture(vec2 size, GLint internalFormat, GLenum format, GLenum type) {
	this->size = size;
	this->internalFormat = internalFormat;
	this->format = format;
	this->type = type;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::Texture(vec2 size) : Texture(size, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE) {}


Texture::~Texture(){

}

void Texture::dispose() {
	if (!texID) return;
	glDeleteTextures(1, &texID);
	texID = 0;
	size = 0.f;
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
