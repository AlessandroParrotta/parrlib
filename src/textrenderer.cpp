#include <parrlib/TextRenderer.h>

#include <parrlib/debug.h>

TextRenderer::TextRenderer() {

}

void TextRenderer::addFont(std::string const& font) {
	fonts[font];//initialize the font
	auto& f = fonts[font];  //reference to the font
	
	std::string ffont = stru::fallbackPath(font);

	if (FT_New_Face(library, ffont.c_str(), 0, &f.face)) {
		fprintf(stderr, "Error: Could not load font '%s'.", font.c_str());
		fonts.erase(font);
		return;
	}

	FT_Set_Pixel_Sizes(f.face, 0, fontSize);
	FT_Select_Charmap(f.face, ft_encoding_unicode);

	f.g = f.face->glyph;

	FT_Stroker_New(library, &f.stroker);

	//f.atlas.atlas.setFiltering(minFilter, magFilter);
	//f.outlineAtlas.atlas.setFiltering(minFilter, magFilter);

	if (fontName.compare("") == 0) fontName = font;
}

TextRenderer::TextRenderer(std::vector<std::string> const& fontNames, int const& fontSize) {
	this->fontSize = fontSize;
	//modifier = util::getAspectOrtho();

	atlas.tex.setFiltering(minFilter, magFilter);

	sh = Shader("assets/shaders/textRender.vert", "assets/shaders/textRender.frag");

	vao = { { {{}, 4}, {{}, 4 } } }; //4 cause xy = pos, zw = texCoord, 4 color

	if (FT_Init_FreeType(&library)) {
		fprintf(stderr, "\nError: Could not load FreeType library.");
		std::terminate();
	}

	if (fontNames.size() <= 0) return;

	for (auto& sf : fontNames) {
		addFont(sf);
	}

	int maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	//std::cout << "max tex size " << maxTextureSize << "\n";

	maxAtlasSize = vec2((float)maxTextureSize);
}

void TextRenderer::updateAndResizeAtlas(Atlas& at, vec2 gSize) {
	at.atlasCurMax = { at.atlasCur.x + gSize.x, std::fmax(at.atlasCurMax.y, gSize.y) };
	at.atlasCurLine = { at.atlasCur.x + gSize.x, at.atlasCur.y + at.atlasCurMax.y };

	if (at.tex.null()) {
		if (gSize.x == 0.f || gSize.y == 0.f) gSize = 1.f;
		at.tex = Texture(vec2(gSize.x, gSize.y + border.y), GL_RED, GL_RED, GL_UNSIGNED_BYTE);
		//at.atlas.setFiltering(GL_NEAREST, GL_NEAREST);
		at.tex.setFiltering(minFilter, magFilter);
		at.atlasCur.x = border.x + 0.f;
		at.atlasCur.y = border.y + 0.f;
		at.tex.setData(vec4(0.f));
	}

	while (at.atlasCurLine.x > at.tex.size.x - border.x && at.atlasCurLine.x < maxAtlasSize.x - border.x) {
		if (at.tex.size.x == 0.f) at.tex.size.x = 1.f;
		at.tex.resize(vec2(std::fmin(at.tex.size.x * 2, maxAtlasSize.x), at.tex.size.y));
	}

	if (at.atlasCurLine.x > maxAtlasSize.x - border.x) {
		at.tex.resize(at.tex.size + vec2(0.f, gSize.y + spacing.y));
		at.atlasCur.x = border.x + 0.f;
		at.atlasCur.y += at.atlasCurMax.y + spacing.y;
		at.atlasCurMax = 0.f;
	}

	if (at.atlasCurLine.y > at.tex.size.y - border.y) {
		at.tex.resize(vec2(at.tex.size.x, at.atlasCurLine.y + border.y));
	}
}

void TextRenderer::loadGlyph(std::string const& font, unsigned long id) {
	//if (!outlineEnabled) return;

	auto& f = fonts[font];
	Atlas& at = atlas;

	for (int i = 0; i < 2; i++) { //once normal once outline

		std::unordered_map<wchar_t, glyph>* ginfot = nullptr;
		if (i > 0)	ginfot = &fonts[font].goInfo;
		else		ginfot = &fonts[font].gInfo;

		if (ginfot->find(id) != ginfot->end()) continue;

		FT_UInt gIndex = FT_Get_Char_Index(f.face, id);
		int oldid = id;
		if (gIndex == 0 && id != 0) { //if this font doesn't have the glyph
			//char not recognized (shows as a tofu, an empty rectangle)
			//std::cout << "char not found " << id << "\n";
			//if (at.tofuLoaded) { (*ginfot)[id] = (*ginfot)[0]; return; }
			//else { id = 0; at.tofuLoaded = true; }

			loadGlyph(font, 0);

			(*ginfot)[id] = (*ginfot)[0];

			(*ginfot)[id].loaded = false;

			continue;
		}
		if (FT_Load_Glyph(f.face, gIndex, FT_LOAD_DEFAULT)) return;

		FT_Glyph gl;
		if (FT_Get_Glyph(f.face->glyph, &gl)) return;

		if (i > 0) { if (FT_Glyph_StrokeBorder(&gl, f.stroker, false, true)) return; }

		if (FT_Glyph_To_Bitmap(&gl, FT_RENDER_MODE_NORMAL, nullptr, true)) return;
		FT_BitmapGlyph bitmapg = reinterpret_cast<FT_BitmapGlyph>(gl);

		FT_Bitmap bitmap = bitmapg->bitmap;

		unsigned char* buffer = bitmap.buffer;
		vec2 gSize = vec2(bitmap.width, bitmap.rows);

		updateAndResizeAtlas(at, gSize);

		vec2 start = at.atlasCur;

		(*ginfot)[id].advx = gl->advance.x >> 16;
		(*ginfot)[id].advy = gl->advance.y >> 16;

		(*ginfot)[id].bitw = bitmap.width;
		(*ginfot)[id].bitr = bitmap.rows;

		(*ginfot)[id].bitl = bitmapg->left;
		(*ginfot)[id].bitt = bitmapg->top;

		(*ginfot)[id].txmin = at.atlasCur;
		(*ginfot)[id].txmax = at.atlasCur + gSize;

		//if (gIndex == 0) { (*ginfot)[0].loaded = false; (*ginfot)[oldid] = (*ginfot)[0]; }

		//skip ' ' character, generates an error if it's the first element to be loaded into the atlas
		if ((wchar_t)id != L' ') {
			at.tex.fillRegion(at.atlasCur, gSize, GL_RED, buffer);
		}

		at.atlasCur.x += bitmap.width + spacing.x;
	}
}

//check if the glyph is loaded or is loadable, in case it's not loadable with the default font 
//load it with the first font that is able to render it
//returns the font with which this character has been rendered (default font if it's not renderable with any font)
std::string TextRenderer::checkAndLoadGlyph(std::string const& font, wchar_t const& c) {
	std::unordered_map<wchar_t, glyph>* gInfo = nullptr;
	if (outlineEnabled) gInfo = &fonts[font].goInfo;
	else				gInfo = &fonts[font].gInfo;

	if (gInfo->find(c) == gInfo->end()) { loadGlyph(font, (unsigned long)c); }
	if (fonts.size() > 1 && !(*gInfo)[c].loaded) { //if the char is not supported by the font, find the first font that supports it
		for (auto& f : fonts) {
			if (f.first.compare(font) == 0) continue;

			//std::unordered_map<wchar_t, glyph>* ginfot = &f.second.gInfo;
			//if (outlineEnabled) gInfo = &f.second.goInfo;					//this seems like an error, gInfo? not ginfot??

			std::unordered_map<wchar_t, glyph>* ginfot = nullptr;
			if (outlineEnabled) ginfot = &f.second.goInfo;
			else				ginfot = &f.second.gInfo;

			//if the char is not loaded for that font, load it
			if (ginfot->find(c) == ginfot->end()) { loadGlyph(f.first, (unsigned long)c); }

			if (ginfot->at(c).loaded) return f.first;
		}
	}

	return font;
}

TextRenderer::DrawRes TextRenderer::drawWStringPriv(std::wstring const& str, std::string const& font, vec2 const& off, mat3 const& transform) {
	int fnum = str.length() * (3 * 2) * 4 * 2;

	if (data.size() < fnum) data.resize(fnum);
	int dcount = 0;

	if (dataColor.size() < fnum) dataColor.resize(fnum);
	int ccount = 0;

	bool failedChar = false;

	aabb2 bb = 0.f;

	int count = 0;
	vec2 cur = 0.f;
	for (int i = 0; i < 1 + outlineEnabled; i++) {
		vec4 color = voutlineColor;
		if (i > 0 || !outlineEnabled) color = vcolor;

		std::string tfont = font;

		for (wchar_t c : str) {
			std::string cfont = checkAndLoadGlyph(tfont, c);
			if (cfont.compare(tfont) != 0) tfont = cfont;

			std::unordered_map<wchar_t, glyph>* ginfot = nullptr;
			if (i > 0 || !outlineEnabled)	ginfot = &fonts[cfont].gInfo;
			else							ginfot = &fonts[cfont].goInfo;

			vec2 pos = -1.f, txmin;
			vec2 size = 2.f, txmax;

			glyph& g = ginfot->at(c);
			pos = cur + vec2(g.bitl, -(g.bitr - g.bitt)) - (minFilter == GL_LINEAR ? txBias : 0.f);
			size = vec2(g.bitw, g.bitr) + (minFilter == GL_LINEAR ? txBias : 0.f) * 2.f;

			txmin = g.txmin - (minFilter == GL_LINEAR ? txBias : 0.f);
			txmax = g.txmax + (minFilter == GL_LINEAR ? txBias : 0.f);

			cur += vec2(g.advx, g.advy);

			vec2 v0 = pos;
			vec2 v1 = pos + size.oy();
			vec2 v2 = pos + size;
			vec2 v3 = pos + size.xo();

			vec2 tx0 = { txmin.x, txmax.y };
			vec2 tx1 = txmin;
			vec2 tx2 = { txmax.x, txmin.y };
			vec2 tx3 = txmax;

			data[dcount++] = v0.x; data[dcount++] = v0.y; data[dcount++] = tx0.x; data[dcount++] = tx0.y;
			data[dcount++] = v1.x; data[dcount++] = v1.y; data[dcount++] = tx1.x; data[dcount++] = tx1.y;
			data[dcount++] = v2.x; data[dcount++] = v2.y; data[dcount++] = tx2.x; data[dcount++] = tx2.y;

			data[dcount++] = v2.x; data[dcount++] = v2.y; data[dcount++] = tx2.x; data[dcount++] = tx2.y;
			data[dcount++] = v3.x; data[dcount++] = v3.y; data[dcount++] = tx3.x; data[dcount++] = tx3.y;
			data[dcount++] = v0.x; data[dcount++] = v0.y; data[dcount++] = tx0.x; data[dcount++] = tx0.y;

			dataColor[ccount++] = color.x; dataColor[ccount++] = color.y; dataColor[ccount++] = color.z; dataColor[ccount++] = color.w;
			dataColor[ccount++] = color.x; dataColor[ccount++] = color.y; dataColor[ccount++] = color.z; dataColor[ccount++] = color.w;
			dataColor[ccount++] = color.x; dataColor[ccount++] = color.y; dataColor[ccount++] = color.z; dataColor[ccount++] = color.w;

			dataColor[ccount++] = color.x; dataColor[ccount++] = color.y; dataColor[ccount++] = color.z; dataColor[ccount++] = color.w;
			dataColor[ccount++] = color.x; dataColor[ccount++] = color.y; dataColor[ccount++] = color.z; dataColor[ccount++] = color.w;
			dataColor[ccount++] = color.x; dataColor[ccount++] = color.y; dataColor[ccount++] = color.z; dataColor[ccount++] = color.w;

			if (i == 0) count++;

			if (i == 0) {
				if (bb == 0.f) {
					bb = { pos, pos + size };
				}
				else {
					bb.rescale(pos);
					bb.rescale(pos + size);
					bb.rescale(cur);
				}
			}
		}

		if (count == 0) return { 0, cur };

		if (i == 0) cur = 0.f;
	}
	//std::cout << stru::tostr(str) << " " << bb << "\n";
	aabb2 targetbb = bb.centered();
	targetbb = targetbb - targetbb.fmax() + targetbb.size() * ((off + 1.f) / 2.f);

	vao.vbos[0].setData(data, dcount);
	vao.vbos[1].setData(dataColor, ccount);

	util::texEnable();
	bool blenden = util::blendEnabled(); if(!blenden) util::blendEnable();
	sh.use();
	sh.setUniform("transform", util::getTopMatrix() * transform * pmat3::translate(targetbb.center() - bb.center()));
	sh.setUniform("tex", 0);
	sh.setUniform("atlasSize", atlas.tex.size);
	util::bindTexture(0, atlas.tex.texID);

	vao.draw();
	sh.release();
	if (!blenden) util::blendDisable();
	util::texDisable();

	return { count, cur };
}

void TextRenderer::drawWStringReal(std::wstring const& str, std::string const& font, vec2 const& off, mat3 const& transform) {
	drawWStringPriv(str, font, off, transform);
}
void TextRenderer::drawWStringReal(std::wstring const& str, mat3 const& transform) { drawWStringReal(str, fontName, 0.f, transform); }
void TextRenderer::drawWStringReal(mat3 const& transform) { drawWStringReal(wostr.str(), transform); wostr.str(L""); wostr.clear(); }

void TextRenderer::drawWString(std::wstring const& str, std::string const& font, mat3 const& transform) {
	aabb2 bb = getAABBReal(str, font, 0.f, transform);
	//mat3 mod = correctNdc ? pmat3::orthoMinAspect(bb.size()).inverted() : 1.f;
	drawWStringReal(str, font, 0.f, transform * pmat3::getNdc(bb));
}
void TextRenderer::drawWString(std::wstring const& str, mat3 const& transform) { drawWString(str, fontName, transform); }
void TextRenderer::drawWString(std::wstring const& str, vec2 pos) { drawWString(str, pmat3::translate(pos)); }

void TextRenderer::drawString(std::string const& str, mat3 const& transform) { drawWString(stringutils::toWString(str), transform); }
void TextRenderer::drawString(std::string const& str, vec2 pos) { drawWString(stringutils::toWString(str), pmat3::translate(pos)); }

void TextRenderer::drawWStringpx(std::wstring const& str, std::string const& font, vec2 const& offset, mat3 const& transform) {
	GLint oldMag = magFilter, oldMin = minFilter;
	if (oldMag != GL_NEAREST || oldMin != GL_NEAREST) setFiltering(GL_NEAREST, GL_NEAREST);

	//aabb2 bb = getAABBReal(str, font, 0.f, 1.f);

	//vec2 bias = 0.01f;
	//mat3 retranslate = pmat3::translate(bias);
	//vec2 precalc = vec2(2.f / cst::resx(), 2.f / cst::resy());
	//vec2 percsize = vec2(precalc.x * (precalc.y / precalc.x), precalc.y);
	//retranslate = pmat3::scale(percsize) * pmat3::translate(-bb.fmin()) * retranslate;
	//retranslate = retranslate * pmat3::translate(-bb.size() / 2 + (bb.size() / 2) * offset);

	//aabb2 bbpx = pmat3::getNdc({ 0.f, std::fmin(cst::resx(), cst::resy()) }) * bb;
	//mat3 retranslate = pmat3::translate(-bbpx.size() / 2 + (bbpx.size() / 2) * offset) * pmat3::scale(bbpx.size()/bb.size()) * pmat3::translate(0.01f);

	//drawWStringReal(str, font, 0.f, transform * retranslate);
	drawWStringReal(str, font, offset, transform * util::getAspectOrtho().inverted() * pmat3::toNdc({0.f, cst::res()}) * pmat3::translate(cst::res()/2.f) * pmat3::translate(0.01f));

	if (oldMag != GL_NEAREST || oldMin != GL_NEAREST) setFiltering(oldMin, oldMag);
}
void TextRenderer::drawWStringpx(std::wstring const& str, std::string const& font, mat3 const& transform) { drawWStringpx(str, font, 0.f, transform); }
void TextRenderer::drawWStringpx(std::wstring const& str, vec2 const& offset, mat3 const& transform) { drawWStringpx(str, fontName, offset, transform); }
void TextRenderer::drawWStringpx(std::wstring const& str, mat3 const& transform) { drawWStringpx(str, fontName, transform); }

void TextRenderer::drawWStringpx(std::wstring const& str, std::wstring const& modstr, std::string const& font, vec2 const& offset, aabb2 const& tbound, mat3 const& transform) {
	bwstring bws = getWStringBoundedReal(str, font, util::fromNdcAspect() * tbound, offset, modstr, transform);
	drawWStringpx(bws.str, font, offset, transform * pmat3::translate(pmat3::scale(cst::res().aspectmaxv()) * pmat3::scale(2.f / cst::res()) * bws.inoffset));
}
void TextRenderer::drawWStringpx(std::wstring const& str, std::string const& font, vec2 const& offset, aabb2 const& tbound, mat3 const& transform) { drawWStringpx(str, L"...", font, offset, tbound, transform); }
void TextRenderer::drawWStringpx(std::wstring const& str, vec2 const& offset, aabb2 const& bound, mat3 const& transform)	{ drawWStringpx(str, fontName, offset, bound, transform); }
void TextRenderer::drawWStringpx(std::wstring const& str, std::wstring const& modstr, vec2 const& offset, aabb2 const& tbound, mat3 const& transform) { drawWStringpx(str, modstr, fontName, offset, tbound, transform); }
void TextRenderer::drawWStringpx(std::wstring const& str, aabb2 const& bound, mat3 const& transform)						{ drawWStringpx(str, fontName, 0.f, bound, transform); }

void TextRenderer::drawWString(mat3 const& transform) {
	std::wstring str = wostr.str();
	wostr.str(L"");
	wostr.clear();

	drawWString(str, transform);
}
void TextRenderer::drawWString(vec2 pos) { drawWString(pmat3::translate(pos)); }

void TextRenderer::drawWStringpx(std::string const& font, vec2 const& offset, mat3 const& transform) {
	std::wstring str = wostr.str();
	wostr.str(L"");
	wostr.clear();

	drawWStringpx(str, font, offset, transform);
}
void TextRenderer::drawWStringpx(vec2 const& offset, mat3 const& transform) { drawWStringpx(fontName, offset, transform); }
void TextRenderer::drawWStringpx(mat3 const& transform)						{ drawWStringpx(fontName, 0.f, transform); }

void TextRenderer::drawWStringpx(std::string const& font, vec2 const& offset, aabb2 const& bound, mat3 const& transform) {
	std::wstring str = wostr.str();
	wostr.str(L"");
	wostr.clear();

	drawWStringpx(str, font, offset, bound, transform);
}

aabb2 TextRenderer::getAABBReal(std::wstring const& str, std::string const& font, vec2 const& off, mat3 const& transform) {
	std::string tfont = font;

	aabb2 bb = 0.f;
	vec2 cur = 0.f;
	for (wchar_t c : str) {
		//check if the glyph is loaded or is loadable, in case it's not loadable with the default font 
		//load it with the first font that is able to render it
		std::string cfont = checkAndLoadGlyph(tfont, c);
		if (cfont.compare(tfont) != 0) tfont = cfont;

		std::unordered_map<wchar_t, glyph>* gInfo = nullptr;
		if (outlineEnabled) gInfo = &fonts[cfont].goInfo;
		else				gInfo = &fonts[cfont].gInfo;

		glyph& g = (*gInfo)[c];
		vec2 pos = cur + vec2(g.bitl, -(g.bitr - g.bitt)) - (minFilter == GL_LINEAR ? txBias : 0.f);
		vec2 size = vec2(g.bitw, g.bitr) + (minFilter == GL_LINEAR ? txBias : 0.f) * 2.f;
		cur += vec2(g.advx, g.advy);

		if (bb.fmin() == 0.f && bb.fmax() == 0.f) {
			bb = aabb2(pos, pos + size);
			//std::cout << bb << "\n";
		}
		else {
			bb.rescale(pos);
			bb.rescale(pos + size);
			bb.rescale(cur);
		}
	}

	bb = bb.centered();
	bb = bb - bb.fmax() + bb.size() * ((off+1.f)/2.f);

	return transform * bb;
}
aabb2 TextRenderer::getAABBReal(std::wstring const& str, mat3 const& transform) { return getAABBReal(str, fontName, 0.f, transform); }
aabb2 TextRenderer::getAABBReal(mat3 const& transform) { return getAABBReal(wostr.str(), fontName, 0.f, transform); }

aabb2 TextRenderer::getAABB(std::wstring const& str, mat3 const& transform = 1.f) {
	//if (correctNdc) return transform * aabb2(-1.f, 2.f);

	aabb2 bb = getAABBReal(str, fontName, 0.f, transform);
	//mat3 mod = correctNdc ? pmat3::orthoMinAspect(bb.size()).inverted() : 1.f;
	return  transform * pmat3::getNdc(bb) * bb;
}
aabb2 TextRenderer::getAABB(mat3 const& transform = 1.f) { return getAABB(wostr.str()); }

aabb2 TextRenderer::getAABBpx(std::wstring const& str, std::string const& font, vec2 const& offset, mat3 const& transform = 1.f) {
	GLint oldMag = magFilter, oldMin = minFilter;
	if (oldMag != GL_NEAREST || oldMin != GL_NEAREST) setFiltering(GL_NEAREST, GL_NEAREST);

	aabb2 bb = getAABBReal(str, font, offset, 1.f);

	//vec2 bias = 0.01f;
	//mat3 retranslate = pmat3::translate(bias);
	////vec2 precalc = vec2(((2.f / ((float)constants::WIDTH))), ((2.f / ((float)constants::HEIGHT))));
	//vec2 precalc = 2.f / cst::res();
	//vec2 percsize = vec2(precalc.x * (precalc.y / precalc.x), precalc.y);
	//retranslate = pmat3::scale(percsize) * pmat3::translate(-bb.fmin()) * retranslate;
	//retranslate = retranslate * pmat3::translate(-bb.size() / 2 + (bb.size() / 2) * offset);

	//aabb2 bbpx = pmat3::orthoMaxAspect(cst::res()).inverted() * pmat3::getNdc({ 0.f, cst::res() }) * bb; //its not really NDC, you gotta take into account the aspect ratio
	//mat3 retranslate = pmat3::translate(-bbpx.size() / 2 + (bbpx.size() / 2) * offset) * pmat3::scale(bbpx.size() / bb.size()) * pmat3::translate(0.01f);  //for some reason a bias of 0.01 fixes the artifacts created from scaling down to screen space, idk

	//mat3 mul = transform * retranslate;

	if (oldMag != GL_NEAREST || oldMin != GL_NEAREST) setFiltering(oldMin, oldMag);

	//return mul * bb;
	return (transform * util::getAspectOrtho().inverted()* pmat3::toNdc({ 0.f, cst::res() }) * pmat3::translate(cst::res() / 2.f) * pmat3::translate(0.01f)) * bb;
}
aabb2 TextRenderer::getAABBpx(std::wstring const& str, vec2 const& offset, mat3 const& transform = 1.f) { return getAABBpx(str, fontName, offset, transform); }
aabb2 TextRenderer::getAABBpx(std::wstring const& str, mat3 const& transform = 1.f) { return getAABBpx(str, fontName, 0.f, transform); }
aabb2 TextRenderer::getAABBpx(mat3 const& transform = 1.f) { return getAABBpx(wostr.str(), transform); }

TextRenderer::bwstring TextRenderer::getWStringBoundedReal(std::wstring const& str, std::string const& font, aabb2 const& bound, vec2 const& offset, std::wstring const& modstr, mat3 const& transform) {
	aabb2 strbb = getAABBReal(str, font, offset, 1.f);
	aabb2 modbb = getAABBReal(modstr, font, offset, 1.f);
	vec2 realoffset = strbb.fmin() * offset + (strbb + strbb.fmin()).fmax() * ((-offset + 1.f) / 2.f);

	strbb += cst::res() / 2.f;
	strbb = util::fromNdcAspect() * transform * util::toNdcAspectInv() * (strbb);  //in order to correctly apply the transform (since it resides in another coordinate space), i need to first scale down strbb to its coordinate scale, apply the transform and then retransform it in its original coordinate space
	//debug::rtlog << strbb << "\n";

	//mat3 proj = util::getAspectOrtho().inverted() * util::toNdc();
	//debug::drawQuad(util::getTopMatrix() * proj *aabb2(0.f, cst::res()), vc4::green);
	//debug::drawQuad(util::getTopMatrix() * proj * bound);
	//debug::drawText(stru::tostr(str), (util::getTopMatrix() * proj * bound).center(), 1.f);
	//debug::drawQuad(util::getTopMatrix() * proj * strbb);

	vec2 inoffset = (bound.fmin() - strbb.fmin()).maxed(0.f) +
		(bound.fmax() - strbb.fmax()).minned(0.f);


	//debug::rtlog << "inoffset " << str << ": " << inoffset << "\n";
	//debug::rtlog << inoffset << "\n";

	//debug::rtlog << "inoffset " << inoffset.toString() << "\n";

	if (strbb.size().y > bound.size().y) {
		//realoffset = modbb.fmin() * offset + (modbb + modbb.fmin()).fmax() * ((-offset + 1.f) / 2.f);
		modbb += cst::res() / 2.f;
		modbb = util::fromNdcAspect() * transform * util::toNdcAspectInv() * (modbb);
		inoffset = (bound.fmin() - modbb.fmin()).maxed(0.f) + (bound.fmax() - modbb.fmax()).minned(0.f);
		return { modstr, inoffset, modbb };
	}
	if (strbb.inside(bound)) { return { str, inoffset, strbb }; }

	//this string is not inside the bound

	if (strbb.size().x <= bound.size().x) { //if the string is not inside the bound but its size is less than the bound it means that i can 'shift' it without changing its text
		return { str, inoffset, strbb };
	}
	
	//if the code reaches here it means that the string's bound size is greater than the bound itself
	//so ill have to trim it
	//i can take for granted that the string will begin at coordinate X equal to bound.fmin().x

	std::string tfont = font;
	std::wstring resstr = str;

	aabb2 bb = 0.f;
	vec2 cur = 0.f;
	for (int i = 0; i < str.length(); i++) {
		wchar_t c = str[i];

		std::string cfont = checkAndLoadGlyph(tfont, c);
		if (cfont.compare(tfont) != 0) tfont = cfont;

		std::unordered_map<wchar_t, glyph>* gInfo = nullptr;
		if (outlineEnabled) gInfo = &fonts[cfont].goInfo;
		else				gInfo = &fonts[cfont].gInfo;

		glyph& g = (*gInfo)[c];
		vec2 pos = cur + vec2(g.bitl, -(g.bitr - g.bitt)) - (minFilter == GL_LINEAR ? txBias : 0.f);
		vec2 size = vec2(g.bitw, g.bitr) + (minFilter == GL_LINEAR ? txBias : 0.f) * 2.f;
		cur += vec2(g.advx, g.advy);

		if (cur.x + modbb.size().x > bound.size().x) { resstr = resstr.substr(0, i); break; }

		if (bb == 0.f) {
			bb = aabb2(pos, pos + size);
			//std::cout << bb << "\n";
		}
		else {
			bb.rescale(pos);
			bb.rescale(pos + size);
			bb.rescale(cur);
		}
	}

	//debug::rtlog << inoffset << " resstr='" << resstr << "'\n";

	bb = bb.centered();
	bb = bb - bb.fmax() + bb.size() * ((offset + 1.f) / 2.f);

	//realoffset has to change
	bb.fmaxr({ bb.fmax().x + modbb.size().x, bb.fmax().y });
	realoffset = bb.fmin() * offset + (bb + bb.fmin()).fmax() * ((-offset + 1.f) / 2.f);

	bb += cst::res() / 2.f;
	bb = util::fromNdcAspect() * transform * util::toNdcAspectInv() * (bb - realoffset);

	//debug::rtlog << bb.fmin() << " " << bound.fmin() << "\n";

	inoffset = {
		bound.fmin().x - bb.fmin().x,

		std::fmin(bound.fmax().y - bb.fmax().y, 0.f) +
		std::fmax(bound.fmin().y - bb.fmin().y, 0.f)
	};

	//debug::rtlog << "new inoffset " << inoffset << "\n";

	//bb.fmax({ bb.fmax().x + modbb.size().x/2.f, bb.fmax().y });

	//return { resstr + modstr, inoffset, aabb2(bb.fmin(), { bb.fmax().x+modbb.size().x, std::fmax(bb.fmax().y, modbb.size().y) }) - realoffset };
	return { resstr + modstr, inoffset, bb };
}
TextRenderer::bwstring TextRenderer::getWStringBoundedpx(std::wstring const& str, std::string const& font, aabb2 const& bound, vec2 const& offset, std::wstring const& modstr, mat3 const& transform) {
	bwstring bws = getWStringBoundedReal(str, font, util::fromNdcAspect() * bound, offset, modstr, transform);
	bws.bound = util::toNdcAspectInv() * bws.bound;
	bws.inoffset = transform * pmat3::scale(cst::res().aspectmaxv()) * pmat3::scale(2.f / cst::res()) * bws.inoffset;
	return bws;
}
TextRenderer::bwstring TextRenderer::getWStringBoundedpx(std::wstring const& str, aabb2 const& bound, vec2 const& offset, std::wstring const& modstr, mat3 const& transform) { return getWStringBoundedpx(str, fontName, bound, offset, modstr, transform); }
TextRenderer::bwstring TextRenderer::getWStringBoundedpx(std::wstring const& str, aabb2 const& bound, vec2 const& offset, mat3 const& transform) { return getWStringBoundedpx(str, fontName, bound, offset, L"...", transform); }
TextRenderer::bwstring TextRenderer::getWStringBoundedpx(std::wstring const& str, aabb2 const& bound, mat3 const& transform) { return getWStringBoundedpx(str, fontName, bound, vec2(0.f), L"...", transform); }

void TextRenderer::setFontSize(int fontSize) {
	this->fontSize = fontSize;
	for (auto& f : fonts) FT_Set_Pixel_Sizes(f.second.face, 0, fontSize);
	clear();
}
int TextRenderer::getFontSize() { return fontSize; }

void TextRenderer::setOutline(unsigned int px) {
	outlineEnabled = px > 0;

	outlinepx = px;
	//1/64 of a pixel * 64 * 2 = 2 pixels
	//FT_Stroker_Set(fonts[fontName].stroker, outlinepx*64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

	//outlineAtlas.dispose();
	//for (auto& g : atlas.gInfo) loadOutlineGlyph(g.first);

	for (auto& f : fonts) {
		FT_Stroker_Set(fonts[f.first].stroker, outlinepx * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
		//f.second.outlineAtlas.dispose();
		//for (auto& g : f.second.atlas.gInfo) loadOutlineGlyph(f.first, g.first);
	}

	clear();
}
unsigned int TextRenderer::getOutline()					{ return outlinepx; }

void TextRenderer::setFont(std::string const& fontName) { this->fontName = fontName; /*clear();*/ }
std::string TextRenderer::getFontName()					{ return fontName; }

void TextRenderer::color(vec4 color)					{ vcolor = color; }
vec4 TextRenderer::color()								{ return vcolor; }

void TextRenderer::outlineColor(vec4 outlineColor)		{ voutlineColor = outlineColor; }
vec4 TextRenderer::outlineColor()						{ return voutlineColor; }

void TextRenderer::setFiltering(GLint min, GLint mag) {
	minFilter = min;
	magFilter = mag;
	//for (auto& f : fonts) {
	//	//std::cout << f.first << " setting filter " << util::glIntToString(min) << " " << util::glIntToString(mag) << "\n";
	//	f.second.atlas.atlas.setFiltering(min, mag);
	//	f.second.outlineAtlas.atlas.setFiltering(min, mag);
	//}
	atlas.tex.setFiltering(min, mag);
}
GLint TextRenderer::getMagFilter() { return magFilter; }
GLint TextRenderer::getMinFilter() { return minFilter; }

//Texture TextRenderer::getOutlineAtlas(std::string const& font) { return fonts[font].outlineAtlas.atlas; }
Texture TextRenderer::getAtlas() { return atlas.tex; }
//Texture TextRenderer::getOutlineAtlas() { return getOutlineAtlas(fontName); }

//std::unordered_map<std::string, TextRenderer::Font>* TextRenderer::getUM() { return &fonts; }
std::vector<std::string> TextRenderer::getFonts() {
	std::vector<std::string> res;
	res.reserve(fonts.size());

	for (auto& f : fonts) res.push_back(f.first);

	return res;
}

void TextRenderer::setCorrectNdc(bool correctNdc) { this->correctNdc = correctNdc; }
bool TextRenderer::getCorrectNdc() const { return correctNdc; }

void TextRenderer::clear() {
	atlas.dispose();
	//outlineAtlas.dispose();

	for (auto& f : fonts) {
		//f.second.atlas.dispose();
		//f.second.outlineAtlas.dispose();
		f.second.gInfo.clear();
		f.second.goInfo.clear();
	}
}

void TextRenderer::dispose() {
	//atlas.atlas.dispose();
	clear();
	vao.dispose();
}

//GETWRSTRINGBOUNDEDREALOLD
//
//TextRenderer::bwstring TextRenderer::getWStringBoundedReal(std::string const& font, aabb2 const& bound, std::wstring const& str, vec2 const& offset, std::wstring const& modstr, mat3 const& transform) {
//	aabb2 strbb = getAABBReal(font, str);
//	aabb2 modbb = getAABBReal(font, modstr);
//	vec2 realoffset = strbb.fmin() * offset + (strbb + strbb.fmin()).fmax() * ((-offset + 1.f) / 2.f);
//
//	strbb = strbb - realoffset;
//	modbb = modbb - realoffset;
//
//	debug::rtlog << (bound) << "\n";
//	//debug::drawQuad(bound);
//	debug::rtlog << "transformed " << (pmat3::getNdc({ 0.f, cst::res() }).inverted() * bound) << "\n";
//
//	//aabb2 boundmod = bound; //modifier * bound;
//
//	mat3 invndc = pmat3::getNdc({ 0.f, cst::res() });
//
//	//assumes bound is in NDC space, -1 -> 1
//	//aabb2 realBound = pmat3::scale(cst::res() / 2.f) * bound;//boundmod;
//	aabb2 realBound = (invndc.inverted() * bound) - cst::res() / 2.f;
//	debug::rtlog << (realBound) << "\n";
//	//debug::rtlog << "realbound " << realBound.toString() << " " << realoffset.toString() << "\n";
//
//	debug::drawQuad(util::getTopMatrix() * (invndc * (strbb + cst::res() / 2.f)), vc4::green);
//	debug::drawQuad(util::getTopMatrix() * (invndc * (realBound + cst::res() / 2.f)));
//	debug::drawText(stru::tostr(str), (util::getTopMatrix() * (invndc * (realBound + cst::res() / 2.f))).center(), 1.f);
//
//	vec2 inoffset = (realBound.fmin() - strbb.fmin()).maxed(0.f) +
//		(realBound.fmax() - strbb.fmax()).minned(0.f);
//
//	debug::rtlog << inoffset << "\n";
//
//	//debug::rtlog << "inoffset " << inoffset.toString() << "\n";
//
//	if (strbb.size().y > realBound.size().y) { return { modstr, inoffset, modbb }; }
//	if (strbb.inside(realBound)) { return { str, inoffset, strbb }; }
//
//	//this string is not inside the bound
//
//	if (strbb.size().x <= realBound.size().x) { //if the string is not inside the bound but its size is less than the bound it means that i can 'shift' it without changing its text
//		return { str, inoffset, strbb };
//	}
//
//	std::string tfont = font;
//	std::wstring resstr = str;
//
//	aabb2 bb = 0.f;
//	vec2 cur = 0.f;
//	for (int i = 0; i < str.length(); i++) {
//		wchar_t c = str[i];
//
//		std::string cfont = checkAndLoadGlyph(tfont, c);
//		if (cfont.compare(tfont) != 0) tfont = cfont;
//
//		std::unordered_map<wchar_t, glyph>* gInfo = &fonts[cfont].gInfo;
//		if (outlineEnabled) gInfo = &fonts[cfont].goInfo;
//
//		glyph g = (*gInfo)[c];
//		vec2 pos = cur + vec2(g.bitl, -(g.bitr - g.bitt)) - (minFilter == GL_LINEAR ? txBias : 0.f);
//		vec2 size = vec2(g.bitw, g.bitr) + (minFilter == GL_LINEAR ? txBias : 0.f) * 2.f;
//		cur += vec2(g.advx, g.advy);
//
//		if (cur.x + modbb.size().x > realBound.size().x) { resstr = resstr.substr(0, i); break; }
//
//		if (bb.fmin() == 0.f && bb.fmax() == 0.f) {
//			bb = aabb2(pos, pos + size);
//			//std::cout << bb << "\n";
//		}
//		else {
//			bb.rescale(pos);
//			bb.rescale(pos + size);
//		}
//	}
//
//	//realoffset has to change
//	bb.fmax({ bb.fmax().x + modbb.size().x, bb.fmax().y });
//	realoffset = bb.fmin() * offset + (bb + bb.fmin()).fmax() * ((-offset + 1.f) / 2.f);
//
//	bb = bb - realoffset;
//
//	inoffset = {
//		realBound.fmin().x - bb.fmin().x,
//
//		std::fmin(realBound.fmax().y - bb.fmax().y, 0.f) +
//		std::fmax(realBound.fmin().y - bb.fmin().y, 0.f)
//	};
//
//	//bb.fmax({ bb.fmax().x + modbb.size().x/2.f, bb.fmax().y });
//
//	//return { resstr + modstr, inoffset, aabb2(bb.fmin(), { bb.fmax().x+modbb.size().x, std::fmax(bb.fmax().y, modbb.size().y) }) - realoffset };
//	return { resstr + modstr, inoffset, bb };
//}

//vec2 TextRenderer::getStringOffset(std::string const& text, float scale) { return getAABBpx().min; }
//vec2 TextRenderer::getStringSize(std::string const& text, float scale) { return getAABBpx().size(); }
//aabb2 TextRenderer::getStringBounds(std::string const& text, float scale) { return getAABBpx(); }
//float TextRenderer::getScaleToFit(std::string const& text, vec2 const& bound) {
//	vec2 size = getAABBpx().size();
//	return std::fmin(std::fabs(bound.x / size.x), std::fabs(bound.y / size.y));
//}
//
//float TextRenderer::getScreenPercScaleFactor(float perc) { return (1.f / (float)fontSize) * perc; }

//DRAW STRING BOUNDED UNOPTIMIZED
//
//void TextRenderer::drawWStringpx(std::wstring const& str, std::string const& font, vec2 const& offset, aabb2 const& tbound, mat3 const& transform) {
//	aabb2 bound = tbound;
//	aabb2 bb = getAABBpx(str, font, offset, transform);
//
//	if (bb.size().y > bound.size().y) { drawWStringpx(L"...", font, offset, transform); return; }
//	if (bb.inside(bound)) { drawWStringpx(str, font, offset, transform); return; }
//
//	if (bb.size().x <= bound.size().x) { //if the string is not inside the bound but its size is less than the bound it means that i can 'shift' it without changing its text
//		vec2 inoffset = (bound.fmin() - bb.fmin()).maxed(0.f) +
//			(bound.fmax() - bb.fmax()).minned(0.f);
//		drawWStringpx(str, font, offset, pmat3::translate(inoffset) * transform);
//
//		return;
//	}
//
//	//if the code reaches here it means that bb.sizex is greater than bound.sizex
//
//	mat3 tr = pmat3::translate(
//		vec2(
//			bound.fmin().x - bb.fmin().x,
//			std::fmax(bound.fmin().y - bb.fmin().y, 0.f) + std::fmin(bound.fmax().y - bb.fmax().y, 0.f)
//			)
//		) * transform;
//
//	bb = getAABBpx(str, font, offset, tr);
//
//	std::wstring tstr = str;
//	while (tstr.length() > 3 && bb.fmax().x > bound.fmax().x) {
//		tstr = tstr.substr(0, tstr.length() - 4) + L"...";
//
//		bb = getAABBpx(tstr, font, offset, tr);
//		tr = pmat3::translate(vec2(bound.fmin().x - bb.fmin().x, 0.f)) * tr;
//		bb = getAABBpx(tstr, font, offset, tr);
//	}
//
//	drawWStringpx(tstr, font, offset, tr);
//}

//1 to 1 pixel-perfect sampling of text (avoids artifacts, use GL_NEAREST filtering for texture)
//the bias is weird, it seems like without bias it generates artifacts
//any bias value <0.5 will fix artifacts (except 0.)
//vec2 bias = 0.1f;
//mat3 retranslate = pmat3::translate(bias);
//vec2 precalc = vec2(((2.f / ((float)constants::WIDTH))), ((2.f / ((float)constants::HEIGHT))));
//vec2 percsize = vec2(precalc.x * (precalc.y / precalc.x), precalc.y);
//retranslate = pmat3::scale(percsize) * pmat3::translate(-bb.min) * retranslate;


//POSSIBLE LOADING OF OUTLINED GLYPH
//
//if (glyphs.find(id) != glyphs.end()) return;
//FT_UInt gIndex = (unsigned int)id;
//if (FT_Load_Glyph(face, gIndex, FT_LOAD_DEFAULT)) return;
//
//FT_Glyph gl;
//if (FT_Get_Glyph(face->glyph, &gl)) return;
//if (FT_Glyph_StrokeBorder(&gl, stroker, false, true)) return;
//if (FT_Glyph_To_Bitmap(&gl, FT_RENDER_MODE_NORMAL, nullptr, true)) return;
//FT_BitmapGlyph bitmapg = reinterpret_cast<FT_BitmapGlyph>(gl);
//
//FT_Bitmap bitmap = bitmapg->bitmap;
//
//unsigned char* buffer = bitmap.buffer;
//vec2 gSize = vec2(bitmap.width, bitmap.rows);
//std::cout << gSize << "\n";
//
//atlasCurMax = { atlasCur.x + gSize.x, std::fmax(atlasCurMax.y, gSize.y) };
//atlasCurLine = { atlasCur.x + gSize.x, atlasCur.y + atlasCurMax.y };
//
//if (atlas.null()) {
//	if (gSize.x == 0 || gSize.y == 0) gSize = 1.f;
//	atlas = Texture(vec2(gSize.x, gSize.y + border.y), GL_RED, GL_RED, GL_UNSIGNED_BYTE);
//	atlas.setFiltering(GL_NEAREST, GL_NEAREST);
//	atlasCur.x = border.x + 0.f;
//	atlasCur.y = border.y + 0.f;
//	atlas.setData(vec4(0.f));
//}
//
//while (atlasCurLine.x > atlas.size.x - border.x && atlasCurLine.x < maxAtlasSize.x - border.x) {
//	if (atlas.size.x == 0.f) atlas.size.x = 1.f;
//	atlas.resize(vec2(std::fmin(atlas.size.x * 2, maxAtlasSize.x), atlas.size.y));
//}
//
//if (atlasCurLine.x > maxAtlasSize.x - border.x) {
//	atlas.resize(atlas.size + vec2(0.f, gSize.y + spacing.y));
//	atlasCur.x = border.x + 0.f;
//	atlasCur.y += atlasCurMax.y + spacing.y;
//	atlasCurMax = 0.f;
//}
//
//if (atlasCurLine.y > atlas.size.y - border.y) {
//	atlas.resize(vec2(atlas.size.x, atlasCurLine.y + border.y));
//}
//
//vec2 start = atlasCur;
//
//glyphs[id].advx = g->advance.x >> 6;
//glyphs[id].advy = g->advance.y >> 6;
//
//glyphs[id].bitw = bitmap.width;
//glyphs[id].bitr = bitmap.rows;
//
//glyphs[id].bitl = bitmapg->left;
//glyphs[id].bitt = bitmapg->top;
//
//std::cout << (gl->advance.x >> 12) << " " << (gl->advance.y >> 12) << ", " << bitmapg->left << " " << bitmapg->top << ", (" << g->bitmap_left << " " << g->bitmap_top << ")\n";
//
//glyphs[id].txmin = atlasCur - txBias;
//glyphs[id].txmax = atlasCur + gSize + txBias;
//
////skip ' ' character, generates an error if it's the first element to be loaded into the atlas
//if ((wchar_t)id != L' ') {
//	atlas.fillRegion(atlasCur, gSize, GL_RED, buffer);
//}
//
//atlasCur.x += bitmap.width + spacing.x;
//std::cout << atlasCur << "\n";

//NORMAL GLYPH LOADING
//
//if (glyphs.find(id) != glyphs.end()) return;
//if (FT_Load_Char(face, id, FT_LOAD_RENDER)) return;
//
//vec2 gSize = vec2(g->bitmap.width, g->bitmap.rows);
//
//atlasCurMax = { atlasCur.x + gSize.x, std::fmax(atlasCurMax.y, gSize.y) };
//atlasCurLine = { atlasCur.x + gSize.x, atlasCur.y + atlasCurMax.y };
//
//if (atlas.null()) {
//	if (gSize.x == 0 || gSize.y == 0) gSize = 1.f;
//	atlas = Texture(vec2(gSize.x, gSize.y + border.y), GL_RED, GL_RED, GL_UNSIGNED_BYTE);
//	atlas.setFiltering(GL_NEAREST, GL_NEAREST);
//	atlasCur.x = border.x + 0.f;
//	atlasCur.y = border.y + 0.f;
//	atlas.setData(vec4(0.f));
//}
//
//while (atlasCurLine.x > atlas.size.x - border.x && atlasCurLine.x < maxAtlasSize.x - border.x) {
//	if (atlas.size.x == 0.f) atlas.size.x = 1.f;
//	atlas.resize(vec2(std::fmin(atlas.size.x * 2, maxAtlasSize.x), atlas.size.y));
//}
//
//if (atlasCurLine.x > maxAtlasSize.x - border.x) {
//	atlas.resize(atlas.size + vec2(0.f, gSize.y + spacing.y));
//	atlasCur.x = border.x + 0.f;
//	atlasCur.y += atlasCurMax.y + spacing.y;
//	atlasCurMax = 0.f;
//}
//
//if (atlasCurLine.y > atlas.size.y - border.y) {
//	atlas.resize(vec2(atlas.size.x, atlasCurLine.y + border.y));
//}
//
//vec2 start = atlasCur;
//
////glyph gl = glyphs[id];
//glyphs[id].advx = g->advance.x >> 6;
//glyphs[id].advy = g->advance.y >> 6;
//
//glyphs[id].bitw = g->bitmap.width;
//glyphs[id].bitr = g->bitmap.rows;
//
//glyphs[id].bitl = g->bitmap_left;
//glyphs[id].bitt = g->bitmap_top;
//
////tx as a percentage won't work since the atlas could resize itself at any time
////gl.txmin = (atlasCur) / atlas.size;
////gl.txmax = (atlasCur + gSize) / atlas.size;
////the best i can do is pass tx to the vertexshader (along with the atlas' size) and let it 
////interpolate for the frag shader
//
//glyphs[id].txmin = atlasCur - txBias;
//glyphs[id].txmax = atlasCur + gSize + txBias;
//
////std::cout << "char " << (char)id << ": " << gSize << " " << atlasCur << " " << glyphs[id].txmin << " " << glyphs[id].txmax << "\n";
//
////skip ' ' character, generates an error if it's the first element to be loaded into the atlas
//if ((wchar_t)id != L' ') {
//	atlas.fillRegion(atlasCur, gSize, GL_RED, g->bitmap.buffer);
//}
//
//atlasCur.x += g->bitmap.width + spacing.x;

//DRAW STRING PRIV BEFORE DRAWRES
//
//void TextRenderer::drawWStringPriv(Atlas& at, std::wstring const& str, mat3 transform, vec4 color) {
//	transform = util::getAspectOrtho().inverted() * transform;
//
//	std::vector<float> data;
//	data.reserve(str.length() * 4 * 4);
//
//	vec2 cur = 0.f;
//	for (wchar_t c : str) {
//		if (at.gInfo.find(c) == at.gInfo.end()) { loadGlyph((unsigned long)c); loadOutlineGlyph((unsigned long)c); }
//
//		vec2 pos = -1.f, txmin;
//		vec2 size = 2.f, txmax;
//
//		glyph g = at.gInfo[c];
//		pos = cur + vec2(g.bitl, -(g.bitr - g.bitt)) - txBias;
//		size = vec2(g.bitw, g.bitr) + txBias * 2.f;
//
//		txmin = g.txmin;
//		txmax = g.txmax;
//
//		cur += vec2(g.advx, g.advy);
//
//		vec2 v0 = pos;
//		vec2 v1 = pos + size.oy();
//		vec2 v2 = pos + size;
//		vec2 v3 = pos + size.xo();
//
//		//vec2 tx0 = txmin;
//		//vec2 tx1 = { txmin.x, txmax.y };
//		//vec2 tx2 = txmax;
//		//vec2 tx3 = { txmax.x, txmin.y };
//
//		vec2 tx0 = { txmin.x, txmax.y };
//		vec2 tx1 = txmin;
//		vec2 tx2 = { txmax.x, txmin.y };
//		vec2 tx3 = txmax;
//
//		data.insert(data.end(), {
//			v0.x, v0.y, tx0.x, tx0.y,
//			v1.x, v1.y, tx1.x, tx1.y,
//			v2.x, v2.y, tx2.x, tx2.y,
//
//			v2.x, v2.y, tx2.x, tx2.y,
//			v3.x, v3.y, tx3.x, tx3.y,
//			v0.x, v0.y, tx0.x, tx0.y,
//			});
//
//		//data.insert(data.end(), {
//		//	v0.x, v0.y, tx0.x, tx0.y,
//		//	v1.x, v1.y, tx1.x, tx1.y,
//		//	v2.x, v2.y, tx2.x, tx2.y,
//
//		//	v2.x, v2.y, tx2.x, tx2.y,
//		//	v3.x, v3.y, tx3.x, tx3.y,
//		//	v0.x, v0.y, tx0.x, tx0.y,
//		//});
//	}
//
//	vao.vbos[0].setData(data);
//
//	util::texEnable();
//	util::blendEnable();
//	sh.use();
//	sh.setUniformMat3f("transform", transform);
//	sh.setUniform1i("tex", 0);
//	sh.setUniform2f("atlasSize", at.atlas.size);
//	sh.setUniform3f("colorincustom", color);
//	util::bindTexture(0, at.atlas.texID);
//
//	vao.draw();
//	sh.release();
//	util::blendDisable();
//	util::texDisable();
//}
//
//void TextRenderer::drawWStringReal(std::wstring const& str, mat3 const& transform) {
//	if (outlineEnabled) drawWStringPriv(outlineAtlas, str, transform, vc4::black);
//	drawWStringPriv(atlas, str, transform, vc4::white);
//}

//DRAWSTRINGREAL OLD
//
//void TextRenderer::drawWStringReal(std::string const& font, std::wstring const& str, mat3 const& transform) {
//	DrawRes outdr = { 0, 0.f };
//	if (outlineEnabled) outdr = drawWStringPriv(font, fonts[font].outlineAtlas, str, 0.f, transform, vc4::black);
//	DrawRes dr = drawWStringPriv(font, fonts[font].atlas, str, 0.f, transform, vc4::white);
//
//	if (dr.count < str.length()) {
//		std::wstring tstr = str.substr(dr.count, str.size() - dr.count);
//
//		//the string was not drawn completely because it contained a character that was not 
//		//present in the font, try finding a font that contains that char and continue drawing
//		for (auto& f : fonts) {
//			if (f.first.compare(font) == 0) continue; //skip the current font, we already know it can't render the character
//
//			//std::cout << "attempting drawing with " << f.first << "\n";
//
//			int oldCount = dr.count;
//			if (outlineEnabled) outdr = drawWStringPriv(f.first, f.second.outlineAtlas, tstr, outdr.cur, transform, vc4::black);
//			dr = drawWStringPriv(f.first, f.second.atlas, tstr, dr.cur, transform, vc4::white);
//			if (dr.count > 0 && dr.count < tstr.size()) tstr = tstr.substr(dr.count, tstr.size() - dr.count);
//			if (dr.count > oldCount) break;
//		}
//
//		if (dr.count < tstr.length()) { //if no font can render the character, just render the tofu in place of it
//			if (outlineEnabled) outdr = drawWStringPriv(font, fonts[font].outlineAtlas, tstr, outdr.cur, transform, vc4::black, true);
//			dr = drawWStringPriv(font, fonts[font].atlas, tstr, dr.cur, transform, vc4::white, true);
//		}
//	}
//}
