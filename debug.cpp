#include "Debug.h"

namespace debug {

	bool enabled = true;

	logss log, rtlog;

	float slider = 0.0f;
	float sliderPerc = 0.0f;
	bool sliderLock = false;
	Vector2f sliderPos;
	Vector2f sliderSize;

	TextRenderer txr;

	int maxStrs = 1000;
	int maxRTStrs = 100;

	bool enableMsgCorner = false;

	std::vector<std::wstring> strs;
	std::vector<std::wstring> rtStrs;

	void startResizing() {

	}

	void resize() {

	}

	void endResizing() {

	}
	
	bool alreadyInited = false;
	void init() {
		txr = { { ("assets/fonts/segoeui.ttf") }, 18 }; //(int)(prc::res().y * 0.02f) };
		txr.setOutline(2);
		txr.setFiltering(GL_NEAREST, GL_NEAREST);

		if (!alreadyInited) {
			strs.reserve(5000);
			rtStrs.reserve(100);

			alreadyInited = true;
		}
	}

	Vector2f beginSliderMovePos;

	void update() {
		if (enabled) {
			//something
		}
		else {
			log.clear();
			log.str(L"");
			rtlog.clear();
			rtlog.str(L"");
		}
	}

	void print(std::string s)		{	if (enabled) log << s.c_str();				}
	void println(std::string s)		{	if (enabled) log << s.c_str() << "\n";		}
	void rtPrint(std::string s)		{	if (enabled) rtlog << s.c_str();			}
	void rtPrintln(std::string s)	{	if (enabled) rtlog << s.c_str() << "\n";	}

	struct DebugObject {
		Vector2f pos;

		DebugObject(Vector2f pos) {
			this->pos = pos;
		}

		virtual void draw() {

		}
	};

	struct Line : public DebugObject {
		Vector2f p2;
		Vector4f color;

		Line(Vector2f pos, Vector2f p2, Vector4f color) : DebugObject(pos) {
			this->p2 = p2;
			this->color = color;
		}

		void draw() {
			//glColor4f(color.x, color.y, color.z, color.w);
			util::setColor(color);
			util::drawLine(pos, p2);
		}
	};

	struct Quad : public DebugObject {
		Vector2f size;
		Vector4f color;

		Quad(Vector2f pos, Vector2f size, Vector4f color) : DebugObject(pos){
			this->size = size;
			this->color = color;
		}

		void draw() {
			//glColor4f(color.x, color.y, color.z, color.w);
			util::setColor(color);
			util::drawQuad(pos, size, GL_LINE_LOOP);
		}
	};

	struct Circle : public DebugObject {
		float radius;
		Vector4f color;

		Circle(Vector2f pos, float radius, Vector4f color) : DebugObject(pos) {
			this->radius = radius;
			this->color = color;
		}

		void draw() {
			//glColor4f(color.x, color.y, color.z, color.w);
			util::setColor(color);
			util::drawCircle(pos, radius, GL_LINE_LOOP);
		}
	};

	struct TextString : public DebugObject {
		std::string text;
		float textScale;
		Vector4f color;

		TextString(std::string text, Vector2f pos, float textScale, Vector4f color) : DebugObject(pos) {
			this->text = text;
			this->textScale = textScale;
			this->color = color;
		}

		void draw() {
			Vector4f oldColor = txr.color();
			txr.color(color);
			txr.drawWStringpx(stru::toWString(text), pmat3::translate(pos));
			txr.color(oldColor);
		}
	};

	std::vector<std::shared_ptr<DebugObject>> debugObjs;
	vec4 defcolor = vc4::red;

	void drawQuad(Vector2f pos, Vector2f size, Vector4f color) {debugObjs.push_back(std::shared_ptr<DebugObject>(new Quad(pos, size, color)));}
	void drawQuad(Vector2f pos, Vector2f size) {drawQuad(pos, size, defcolor);}

	void drawQuad(AxisAlignedBoundingBox2D const& bb, vec4 color) { drawQuad(bb.fmin(), bb.size(), color); }
	void drawQuad(AxisAlignedBoundingBox2D const& bb) { drawQuad(bb, defcolor); }


	void drawCircle(Vector2f pos, float radius, Vector4f color) {debugObjs.push_back(std::shared_ptr<DebugObject>(new Circle(pos, radius, color)));}
	void drawCircle(Vector2f pos, float radius) {drawCircle(pos, radius, defcolor);}


	void drawLine(Vector2f p1, Vector2f p2, Vector4f color) {debugObjs.push_back(std::shared_ptr<DebugObject>(new Line(p1, p2, color)));}
	void drawLine(Vector2f p1, Vector2f p2) {drawLine(p1, p2, defcolor);}


	void drawText(std::string text, Vector2f pos, float textScale, Vector4f color) {debugObjs.push_back(std::shared_ptr<DebugObject>(new TextString(text, pos, textScale, color)));}
	void drawText(std::string text, Vector2f pos, float textScale) {drawText(text, pos, textScale, defcolor);}

	void parseLogs(std::wstringstream& ss, std::vector<std::wstring>& wstrs) {
		wstrs = stru::toLines(ss.str());
	}

	void drawObjects() {
		glDisable(GL_BLEND);
		if (debugObjs.size() > 0) {
			for (int i = 0; i < debugObjs.size(); i++) { debugObjs[i]->draw(); }
			debugObjs.clear();
		}
	}

	void draw() {
		if (!enabled) return;

		if (strs.size() > maxStrs) {
			strs.erase(strs.begin(), strs.begin() + (strs.size() - maxStrs));
		}

		if (log.str().length() > 0) { parseLogs(log, strs); log.str(L""); log.clear();}
		if (rtlog.str().length() > 0) { parseLogs(rtlog, rtStrs); rtlog.str(L""); rtlog.clear(); }

		float aspectCorrect = cst::resaspect();
		float scaleFactor = 1.f;
		float cheight = (txr.getFontSize() + txr.getOutline()) / prc::wres().y * (prc::wres().aspectmaxv().y*2.f);

		txr.color(vc4::white);

		for (int i = 0; i < rtStrs.size(); i++) {
			//float y = 1.0f - cheight * i;
			vec2 screen = cst::res().aspectmaxv();
			vec2 pos = screen.nx() - vec2y(cheight * i);
				
			if (pos.y >= -screen.y && pos.y <= screen.y) {
				//txr.drawWStringpx(rtStrs[i], vec2(1.f), pmat3::translate(vec2(-prc::wres().aspectmax(), y)));
				//txr.drawWStringpx(rtStrs[i], vec2(1.f).ny(), pmat3::translate(vec2(-std::fmax(prc::wres().aspectx(),1.f),y)));
				txr.drawWStringpx(rtStrs[i], vec2(1.f).ny(), pmat3::translate(pos));
			}

			rtStrs[i] = L"";
		}

		rtStrs.clear();

		if (enableMsgCorner) {
			glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
			util::drawQuad(Vector2f(-1.0f), Vector2f(0.4f, 1.0f));

			float sliderVal = slider/2.0f;

			if (cheight *strs.size() < 1.0f) {
				sliderVal = 0.0f;
			}

			for (int i = strs.size() - 1; i >= 0; i--) {
				float ypos = -0.99f + cheight * (strs.size() - 1 - i) - sliderVal;

				if (ypos < -1.0f - cheight) continue;
				if (ypos >= 0.0f) break;


				txr.drawWStringpx(strs[i], vec2(1.f), pmat3::translate(pmat3::orthoAspectX(prc::res()) * vec2(-1.f, ypos)));
			}
			
			if (cheight *strs.size() >= 1.0f) {
				if (sliderLock) {
					glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
				}
				else if (input::getMouseInQuad(sliderPos, sliderSize)) {
					glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
				}
				else {
					glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
				}

				util::drawQuad(sliderPos, Vector2f(0.01f, 0.06f));
			}
		}		
	}
}
