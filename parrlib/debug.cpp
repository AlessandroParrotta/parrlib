#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS


#include "debug.h"

#include <locale>
#include <codecvt>
#include <string>

#include <parrlibcore/otherutil.h>
#include <parrlibcore/stringutils.h>

#include "util.h"
#include "input.h"

namespace prb {
	namespace debug {

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::vector<std::wstring> debug;
		std::wstringstream tss;
		bool drawStrs = true;
		int deblimit = 5000;

		TextRenderer txr;
		std::mutex dmtx;

		uss ss;
		std::wstringstream rtss;
		std::vector<std::wstring> strs;


		vec4 color = 1.f;
		void setColor(vec4 const& color) { deb::color = color; }

		float strokeWidth = .005f;
		void setStrokeWidth(float strokeWidth) { deb::strokeWidth = strokeWidth; }

		struct TextStr {
			mat3 tr;
			std::wstring text;
			vec4 color;
		};
		std::vector<TextStr> textStrs;
		void drawText(mat3 const& tr) {
			textStrs.push_back({ tr, tss.str(), color });
			tss.clear(); tss.str(L"");
		}
		void drawText(std::wstring const& wstr, mat3 const& tr) { tss << wstr; drawText(tr); }

		struct LineObj {
			vec2 l1, l2;
			mat3 tr = 1.f;
			vec4 color;
		};
		std::vector<LineObj> lineObjs;
		void drawLine(vec2 const& l1, vec2 const& l2, mat3 const& tr) { lineObjs.push_back({ l1, l2, tr, color }); }
		void drawLine(utl2::Line const& ln, mat3 const& tr) { drawLine(ln.p1, ln.p2, tr); }

		struct ShapeObj {
			std::vector<vec2> sh;
			mat3 tr;
			vec4 color;
		};
		std::vector<ShapeObj> shapeObjs;
		void drawShape(std::vector<vec2> const& shape, mat3 const& tr) { shapeObjs.push_back({ shape, tr, color }); }
		
		struct CircleObj {
			mat3 tr;
			vec4 color;
		};
		std::vector<CircleObj> circleObjs;
		void drawCircle(mat3 const& tr){ circleObjs.push_back({ tr, color }); }
		
		struct QuadObj {
			mat3 tr;
			vec4 color;
		};
		std::vector<QuadObj> quadObjs;
		void drawQuad(mat3 const& tr){ quadObjs.push_back({ tr, color }); }

		bool windowEnabled = true;

		outl::AsyncProgram prog;
		HWND debHwnd = NULL;
		LRESULT sendMessage(HWND hWnd, std::wstring str) {
			LPCWSTR sstr = str.c_str();
			COPYDATASTRUCT cds;
			cds.dwData = 8952;
			cds.cbData = sizeof(WCHAR) * (str.length() + 1);
			cds.lpData = (PVOID)sstr;
			//deb::ss.ss << "sending  '" << str << "' '" << sstr << "'\n";
			return SendMessage(hWnd, WM_COPYDATA, (WPARAM)(HWND)hWnd, (LPARAM)(LPVOID)&cds);
		}
		LRESULT sendMessage(std::wstring str) { return sendMessage(debHwnd, str); }

		void createWindow() {
			if (windowEnabled) {
				std::wstring path = L"DebugWindow.exe";
				prog = outl::openProgramAsync(path, L"");
				//deb::ss.ss << prog.dwProcessID << " " << prog.dwThreadID << "\n";

				for (int i = 0; i < 10; i++) {
					std::this_thread::sleep_for(std::chrono::milliseconds(2));

					std::wstringstream ss;
					std::wstring _class; ss << prog.dwProcessID << "_Class"; _class = ss.str(); ss.str(L""); ss.clear();
					std::wstring name; ss << prog.dwProcessID << "_DebugHidden"; name = ss.str(); ss.str(L""); ss.clear();

					//deb::ss.ss << "looking for " << _class << " " << name << "\n";

					//debHwnd = otherutil::findWindowByClassName(L"WindowClass", L"Debug Window");
					debHwnd = otherutil::findWindowByClassName(_class, name);
					if (debHwnd != (HWND)0) break;
					//deb::ss.ss << "tried getting window " << i << "\n";
				}
			}
		}

		void init() {
			txr = TextRenderer({ "segoeui.ttf" }, 26);
		}

		void dispose() {
			if (windowEnabled) {
				outl::closeProgramAsync(prog);
			}
		}

		std::wstring replaceCharWithString(std::wstring str, wchar_t c, std::wstring rstr) {
			std::wstring res;

			for (int i = 0; i < str.length(); i++) {
				if (str[i] == c) {
					res += rstr;
				}
				else res += str[i];
			}

			return res;
		}

		std::vector<std::wstring> getDebs(std::wstringstream& wss) {

			std::wstring bigstr = wss.str();
			bigstr = replaceCharWithString(bigstr, L'\t', L"    ");

			std::vector<std::wstring> strs;

			bool foundNewLine = false;
			//int len = bigstr.length();
			for (int i = 0; i < bigstr.length(); i++) {
				if (bigstr[i] == L'\n') {
					if (i == 0) { strs.push_back(L""); bigstr.erase(bigstr.begin(), bigstr.begin() + 1); i = -1; }
					else {
						strs.push_back(bigstr.substr(0, i));
						bigstr.erase(bigstr.begin(), bigstr.begin() + i + 1);
						i = -1;
						//debssw << strs.back() << " " << bigstr << " " << bigstr.length(); msbinfow();
					}
					foundNewLine = true;
				}
			}

			if (foundNewLine) {
				wss.clear();
				wss.str(L"");
			}

			return strs;
		}

		float strSpacing = .04f;
		float strDebStart = .575f;
		int startOffset = 0;
		float offsetSpeed = 1.f;
		int debDrawLimit = 40;
		void drawDebStrs(vec2 screenSize) {
			//debssw << deb.str() << "\n" << rtdeb.str(); msbinfow();

			std::lock_guard<std::mutex> lck(dmtx);

			std::vector<std::wstring> rtdebs = getDebs(rtss);
			if (drawStrs) {
				for (int i = 0; i < rtdebs.size(); i++) {
					txr.drawText(rtdebs[i], { -.99f, .95f - strSpacing * i });
				}
			}

			if (input::getKey(VK_PRIOR)) { //page up
				if (input::getKey(VK_SHIFT))startOffset = (std::min)(-(int)strs.size() + debDrawLimit, 0);
				else if (input::getKey(VK_LCONTROL)) startOffset--;
				else startOffset -= offsetSpeed; offsetSpeed += .005f; rtss << "offset " << startOffset << "/" << strs.size() << "\n";
			}
			else if (input::getKey(VK_NEXT)) { //page down
				if (input::getKey(VK_SHIFT))startOffset = 0;
				else if (input::getKey(VK_LCONTROL)) startOffset++;
				else startOffset += offsetSpeed; offsetSpeed += .005f; rtss << "offset " << startOffset << "/" << strs.size() << "\n";
			}
			else offsetSpeed = 1.f;
			startOffset = outl::clamp(startOffset, (std::min)(-(int)strs.size() + debDrawLimit, 0), debDrawLimit);

			std::vector<std::wstring> debs = getDebs(ss.ss);
			if (debs.size() > 0) strs.insert(strs.end(), debs.begin(), debs.end());
			if (strs.size() > deblimit) strs.erase(strs.begin(), strs.end() - deblimit);
			if (drawStrs) {
				int start = (std::max)((int)strs.size() - debDrawLimit + startOffset, 0);
				int end = (std::min)(start + debDrawLimit, (int)strs.size());
				for (int i = start; i < end; i++) {
					txr.drawText(strs[i], { -.99f, strDebStart - strSpacing * (i - start) });
				}
			}

			vec4 tcolor = txr.color();
			for (auto& t : textStrs) {
				txr.color(color);
				txr.drawText(t.text, t.tr);
			}
			textStrs.clear();
			txr.color(tcolor);

			for (auto& t : lineObjs) {
				util::setColor(t.color);
				vec2 p1 = pmat3::orthoMaxAspect(cst::res()).inverted() * t.tr * t.l1;
				vec2 p2 = pmat3::orthoMaxAspect(cst::res()).inverted() * t.tr * t.l2;
				mat3 tr = pmat3::orthoMaxAspect(cst::res()) * pmat3::translate(p1) * pmat3::lookAt(p1, p2) * pmat3::scale(vec2((p2 - p1).magnitude(), strokeWidth));
				util::drawQuad({ { 0.f,-.5f }, { 1.f, .5f } }, tr);
			}
			lineObjs.clear();

			for (auto& t : shapeObjs) {
				util::setColor(t.color);
				util::drawShape(t.sh, t.tr);
			}
			shapeObjs.clear();

			for (auto& t : circleObjs) {
				util::setColor(t.color);
				util::drawCircle(t.tr);
			}
			circleObjs.clear();
			
			for (auto& t : quadObjs) {
				util::setColor(t.color);
				util::drawQuad(aabb2s(1.f), t.tr);
			}
			quadObjs.clear();
		}

		//std::wstring tows(int i) { return std::to_wstring(i); }
		//std::wstring tows(unsigned int i) { return std::to_wstring(i); }
		//std::wstring tows(float i) { return std::to_wstring(i); }
		//std::wstring tows(double i) { return std::to_wstring(i); }
		//std::wstring tows(const wchar_t* c) { return std::wstring(c); }
		//std::wstring tows(const char* c) { return converter.from_bytes(c); }
		
		std::wstring tows(const wchar_t* c) { return std::wstring(c); }
		std::wstring tows(const char* c) { return converter.from_bytes(c); }
		std::wstring tows(std::string const& str) { /*std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;*/ return converter.from_bytes(str); }

		std::u32string toutf32(const std::string& s)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
			return conv.from_bytes(s);
		}

		std::string tos(std::wstring wstr) { return converter.to_bytes(wstr); }

		void mbe(std::wstring const& errstr) { MessageBox(0, errstr.c_str(), L"Error", MB_ICONERROR); }
		void mbe(std::string const& errstr) { mbe(tows(errstr)); }
		void mbe() { mbe(tss.str()); tss.clear(); tss.str(L""); }

		void mbi(std::wstring const& infostr) { MessageBox(0, infostr.c_str(), L"Info", MB_ICONINFORMATION); }
		void mbi(std::string const& infostr) { mbi(tows(infostr)); }
		void mbi() { mbi(tss.str()); tss.clear(); tss.str(L""); }


		void outStr(std::wstring const& str) { OutputDebugString(str.c_str()); }
		void outStr() { std::wstring str = tss.str(); outStr(str); tss.clear(); tss.str(L""); }
	}
}
