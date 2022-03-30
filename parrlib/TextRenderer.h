#pragma once

#include <string>
#include <unordered_map>
#include <sstream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_STROKER_H

#include "math/vector2f.h"
#include "math/vector4f.h"
#include "math/utils2d/axisalignedboundingbox2d.h"

#include "Texture.h"
#include "vertexbuffer.h"
#include "math/stringutils.h"

namespace prb {
	class TextRenderer {
	protected:
		void iterGlyphs(std::function<void(std::wstring const& str, vec2& cur)> const& init, std::function<void(std::wstring& str, int curi, vec2 const& pos, vec2 const& size, vec2 const& txmin, vec2 const& txmax, bool& stop)> const& loop, std::function<void()> const& end);

		struct glyph {
			float advx; // advance.x
			float advy; // advance.y

			float bitw = 5.f; // bitmap.width;
			float bitr = 5.f; // bitmap.rows;

			float bitl; // bitmap_left;
			float bitt; // bitmap_top;

			vec2 txmin, txmax;		// offset of glyph in texture coordinates
			int atlas = 0;			// which atlas this glyph is on
			bool loaded = true;
		};

		struct Atlas {
			vec2 curMin = 2.f, curMax = 2.f;
			Texture tex;
			Atlas() { tex = Texture(vec2{ 1.f,1.f }); }
		};
		std::vector<Atlas> atlases;

		std::unordered_map<unsigned int, glyph> glyphs; //glyph information

		FT_Library library;

		//font info
		std::string font;
		FT_Face face;
		FT_GlyphSlot g;
		FT_Stroker stroker;

		vec2 txBias = 0.5f;
		vbuf vb;

		void defInit();

		// non-backend member variables
		vec4 vcolor = 1.f;
		int vfontSize = 16;
		std::wstring vboundReplacer = L"...";
		vec2 vdrawOffset = -1.f;

	public:
		TextRenderer();
		TextRenderer(std::vector<std::string> const& fonts, int fontSize);

		std::wstringstream tss; //text string stream, to accumulate text to then flush

		void fontSize(int fontSize);
		int fontSize() const;

		void color(vec4 color);
		vec4 color();

		void boundReplacer(std::wstring const& boundReplacer);
		std::wstring boundReplacer() const;

		void drawOffset(vec2 const& drawOffset);
		vec2 drawOffset() const;

		Atlas& getBackAtlas();
		glyph loadGlyph(unsigned int glyph);
		void preloadGlyphs(std::wstring const& str);

		struct batchedText {
			std::vector<float> vdata;
			aabb2 bb;
		};
	protected:
		void batchText(batchedText& res, mat3 const& tr);

	public:
		aabb2 getTextBoundingBox(mat3 const& tr);
		aabb2 getTextBoundingBox(std::wstring const& str, mat3 const& tr);
		template<typename... Args> aabb2 getTextBoundingBox(mat3 const& tr, Args... args) { std::wstring wstr = stru::compose(args...); return getTextBoundingBox(wstr, tr); }

		batchedText batchText(mat3 const& tr);
		batchedText batchText(std::wstring const& str, mat3 const& tr);
		template<typename... Args> batchedText batchText(mat3 const& tr, Args... args) { std::wstring wstr = stru::compose(args...); return batchText(wstr, tr); }

		void drawText(mat3 const& tr);
		void drawText(std::wstring const& str, mat3 const& tr);
		template<typename... Args> void drawText(mat3 const& tr, Args... args) { std::wstring wstr = stru::compose(args...); drawText(wstr, tr); }

		// pos variants
		aabb2 getTextBoundingBox(vec2 const& pos);
		aabb2 getTextBoundingBox(std::wstring const& str, vec2 const& pos);
		template<typename... Args> aabb2 getTextBoundingBox(vec2 const& pos, Args... args) { std::wstring wstr = stru::compose(args...); return getTextBoundingBox(wstr, pos); }

		batchedText batchText(vec2 const& pos);
		batchedText batchText(std::wstring const& str, vec2 const& pos);
		template<typename... Args> batchedText batchText(vec2 const& pos, Args... args) { std::wstring wstr = stru::compose(args...); return batchText(wstr, pos); }

		void drawText(vec2 const& pos);
		void drawText(std::wstring const& str, vec2 const& pos);
		template<typename... Args> void drawText(vec2 const& pos, Args... args) { std::wstring wstr = stru::compose(args...); drawText(wstr, pos); }

		// batched bounded text string
		struct bndtx {
			std::wstring str;
			std::vector<float> vdata;
			aabb2 bb;
		};
	protected:
		void batchTextBounded(bndtx& res, mat3 const& tr, aabb2 const& bound);

	public:
		bndtx batchTextBounded(mat3 const& tr, aabb2 const& bound);
		bndtx batchTextBounded(std::wstring const& str, mat3 const& tr, aabb2 const& bound);
		template<typename... Args> bndtx batchTextBounded(mat3 const& tr, aabb2 const& bound, Args... args) { std::wstring wstr = stru::compose(args...); return batchTextBounded(wstr, tr, bound); }

		void drawTextBounded(mat3 const& tr, aabb2 const& bound);
		void drawTextBounded(std::wstring const& str, mat3 const& tr, aabb2 const& bound);
		template<typename... Args> void drawTextBounded(mat3 const& tr, aabb2 const& bound, Args... args) { std::wstring wstr = stru::compose(args...); drawTextBounded(wstr, tr, bound); }

		bndtx batchTextBounded(vec2 const& pos, aabb2 const& bound);
		bndtx batchTextBounded(std::wstring const& str, vec2 const& pos, aabb2 const& bound);
		template<typename... Args> bndtx batchTextBounded(vec2 const& pos, aabb2 const& bound, Args... args) { std::wstring wstr = stru::compose(args...); return batchTextBounded(wstr, pos, bound); }

		void drawTextBounded(vec2 const& pos, aabb2 const& bound);
		void drawTextBounded(std::wstring const& str, vec2 const& pos, aabb2 const& bound);
		template<typename... Args> void drawTextBounded(vec2 const& pos, aabb2 const& bound, Args... args) { std::wstring wstr = stru::compose(args...); drawTextBounded(wstr, pos, bound); }

		void dispose();
	};

	typedef TextRenderer txrnd;

	namespace txrutl {
		mat3 getDefaultScaleMatrix(vec2 const& pos);
		mat3 matToPos(mat3 const& tr);
	}
}
