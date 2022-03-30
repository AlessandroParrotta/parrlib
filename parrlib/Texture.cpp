#include "Texture.h"

#include "math/stringutils.h"
#include "debug.h"
#include "globals.h"
#include "util.h"

namespace prb {
	Texture::Texture() { /*ptr = impl::createTexture_1();*/ }
	Texture::Texture(std::wstring const& path) { ptr = impl::createTexture_2(path); }
	Texture::Texture(const wchar_t* path) : Texture(std::wstring(path)) {}
	Texture::Texture(const char* path) : Texture(stru::towstr(std::string(path))) {}
	Texture::Texture(unsigned char* data, vec2 size) { ptr = impl::createTexture_3(data, size); }
	Texture::Texture(unsigned char* data, vec2 size, int linesize) { ptr = impl::createTexture_4(data, size, linesize); }
	Texture::Texture(vec2 size) { ptr = impl::createTexture_5(size); }
	Texture::Texture(vec2 size, int linesize) { ptr = impl::createTexture_6(size, linesize); }

	void Texture::fillRegion(unsigned char* subData, vec2 start, vec2 end) { ptr->fillRegion(subData, start, end); }
	std::vector<unsigned char> Texture::getData()  const { return ptr->getData(); }

	void Texture::resize(vec2 newSize) { ptr->resize(newSize); }
	vec2 Texture::getSize() const { return ptr->getSize(); }

	void Texture::setData(unsigned char* data, vec2 size, int linesize) { ptr->setData(data, size, linesize); }
	void Texture::setData(unsigned char* data, vec2 size) { ptr->setData(data, size); }
	void Texture::drawImmediate(std::vector<float> const& verts) const { ptr->drawImmediate(verts); }
	void Texture::bind() const { ptr->bind(); }
	void Texture::drawImmediate(vec2 pos, vec2 size) const { 
		//ptr->drawImmediate(pos, size); 
		
		Shader& sh = glb::getShader("shaders/texture",
			{
				{"POSITION", "r32g32_float"},
				{"COLOR", "r32g32b32a32_float"},
				{"TEXCOORD", "r32g32_float"}
			}
		);

		vbuf vb = { util::batchTexture(pmat3::translate(pos) * pmat3::scale(size)), {2,4,2} };

		vb.use();
			vb.draw();
		vb.release();

		vb.dispose();
	}
	void Texture::dispose() { ptr->dispose(); delete ptr; }

	bool Texture::null() const { return !ptr || (ptr && ptr->null()); }
}
