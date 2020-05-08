#pragma once

#include <string>
#include <ostream>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_STROKER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "VertexArrayObject.h"

#include "Vector2f.h"
#include "Matrix3f.h"
#include "Utils2D/AxisAlignedBoundingBox2D.h"
#include "StringUtils.h"

class TextRenderer{
protected:
	FT_Library library;

	struct glyph {
		float advx; // advance.x
		float advy; // advance.y

		float bitw = 5.f; // bitmap.width;
		float bitr = 5.f; // bitmap.rows;

		float bitl; // bitmap_left;
		float bitt; // bitmap_top;

		vec2 txmin, txmax;		// offset of glyph in texture coordinates
		int atlas=0;			// which atlas this glyph is on
		bool loaded = true;
	};

	struct Atlas {
		Texture tex;
		vec2 atlasCur = 0.f; //current atlas fill value, expands as more glyphs are loaded
		vec2 atlasCurMax = 0.f;
		vec2 atlasCurLine = 0.f;
		bool tofuLoaded = false;
		void dispose() {
			tex.dispose();
			atlasCur = 0.f;
			atlasCurMax = 0.f;
			atlasCurLine = 0.f;
			tofuLoaded = false;
		}
	};
	Atlas atlas;

	struct Font {
		std::unordered_map<wchar_t, glyph> gInfo;	//info
		std::unordered_map<wchar_t, glyph> goInfo;  //outlineinfo

		FT_Face face;
		FT_GlyphSlot g;
		FT_Stroker stroker;

		void dispose() {
			gInfo.clear();
			goInfo.clear();
		}
	};
	//Font name -> font information
	std::unordered_map<std::string, Font> fonts;

	void updateAndResizeAtlas(Atlas& at, vec2 gSize);

	bool outlineEnabled = false;
	unsigned int outlinepx = 0;

	GLint minFilter = GL_LINEAR, magFilter = GL_LINEAR;

	vec2 border = 2.f;
	vec2 spacing = { 2.f, 2.f };
	vec2 txBias = 0.5f;

	vec2 maxAtlasSize = 512;

	std::string fontName = "";
	int fontSize;

	bool correctNdc = true;

	Shader sh;
	VAO vao;

	vec4 vcolor = vc4::black, voutlineColor = vc4::black;

	std::vector<float> data;
	std::vector<float> dataColor;

	struct DrawRes { int count; vec2 cur; };
	DrawRes drawWStringPriv(std::wstring const& str, std::string const& font, vec2 const& off, mat3 const& transform);
	
public:

	TextRenderer();
	TextRenderer(std::vector<std::string> const& fontNames, int const& fontSize);
	//bool OutlineExists() const;

	//void loadOutlineGlyph(std::string const& font, unsigned long id);
	void loadGlyph(std::string const& font, unsigned long id);

	//draws the string in integer coordinate space, without any modification
	void drawWStringReal(std::wstring const& str, std::string const& font, vec2 const& off, mat3 const& transform);
	void drawWStringReal(std::wstring const& str, mat3 const& transform);
	void drawWStringReal(mat3 const& transform);

	//draws a string in NDC space, so [-1,1],[-1,1]
	void drawWString(std::wstring const& str, std::string const& font, mat3 const& transform);
	void drawWString(std::wstring const& str, mat3 const& transform);
	void drawWString(std::wstring const& str, vec2 pos);

	void drawString(std::string const& str, mat3 const& transform);
	void drawString(std::string const& str, vec2 pos);

	//draws a string as-is in real pixels
	void drawWStringpx(std::wstring const& str, std::string const& font, vec2 const& offset, mat3 const& transform);
	void drawWStringpx(std::wstring const& str, vec2 const& offset, mat3 const& transform);
	void drawWStringpx(std::wstring const& str, std::string const& font, mat3 const& transform);
	void drawWStringpx(std::wstring const& str, mat3 const& transform);

	//draws a string as-is in real pixels inside a bound, trimming it if necessary
	void drawWStringpx(std::wstring const& str, std::wstring const& modstr, std::string const& font, vec2 const& offset, aabb2 const& tbound, mat3 const& transform);
	void drawWStringpx(std::wstring const& str, std::string const& font, vec2 const& offset, aabb2 const& bound, mat3 const& transform);
	void drawWStringpx(std::wstring const& str, std::wstring const& modstr, vec2 const& offset, aabb2 const& tbound, mat3 const& transform);
	void drawWStringpx(std::wstring const& str, vec2 const& offset, aabb2 const& bound, mat3 const& transform);
	void drawWStringpx(std::wstring const& str, aabb2 const& bound, mat3 const& transform);

	std::wstringstream wostr;
	void drawWString(mat3 const& transform);
	void drawWString(vec2 pos);

	void drawWStringpx(std::string const& font, vec2 const& offset, mat3 const& transform);
	void drawWStringpx(vec2 const& offset, mat3 const& transform);
	void drawWStringpx(mat3 const& transform);

	void drawWStringpx(std::string const& font, vec2 const& offset, aabb2 const& bound, mat3 const& transform);

	aabb2 getAABBReal(std::wstring const& str, std::string const& font, vec2 const& off, mat3 const& transform);
	aabb2 getAABBReal(std::wstring const& str, mat3 const& transform);
	aabb2 getAABBReal(mat3 const& transform);

	aabb2 getAABB(std::wstring const& str, mat3 const& transform);
	aabb2 getAABB(mat3 const& transform);

	aabb2 getAABBpx(std::wstring const& str, std::string const& font, vec2 const& offset, mat3 const& transform);
	aabb2 getAABBpx(std::wstring const& str, vec2 const& offset, mat3 const& transform);
	aabb2 getAABBpx(std::wstring const& str, mat3 const& transform);
	aabb2 getAABBpx(mat3 const& transform);

	struct bwstring { std::wstring str; vec2 inoffset; aabb2 bound; };
	bwstring getWStringBoundedReal(std::wstring const& str, std::string const& font, aabb2 const& bound, vec2 const& offset, std::wstring const& modstr, mat3 const& transform);

	bwstring getWStringBoundedpx(std::wstring const& str, std::string const& font, aabb2 const& bound, vec2 const& offset, std::wstring const& modstr, mat3 const& transform);
	bwstring getWStringBoundedpx(std::wstring const& str, aabb2 const& bound, vec2 const& offset, std::wstring const& modstr, mat3 const& transform);
	bwstring getWStringBoundedpx(std::wstring const& str, aabb2 const& bound, vec2 const& offset, mat3 const& transform);
	bwstring getWStringBoundedpx(std::wstring const& str, aabb2 const& bound, mat3 const& transform);

	void addFont(std::string const& font);

	void setFontSize(int fontSize);
	int getFontSize();

	void setOutline(unsigned int px);
	unsigned int getOutline();

	void setFont(std::string const& fontName);
	std::string getFontName();

	void color(vec4 color);
	vec4 color();

	void outlineColor(vec4 outlineColor);
	vec4 outlineColor();

	void setFiltering(GLint min, GLint mag);
	GLint getMagFilter();
	GLint getMinFilter();

	Texture getAtlas();

	//std::unordered_map<std::string, Font>* getUM();
	std::vector<std::string> getFonts();

	std::string checkAndLoadGlyph(std::string const& font, wchar_t const& c);

	//std::wstring getBoundedString(std::wstring const& str, aabb2 const& bound);

	void setCorrectNdc(bool correctNdc);
	bool getCorrectNdc() const;

	void clear();

	void dispose();


	//backwards compatibility funcs
	//vec2 getStringOffset(std::string const& text, float scale);
	//vec2 getStringSize(std::string const& text, float scale);
	//aabb2 getStringBounds(std::string const& text, float scale);
	//float getScaleToFit(std::string const& text, vec2 const& bound);

	//float getScreenPercScaleFactor(float perc);

	template<typename T>
	std::wstringstream& operator<<(const T& p) {
		txr.wostr << p;
		return txr.wostr;
	}
};
