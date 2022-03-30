#include "Sprite.h"

namespace prb {
	Sprite::Sprite() {}
	Sprite::Sprite(Texture const& tex) : tex(tex) {/* this->tex.setFiltering(GL_NEAREST);*/ }
	Sprite::Sprite(const char* tex) : Sprite(Texture(tex)) {
		//try loading an spt file if present (in the same folder of the texture)
		std::string tTex = stru::fallbackPath(tex);

		std::vector<std::string> spt = stru::splitFilePath(tTex);

		std::cout << (spt[0] + spt[1] + ".spt") << "\n";
		std::ifstream f((spt[0] + spt[1] + ".spt").c_str(), std::ios::in);
		if (!f.good()) { deb::outStr("could not find sprite '",tex,"'\n"); return; }

		using namespace outl;

		size = this->tex.getSize();

		std::string word = loadString(f);
		while (f.good()) {
			if (word.compare("size") == 0) size = vec2(loadInt(f), loadInt(f));
			else if (word.compare("anim") == 0) {
				std::string name = loadString(f);

				Sprite::Animation anim;
				anim.coord = loadVec2(f);
				anim.frames = loadInt(f);
				std::string type = loadString(f);
				if (type.compare("fps") == 0) {
					anim.fps = loadInt(f);
				}
				anim.loop = loadInt(f);

				anims[name] = anim;
				//std::cout << "anim loaded " << name << " " << anim.coord << " " << anim.frames << " " << type << " " <<anim.fps << " " << anim.loop << "\n";
			}
			else if (word.compare("state") == 0) {
				std::string name = loadString(f);

				Sprite::Animation anim;
				anim.coord = loadVec2(f);

				anims[name] = anim;
			}
			else if (word.compare("tiledstate") == 0) {
				std::string name = loadString(f);

				Sprite::Animation anim;
				anim.tiled = true;
				anim.coord = loadVec2(f);
				anim.tiledarea = { loadVec2(f), loadVec2(f) };

				anims[name] = anim;
			}
			else if (word.compare("tiledanim") == 0) {
				std::string name = loadString(f);

				Sprite::Animation anim;
				anim.tiled = true;
				anim.coord = loadVec2(f);
				anim.tiledarea = { loadVec2(f), loadVec2(f) };
				anim.frames = loadInt(f);
				std::string type = loadString(f);
				if (type.compare("fps") == 0) {
					anim.fps = loadInt(f);
				}
				anim.loop = loadInt(f);

				anims[name] = anim;
			}

			word = loadString(f);
		}

		f.close();
	}
	Sprite::Sprite(Texture const& tex, vec2 const& size) : Sprite(tex) { this->size = size; }

	aabb2 Sprite::getTiledAreaScreenSpace(Sprite::AnimationPlayer const& ap, vec2 const& size) {
		aabb2 acbb = { 0.f, 2.f };
		aabb2 tileVs = ap.getTiledArea() / this->size * 2.f; //tile vertices (vertices of the tiled area)
		aabb2 diff = { acbb.fmin() - tileVs.fmin(), acbb.fmax() - tileVs.fmax() };
		acbb = acbb.centered() * size;
		return { acbb.fmin() - diff.fmin(), acbb.fmax() - diff.fmax() };
	}
	aabb2 Sprite::getTiledAreaScreenSpace(std::string const& state, vec2 const& size) { return getTiledAreaScreenSpace(Sprite::AnimationPlayer{ *this, state }, size); }

	vec2 Sprite::getOffset(Sprite::AnimationPlayer const& sa) const {
		//if (!sa.getAnim()) return -1.f;
		//std::cout << sa.getAnim().coord << " " << vec2(size.x, size.y) << " " << sa.curFrame << "\n";
		//return sa.getAnim().coord*size + size.xo() * sa.curFrame;
		return vec2(
			sa.getAnim().coord.x * size.x + size.x * sa.curFrame,
			tex.getSize().y - size.y - sa.getAnim().coord.y * size.y
		);
	}

	std::array<aabb2, 9> Sprite::getTiledOffsets(AnimationPlayer const& sa) const {
		vec2 off = getOffset(sa);
		std::array<aabb2, 9> res;

		aabb2 ta = sa.getTiledArea();

		res[0] = { 0.f, ta[0] };
		res[1] = { ta[0], { ta[3].x, 0.f } };
		res[2] = { ta[3], { size.x, 0.f } };

		res[3] = { ta[0], { 0.f, ta[1].y } };
		res[4] = { ta[0], ta[2] };
		res[5] = { ta[3], { size.x, ta[2].y } };

		res[6] = { { 0.f, size.y }, ta[1] };
		res[7] = { ta[1], { ta[2].x, size.y } };
		res[8] = { ta[2], size };

		for (int i = 0; i < 9; i++) res[i] += off;

		return res;
	}

	void Sprite::draw(std::vector<float> const& batch) const {
		vbuf vb(batch, { 2,4,2 });
		util::defTexShader.use();
		tex.bind();
			vb.draw();
		vb.dispose();
	}

	void Sprite::draw(mat3 const& transform) const {
		draw(util::batchTexture(transform));
	}

	void Sprite::draw(AnimationPlayer const& ap, mat3 const& transform) const {
		vec2 off = getOffset(ap) / tex.getSize();
		draw(util::batchTextureSub(transform, { off, off + size / tex.getSize() }));
	}

	void Sprite::draw(std::string const& statename, mat3 const& transform) const {
		draw(Sprite::AnimationPlayer{ *this, statename }, transform);
	}

	void Sprite::draw(aabb2 const& bb, mat3 const& transform) const { draw(transform * pmat3::translate(bb.center()) * pmat3::scale(bb.size() / 2.f)); }
	void Sprite::draw(AnimationPlayer const& ap, aabb2 const& bb, mat3 const& transform) const { draw(ap, transform * pmat3::translate(bb.center()) * pmat3::scale(bb.size() / 2.f)); }
	void Sprite::draw(std::string const& statename, aabb2 const& bb, mat3 const& transform) const { draw(Sprite::AnimationPlayer{ *this, statename }, bb, transform); }

	void Sprite::drawTiled(AnimationPlayer const& ap, vec2 const& size, mat3 const& transform) const {
		if (size == 1.f || ap.getTiledArea() == 0.f || ap.getTiledArea() == aabb2{ 0.f, this->size - 1.f }) { draw(ap, transform * pmat3::scale(size)); return; }

		std::array<aabb2, 9> offs = getTiledOffsets(ap);	//texture offset coordinatess
		std::array<aabb2, 9> coords;						//actual coordinates

		aabb2 acbb = { 0.f, 2.f };
		vec2 sctex = 1.f / tex.getSize();

		aabb2 tileVs = ap.getAnim().tiledarea / this->size * 2.f; //tile vertices (vertices of the tiled area)

		aabb2 diff = { acbb.fmin() - tileVs.fmin(), acbb.fmax() - tileVs.fmax() };

		acbb = acbb.centered() * size;
		tileVs = { acbb.fmin() - diff.fmin(), acbb.fmax() - diff.fmax() };


		coords[0] = { acbb[0], tileVs[0] };
		coords[1] = { tileVs[0], { tileVs[3].x, acbb.miny() } };
		coords[2] = { acbb[3], tileVs[3] };

		coords[3] = { { acbb.minx(), tileVs[0].y }, tileVs[1] };
		coords[4] = { tileVs[0], tileVs[2] };
		coords[5] = { tileVs[3], { acbb.maxx(), tileVs[2].y } };

		coords[6] = { acbb[1], tileVs[1] };
		coords[7] = { tileVs[1], { tileVs[2].x, acbb.maxy() } };
		coords[8] = { acbb[2], tileVs[2] };

		//for (int i = 0; i < 9; i++) debug::rtlog << (offs[i] / tex.size).toString() << "\n";

		//for (int i = 0; i < 9; i++) util::drawSubTex(tex.texID, coords[i], offs[i]/tex.size, transform);
		//for (int i = 0; i < 9; i++) { util::drawQuad(transform * coords[i], GL_LINE_LOOP); }

		draw(util::batchTextureSub(transform, coords[4], offs[4] / tex.getSize()));

		if (diff.fmin() != 0.f || diff.fmax() != 0.f) {
			for (int i = 0; i < 9; i++) {
				if (i != 4) draw(util::batchTextureSub(transform, coords[i], offs[i] / tex.getSize()));
			}
		}

		//util::setColor(vc4::red);
		//for (int i = 0; i < 9; i++) { util::drawQuad(transform * coords[i], GL_LINE_LOOP); }

		//for (int i = 0; i < 4; i++) debug::drawCircle(tileVs[i], .05f, vc4::red);
		//for(int i=0; i<9; i++) debug::drawQuad(coords[i], vc4::red);
	}
	void Sprite::drawTiled(std::string const& state, vec2 const& size, mat3 const& transform) const { drawTiled(Sprite::AnimationPlayer{ *this, state }, size, transform); }

	void Sprite::drawTiled(AnimationPlayer const& ap, aabb2 const& bb, mat3 const& transform) const { drawTiled(ap, bb.size() / 2.f, transform * pmat3::translate(bb.center())); }
	void Sprite::drawTiled(std::string const& state, aabb2 const& bb, mat3 const& transform) const { drawTiled(Sprite::AnimationPlayer{ *this, state }, bb, transform); }
}
