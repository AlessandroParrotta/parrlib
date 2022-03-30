#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <sstream>
#include <mutex>
#include <algorithm>


#include "math/stringutils.h"
#include "math/utils2d/utils2d.h"
#include "TextRenderer.h"

namespace prb {
	namespace debug {

		extern std::vector<std::wstring> debug;
		extern std::wstringstream tss;
		extern TextRenderer txr;
		extern bool drawStrs;
		extern int deblimit;

		extern bool windowEnabled;


		void setColor(vec4 const& col);
		void setStrokeWidth(float strokeWidth);

		void drawText(mat3 const& tr);
		void drawText(std::wstring const& wstr, mat3 const& tr);
		template<typename... Args> void drawText(mat3 const& tr, Args... args) { std::wstring wstr = stru::compose(args...);	drawText(wstr, tr); }
		template<typename... Args> void drawText(vec2 const& tr, Args... args) { std::wstring wstr = stru::compose(args...);	drawText(wstr, txrutl::matToPos(pmat3::translate(tr))); }
		template<typename... Args> void drawTextPos(mat3 const& tr, Args... args) { std::wstring wstr = stru::compose(args...);	drawText(wstr, txrutl::matToPos(tr)); }

		void drawLine(vec2 const& l1, vec2 const& l2, mat3 const& tr = 1.f);
		void drawLine(utl2::Line const& ln, mat3 const& tr = 1.f);
		void drawShape(std::vector<vec2> const& shape, mat3 const& tr = 1.f);
		void drawCircle(mat3 const& tr);
		void drawQuad(mat3 const& tr);


		LRESULT sendMessage(std::wstring str);

		void createWindow();

		void init();
		void dispose();

		extern std::mutex dmtx;
		class uss {
		public:
			std::wstringstream ss;
			std::wstringstream tss;
			template<class T> uss& operator<< (T val) {
				if (windowEnabled) {
					tss << val;
					if (tss.str().find('\n' != std::string::npos)) {
						sendMessage(tss.str()); tss.str(L""); tss.clear();
					}
				}
				else ss << val;

				return *this;
			}
		};

		extern uss ss;
		extern std::wstringstream rtss;
		//void printdeb();
		void drawDebStrs(vec2 screenSize);

		//std::wstring tows(int i);
		//std::wstring tows(unsigned int i);
		//std::wstring tows(float i);
		//std::wstring tows(double i);

		template<typename T> std::wstring tows(T t) { return std::to_wstring(t); }
		std::wstring tows(const wchar_t* c);
		std::wstring tows(const char* c);
		std::wstring tows(std::string const& str);

		std::u32string toutf32(const std::string& s);

		std::string tos(std::wstring wstr);

		template<typename... Args> void pr(Args... args)	{ 
			std::wstring wstr = stru::composew(args...);	ss << wstr;	

			//int unpack[] = { ([](auto& arg) {
			//	ss << arg;
			//}(args), 0)..., 0 };
			//static_cast<void>(unpack);

			//auto values = { args... };
			//std::for_each(values.begin(), values.end(), [](auto& val) { ss << val; });
		}
		template<typename... Args> void prt(Args... args)	{ std::wstring wstr = stru::composew(args...);	rtss << wstr;	}


		void mbe(std::wstring const& errstr);
		void mbe(std::string const& errstr);
		template<typename... Args> void mbe(Args... args) { std::wstring wstr = stru::composew(args...);	mbe(wstr); }
		void mbe();

		void mbi(std::wstring const& errstr);
		void mbi(std::string const& errstr);
		template<typename... Args> void mbi(Args... args) { std::wstring wstr = stru::composew(args...);	mbi(wstr); }
		void mbi();


		void outStr(std::wstring const& str);
		template<typename... Args> void outStr(Args... args) { std::wstring wstr = stru::composew(args...);	outStr(wstr); }
		void outStr();

	}

	namespace deb = debug;
}
