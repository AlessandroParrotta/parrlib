#include "texture.h"

#include <SOIL/SOIL.h>

#include "../parrlib/math/stringutils.h"

#include "statemachine.h" 

namespace softst {
	Texture::Texture() {  }
	Texture::Texture(std::wstring const& path) { 
		int w = 0, h = 0;
		int channels = 0;
		unsigned char* data = SOIL_load_image(stru::tostr(path).c_str(), &w,&h, &channels, SOIL_LOAD_RGBA);

		this->data = data;
		size.x = w;
		size.y = h;
	}
	Texture::Texture(unsigned char* data, vec2 size) { 
		this->size = size;
		this->data = new unsigned char[size.x * size.y * 4];
		memcpy(this->data, data, size.x * size.y * 4);
	}
	Texture::Texture(unsigned char* data, vec2 size, int linesize) { 
		this->size = size;
		this->data = new unsigned char[size.x * size.y * 4];
		memcpy(this->data, data, size.x * size.y * 4);
	}
	Texture::Texture(vec2 size) { 
		this->size = size;
		this->data = new unsigned char[size.x * size.y * 4];
		memset(this->data, 0, size.x * size.y * 4);
	}
	Texture::Texture(vec2 size, int linesize) {
		this->size = size;
		this->data = new unsigned char[size.x * size.y * 4];
		memset(this->data, 0, size.x * size.y * 4);
	}

	void Texture::fillRegion(unsigned char* subData, vec2 start, vec2 end) {

	}
	std::vector<unsigned char> Texture::getData() const {
		std::vector<unsigned char> res; res.resize(size.x * size.y * 4);
		for (int i = 0; i < size.x * size.y * 4; i++) res[i] = data[i];
		return res;
	}

	void Texture::resize(vec2 newSize) {
		delete[] data;
		this->data = new unsigned char[newSize.x * newSize.y * 4];
	}
	vec2 Texture::getSize() const {
		return size;
	}

	void Texture::setData(unsigned char* data, vec2 size, int linesize) {

	}
	void Texture::setData(unsigned char* data, vec2 size) {

	}
	void Texture::drawImmediate(std::vector<float> const& verts) const {

	}
	void Texture::drawImmediate(vec2 pos, vec2 size) const {

	}
	void Texture::bind() const {
		statemachine::bindTexture(this);
		//statemachine::a();
	}
	void Texture::dispose() { delete[] data; }

	bool Texture::null() const { return data; }

	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_1() { return new Texture(); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_2(std::wstring const& path) { return new Texture(path); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_3(unsigned char* data, vec2 size) { return new Texture(data, size); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_4(unsigned char* data, vec2 size, int linesize) { return new Texture(data, size, linesize); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_5(vec2 size) { return new Texture(size); }
	extern "C" __declspec(dllexport) impl::Texture_base * createTexture_6(vec2 size, int linesize) { return new Texture(size, linesize); }
}
