#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <parrlib/vector2f.h>
#include <parrlib/input.h>
#include <parrlib/sprite.h>

#include <parrlib/debug.h>

#include <parrlib/framebufferobject.h>

namespace util {

	bool tex2denabled = false;

	float toRadians(float degrees) {
		return (degrees * PI) / 180.0f;
	}

	float toDegrees(float radians) {
		return (radians * 180.0f) / PI;
	}

	Vector2f camPos;

	std::vector<mat3> mStack = { 1.f };
	void pushMatrix() { mStack.push_back(mStack.back()); }
	void pushMatrix(const mat3& modifier) { mStack.push_back(mStack.back() * modifier); }
	void popMatrix() { mStack.pop_back(); }
	mat3 getTopMatrix() { return mStack.back(); }
	mat3 getTopMatrixAspectInv() { return getAspectOrtho().inverted() * mStack.back(); }
	mat3 getTopMatrixInvAspect() { return mStack.back().inverted() * getAspectOrtho(); }
	mat3 getTopMatrixOrthoInverted() { return mStack.back().inverted() * getAspectOrtho(); }

	mat3 getAspectOrthoX() { return pmat3::orthoAspectX(cst::res()); }
	mat3 getAspectOrthoY() { return pmat3::orthoAspectY(cst::res()); }
	mat3 getMinAspectOrtho() { return pmat3::orthoMinAspect(cst::res()); }
	mat3 getMaxAspectOrtho() { return pmat3::orthoMaxAspect(cst::res()); }
	mat3 getAspectOrtho() { return pmat3::orthoMaxAspect(cst::res()); }

	mat3 fromNdc() { return pmat3::fromNdc({ 0.f, cst::res() }); }
	mat3 toNdc() { return pmat3::toNdc({ 0.f, cst::res() }); }

	mat3 fromNdcAspect() { return pmat3::fromNdc({ 0.f, cst::res() }) * getAspectOrtho(); }
	mat3 toNdcAspect() { return getAspectOrtho() * pmat3::toNdc({ 0.f, cst::res() }); }
	mat3 fromNdcAspectInv() { return pmat3::fromNdc({ 0.f, cst::res() }) * getAspectOrtho().inverted(); }
	mat3 toNdcAspectInv() { return getAspectOrtho().inverted() * pmat3::toNdc({ 0.f, cst::res() }); }

	aabb2 getResbbNDCAspect() { return toNdcAspectInv() * cst::resbb(); }

	void translate(vec2 v) { mStack.back() *= pmat3::translate(v); }
	void rotate(float angle) { mStack.back() *= pmat3::rotate(angle); }
	void scale(vec2 v) { mStack.back() *= pmat3::scale(v); }
	void ortho(float left, float right, float bottom, float top) { mStack.back() *= pmat3::ortho(left, right, bottom, top); }
	void lookAt(vec2 eye, vec2 pos) { mStack.back() *= pmat3::lookAt(eye, pos); }
	void multMatrix(const mat3& m) { mStack.back() *= m; }
	void resetMatrix() { mStack.back() = 1.f; }

	mat3 getQuadrantMatrix(int idx, int nPerRow) {
		vec2 pos(idx % nPerRow, floor(idx / nPerRow));

		vec2 perc = pos / (float)nPerRow;

		return pmat3::translate(vec2(-1.f).ny() + perc.ny() * 2.f) * pmat3::scale(1.f / (float)nPerRow) * pmat3::translate(vec2(1.f).ny());
	}

	void drawPoly(std::vector<vec2> const& vs, const int& style) {
		bool oldtx = tex2denabled; if (tex2denabled) util::texDisable();
		glBegin(style);
			for (auto& v : vs) {
				vec2 transformed = mStack.back() * v;
				glVertex2f(transformed.x, transformed.y);
			}
		glEnd();
		if (oldtx) texEnable();
	}

	void drawPoly(std::vector<vec2> const& vs) {
		drawPoly(vs, GL_TRIANGLE_FAN);
	}


	void drawTexture(GLuint texID, float x, float y, float sizeX, float sizeY) {
		//if your image doesn't show up, set the parameters when you create it (GL_TEXTURE_WRAP_S/T and GL_TEXTURE_WRAP_S/T), you HAVE to, otherwise it doesn't show up
		bool texEnabled = tex2denabled;//(bool)glIsEnabled(GL_TEXTURE_2D);
		if (!texEnabled) glEnable(GL_TEXTURE_2D);
		
		vec2 v0 = mStack.back() * vec2(x, y);
		vec2 v1 = mStack.back() * vec2(x, y + sizeY);
		vec2 v2 = mStack.back() * vec2(x + sizeX, y + sizeY);
		vec2 v3 = mStack.back() * vec2(x + sizeX, y);

		bindTexture(0, texID);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex2f(v0.x, v0.y);

			glTexCoord2f(0.0f, 1.0f); 
			glVertex2f(v1.x, v1.y);

			glTexCoord2f(1.0f, 1.0f); 
			glVertex2f(v2.x, v2.y);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex2f(v3.x, v3.y);
		glEnd();
	
		if (!texEnabled) glDisable(GL_TEXTURE_2D);
	}

	void drawTexture(GLuint texID, float x, float y, float sizeX, float sizeY, float cordMultX, float cordMultY) {
		bool texEnabled = tex2denabled; if (!texEnabled) texEnable();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glBegin(GL_QUADS);
			glTexCoord2f(0.5f - 0.5f * cordMultX, 0.5f - 0.5f * cordMultY); glVertex2f(x, y);
			glTexCoord2f(0.5f + 0.5f * cordMultX, 0.5f - 0.5f * cordMultY); glVertex2f(x + sizeX, y);
			glTexCoord2f(0.5f + 0.5f * cordMultX, 0.5f + 0.5f * cordMultY); glVertex2f(x + sizeX, y + sizeY);
			glTexCoord2f(0.5f - 0.5f * cordMultX, 0.5f + 0.5f * cordMultY); glVertex2f(x, y + sizeY);
		glEnd();

		if (!texEnabled) texDisable();
	}

	void drawTexture(GLuint texID, Vector2f quadPos, Vector2f quadSize, Vector2f coordMult) {
		drawTexture(texID, quadPos.x, quadPos.y, quadSize.x, quadSize.y, coordMult.x, coordMult.y);
	}

	void drawTexture(const GLuint &texID, const Vector2f &quadPos, const Vector2f &quadSize) {
		drawTexture(texID, quadPos.x, quadPos.y, quadSize.x, quadSize.y);
	}

	void drawQuadTexCoord(float x, float y, float sizeX, float sizeY) {
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(x + sizeX, y);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(x + sizeX, y + sizeY);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + sizeY);
		glEnd();
	}

	void drawQuadTexCoord(Vector2f quadPos, Vector2f quadSize) {
		drawQuadTexCoord(quadPos.x, quadPos.y, quadSize.x, quadSize.y);
	}

	void drawTexture(GLuint const& tex, mat3 const& transform) {
		drawSubTex(tex, 0.f, 1.f, transform);
	}

	void drawTexture(GLuint const& tex, aabb2 const& coord, mat3 const& transform) {
		drawTexture(tex, pmat3::translate(coord.center()) * pmat3::scale(coord.size() / 2.f));
	}

	void  drawTextureRegion(GLuint tex, vec2 pos, vec2 size, vec2 s, vec2 t) {
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);
			glTexCoord2f(s.x, s.y); glVertex2f(pos.x, pos.y);
			glTexCoord2f(t.x, s.y); glVertex2f(pos.x + size.x, pos.y);
			glTexCoord2f(t.x, t.y); glVertex2f(pos.x + size.x, pos.y + size.y);
			glTexCoord2f(s.x, t.y); glVertex2f(pos.x, pos.y + size.y);
		glEnd();
	}

	void drawSubTex(GLuint const& tex, vec2 const& txmin, vec2 const& txmax, mat3 const& transform) {
		//if your image doesn't show up, set the parameters when you create it (GL_TEXTURE_WRAP_S/T and GL_TEXTURE_WRAP_S/T), you HAVE to, otherwise it doesn't show up
		bool texEnabled = tex2denabled;//(bool)glIsEnabled(GL_TEXTURE_2D);
		if (!texEnabled) texEnable();

		vec2 v0 = mStack.back() * transform * vec2(-1.f, -1.f);
		vec2 v1 = mStack.back() * transform * vec2(-1.f, 1.f);
		vec2 v2 = mStack.back() * transform * vec2(1.f, 1.f);
		vec2 v3 = mStack.back() * transform * vec2(1.f, -1.f);

		bindTexture(0, tex);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
			glTexCoord2f(txmin.x, txmin.y);
			glVertex2f(v0.x, v0.y);

			glTexCoord2f(txmin.x, txmax.y);
			glVertex2f(v1.x, v1.y);

			glTexCoord2f(txmax.x, txmax.y);
			glVertex2f(v2.x, v2.y);

			glTexCoord2f(txmax.x, txmin.y);
			glVertex2f(v3.x, v3.y);
		glEnd();

		if (!texEnabled) texDisable();
	}

	void drawSubTex(GLuint const& tex, aabb2 const& coord, aabb2 const& tx, mat3 const& transform) {
		drawSubTex(tex, tx.fmin(), tx.fmax(), transform * pmat3::translate(coord.center()) * pmat3::scale(coord.size()/2.f));
	}


	void drawQuad(Vector2f quadPos, Vector2f quadSize, float rotation, Vector2f pivot, float ortho, const int &style) {
		//Vector2f v1 = quadPos + (-quadSize/2.0f).rotate(pivot, rotation, ortho);
		//Vector2f v2 = quadPos + Vector2f(quadSize.x / 2.0f, -quadSize.y / 2.0f).rotate(pivot, rotation, ortho);
		//Vector2f v3 = quadPos + Vector2f(quadSize.x / 2.0f, quadSize.y / 2.0f).rotate(pivot, rotation, ortho);
		//Vector2f v4 = quadPos + Vector2f(-quadSize.x / 2.0f, quadSize.y / 2.0f).rotate(pivot, rotation, ortho);
		//glBegin(style);
		//	glVertex2f(v1.x, v1.y);
		//	glVertex2f(v2.x, v2.y);
		//	glVertex2f(v3.x, v3.y);
		//	glVertex2f(v4.x, v4.y);
		//glEnd();

		std::vector<vec2> vs = {
			quadPos,
			quadPos + vec2{ quadSize.x, 0.f },
			quadPos + quadSize,
			quadPos + vec2{ 0.f, quadSize.y }
		};

		mat3 trans = pmat3::translate(pivot * vec2{ortho, 1.f}) * pmat3::rotate(rotation);

		for (auto& v : vs) v = trans * v;
		
		drawPoly(vs, style);
	}

	void drawQuad(Vector2f quadPos, Vector2f quadSize, float rotation, Vector2f pivot, const int &style) {
		drawQuad(quadPos, quadSize, rotation, pivot, 1.0f, style);
	}

	void drawQuad(Vector2f quadPos, Vector2f quadSize, Vector2f lookat, const int &style) {
		//debug << rtlog << atan2(lookat.y - quadPos.y, lookat.x - quadPos.x)
		drawQuad(quadPos, quadSize, toDegrees(atan2(lookat.y-quadPos.y, lookat.x-quadPos.x)), style);
	}

	void drawQuad(Vector2f quadPos, Vector2f quadSize, float rotation, const int &style) {
		drawQuad(quadPos, quadSize, rotation, 0.0f, style);
	}

	void drawQuad(float x, float y, float sizeX, float sizeY, const int &style) {
		drawPoly({	
			{ x, y }, 
			{ x, y+sizeY },
			{ x+sizeX, y+sizeY } ,
			{ x+sizeX, y } 
		}, style);
	}

	void drawQuad(Vector2f quadPos, Vector2f quadSize, const int &style) {
		drawQuad(quadPos.x, quadPos.y, quadSize.x, quadSize.y, style);
	}

	void drawQuad(vec2 const& pos, vec2 const& size, mat3 const& transform, int const& style) {
		std::vector<vec2> vs = {
			transform * pos, 
			transform * (pos + size.oy()),
			transform * (pos + size),
			transform * (pos + size.xo()),
		};
		drawPoly(vs, style);
	}

	void drawQuad(aabb2 const& bb, mat3 const& transform, const int& style) {
		drawQuad(bb.fmin(), bb.size(), transform, style);
	}

	void drawQuad(aabb2 const& bb, const int& style) {
		drawQuad(bb.fmin(), bb.size(), style);
	}

	int circleDetail = 12;

	void drawCircle(float x, float y, float radius, const int &style) {
		std::vector<vec2> vs;
		for (int i = 0; i < circleDetail; i++) {
			float theta = 2.0f * 3.1415926f * float(i) / float(circleDetail);

			float tx = radius * cosf(theta);
			float ty = radius * sinf(theta);

			vs.push_back({ x+tx, y+ty });
		}

		drawPoly(vs, style);
	}

	void drawCircle(Vector2f cPos, float radius, const int &style) {
		drawCircle(cPos.x, cPos.y, radius, style);
	}

	void drawLine(Vector2f p1, Vector2f p2) {
		//glBegin(GL_LINES);
		//	glVertex2f(p1.x, p1.y);
		//	glVertex2f(p2.x, p2.y);
		//glEnd();

		drawPoly({ p1, p2 }, GL_LINES);
	}

	std::unordered_map<int, TextRenderer> txrs;
	outl::uniss text;
	void drawText(std::string const& font, int const& fontSize, mat3 const& transform) {
		if (text.str().length() == 0) return;
		if (txrs.find(fontSize) == txrs.end()) { txrs[fontSize] = TextRenderer({ font }, fontSize);  txrs[fontSize].setOutline(2); txrs[fontSize].color(vc4::white); txrs[fontSize].outlineColor(vc4::black); }

		std::vector<std::wstring> strs = stru::toLines(text.str());

		if (strs.size() > 0) for (int i = 0; i < strs.size(); i++) txrs[fontSize].drawWStringpx(strs[i], transform * pmat3::translate(vec2(0.f, -(fontSize / cst::resy()*2.f) * i)));
		else txrs[fontSize].drawWStringpx(text.str(), font, 0.f, transform);

		text.str(L"");
		text.clear();
	}
	void drawText(int const& fontSize, mat3 const& transform) { drawText("assets/fonts/segoeui.ttf", fontSize, transform); }
	void drawText(int const& fontSize, vec2 const& pos) { drawText(fontSize, pmat3::translate(pos)); }
	void drawText(mat3 const& transform) { drawText("assets/fonts/segoeui.ttf", 15, transform); }
	void drawText(vec2 const& pos) { drawText(pmat3::translate(pos)); }

	aabb2 getTextBound(std::string const& font, int const& fontSize, mat3 const& transform) {
		if(txrs.find(fontSize) == txrs.end()) txrs[fontSize] = TextRenderer({ font }, fontSize);
		return txrs[fontSize].getAABBpx(transform);
	}
	aabb2 getTextBound(mat3 const& transform) { return getTextBound("assets/fonts/segoeui.ttf", 15, transform); }

	std::unordered_map<std::string, Sprite> sps;
	void drawSprite(std::string const& filename, mat3 const& transform) {
		if (sps.find(filename) == sps.end()) sps[filename] = Sprite(filename);
		sps[filename].draw(transform);
	}
	
	std::unordered_map<std::string, Sprite::AnimationPlayer> spsanims;
	void drawSprite(std::string const& filename, std::string const& identifier, std::string const& animation, mat3 const& transform) {
		if (sps.find(filename) == sps.end()) sps[filename] = { filename };
		
		std::string aid = identifier;
		if (spsanims.find(aid) == spsanims.end()) spsanims[aid] = { sps[filename], animation };
		
		spsanims[aid].setAnim(sps[filename], animation);

		sps[filename].draw(spsanims[identifier], transform);
		spsanims[aid].advance();
	}
	void drawSprite(std::string const& filename, std::string const& animation, mat3 const& transform) { drawSprite(filename, "def", transform); }

	vec4 ucolor = 0.f;
	void setColor3f(Vector3f col)	{ ucolor = vec4(col,1.f);	glColor3f(col.x, col.y, col.z); }
	void setColor4f(Vector4f col)	{ ucolor = col;				glColor4f(col.x, col.y, col.z, col.w);	}
	void setColor(Vector3f col)		{ ucolor = vec4(col, 1.f);	glColor3f(col.x, col.y, col.z);			}
	void setColor(Vector4f col)		{ ucolor = col;				glColor4f(col.x, col.y, col.z, col.w); }
	void setColor(std::string const& str) {
		if (str.length() > 7) setColor(vec4(str));
		else setColor(vec3(str));
	}
	void setColor(const char* str) { setColor(std::string(str)); }

	vec4 getColor() { return ucolor; }

	void setClearColor(Vector4f col) {
		glClearColor(col.x, col.y, col.z, col.w);
	}

	bool blendisenabled = false;
	bool blendEnabled() { return blendisenabled; }
	void blendEnable(int blendFuncSrc, int blendFuncDst) {
		blendisenabled = true;
		glEnable(GL_BLEND);
		glBlendFunc(blendFuncSrc, blendFuncDst);
	}

	void blendEnable() {
		blendEnable(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void blendDisable() {
		blendisenabled = false;
		glDisable(GL_BLEND);
	}

	
	void textureEnable(int texture) {
		glEnable(texture);
	}

	void textureEnable() {
		glEnable(GL_TEXTURE_2D);
		tex2denabled = true;
	}

	void textureDisable(int texture) {
		glDisable(texture);
	}

	void textureDisable() {
		glDisable(GL_TEXTURE_2D);
		tex2denabled = false;
	}


	void texEnable() {
		tex2denabled = true;
		glEnable(GL_TEXTURE_2D);
	}
	void texDisable() {
		glDisable(GL_TEXTURE_2D);
		tex2denabled = false;
	}

	void enable3D() {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}
	void disable3D() {
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	}

	void tex3DEnable() {
		texEnable();
		enable3D();
	}
	void tex3DDisable() {
		disable3D();
		texDisable();
	}

	void enable3DTexABlend() {
		tex3DEnable();
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}

	void disable3DTexABlend(){
		glDisable(GL_BLEND);
		tex3DDisable();
	}


	void stencilEnable() { glEnable(GL_STENCIL_TEST); }
	void stencilEnableWritingMask(){
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	//if the test fails, keep the fragment value, else set it to 1 (it this case it always passes the test, since func is GL_ALWAYS)
		glStencilFunc(GL_ALWAYS, 1, 0xFF);			//always set the fragment's value to 1
		glStencilMask(0xFF);						//enable writing
	}
	void stencilDisableWriting() {
		glStencilFunc(GL_EQUAL, 1, 0xFF);			//pass the test if it's equal to 1
		glStencilMask(0x00);						//disable writing
	}
	void stencilDisable() { glDisable(GL_STENCIL_TEST); }
	void stencilClear() { glStencilMask(0xFF); glClear(GL_STENCIL_BUFFER_BIT); }	//to clear you need to enable the mask

	void stencilDisableWritingInv() {
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);		//pass the test if it's not equal to 1
		glStencilMask(0x00);						//disable writing
	}

	void stencilMask(GLuint mask) { glStencilMask(mask); }
	void stencilSetOp(GLenum sfail, GLenum dsfail, GLenum pass) { glStencilOp(sfail, dsfail, pass); }
	void stencilSetOp(GLenum fail, GLenum pass) { stencilSetOp(fail, fail, pass); }

	void stencilSetFunc(GLenum func, GLint value, GLuint mask){ glStencilFunc(func, value, mask); }
	void stencilSetFunc(GLenum func, GLint value) { stencilSetFunc(func, value, 0xFF); }

	void stencilSetAlwaysPass(GLint value) { stencilSetFunc(GL_ALWAYS, value); }

	void stencilSetMode(GLenum passif, GLint value, GLuint mask) {
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(passif, value, 0xFF);
		glStencilMask(mask);
	}

	void stencilSetModeEnableBlendAlpha(GLenum passif, GLint value, GLuint mask) { util::blendEnable(); setColor4f(0.f); stencilSetMode(passif, value, mask); }
	void stencilSetModeDisableBlend(GLenum passif, GLint value, GLuint mask) { util::blendDisable(); stencilSetMode(passif, value, mask); }


	GLint getActiveTexture() {
		GLint id = -1;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);
		return id;
	}

	void setActiveTexture(GLuint texture) {
		glActiveTexture(GL_TEXTURE0 + texture);
	}

	void bindTexture(GLuint target, GLuint texture){
		//GLint oldId = getActiveTexture();
		glActiveTexture(GL_TEXTURE0 + target);
		glBindTexture(GL_TEXTURE_2D, texture);
		//setActiveTexture(oldId);
	}

	void bindTexture(GLuint texture) {
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	std::vector<GLuint> fboStack; //stack of bound fbos
	void bindFramebuffer(GLuint fboid) {
		if (fboStack.capacity() == 0) fboStack.reserve(10); 

		fboStack.push_back(prc::fboid);
		glBindFramebuffer(GL_FRAMEBUFFER, fboid);
		prc::fboid = fboid;
	}
	void bindFramebuffer(FrameBufferObject const& fbo) { bindFramebuffer(fbo.getID()); }


	void unbindFramebuffer() {
		GLuint fbo = fboStack.size() == 0 ? 0 : fboStack.back();
		if (fboStack.size() > 0) fboStack.pop_back();

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		prc::fboid = fbo;
	}

	void unbindFramebufferReset() {
		fboStack.clear();
		prc::fboid = 0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint getCurrentFramebuffer() { return prc::fboid; }


	bool getVecInQuad(Vector2f v, Vector2f quadPos, Vector2f quadSize) {
		return v.x > quadPos.x && v.x < quadPos.x + quadSize.x &&
				v.y > quadPos.y && v.y < quadPos.y + quadSize.y;
	}

	bool getMouseInQuad(Vector2f quadPos, Vector2f quadSize) {
		//return getVecInQuad(input::getMouseCoords(), quadPos, quadSize);
		return getVecInQuad(input::getMousePos(), quadPos, quadSize);
	}


	

	//std::vector<GLScissor> scissors;

	//void pushGLScissor(Vector2f pos, Vector2f size) {
	//	if(scissors.size() > 0){
	//		int idx = scissors.size() - 1;
	//		float nPosX = std::fmin(std::fmax(pos.x, scissors[idx].pos.x), scissors[idx].pos.x + scissors[idx].size.x);
	//		float nPosY = std::fmin(std::fmax(pos.y, scissors[idx].pos.y), scissors[idx].pos.y + scissors[idx].size.y);

	//		size.x -= nPosX - pos.x;
	//		size.y -= nPosY - pos.y;

	//		pos.x = nPosX;
	//		pos.y = nPosY;

	//		float posSizeX = pos.x + size.x;
	//		float posSizeY = pos.y + size.y;

	//		float distX = std::fmin(std::fmax(posSizeX, scissors[idx].pos.x), scissors[idx].pos.x + scissors[idx].size.x);
	//		float distY = std::fmin(std::fmax(posSizeY, scissors[idx].pos.y), scissors[idx].pos.y + scissors[idx].size.y);

	//		size.x -= (posSizeX - distX);
	//		size.y -= (posSizeY - distY);
	//	}
	//	else {
	//		glEnable(GL_SCISSOR_TEST);
	//	}

	//	//debug::drawQuad(pos, size);

	//	scissors.push_back(GLScissor(pos, size));

	//	float rOrthoAspect = prc::orthoAspect;
	//	if (prc::orthoAspect == 0.0f) {
	//		rOrthoAspect = (float)constants::WIDTH / (float)constants::HEIGHT;
	//	}

	//	Vector2f rCoordsPos = Vector2f(	constants::WIDTH*((pos.x - prc::orthoLeft*rOrthoAspect) / (prc::orthoRight*rOrthoAspect - prc::orthoLeft*rOrthoAspect)),
	//									constants::HEIGHT*((pos.y - prc::orthoBottom) / (prc::orthoTop - prc::orthoBottom)));
	//	Vector2f rCoordsSize = Vector2f(constants::WIDTH*(size.x / (prc::orthoRight*rOrthoAspect - prc::orthoLeft*rOrthoAspect)),
	//									constants::HEIGHT*(size.y / (prc::orthoTop - prc::orthoBottom)));

	//	glScissor((int)rCoordsPos.x, (int)rCoordsPos.y, (int)rCoordsSize.x, (int)rCoordsSize.y);
	//}

	//void popGLScissor() {
	//	if (scissors.size() > 0) {
	//		scissors.pop_back();

	//		if (scissors.size() > 0) {
	//			Vector2f pos = scissors[scissors.size() - 1].pos, size = scissors[scissors.size() - 1].size;

	//			//Vector2f rCoordsPos = (pos + 1.0f) / 2.0f*Vector2f(constants::WIDTH, constants::HEIGHT);
	//			//Vector2f rCoordsSize = size / 2.0f*Vector2f(constants::WIDTH, constants::HEIGHT);
	//			
	//			float rOrthoAspect = prc::orthoAspect;
	//			if (prc::orthoAspect == 0.0f) {
	//				rOrthoAspect = (float)constants::WIDTH / (float)constants::HEIGHT;
	//			}

	//			Vector2f rCoordsPos = Vector2f(	constants::WIDTH*((pos.x-prc::orthoLeft*rOrthoAspect) / (prc::orthoRight*rOrthoAspect - prc::orthoLeft*rOrthoAspect)),
	//											constants::HEIGHT*((pos.y - prc::orthoBottom) / (prc::orthoTop- prc::orthoBottom)));
	//			Vector2f rCoordsSize = Vector2f(constants::WIDTH*(size.x/(prc::orthoRight*rOrthoAspect - prc::orthoLeft*rOrthoAspect)),
	//											constants::HEIGHT*(size.y/(prc::orthoTop - prc::orthoBottom)));

	//			glScissor((int)rCoordsPos.x, (int)rCoordsPos.y, (int)rCoordsSize.x, (int)rCoordsSize.y);
	//		}
	//		else {
	//			glDisable(GL_SCISSOR_TEST);
	//		}
	//	}
	//}


	bool checkForGLErrors() {
		GLenum err= GL_NO_ERROR;
		bool wasError = false;

		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "GL error: " << printGLError(err) << "\n";
			wasError = true;
		}

		return wasError;
	}

	std::string printGLError(GLenum err) {
		std::string result="";
		if (err != GL_NO_ERROR) {
			switch (err) {
			case GL_INVALID_ENUM: result += "GL_INVALID_ENUM"; break;
			case GL_INVALID_VALUE: result += "GL_INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: result += "GL_INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW: result += "GL_STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW: result += "GL_STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY: result += "GL_OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: result += "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
			case GL_CONTEXT_LOST: result += "GL_CONTEXT_LOST"; break;
				//case GL_TABLE_TOO_LARGE1: ARB image extension (deprecated)
			default:
				result += "unknown GL error\n";
			}
		}
		return result;
	}

	std::string fboAttachmentToString(GLuint attachment) {
		if (attachment >= GL_COLOR_ATTACHMENT0 && attachment <= GL_COLOR_ATTACHMENT31) return "GL_COLOR_ATTACHMENT" + std::to_string(attachment - GL_COLOR_ATTACHMENT0);
		switch (attachment) {
			case GL_DEPTH_ATTACHMENT: return "GL_DEPTH_ATTACHMENT";
			case GL_STENCIL_ATTACHMENT: return "GL_STENCIL_ATTACHMENT";
			case GL_DEPTH_STENCIL_ATTACHMENT: return "GL_DEPTH_STENCIL_ATTACHMENT";
		}
		return "?";
	}

	std::string textureFormatToString(GLenum format) {
		switch (format) {
			case GL_RGBA: return "GL_RGBA";
			case GL_RGB: return "GL_RGB";
			case GL_RG: return "GL_RG";
			case GL_RED: return "GL_RED";
			case GL_GREEN: return "GL_GREEN";
			case GL_BLUE: return "GL_BLUE";
		}
		return "unknown";
	}

	std::string glCodeToString(GLenum code) {
		switch (code) {
			case GL_RGBA:			return "GL_RGBA";
			case GL_RGB:			return "GL_RGB";
			case GL_RG:				return "GL_RG";
			case GL_RED:			return "GL_RED";
			case GL_GREEN:			return "GL_GREEN";
			case GL_BLUE:			return "GL_BLUE";
			case GL_ALPHA:			return "GL_ALPHA";

			case GL_UNSIGNED_BYTE:	return "GL_UNSIGNED_BYTE";
			case GL_FLOAT:			return "GL_FLOAT";
			case GL_INT:			return "GL_INT";
			case GL_UNSIGNED_INT:	return "GL_UNSIGNED_INT";

			case GL_DEPTH_ATTACHMENT:			return "GL_DEPTH_ATTACHMENT";
			case GL_STENCIL_ATTACHMENT:			return "GL_STENCIL_ATTACHMENT";
			case GL_DEPTH_STENCIL_ATTACHMENT:	return "GL_DEPTH_STENCIL_ATTACHMENT";
		}
		return "unknown";
	}

	std::string glIntToString(GLint code) {
		switch (code) {
			case GL_NEAREST:		return "GL_NEAREST";
			case GL_LINEAR:			return "GL_LINEAR";
		}
		return "unknown";
	}


	float randomInterval(float low, float high) {
		return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
	}

	float randomInterval(float range) {
		return randomInterval(-range, range);
	}

	
	float frand() {
		return (float)rand() / (float)RAND_MAX;
	}


	int approxEquals(float f, float val, float low, float high) {
		return f < val-low ? -1 : f > val+high ? 1 : 0;
	}

	int approxEquals(float f, float val, float range) {
		return approxEquals(f, val, range, range);
	}


	int clamp(int i, int imin, int imax) {
		return std::min(std::max(i, imin), imax);
	}

	int pmod(int i, int n) {
		return (i % n + n) % n;
	}

	int writeToBMP(std::string fileName, std::vector<std::vector<Vector3f>> pixels) {
		FILE *f;
		unsigned char *img = NULL;

		int w = pixels[0].size(), h = pixels.size();

		int filesize = 54 + 3 * w*h;  //w is your image width, h is image height, both int

		img = (unsigned char *)malloc(3 * w*h);
		memset(img, 0, 3 * w*h);

		int x, y, r, g, b;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				x = j; y = (h - 1) - i;
				r = pixels[i][j].x * 255;
				g = pixels[i][j].y * 255;
				b = pixels[i][j].z * 255;
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;
				img[(x + y * w) * 3 + 2] = (unsigned char)(r);
				img[(x + y * w) * 3 + 1] = (unsigned char)(g);
				img[(x + y * w) * 3 + 0] = (unsigned char)(b);
			}
		}

		unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
		unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
		unsigned char bmppad[3] = { 0,0,0 };

		bmpfileheader[2] = (unsigned char)(filesize);
		bmpfileheader[3] = (unsigned char)(filesize >> 8);
		bmpfileheader[4] = (unsigned char)(filesize >> 16);
		bmpfileheader[5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[4] = (unsigned char)(w);
		bmpinfoheader[5] = (unsigned char)(w >> 8);
		bmpinfoheader[6] = (unsigned char)(w >> 16);
		bmpinfoheader[7] = (unsigned char)(w >> 24);
		bmpinfoheader[8] = (unsigned char)(h);
		bmpinfoheader[9] = (unsigned char)(h >> 8);
		bmpinfoheader[10] = (unsigned char)(h >> 16);
		bmpinfoheader[11] = (unsigned char)(h >> 24);

		fopen_s(&f, fileName.c_str(), "wb");
		fwrite(bmpfileheader, 1, 14, f);
		fwrite(bmpinfoheader, 1, 40, f);
		for (int i = 0; i < h; i++) {
			fwrite(img + (w*(h - i - 1) * 3), 3, w, f);
			fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
		}

		free(img);
		fclose(f);

		return 0;
	}
}