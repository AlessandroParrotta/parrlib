#include <parrlib/ui/imui.h>

#include <parrlib/sprite.h>

namespace imui {
	const std::string glbid = "imui";
	statec state;
	statec oldstate = state;
	//prefss prefs;
	unsigned int gid = 0;
	unsigned int activegid = 0;

	template<typename T>
	struct stack {
		std::vector<T> st;
		void push(T const& t) { st.push_back(t); }
		void pop() { st.pop_back(); }
		T popr() { T cp = st.back(); st.pop_back(); return cp; }
		T& back() { return st.back(); }
		T& top() { return back(); }
		T back() const { return st.back(); }
		T top() const { return back(); }
		int size() const { return st.size(); }
	};
	stack<statec> ststack;

	//std::string miqid = ""; //mouse in quad identifier

	mat3 space = 1.f;
	void setSpace(mat3 const& sp) { space = sp; }
	mat3 getSpace() { return space; }

	//the line of thought regarding containers is that the ui elements inside the same container
	//should never overlap (unless those are drop down menus), however containers can overlap
	//so i only check for containers
	//std::vector<aabb2> containers;	//right-click quads for mouseInQuad
	//int currentContainer = 0;

	//void pushContainer(aabb2 const& bb) {
	//	containers.push_back(bb);
	//	currentContainer++;
	//}

	std::unordered_map<std::string, aabb2> containers;
	std::unordered_map<std::string, int> posContainers;
	std::unordered_map<std::string, bool> pingContainers;
	stack<std::string> curContainer;
	int curposcontainer = 0;

	void beginContainer(std::string const& id, aabb2 const& bb) {
		pingContainers[id] = true;
		containers[id] = bb;
		curContainer.push(id);
		curposcontainer++;
		posContainers[id] = curposcontainer;
	}

	void endContainer() {
		curContainer.pop();
	}


	//dropmenu vals
	bool dropmenu = false;
	std::string dropid = "";
	aabb2 dropbb = 0.f;
	aabb2 dropbodybb = 0.f;
	bool dropjustopened = false; //to delay check 1 frame
	std::vector<std::wstring> dropopts;			//how many options does the current opened dropmenu have
	int dropsel = -1;
	mat3 dropspace = 1.f;
	mat3 utildropmat = 1.f;
	statec dropstate;
	//**(fast)dropdown container bodies are always on top of every other ui element

	bool dropmenubeganopened = false;

	mat3 tospace() { return (util::getTopMatrix() * space).inverted() * util::getAspectOrtho(); }
	mat3 fromspace() { return util::getAspectOrtho().inverted() * util::getTopMatrix() * space; }

	//projects the mouse from orthoNDC space to imui space
	vec2 mouseproj() {
		return (util::getTopMatrix() * space).inverted() * util::getAspectOrtho() * input::getMousePos();
	}

	bool mouseInQuad(aabb2 const& bb) {
		return input::mouseInQuad(util::getAspectOrtho().inverted() * util::getTopMatrix() * space * bb);
	}

	//which container the mouse is in
	std::string mouseInContainer() {
		int maxpos = 0;
		std::string contname = "";
		for (auto& c : containers) { 
			if (posContainers[c.first] > maxpos && mouseInQuad(c.second)) { 
				maxpos = posContainers[c.first]; contname = c.first; 
			} 
		}
		return contname;
	}

	bool manuallayout = false; //when the user sets the pos and size manually even when a layout != NOLAYOUT is active
	void setLayout(int la) { state.layout = la; }
	int getLayout() { return state.layout; }

	void saveState() {
		ststack.push(state);
	}

	void restoreState() {
		state = ststack.popr();
	}

	void setmanuallayout() {
		saveState();
		state.manualLayoutOverride = true;
		state.layout = NOLAYOUT;
	}

	//sets layout to manual and pushes state to stack
	void setmanuallayout(vec2 const& pos, vec2 const& size) {
		setmanuallayout();
		state.pos = pos; state.size = size;
	}

	void setPos(vec2 const& pos) { state.pos = pos; state.nextPos = pos; }
	void setSize(vec2 const& size) { state.size = size; state.nextSize = size; }
	void move(vec2 const& delta) { state.pos += delta; state.nextPos = state.pos; }

	void setOffset(vec2 const& offset) { state.offset = offset; }

	void updatestatelayout() {
		if (state.layout == NOLAYOUT) return;

		state.nextSize = state.size;

		state.pos = state.nextPos;

		switch (state.layout) {
		case HORIZONTAL:
			state.nextPos = state.pos + state.offset.xo();
			break;
		case VERTICAL:
			state.nextPos = state.pos + state.offset.oy();
			break;
		case INCREMENTAL:
			state.nextPos = state.pos + state.offset;
			break;
		case CONTAINER:

			break;
		}


		if (state.indropmenu) {
			//rescale the current drop menu bb to then save it
			if (state.currentDropMenuBB == 0.f) {
				state.currentDropMenuBB = aabb2{ -state.size, state.size }/2.f + state.pos;
			}
			else {
				state.currentDropMenuBB.rescale(state.pos - state.size/2.f);
				state.currentDropMenuBB.rescale(state.pos + state.size/2.f);
			}
		}

		//savelayout();
	}

	void layoutSkip() {
		updatestatelayout();
	}

	void useAtlas(std::string const& name) { state.atlas = name; }

	void setTxr(std::vector<std::string> const& fonts, int const& fontSize) { glb::setTxr("imui", fonts, fontSize); }
	void setTxr(std::string const& font, int const& fontSize) { setTxr(std::vector<std::string>{ font }, fontSize); }
	TextRenderer& getTxr() { return glb::getTxr(glbid); }

	void setTextColor(vec4 color) { getTxr().color(color); }
	void setTextOutlineColor(vec4 color) { getTxr().outlineColor(color); }
	vec4 getTextColor() { return getTxr().color(); }
	vec4 getTextOutlineColor() { return getTxr().outlineColor(); }

	Sprite& getSprite() { return glb::getSprite(state.atlas); }

	void drawsprite(mat3 const& transform) {
		if (imui::state.atlas.compare("") == 0) return;
		getSprite().draw(space * transform);
	}

	void drawsprite(std::string const& animation, mat3 const& transform) {
		if (imui::state.atlas.compare("") == 0) return;
		Sprite& at = getSprite();
		Sprite::AnimationPlayer ap = { at, animation };
		at.draw(ap, space * transform);
	}

	void drawsprite(std::string const& state, aabb2 const& bb, mat3 const& transform) {
		if (imui::state.atlas.compare("") == 0) return;
		getSprite().draw(state, bb, space * transform);
	}

	void drawspritetiledstate(std::string const& state, vec2 const& size, mat3 const& transform) {
		if (imui::state.atlas.compare("") == 0) return;
		getSprite().drawTiled(state, size, space * transform);
	}

	void drawspritetiledstate(std::string const& state, aabb2 const& bb, mat3 const& transform) {
		if (imui::state.atlas.compare("") == 0) return;
		getSprite().drawTiled(state, bb, space * transform);
	}

	aabb2 gettiledareascreenspace(std::string const& atlas, vec2 const& size) {
		if (state.atlas.compare("") == 0) return { -size, size };
		return getSprite().getTiledAreaScreenSpace(atlas, size);
	}

	void drawstr(std::wstring const& str, std::wstring const& modstr, vec2 const& off, aabb2 const& bb, mat3 const& tr) {
		glb::getTxr(glbid).drawWStringpx(str, modstr, off, space * bb, tr);
	}
	void drawstr(std::wstring const& str, vec2 const& off, aabb2 const& bb, mat3 const& tr) {
		glb::getTxr(glbid).drawWStringpx(str, off, space * bb, tr);
	}

	void drawstr(std::wstring const& str, vec2 const& off, mat3 const& tr) {
		glb::getTxr(glbid).drawWStringpx(str, off, tr);
	}

	TextRenderer::bwstring strbbw(std::wstring const& str, vec2 const& off, aabb2 const& bb, mat3 const& tr){
		return glb::getTxr(glbid).getWStringBoundedpx(str, space * bb, off, L"", tr);
	}

	TextRenderer::bwstring strbbw(std::wstring const& str, aabb2 const& bb, vec2 const& off, std::wstring const& modstr, mat3 const& tr) {
		return glb::getTxr(glbid).getWStringBoundedpx(str, space * bb, off, modstr, tr);
	}

	aabb2 strbb(std::wstring const& str, vec2 const& off, aabb2 const& bb, mat3 const& tr) {
		return strbbw(str, off, bb, tr).bound;
	}

	bool button(std::wstring const& text) {
		updatestatelayout();
		aabb2 bb = aabb2{ -state.size, state.size }/2.f + state.pos;
		bool miq = mouseInContainer() == curContainer.top() && mouseInQuad(bb);
		
		//Sprite& at = glb::getSprite(atlas);
		//Sprite::AnimationPlayer ap = { at, miq ? (input::getMouse(0) ? "button-click" : "button-hover") : "button-passive" };
		//aabb2 ta = (space * at.getTiledAreaScreenSpace(ap, state.size)) + bb.center();
		//at.drawTiled(ap, state.size, space * pmat3::translate(state.pos));

		std::string state = miq ? input::getMouse(0) ? "button-click" : "button-hover" : "button-passive";
		aabb2 ta = gettiledareascreenspace(state, imui::state.size);
		drawspritetiledstate(state, bb, 1.f);

		//debug::drawQuad(util::getTopMatrixAspectInv() * space * bb);

		//glb::getTxr("imui").drawWStringpx(text, 0.f, bb, pmat3::translate(space*pos));
		drawstr(text, 0.f, bb, pmat3::translate(space * (ta.center() + imui::state.pos)));

		return miq && input::getMouseUp(0);
	}
	bool button(std::wstring const& text, vec2 const& pos, vec2 const& size) { setmanuallayout(pos, size); bool res = button(text); restoreState(); return res; }


	struct gmenu { bool invoked = false; bool open = false; };
	std::unordered_map<std::string, gmenu> gmenus;

	bool groupMenu(Sprite const& s, mat3 const& tr) { return false; }
	bool groupMenu(std::string const& name, mat3 const& tr){ return false; }

	//
	//----------------------------------TEXTFIELD-----------------------------------------------
	//
	struct textfieldo {
		std::wstring* text;		//full text string
		std::wstring vistext = L"";	//visible text on screen (based on the textfield's bounding box)
		int off = 0;			//offset from beginning of text
		int sel = 0;			//current selected character
		int selmax = 0;			//second selection for multiple character selection (always greater than sel)
		int shadowSel = 0;		//shadow selection when hovering mouse over characters
		int selstartpoint = -1;	//control variable for when you hold the mouse to select multiple characters
		int selvis = 0;			//up to which character is the string visible on screen
		bool active = false;
		float txtimer = 0.f;	//timer for text-like polling when holding control keys
		bool txfirst = false;
		bool modified = false;	//whether this textfield's text has been modified (inserted or removed characters)
		
		float clickTimer = 0.f;
		bool enableClickTimer = false;
		int clickTimes = 0;
	};
	std::unordered_map<std::string, textfieldo> txfs;

	void apptext(textfieldo const& tf, std::wstring const& str, int off) {
		tf.text->insert(off, str);
	}

	void remtext(textfieldo const& tf, int o1, int o2) {
		if (o1 > o2 || o1 < 0 || o1 > tf.text->length() || o2 < 0 || o2 > tf.text->length()) return;
		*tf.text = tf.text->erase(o1, o2 - o1);
	}

	//replace text
	void txreptext(textfieldo& tf, int o1, int o2, std::wstring const& str) {
		remtext(tf, o1, o2);
		apptext(tf, str, o1);
	}

	std::wstring substr(std::wstring const& text, int o1, int o2) {
		return text.substr(outl::clamp(o1, 0, text.length()), outl::clamp(o2 - o1, 0, text.length()));
	}

	std::wstring substr(textfieldo const& tf, int o1, int o2) {
		return tf.text->substr(o1, o2 - o1);
	}

	bool txcontrol(textfieldo& tx, float const& speed) {
		return tx.txtimer == 0.f || tx.txtimer > speed; 
	}

	void movetfoff(textfieldo& tf, int const& delta) {
		tf.sel += delta;
		tf.sel = outl::clamp(tf.sel, 0, tf.text->length());
	}
	
	void movetfoff(textfieldo& tf, int& val, int const& delta) {
		val += delta;
		val = outl::clamp(val, 0, tf.text->length());
	}

	void tfsetsel(textfieldo& tf, int sel) { tf.sel = sel;  }
	void tfsetoff(textfieldo& tf, int selmax) { tf.selmax = selmax; }

	void tfdrawcursor(textfieldo& tf, aabb2 const& subb, vec4 const& col) {
		util::setColor(col);
		vec2 tsz = vec2(subb.sizey() / 16.f, subb.size().y);
		vec2 tof = subb[3] - (tsz / 2.f).xo();
		util::drawQuad(tof, tsz);
	}

	void tfdrawcursor(textfieldo& tf, int sel, vec4 const& col, vec2 const& off, aabb2 const& ta, mat3 const& tr) {
		aabb2 stbb = strbb(tf.vistext, off, ta, tr);
		aabb2 subb = strbb(substr(tf, tf.off, sel), off, ta, tr);
		subb.minyr(stbb.miny());
		subb.maxyr(stbb.maxy());

		tfdrawcursor(tf, subb, col);
	}

	void tfdrawcursor(textfieldo& tf, int sel, vec4 const& col, aabb2 const& stbb, vec2 const& off, aabb2 const& ta, mat3 const& tr) {
		aabb2 subb = strbb(substr(tf, tf.off, sel), off, ta, tr);
		subb.minyr(stbb.miny());
		subb.maxyr(stbb.maxy());

		tfdrawcursor(tf, subb, col);
	}

	int tfgetsel(textfieldo& tf, vec2 const& v, aabb2 const& stbb, vec2 const& off, aabb2 const& ta, mat3 const& tr) {
		aabb2 stbbspace = space.inverted() * stbb;										//stbb converted in imui::space
		aabb2 mbb = aabb2(ta.fmin(), vec2(v.x, ta.maxy()));
		TextRenderer::bwstring shadowbb = strbbw(tf.vistext, mbb, off, L"", tr);
		//debug::drawQuad(util::getAspectOrtho().inverted() * util::getTopMatrix() * stbb);
		//debug::drawQuad(util::getAspectOrtho().inverted() * util::getTopMatrix() * space * stbbspace, vc4::blue);
		//debug::drawQuad(util::getAspectOrtho().inverted() * util::getTopMatrix() * space * mbb, vc4::green);
		//debug::drawQuad(util::getAspectOrtho().inverted() * util::getTopMatrix() * space * shadowbb.bound, vc4::white);
		//debug::rtlog << "stbb" << stbb << "\n";
		//debug::rtlog << "stbbspace" << stbbspace << "\n";
		//debug::rtlog << "mbb" << mbb << "\n";
		//debug::rtlog << "shadowbb" << shadowbb.bound << "\n";
		return shadowbb.str.length();
	}

	int tfgetsel(textfieldo& tf, vec2 const& v, vec2 const& off, aabb2 const& ta, mat3 const& tr) {
		aabb2 stbb = strbb(tf.vistext, off, ta, tr);
		return tfgetsel(tf, v, stbb, off, ta, tr);
	}

	void tfdrawcursor(textfieldo& tf, vec2 const& v, vec4 const& col, vec2 const& off, aabb2 const& ta, mat3 const& tr) {
		aabb2 stbb = strbb(tf.vistext, off, ta, tr);
		int shadowSel = tfgetsel(tf, v, stbb, off, ta, tr);

		tfdrawcursor(tf, shadowSel, col, stbb, off, ta, tr);
	}

	void tfadjustsels(textfieldo& tf) {
		if (tf.sel > tf.selmax) { int t = tf.sel; tf.sel = tf.selmax; tf.selmax = t; }
		tf.sel = outl::clamp(tf.sel, 0, tf.text->length());
		tf.selmax = outl::clamp(tf.selmax, 0, tf.text->length());
		tf.selvis = outl::clamp(tf.selvis, 0, tf.text->length());
		tf.off = outl::clamp(tf.off, 0, tf.text->length());
	}
	
	void tfmoveoff(textfieldo& tf, int delta, vec2 const& off, aabb2 const& ta, mat3 const& tr) {
		tf.off += delta;
		tfadjustsels(tf);
		tf.selvis = tf.off + strbbw(tf.text->substr(tf.off, tf.text->length() - tf.off), off, ta, tr).str.length();
		tfadjustsels(tf);
	}

	int textField(std::string const& identifier, std::wstring const& hintText, std::wstring* text) {
		if (!text) return 0;
		if (txfs.find(identifier) == txfs.end()) { txfs[identifier]; txfs[identifier].sel = text->length(); }
		updatestatelayout();

		aabb2 bb = aabb2{ -state.size, state.size } / 2.f + state.pos;
		bool miq = mouseInContainer() == curContainer.top() && mouseInQuad(bb);

		textfieldo& tf = txfs[identifier];
		tf.text = text;
		std::string tstate = tf.active ? "textfield-click" : (miq ? "textfield-hover" : "textfield-passive");
		//Sprite& at = glb::getSprite(atlas);
		//Sprite::AnimationPlayer	ap = { at, tf.active ? "textfield-click" : (miq ? "textfield-hover" : "textfield-passive") };
		//at.drawTiled(ap, state.size/2.f, space * pmat3::translate(state.pos));
		
		//aabb2 ta = at.getTiledAreaScreenSpace(ap, state.size/2.f) + bb.center();

		drawspritetiledstate(tstate, state.size/2.f, pmat3::translate(state.pos));
		aabb2 ta = gettiledareascreenspace(tstate, state.size / 2.f) + bb.center();
		ta = ta.scaled(.9f);

		vec2 off = vec2(1.f, 0.f);

		tf.off = outl::clamp(tf.off, 0, tf.text->length());
		tf.vistext = tf.off == 0 ? *tf.text : tf.text->substr(tf.off, tf.text->length() - tf.off);

		mat3 tr = pmat3::translate(space*(ta.center() - ta.size().xo()/2.f));
		if (tf.text->length() == 0 && hintText.length() > 0) { vec4 cl = getTxr().color(); getTxr().color(cl.a(.6f)); drawstr(hintText, state.modstr, off, ta, tr); getTxr().color(cl); }
		else												drawstr(tf.vistext, state.modstr, off, ta, tr);

		int res = 0;

		if (!tf.active && miq && input::getMouseDown(0))			{ tf.active = true; res = 2; tf.sel = tf.text->length(); }
		else if (tf.active && !miq && input::getMouseDown(0))		{ tf.active = false; res = 3; } //when you click await it simulates an 'enter' keypress

		tf.shadowSel = tf.text->length();
		if ((miq || tf.selstartpoint > -1) && !tf.enableClickTimer) {
			vec2 mproj = mouseproj();	//mouse projected in imui space

			aabb2 stbb = strbb(tf.vistext, off, ta, tr);
			tf.shadowSel = tf.off + tfgetsel(tf, mproj.maxed(ta.fmin()), stbb, off, ta, tr);
			tfdrawcursor(tf, tf.shadowSel, vc4::black.a(.5f), stbb, off, ta, tr);

			if (input::getMouseDown(0)) tf.sel = tf.selstartpoint = tf.selmax = tf.shadowSel;
			if (input::getMouse(0)) { 
				if (tf.shadowSel > tf.selstartpoint) { tf.sel = tf.selstartpoint; tf.selmax = tf.shadowSel; }
				else { tf.sel = tf.shadowSel; tf.selmax = tf.selstartpoint; }
			}
			if (input::getMouseUp(0)) tf.selstartpoint = -1;

			if (mproj.x > ta.fmax().x && tf.selvis < tf.text->length()) tfmoveoff(tf, 1, off, ta, tr);
			if (mproj.x < ta.fmin().x) tfmoveoff(tf, -1, off, ta, tr);
		}

		if (tf.active) {

			//click timer management
			if (tf.enableClickTimer && input::getMouseDelta() != 0.f) tf.enableClickTimer = false;
			if (tf.enableClickTimer) {
				tf.clickTimer += gtime::deltaTime;

				if (input::clickDown()) {
					tf.clickTimes++;

					tfadjustsels(tf);

					switch (tf.clickTimes) {
					case 1:
						while (tf.selmax < tf.text->length() && (*tf.text)[tf.selmax] != L' ') tf.selmax++;
						while (tf.sel > 0 && (*tf.text)[std::max(tf.sel,0)-1] != L' ') tf.sel--;
						break;
					case 2:
						tf.selmax = tf.text->length();
						tf.sel = 0;
						break;
					}
				}

				if (tf.clickTimer > .5f) tf.enableClickTimer = false;
			}
			else if (input::clickDown() && input::getMouseDelta() == 0.f) {
				tf.enableClickTimer = true;
				tf.clickTimer = 0.f;
				tf.clickTimes = 0;
			}

			std::wstring oldtext = *tf.text;

			//debug::rtlog << tf.sel << " -> " << tf.selmax << " " << tf.shadowSel << "\n";

			TextRenderer::bwstring bwstr = strbbw(tf.vistext, off, ta, tr);
			aabb2 stbb = bwstr.bound;
			tfdrawcursor(tf, tf.sel, vc4::black, stbb, off, ta, tr);

			if (tf.sel != tf.selmax) {
				//draw a cyan quad to highlight the selected area
				aabb2 minbb = strbb(substr(tf.vistext, 0, tf.sel - tf.off), off, ta, tr);
				aabb2 maxbb = strbb(substr(tf.vistext, 0, tf.selmax - tf.off), off, ta, tr);
				aabb2 selbb = stbb.minx(minbb.maxx()).maxx(maxbb.maxx());
				util::setColor(vc4::cyan.a(.5f));
				util::drawQuad(selbb);

				tfdrawcursor(tf, tf.selmax, vc4::black, stbb, off, ta, tr);
			}

			tf.selvis = tf.off + bwstr.str.length(); //max character visible on screen
			//debug::rtlog << "selvis " << tf.selvis << " (" << bwstr.str << ")\n";

			int oldsel = tf.sel;

			unsigned int txt = input::pollTextKey();
			//if (txt > 0) { apptext(tf, std::wstring({ (wchar_t)txt }), tf.sel); tf.sel++; }
			if (txt > 0) { 
				txreptext(tf, tf.sel, tf.selmax, std::wstring({ (wchar_t)txt })); tf.sel++; tf.selmax = tf.sel; 

				tfadjustsels(tf);
			}

			float speed = .1f * (tf.txfirst ? .5f : 4.f);	//text control speed

			if (input::getKey(GLFW_KEY_BACKSPACE) && txcontrol(tf, speed)) { 
				if (input::getKey(GLFW_KEY_LEFT_CONTROL)) {
					tf.selmax = tf.sel;
					tf.selmax = outl::clamp(tf.selmax, 0, tf.text->length() - 1);
					while (tf.selmax > 0 && (*tf.text)[tf.selmax] == L' ') movetfoff(tf, tf.selmax, -1);	//skip spaces
					while (tf.selmax > 0 && (*tf.text)[tf.selmax] != L' ') movetfoff(tf, tf.selmax, -1);	//skip word
					tfadjustsels(tf);
					remtext(tf, tf.sel, tf.selmax);
				}
				else { 
					tfadjustsels(tf); 
					if (tf.sel != tf.selmax) { txreptext(tf, tf.sel, tf.selmax, L""); tf.selmax = tf.sel; }
					else						{ remtext(tf, tf.sel - 1, tf.sel); tf.sel--; }
					//remtext(tf, tf.sel - 1, tf.sel); 
					//tf.sel--; 
				}
			}
			if (input::getKey(GLFW_KEY_DELETE) && txcontrol(tf, speed)) { 
				if (input::getKey(GLFW_KEY_LEFT_CONTROL)) {
					tf.selmax = tf.sel;
					tf.selmax = outl::clamp(tf.selmax, 0, tf.text->length() - 1);
					while (tf.selmax < tf.text->length() && (*tf.text)[tf.selmax] == L' ') movetfoff(tf, tf.selmax, 1);	//skip spaces
					while (tf.selmax < tf.text->length() && (*tf.text)[tf.selmax] != L' ') movetfoff(tf, tf.selmax, 1);
					tfadjustsels(tf);
					remtext(tf, tf.sel, tf.selmax);
				}
				else {
					tfadjustsels(tf);
					if (tf.sel != tf.selmax) { txreptext(tf, tf.sel, tf.selmax, L""); tf.selmax = tf.sel; }
					else { remtext(tf, tf.sel, tf.sel+1); }
				}
			}
			if (input::getKey(GLFW_KEY_LEFT) && txcontrol(tf, speed)) { 
				if (input::getKey(GLFW_KEY_LEFT_CONTROL)) {
					tf.sel = outl::clamp(tf.sel, 0, tf.text->length() - 1);
					while (tf.sel > 0 && (*tf.text)[tf.sel] == L' ') movetfoff(tf, -1);	//skip spaces
					while (tf.sel > 0 && (*tf.text)[tf.sel] != L' ') movetfoff(tf, -1);	//skip word
				}
				else tf.sel--;
			}
			if (input::getKey(GLFW_KEY_RIGHT) && txcontrol(tf, speed)) { 
				if (input::getKey(GLFW_KEY_LEFT_CONTROL)) {
					tf.sel = outl::clamp(tf.sel, 0, tf.text->length() - 1);
					while (tf.sel < tf.text->length() && (*tf.text)[tf.sel] == L' ') movetfoff(tf, 1);	//skip spaces
					while (tf.sel < tf.text->length() && (*tf.text)[tf.sel] != L' ') movetfoff(tf, 1);	//skip word
				}
				else tf.sel++;
			}

			if (input::getKey(GLFW_KEY_LEFT_CONTROL) && txcontrol(tf, speed)) {
				if (input::getKey(GLFW_KEY_V)) {
					std::wstring clip = input::getFromClipboardw();
					//apptext(tf, clip, tf.sel);
					tfadjustsels(tf);
					txreptext(tf, tf.sel, tf.selmax, clip);
					tf.sel = tf.selmax = tf.sel + clip.length();
				}
			}

			if (input::getKey(GLFW_KEY_LEFT_CONTROL)) {
				if (input::getKeyDown(GLFW_KEY_C)) {
					if (tf.sel != tf.selmax) {
						tfadjustsels(tf); //in case tf.selmax < tf.sel
						std::wstring cp = tf.text->substr(tf.sel, tf.selmax - tf.sel);
						input::copyToClipboard(cp);
					}
				}
			}

			if (input::getKey(GLFW_KEY_BACKSPACE)	||
				input::getKey(GLFW_KEY_LEFT)		||
				input::getKey(GLFW_KEY_RIGHT)		||
				(input::getKey(GLFW_KEY_LEFT_CONTROL) && input::getKey(GLFW_KEY_V)) ||
				input::getKey(GLFW_KEY_DELETE)
				) tf.txtimer += gtime::deltaTime;
			else { tf.txtimer = 0.f; tf.txfirst = false; }

			if (tf.txtimer > speed) { tf.txtimer = 0.f; tf.txfirst = true; }

			if (input::getKeyDown(GLFW_KEY_X)) tfmoveoff(tf, 1, off, ta, tr);
			if (input::getKeyDown(GLFW_KEY_Z)) tfmoveoff(tf, -1, off, ta, tr);

			tf.sel = outl::clamp(tf.sel, 0, tf.text->length());

			if (oldsel != tf.sel && tf.selmax == oldsel) tf.selmax = tf.sel; //anchor selmax to sel if you're not already selecting more than 1 char

			bool textmodified = oldtext.compare(*tf.text) != 0;

			//debug::rtlog << tf.off << "\n";

			//these loops are for trying to keep tf.sel inside the textfield on screen
			int oldselvis = tf.selvis;
			if (textmodified) {
				tfmoveoff(tf, 0, off, ta, tr);
			}

			if (tf.sel == tf.selmax) {
				//try not making tf.sel go too much to the right (offscreen)
				while (tf.sel > tf.selvis && tf.selvis < tf.text->length()) tfmoveoff(tf, 1, off, ta, tr);

				//try not making tf.sel go too much to the left (offscreen)
				while (tf.off > 0 && tf.sel < tf.off) tfmoveoff(tf, -1, off, ta, tr);
			}

			if (txt > 0 || textmodified) return 4;
			if (input::getKeyDown(GLFW_KEY_ENTER)) return 3;

			return 1;
		}

		return res;
	}
	int textField(std::string const& identifier, std::wstring const& hintText, std::wstring const& modstr, vec2 const& pos, vec2 const& size, std::wstring* text) { setmanuallayout(pos, size); state.modstr = modstr; int res = textField(identifier, hintText, text); restoreState(); return res; }

	template<typename T>
	struct textfieldsnumst {
		T old = static_cast<T>(0);
		std::wstring text;
	};

	template<typename T>
	std::unordered_map<std::string, textfieldsnumst<T>> txfsnt;

	template<typename T>
	int textFieldt(std::string const& identifier, T* val) {	//textfield for numbers/objects
		if (txfsnt<T>.find(identifier) == txfsnt<T>.end()) { textfieldsnumst<T> tf; tf.text = stru::towstr<T>(*val); txfsnt<T>[identifier] = tf; }

		textfieldsnumst<T>& tf = txfsnt<T>[identifier];
		state.modstr = L"";
		int res = textField(identifier, std::is_same<T, int>::value ? L"integer" : L"decimal", &tf.text);
		if (res == 3 || res == 4) {
			T t = stru::tot<T>(tf.text);
			if (tf.text.compare(L"") == 0) t = static_cast<T>(0);
			*val = t;
		}
		if (res == 3) {
			T t = stru::tot<T>(tf.text);
			if (tf.text.compare(L"") == 0) t = static_cast<T>(0);
			tf.text = stru::towstr<T>(t);
		}

		if (res == 0 && *val != tf.old) { tf.text = stru::towstr<T>(*val); }
		
		tf.old = *val;

		return res;
	}
	template<typename T>
	int textFieldt(std::string const& identifier, vec2 const& pos, vec2 const& size, T* val)		{ setmanuallayout(pos, size); int res = textFieldt<T>(identifier, val); restoreState(); return res; }

	int textFieldi(std::string const& identifier, int* val)											{ return textFieldt<int>(identifier, val); }
	int textFieldi(std::string const& identifier, vec2 const& pos, vec2 const& size, int* val)		{ return textFieldt<int>(identifier, pos, size, val); }

	int textFieldd(std::string const& identifier, double* val)										{ return textFieldt<double>(identifier, val); }
	int textFieldd(std::string const& identifier, vec2 const& pos, vec2 const& size, double* val)	{ return textFieldt<double>(identifier, pos, size, val); }

	int textFieldf(std::string const& identifier, float* val)										{ return textFieldt<float>(identifier, val); }
	int textFieldf(std::string const& identifier, vec2 const& pos, vec2 const& size, float* val)	{ return textFieldt<float>(identifier, pos, size, val); }

	void label(std::wstring const& text) { 
		updatestatelayout(); 
		if(state.bound != 0.f)	drawstr(text, state.stroffset, state.bound, pmat3::translate(space * state.pos)); 
		else					drawstr(text, state.stroffset, pmat3::translate(space * state.pos)); 
	}
	void label(std::wstring const& text, vec2 const& pos) { state.pos = pos; label(text); }

	void labelBound(std::wstring const& text, aabb2 const& bound, vec2 const& offset) {
		updatestatelayout();
		aabb2 bb = aabb2{ state.size*bound.fmin(), state.size*bound.fmax() } / 2.f + state.pos;
		drawstr(text, offset, bb, pmat3::translate(space * (state.pos - bb.size()*offset)));
	}
	void labelBound(std::wstring const& text, aabb2 const& bound) { labelBound(text, bound, 0.f); }
	void labelBound(std::wstring const& text, vec2 const& offset) { labelBound(text, { -1.f, 1.f }, offset); }
	void labelBound(std::wstring const& text) { labelBound(text, { -1.f, 1.f }, 0.f); }

	struct slidero {
		bool holding = false;		//if the slider is being held by the user
		vec2 startholdoff = 0.f;	//offset from handle center when user starts holding
	};
	std::unordered_map<std::string, slidero> sliders;

	template<typename T>
	int slidert(std::string const& identifier, T* val, T minval, T maxval, bool textfield) {
		if (val == nullptr) return 0;
		updatestatelayout();

		slidero& sl = sliders[identifier];

		aabb2 bb = aabb2(-state.size, state.size) / 2.f + state.pos;
		float sepval = textfield ? .6f : 1.f;	//separation between slider and textfield (percentage of sizeX/2)

		vec2 handleSize = { bb.size().y / 2.f, bb.size().y };

		aabb2 barbb = bb.centered().movedIn({ handleSize.x/2.f, 0.f });
		barbb = (barbb * vec2(1.f,.5f)).maxx(barbb.maxx()*sepval) + imui::state.pos;
		drawspritetiledstate("slider-body", barbb, 1.f);

		float vperc = (static_cast<float>(*val) - static_cast<float>(minval)) / (static_cast<float>(maxval) - static_cast<float>(minval));
		vperc = outl::clamp(vperc, 0.f, 1.f);

		float handlex = -barbb.size().x/2.f + barbb.size().x * vperc;

		aabb2 handlebb = aabb2(-handleSize, handleSize)/2.f + barbb.center() + vec2x(handlex);

		bool miq = mouseInContainer() == curContainer.top() && mouseInQuad(handlebb);
		std::string state = "";
		if (sl.holding || (miq && input::getMouse(0)))	state = "slider-click";
		else if(miq)									state = "slider-hover";
		else											state = "slider-passive";

		drawspritetiledstate(state, handlebb, 1.f);

		//float slsizey = ssize.y * 2.f;
		//vec2 slsize = vec2(slsizey * .375f, slsizey);
		//aabb2 slbb = aabb2{ -slsize, slsize };
		//aabb2 slbbp = (slbb + vec2(slx, 0.f) + state.pos);
		//std::string astate = (miq || sl.holding) ? (input::getMouse(0) || sl.holding) ? "slider-click" : "slider-hover" : "slider-passive";
		//drawspritetiledstate(astate, vec2(slsizey*.375f, slsizey), pmat3::translate(state.pos+vec2(slx,.0f)));
		
		int res = 0;
		bool oldslholding = sl.holding;
		if (miq && input::getMouseDown(0)) { sl.holding = true; res = 1; sl.startholdoff = handlebb.center()-mouseproj(); }
		else if (input::getMouseUp(0))		{ sl.holding = false; res = 2; }

		if (sl.holding) {
			vec2 minpos = barbb[0];
			vec2 maxpos = barbb[3];
			vec2 nppos = mouseproj()+sl.startholdoff;

			float nperc = (nppos.x - minpos.x) / (maxpos.x - minpos.x);
			nperc = outl::clamp(nperc, 0.f, 1.f);

			*val = static_cast<T>((float)minval + ((float)maxval - (float)minval) * nperc);
			
			if (oldslholding == sl.holding) res = 3;
		}

		if (textfield) {
			saveState();
			imui::state.atlas = "";

			aabb2 tfbb = bb.centered();
			tfbb = tfbb.minx(tfbb.maxx()* sepval) + imui::state.pos;
			textFieldt<T>(identifier + "-txf", tfbb.center(), tfbb.size(), val);

			restoreState();
		}
	}
	template<typename T>
	int slidert(std::string const& identifier, vec2 const& pos, vec2 const& size, T* val, T minval, T maxval, bool textfield) { setmanuallayout(pos, size); int res = slidert<T>(identifier, val, minval, maxval, textfield); restoreState(); return res; }

	int slideri(std::string const& identifier, int* val, int minval, int maxval, bool textfield) { return slidert<int>(identifier, val, minval, maxval, textfield); }
	int slideri(std::string const& identifier, vec2 const& pos, vec2 const& size, int* val, int minval, int maxval, bool textfield) { return slidert<int>(identifier, pos, size, val, minval, maxval, textfield); }

	int sliderd(std::string const& identifier, double* val, double minval, double maxval, bool textfield) { return slidert<double>(identifier, val, minval, maxval, textfield); }
	int sliderd(std::string const& identifier, vec2 const& pos, vec2 const& size, double* val, double minval, double maxval, bool textfield) { return slidert<double>(identifier, pos, size, val, minval, maxval, textfield); }

	int sliderf(std::string const& identifier, float* val, float minval, float maxval, bool textfield) { return slidert<float>(identifier, val, minval, maxval, textfield); }
	int sliderf(std::string const& identifier, vec2 const& pos, vec2 const& size, float* val, float minval, float maxval, bool textfield) { return slidert<float>(identifier, pos, size, val, minval, maxval, textfield); }

	int dropMenu(std::string const& identifier, std::wstring const& text, std::vector<std::wstring> const& opts) {
		//if (cbxs.find(identifier) == cbxs.end()) { cbxs[identifier] = { -1 }; }
		updatestatelayout();

		//dropmenuo& cb = cbxs[identifier];

		//std::wstring sel = text;//L"Select...";
		//if (cb.selected > -1) sel = opts[cb.selected];

		aabb2 bb = aabb2(-state.size, state.size) / 2.f +state.pos;	//bb of the entire combo box
		//aabb2 selbb = bb.maxx(bb.scaled(.6f).maxx());					//bb of 'text' area
		aabb2 selbb = bb.maxx(bb.maxx()-.1f* space.inverted().scalingv().x);					//bb of 'text' area
		aabb2 arrowbb = bb.minx(bb.maxx()-.1f * space.inverted().scalingv().x);				//bb of arrow to drop menu

		bool miq = mouseInContainer() == curContainer.top() && mouseInQuad(bb);

		std::string state = miq ? input::getMouse(0) ? "dropmenu-click" : "dropmenu-hover" : "dropmenu-passive";
		drawspritetiledstate(state, bb, 1.f);
		drawstr(text, 0.f, selbb, pmat3::translate(space * selbb.center()));

		drawsprite("dropmenu-arrow", aabb2(-1.f, 1.f).fitted(arrowbb), 1.f);

		if (miq && input::click()) {
			dropmenu = !dropmenu;
			if (dropmenu) {
				dropjustopened = true;
				dropid = identifier;
				dropbb = bb;
				dropopts = opts;
			}
		}
		else if (!miq && input::click() && dropmenu && dropid.compare(identifier) == 0 && !mouseInQuad(dropbodybb)) dropmenu = false;
		
		if (dropmenu && dropid.compare(identifier) == 0) {
			dropspace = space;
			utildropmat = util::getTopMatrix();
			dropstate = imui::state;
		}

		if (dropsel != -1 && dropmenu && dropid.compare(identifier) == 0) {
			int res = dropsel;
			dropsel = -1;
			//cb.selected = res;
			dropmenu = false;
			return res;
		}

		return -1;
	}
	int dropMenu(std::string const& identifier, std::wstring const& text, vec2 const& pos, vec2 const& size, std::vector<std::wstring> const& opts) { setmanuallayout(pos, size); int res = dropMenu(identifier, text, opts); restoreState(); return res; }

	struct comboboxo {
		int selected = -1;
	};
	std::unordered_map<std::string, comboboxo> cbxs;

	int comboBox(std::string const& identifier, std::wstring const& hintText, std::vector<std::wstring> const& opts, int const& defaultOpt) {
		if (cbxs.find(identifier) == cbxs.end()) { cbxs[identifier] = { -1 }; }
		updatestatelayout();

		comboboxo& cb = cbxs[identifier];
		if (defaultOpt > -1 && cb.selected == -1) cb.selected = defaultOpt;

		std::wstring tsel = cb.selected > -1 ? opts[cb.selected] : hintText;
		int res = dropMenu(identifier + "-dp", tsel, state.pos, state.size, opts);
		if (res != -1) cbxs[identifier].selected = res;
		

		return res;
	}
	int comboBox(std::string const& identifier, std::wstring const& hintText, vec2 const& pos, vec2 const& size, std::vector<std::wstring> const& opts, int const& defaultOpt) { setmanuallayout(pos, size); int res = comboBox(identifier, hintText, opts); restoreState(); return res; }

	int checkBox(std::string const& identifier, bool* val) {
		if (!val) return 0; 
		updatestatelayout();
		aabb2 bb = aabb2(-state.size, state.size)/2.f + state.pos;

		bool miq = mouseInContainer() == curContainer.top() && mouseInQuad(bb);

		std::string state = miq ? input::getMouse(0) ? "checkbox-click" : "checkbox-hover" : "checkbox-passive";
		drawspritetiledstate(state, bb, 1.f);

		//debug::rtlog << state << "\n";

		int res = 0;
		if (miq && input::click()) { *val = !*val; res = 1; }

		if(*val) drawsprite("checkbox-check", aabb2(-1.f,1.f).fitted(bb), 1.f);

		return res;
	}
	int checkBox(std::string const& identifier, vec2 const& pos, vec2 const& size, bool* val) { setmanuallayout(); int res = checkBox(identifier, val); restoreState(); return res; }

	struct dropMenuBoundingBoxStructure {
		dropMenuBoundingBoxStructure* parent = nullptr;
		aabb2 bb;
		std::vector<dropMenuBoundingBoxStructure*> childs;
	};

	bool mouseInChildrenDropMenus(dropMenuBoundingBoxStructure const& dmbs) {
		for (int i = 0; i < dmbs.childs.size(); i++) {
			if (!dmbs.childs[i]) continue;

			//debug::rtlog << "checking child " << i << "\n";
			if (mouseInQuad(dmbs.childs[i]->bb) || mouseInChildrenDropMenus(*dmbs.childs[i])) return true;
		}

		return false;
	}

	struct begdropmeno {
		bool opened = false;
		dropMenuBoundingBoxStructure dmbs;
	};
	std::unordered_map<std::string, begdropmeno> begdps;

	bool beginDropMenu(std::string const& identifier, std::wstring const& text) {
		if (begdps.find(identifier) == begdps.end()) { begdps[identifier]; }
		updatestatelayout();

		begdropmeno& bdm = begdps[identifier];

		aabb2 bb = aabb2(-state.size, state.size) / 2.f + state.pos;	//bb of the entire combo box
		//aabb2 selbb = bb.maxx(bb.scaled(.6f).maxx());					//bb of 'text' area
		aabb2 selbb = bb.maxx(bb.maxx()-.1f * space.inverted().scalingv().x);					//bb of 'text' area
		aabb2 arrowbb = bb.minx(bb.maxx()-.1f * space.inverted().scalingv().x);				//bb of arrow to drop menu

		bool miq = (mouseInContainer() == curContainer.top() || state.indropmenu) && mouseInQuad(bb);

		std::string tstate = miq ? input::getMouse(0) ? "dropmenu-click" : "dropmenu-hover" : "dropmenu-passive";
		drawspritetiledstate(tstate, bb, 1.f);
		drawstr(text, 0.f, selbb, pmat3::translate(space * selbb.center()));

		drawsprite("dropmenu-arrow", aabb2(-1.f, 1.f).fitted(arrowbb), 1.f);

		//debug::drawQuad(util::getTopMatrixAspectInv() * space * bb);
		//debug::drawQuad(util::getTopMatrixAspectInv() * space * selbb);
		//debug::drawQuad(util::getTopMatrixAspectInv() * space * arrowbb);

		//if (button(text, state.pos, state.size)) {
		//	bdm.opened = !bdm.opened; 
		//	if (bdm.opened) { curbegdp = identifier; dropbeganopened = true; }
		//}

		if (miq && input::getMouseUp(0)) {
			bdm.opened = !bdm.opened;
		}

		if (!miq && input::getMouseUp(0) && 
			!mouseInQuad(bdm.dmbs.bb) && 
			!mouseInChildrenDropMenus(bdm.dmbs)) { bdm.opened = false; }

		if (bdm.dmbs.childs.size() > 0) bdm.dmbs.childs.clear();

		if (bdm.opened) {
			bool alreadydrop = state.indropmenu;
			saveState();

			state.currentDropMenuBB = vec2(0.f);
			if (state.indropmenu && state.dropmenuid.compare("") != 0) { //root drop menu
				bdm.dmbs.parent = &begdps[state.dropmenuid].dmbs;
			}

			state.indropmenu = true;
			state.dropmenuid = identifier;

			if (alreadydrop)	{ setPos(state.pos + state.size.xo()); state.offset = state.size.ny(); }
			else				{ setPos(state.pos - state.size.oy()); state.offset = state.size.ny(); }
			state.layout = VERTICAL;

			beginContainer(identifier + "-dm", bdm.dmbs.bb);
		}

		return bdm.opened;
	}
	bool beginDropMenu(std::string const& identifier, std::wstring const& text, vec2 const& pos, vec2 const& size) { setmanuallayout(pos, size); bool res = beginDropMenu(identifier, text); if(!res) restoreState(); return res; }

	void endDropMenu() { 
		endContainer();
		//before restoring, i save the bounding box of the current drop menu and store it in the
		//children of its super drop menu
		if (!state.indropmenu || state.dropmenuid.compare("") == 0) { std::cout << "called enddropmenu before beginDropMenu!\n"; return; }

		begdropmeno& bdm = begdps[state.dropmenuid];
		bdm.dmbs.bb = state.currentDropMenuBB;

		if (bdm.dmbs.parent) {
			bdm.dmbs.parent->childs.push_back(&bdm.dmbs);
		}

		restoreState(); 
		if (state.manualLayoutOverride) restoreState();
	}

	void reset() {
		if (ststack.size() > 0) { std::cout << "warning: degenerate imui stack at end of frame!\n"; ststack.st.clear(); }
		//if(containers.size() > 0) containers.clear();

		if (dropmenu) {	//there can only be one dropmenu activated at one time
			saveState();
			state = dropstate;
			util::pushMatrix();
			util::resetMatrix();
			util::multMatrix(utildropmat);
			mat3 oldsp = space;
			space = dropspace;

			beginContainer("__parr__top__", dropbodybb);

			aabb2 projScreen = tospace() * util::getResbbNDCAspect();
			dropbodybb = aabb2(dropbb.fmin() - vec2y(dropbb.size().y * dropopts.size()), dropbb[3]);
			dropbodybb = dropbodybb.forcedIn(projScreen);

			vec2 startpos = dropbodybb.edgeCenter(1) - vec2y(dropbb.size().y / 2.f);

			drawspritetiledstate("dropmenu-passive", dropbodybb, 1.f);
			int wrapi = 0;
			for (int i = 0; i < dropopts.size(); i++) {
				vec2 pos = startpos - vec2y(dropbb.sizey() * i);
				if (pos.y-dropbb.sizey()/2.f < projScreen.miny()) { 
					if (wrapi == 0) { wrapi = i; } 
					pos = vec2(pos.x + dropbb.sizex(), projScreen[1].y-dropbb.sizey()/2.f) - vec2y(dropbb.sizey() * (i - wrapi));
					dropbodybb.rescale(pos + dropbb.size()/2.f);
					//std::cout << wrapi << " " << pos << "\n";
				}

				if (button(dropopts[i], pos, dropbb.size())) {
					if(!dropjustopened) dropsel = i;
				}
			}
			endContainer();

			if (dropjustopened) dropjustopened = false;
			
			space = oldsp;
			util::popMatrix();
			restoreState();
		}

		//remove every container that was not called in this frame
		bool rem = true;
		while (rem) {
			rem = false;
			for (auto& c : pingContainers) if (!c.second) { containers.erase(c.first); pingContainers.erase(c.first); posContainers.erase(c.first); rem = true; break; }
		}
		

		for (auto& c : pingContainers) {
			c.second = false;
		}

		curposcontainer = 0;

		state.pos = 0.f;	state.size = 0.f;
		state.nextPos = 0.f;	state.nextSize = 0.f;
		gid = 0;
	}

	void init() {
		ststack.st.reserve(10);
		curContainer.push("");
	}
}
