#include "TextRenderer.h"

#include <functional>

#include "../parrlib/stringutils.h"

#include "debug.h"
#include "shader.h"
#include "util.h"


namespace prb {
	void TextRenderer::defInit() {
		vb = vbuf({ 2, 4, 2 });
	}

	TextRenderer::TextRenderer() {}
	TextRenderer::TextRenderer(std::vector<std::string> const& fonts, int fontSize) {
		this->font = strup::fallbackPath(outl::getExeFolder(), fonts[0]); //TODO implement multiple fonts
		this->vfontSize = fontSize;

		if (FT_Init_FreeType(&library)) {
			deb::mbe("Error: Could not load FreeType library.");
			std::terminate();
		}

		if (FT_New_Face(library, font.c_str(), 0, &face)) {
			deb::tss << "Error: Could not load font '" << font.c_str() << "'."; deb::mbe();
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, fontSize);
		FT_Select_Charmap(face, ft_encoding_unicode);

		g = face->glyph;

		FT_Stroker_New(library, &stroker);

		defInit();
	}

	void TextRenderer::fontSize(int fontSize) { vfontSize = fontSize; }
	int TextRenderer::fontSize() const { return vfontSize; }

	void TextRenderer::color(vec4 color) { this->vcolor = color; }
	vec4 TextRenderer::color() { return vcolor; }

	void TextRenderer::boundReplacer(std::wstring const& boundReplacer) { vboundReplacer = boundReplacer; }
	std::wstring TextRenderer::boundReplacer() const { return vboundReplacer; }

	void TextRenderer::drawOffset(vec2 const& drawOffset) { vdrawOffset = drawOffset; }
	vec2 TextRenderer::drawOffset() const { return vdrawOffset; }

	TextRenderer::Atlas& TextRenderer::getBackAtlas() {
		if (atlases.size() == 0) atlases.push_back(Atlas());
		return atlases.back();
	}

	TextRenderer::glyph TextRenderer::loadGlyph(unsigned int gl) {
		if (glyphs.find(gl) != glyphs.end()) return glyphs[gl];

		FT_UInt idx = FT_Get_Char_Index(face, gl);
		if (idx == 0 && gl != 0) { //could not load glyph, load tofu
			glyph tofu = loadGlyph(0);
			glyphs[gl] = tofu;
		}

		if (FT_Load_Glyph(face, idx, FT_LOAD_DEFAULT)) { deb::mbe(L"could not load glyph (" + std::to_wstring((wchar_t)gl) + L")"); if (gl == 0) std::terminate(); return loadGlyph(0); }

		FT_Glyph glp;
		if (FT_Get_Glyph(face->glyph, &glp)) return loadGlyph(0);
		if (FT_Glyph_To_Bitmap(&glp, FT_RENDER_MODE_NORMAL, nullptr, true)) return loadGlyph(0);

		FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glp);
		FT_Bitmap ftbitmap = bitmapGlyph->bitmap;

		vec2 glyphSize = vec2(ftbitmap.width, ftbitmap.rows);

		Atlas& at = getBackAtlas();
		vec2 start = getBackAtlas().curMin;

		glyphs[gl].advx = glp->advance.x >> 16;
		glyphs[gl].advy = glp->advance.y >> 16;

		glyphs[gl].bitw = ftbitmap.width;
		glyphs[gl].bitr = ftbitmap.rows;

		glyphs[gl].bitl = bitmapGlyph->left;
		glyphs[gl].bitt = bitmapGlyph->top;

		glyphs[gl].txmin = at.curMin;
		glyphs[gl].txmax = at.curMin + glyphSize;

		if ((wchar_t)gl != L' ') {
			unsigned char* bitmap = ftbitmap.buffer;

			unsigned char* img = new unsigned char[glyphSize.x * glyphSize.y * 4];

			for (int y = 0; y < glyphSize.y; y++) {
				for (int x = 0; x < glyphSize.x; x++) {
					int idx = y * glyphSize.x + x;

					img[idx * 4] = bitmap[idx];
					img[idx * 4 + 1] = bitmap[idx];
					img[idx * 4 + 2] = bitmap[idx];
					img[idx * 4 + 3] = bitmap[idx];
				}
			}

			if (at.tex.getSize().x < at.curMin.x + glyphSize.x ||
				at.tex.getSize().y < at.curMin.y + glyphSize.y) {

				at.tex.resize((at.curMin + glyphSize + 2.f).maxed(at.tex.getSize()));
			}

			at.tex.fillRegion(img, at.curMin, glyphSize);
			at.curMin += vec2{ glyphSize.x, 0.f } + vec2{ 2.f, 0.f };

			delete[] img;
		}
	}

	void TextRenderer::preloadGlyphs(std::wstring const& str) {
		Atlas& at = getBackAtlas();
		for (int i = 0; i < str.length(); i++) {
			if (glyphs.find(str[i]) == glyphs.end()) loadGlyph(str[i]);
		}
	}

	void TextRenderer::iterGlyphs(std::function<void(std::wstring const& str, vec2& cur)> const& init, std::function<void(std::wstring& str, int curi, vec2 const& pos, vec2 const& size, vec2 const& txmin, vec2 const& txmax, bool& stop)> const& loop, std::function<void()> const& end) {
		std::wstring str = tss.str();
		tss.clear();
		tss.str(L"");

		preloadGlyphs(str);

		vec2 startCur = 0.f;
		vec2 cur = startCur;
		init(str, cur);

		bool stop = false;
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == L'\n') { cur.x = startCur.x; cur.y += vfontSize; continue; }

			glyph g = glyphs[str[i]];

			vec2 pos = 0.f, txmin;
			vec2 size = 2.f, txmax;

			pos = cur + vec2(g.bitl, -(g.bitr - g.bitt)) - txBias;
			size = vec2(g.bitw, g.bitr) + txBias * 2.f;

			txmin = g.txmin - txBias;
			txmax = g.txmax + txBias;

			cur += vec2(g.advx, g.advy);

			txmin /= getBackAtlas().tex.getSize();
			txmax /= getBackAtlas().tex.getSize();

			loop(str, i, pos, size, txmin, txmax, stop); if (stop) break;
		}

		end();
	}

	aabb2 TextRenderer::getTextBoundingBox(mat3 const& tr) {
		aabb2 bb;

		iterGlyphs(
			[&](std::wstring const& str, vec2 const& cur) {
				bb = cur;
			},
			[&](std::wstring const& str, int curi, vec2 const& pos, vec2 const& size, vec2 const& txmin, vec2 const& txmax, bool& stop) {
				bb = bb.rescaled(pos);
				bb = bb.rescaled(pos + size);
			},
			[&] {
				//bb = pmat3::translate(-vec2(bb.size().x / 2.f, bb.size().y) * ((vdrawOffset + 1.f) / 2.f)) * tr * bb;
				bb = tr * bb;
			}
		);

		return bb;
	}
	aabb2 TextRenderer::getTextBoundingBox(std::wstring const& str, mat3 const& pos) { tss << str; return getTextBoundingBox(pos); }

	void TextRenderer::batchText(TextRenderer::batchedText& res, mat3 const& tr) {
		int curData = 0;

		mat3 rtr = tr;
		if (vdrawOffset != -1.f) {
			std::wstring tssstr = tss.str();
			aabb2 totalBB = getTextBoundingBox(tss.str(), tr);
			tss << tssstr;
			rtr = pmat3::translate(-vec2(totalBB.size().x / 2.f, totalBB.size().y) * ((vdrawOffset + 1.f) / 2.f)) * tr;
		}

		iterGlyphs(
			[&](std::wstring const& str, vec2 const& cur) {
				res.vdata.resize(str.length() * 3 * (2 * 4 * 2));
				res.bb = cur;
			}, 
			[&](std::wstring const& str, int curi, vec2 const& pos, vec2 const& size, vec2 const& txmin, vec2 const& txmax, bool& stop) {
				res.bb = res.bb.rescaled(pos);
				res.bb = res.bb.rescaled(pos + size);

				vec2 v0 = rtr * pos;
				vec2 v1 = rtr * (pos + size.oy());
				vec2 v2 = rtr * (pos + size);
				vec2 v3 = rtr * (pos + size.xo());

				vec2 tx0 = { txmin.x, txmax.y };
				vec2 tx1 = txmin;
				vec2 tx2 = { txmax.x, txmin.y };
				vec2 tx3 = txmax;

				res.vdata[curData++] = v0.x; res.vdata[curData++] = v0.y;		res.vdata[curData++] = vcolor.x; res.vdata[curData++] = vcolor.y; res.vdata[curData++] = vcolor.z; res.vdata[curData++] = vcolor.w;			res.vdata[curData++] = tx0.x; res.vdata[curData++] = tx0.y;
				res.vdata[curData++] = v1.x; res.vdata[curData++] = v1.y;		res.vdata[curData++] = vcolor.x; res.vdata[curData++] = vcolor.y; res.vdata[curData++] = vcolor.z; res.vdata[curData++] = vcolor.w;			res.vdata[curData++] = tx1.x; res.vdata[curData++] = tx1.y;
				res.vdata[curData++] = v2.x; res.vdata[curData++] = v2.y;		res.vdata[curData++] = vcolor.x; res.vdata[curData++] = vcolor.y; res.vdata[curData++] = vcolor.z; res.vdata[curData++] = vcolor.w;			res.vdata[curData++] = tx2.x; res.vdata[curData++] = tx2.y;

				res.vdata[curData++] = v2.x; res.vdata[curData++] = v2.y;		res.vdata[curData++] = vcolor.x; res.vdata[curData++] = vcolor.y; res.vdata[curData++] = vcolor.z; res.vdata[curData++] = vcolor.w;			res.vdata[curData++] = tx2.x; res.vdata[curData++] = tx2.y;
				res.vdata[curData++] = v3.x; res.vdata[curData++] = v3.y;		res.vdata[curData++] = vcolor.x; res.vdata[curData++] = vcolor.y; res.vdata[curData++] = vcolor.z; res.vdata[curData++] = vcolor.w;			res.vdata[curData++] = tx3.x; res.vdata[curData++] = tx3.y;
				res.vdata[curData++] = v0.x; res.vdata[curData++] = v0.y;		res.vdata[curData++] = vcolor.x; res.vdata[curData++] = vcolor.y; res.vdata[curData++] = vcolor.z; res.vdata[curData++] = vcolor.w;			res.vdata[curData++] = tx0.x; res.vdata[curData++] = tx0.y;
			}, 
			[&] {
				res.bb = rtr * res.bb;
			}
		);
	}
	TextRenderer::batchedText TextRenderer::batchText(mat3 const& pos) { batchedText res; batchText(res, pos); return res; }
	TextRenderer::batchedText TextRenderer::batchText(std::wstring const& str, mat3 const& tr) { tss << str; return batchText(tr); }

	void TextRenderer::drawText(mat3 const& pos) {
		batchedText btc; batchText(btc, pos);
		if (btc.vdata.size() == 0) return;

		vb.setData(btc.vdata);
		getBackAtlas().tex.bind();
		util::defTexShader.use();
		vb.draw();
	}
	void TextRenderer::drawText(std::wstring const& str, mat3 const& tr) { tss << str; drawText(tr); }

	// pos variants
	mat3 getDefaultPos(vec2 const& pos) { return pmat3::translate(pos) * pmat3::scale(2.f/cst::res()); }

	aabb2 TextRenderer::getTextBoundingBox(vec2 const& pos) { return getTextBoundingBox(getDefaultPos(pos)); }
	aabb2 TextRenderer::getTextBoundingBox(std::wstring const& str, vec2 const& pos) { tss << str; return getTextBoundingBox(pos); }

	TextRenderer::batchedText TextRenderer::batchText(vec2 const& pos) { batchText(getDefaultPos(pos)); }
	TextRenderer::batchedText TextRenderer::batchText(std::wstring const& str, vec2 const& pos) { tss << str; batchText(pos); }

	void TextRenderer::drawText(vec2 const& pos) { drawText(getDefaultPos(pos)); }
	void TextRenderer::drawText(std::wstring const& str, vec2 const& pos) { tss << str; drawText(pos); }

	void TextRenderer::batchTextBounded(bndtx& res, mat3 const& tr, aabb2 const& bound) {
		int curData = 0;

		std::wstring tssstr = tss.str();
		aabb2 totalBB = getTextBoundingBox(tss.str(), tr);
		aabb2 replacerBB = getTextBoundingBox(vboundReplacer, mat3(1.f));
		tss << tssstr;

		//deb::prt("totalbb size ", totalBB.size()," ",(tr.inverted()*totalBB).size(),"\n");

		mat3 rtr = pmat3::translate(-vec2(totalBB.size().x/2.f, totalBB.size().y) * ((vdrawOffset + 1.f) / 2.f)) * tr;

		aabb2 bborderCheck = rtr * tr.inverted() * totalBB;

		if (bborderCheck.minx() < bound.minx()) { float traslVal = (rtr.inverted() * bound.fmin()).x - (rtr.inverted() * bborderCheck.fmin()).x; rtr = rtr * pmat3::translate({ traslVal, 0.f }); }
		if (bborderCheck.miny() < bound.miny()) { float traslVal = (rtr.inverted() * bound.fmin()).y - (rtr.inverted() * bborderCheck.fmin()).y; rtr = rtr * pmat3::translate({ 0.f, traslVal }); }
		
		if (bborderCheck.maxy() > bound.maxy()) { float traslVal = (rtr.inverted() * bound.fmax()).y - (rtr.inverted() * bborderCheck.fmax()).y; rtr = rtr * pmat3::translate({ 0.f, traslVal }); }

		bool bounded = false;

		iterGlyphs(
			[&](std::wstring const& str, vec2& cur) {
				res.vdata.reserve(str.length() * 3 * (2 * 4 * 2));
				res.bb = cur;
			},
			[&](std::wstring& str, int curi, vec2 const& pos, vec2 const& size, vec2 const& txmin, vec2 const& txmax, bool& stop) {
				res.bb = res.bb.rescaled(pos);
				res.bb = res.bb.rescaled(pos + size);

				if (!bounded && curi < str.length()-1 && (rtr * (res.bb.fmax(res.bb.fmax() + vec2((replacerBB).sizex(), 0.f)))).maxx() >= bound.maxx()) {
					str = str.substr(0, curi);
					str += L" ";
					str += vboundReplacer;
					bounded = true;
				}

				vec2 v0 = rtr * pos;
				vec2 v1 = rtr * (pos + size.oy());
				vec2 v2 = rtr * (pos + size);
				vec2 v3 = rtr * (pos + size.xo());

				vec2 tx0 = { txmin.x, txmax.y };
				vec2 tx1 = txmin;
				vec2 tx2 = { txmax.x, txmin.y };
				vec2 tx3 = txmax;

				res.vdata.insert(
					res.vdata.end(), 
					{
						v0.x, v0.y,  vcolor.x, vcolor.y, vcolor.z, vcolor.w,  tx0.x, tx0.y,
						v1.x, v1.y,  vcolor.x, vcolor.y, vcolor.z, vcolor.w,  tx1.x, tx1.y,
						v2.x, v2.y,  vcolor.x, vcolor.y, vcolor.z, vcolor.w,  tx2.x, tx2.y,

						v2.x, v2.y,  vcolor.x, vcolor.y, vcolor.z, vcolor.w,  tx2.x, tx2.y,
						v3.x, v3.y,  vcolor.x, vcolor.y, vcolor.z, vcolor.w,  tx3.x, tx3.y,
						v0.x, v0.y,  vcolor.x, vcolor.y, vcolor.z, vcolor.w,  tx0.x, tx0.y,
					}
				);
			},
			[&] {
				res.bb = rtr * res.bb;
			}
		);
	}

	TextRenderer::bndtx TextRenderer::batchTextBounded(mat3 const& tr, aabb2 const& bound) { bndtx res; batchTextBounded(res, tr, bound); return res; }
	TextRenderer::bndtx TextRenderer::batchTextBounded(std::wstring const& str, mat3 const& tr, aabb2 const& bound) { tss << str; return batchTextBounded(tr, bound); }

	void TextRenderer::drawTextBounded(mat3 const& tr, aabb2 const& bound) { 
		bndtx btx = batchTextBounded(tr, bound); 
		if (btx.vdata.size() == 0) return;

		vb.setData(btx.vdata);
		getBackAtlas().tex.bind();
		util::defTexShader.use();
		vb.draw();
	}
	void TextRenderer::drawTextBounded(std::wstring const& str, mat3 const& tr, aabb2 const& bound) { tss << str; drawTextBounded(tr, bound); }

	TextRenderer::bndtx TextRenderer::batchTextBounded(vec2 const& pos, aabb2 const& bound) { return batchTextBounded(getDefaultPos(pos), bound); }
	TextRenderer::bndtx TextRenderer::batchTextBounded(std::wstring const& str, vec2 const& pos, aabb2 const& bound) { tss << str; return batchTextBounded(pos, bound); }

	void TextRenderer::drawTextBounded(vec2 const& pos, aabb2 const& bound) { drawTextBounded(getDefaultPos(pos), bound); }
	void TextRenderer::drawTextBounded(std::wstring const& str, vec2 const& pos, aabb2 const& bound) { tss << str; drawTextBounded(pos, bound); }

	void TextRenderer::dispose() {
		vb.dispose();
	}
	
	namespace txrutl {
		mat3 getDefaultScaleMatrix(vec2 const& pos) { return getDefaultPos(pos); }
		mat3 matToPos(mat3 const& tr) { return pmat3::translate(tr.translationv()) * pmat3::scale(2.f/cst::res()); }
	}
}
