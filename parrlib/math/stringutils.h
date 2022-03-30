#pragma once

#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <sstream>
#include <any>

#include "timer.h"
#include "otherutil.h"

std::wostream& operator<<(std::wostream& wstr, const char* str);
std::wostream& operator<<(std::wostream& wstr, std::string const& str);
std::wostream& operator<<(std::wostream& wstr, std::string& str);

namespace prb {
	namespace stringutils {

        std::wstring reversew(std::wstring str);
        std::wstring getExtw(std::wstring file);
        std::wstring getFileNamew(std::wstring const& path);
        std::wstring getFolderw(std::wstring const& path);
        std::vector<std::wstring> splitFilePathw(std::wstring const& path);

		std::string reverse(std::string str);
		std::string getExt(std::string file);
		std::string getFileName(std::string const& path);
		std::string getFolder(std::string const& path);
		std::vector<std::string> splitFilePath(std::string const& path);


		bool isAbsolute(std::string const& str);
		std::string correctExeLocIfAbsolute(std::string const& str);

		std::wstring fallbackPath(std::wstring const& prefix, std::wstring const& path);
		std::wstring fallbackPath(std::wstring const& path);

		std::string fallbackPath(std::string const& prefix, std::string const& path);
		std::string fallbackPath(std::string const& path);

		std::string cutDecimals(std::string number, int decimals);

		std::string getByteSizeFormatted(long long numBytes, int decimals);

		enum TimeScale {
			NANO,
			MICRO,
			MILLI,
			SEC
		};
		std::string getTimeFormatted(double inputTime, int decimals, TimeScale inputTimeScale);
		std::string getTimeFormattedNano(double inputTime, int decimals);
		std::string getTimeFormattedMicro(double inputTime, int decimals);
		std::string getTimeFormattedMilli(double inputTime, int decimals);
		std::string getTimeFormattedSec(double inputTime, int decimals);

		std::string toTimestampFormat(int sec);

		int getHexVal(char c);

		std::wstring toWString(std::string const& str);
		std::wstring towstr(std::string const& str);
		std::wstring towstr(float i);
		std::wstring towstr(double i);
		std::wstring towstr(int i);
		std::wstring towstr(const char *str);
		float tof(std::wstring const& wstr);
		double tod(std::wstring const& wstr);
		int toi(std::wstring const& wstr);

		template<typename T>
		std::wstring towstr(T t) {
			std::wstringstream ss;
			ss << t;
			std::wstring str;
			ss >> str;
			return str;
		}
		template<typename T>
		T tot(std::wstring const& str) {
			std::wstringstream ss;
			ss << str;
			T t;
			ss >> t;
			return t;
		}


        inline std::stringstream& composess() { static std::stringstream composesst; return composesst; };
        template<typename... Args> inline std::string compose(Args... args) {
            int unpack[] = { ([](auto& arg) {
                composess() << arg;
            }(args), 0)..., 0 };
            static_cast<void>(unpack);

            std::string str = composess().str();
            composess().clear(); composess().str("");

            return str;
        }
        template<typename... Args> inline std::string fmt(Args... args) { return compose(args...); }
        // alias for fmt(outl::getExeFolder().c_str(), ...)
		template<typename... Args> std::wstring exeFmt(Args... args) { return compose(outl::getExeFolder().c_str(),  args...); }


        inline std::wstringstream& composeWss() { static std::wstringstream composeWsst; return composeWsst; };
        template<typename... Args> inline std::wstring composew(Args... args) {
            int unpack[] = { ([](auto& arg) {
                composeWss() << arg;
            }(args), 0)..., 0 };
            static_cast<void>(unpack);

            std::wstring wstr = composeWss().str();
            composeWss().clear(); composeWss().str(L"");

            return wstr;
        }
        template<typename... Args> inline std::wstring fmtw(Args... args) { return composew(args...); }
        template<typename... Args> inline void prw(Args... args) { std::wcout << composew(args...); }


		std::string toString(std::wstring const& wstr);
		std::string tostr(std::wstring const& wstr);
		std::string tostr(float i);
		std::string tostr(double i);
		std::string tostr(int i);

		float tof(std::string const& str);
		double tod(std::string const& str);
		int toi(std::string const& str);

		float getf(std::stringstream& ss);
		double getd(std::stringstream& ss);
		int geti(std::stringstream& ss);

		float getf(std::ifstream& f);
		double getd(std::ifstream& f);
		int geti(std::ifstream& f);

		template<typename T>
		std::string tostr(T t) {
			std::stringstream ss;
			ss << t;
			std::string str;
			ss >> str;
			return str;
		}
		template<typename T>
		T tot(std::string const& str) {
			std::stringstream ss;
			ss << str;
			T t;
			ss >> t;
			return t;
		}


		std::vector<std::wstring> toLines(std::wstring const& str);
		std::vector<std::string> toLines(std::string const& str);



        inline std::wstring getArg_old(std::wstring const& str, int n) {
            std::wstring res = L"";

            int i = 0;
            int argN = 0;
            while (i < str.length() && argN <= n) {
                while (i < str.length() && (str[i] == ' ' || str[i] == '\n')) { i++; }
                if (i < str.length() && str[i] == '\"') { i++; while (i < str.length() && str[i] != '\"') { if (argN == n) res += str[i]; i++; } i++; }
                else if (i < str.length() && str[i] == '\'') { i++; while (i < str.length() && str[i] != '\'') { if (argN == n) res += str[i]; i++; } i++; }
                else { while (i < str.length() && (str[i] != ' ' && str[i] != '\n')) { if (argN == n) res += str[i]; i++; } }
                argN++;
            }

            return res;
        }

        struct argAdv { std::wstring res; int i; };
        inline argAdv nextArg(std::wstring const& str, int startFrom) {
            std::wstring res = L"";

            int i = startFrom;
            while (i < str.length() && (str[i] == ' ' || str[i] == '\n')) { i++; }
            if (i < str.length() && str[i] == '\"') { i++; while (i < str.length() && str[i] != '\"') { res += str[i]; i++; } i++; }
            else if (i < str.length() && str[i] == '\'') { i++; while (i < str.length() && str[i] != '\'') { res += str[i]; i++; } i++; }
            else { while (i < str.length() && (str[i] != ' ' && str[i] != '\n')) { res += str[i]; i++; } }

            return { res, i };
        }

        inline int skipNextArg(std::wstring const& str, int startFrom) {
            int i = startFrom;

            while (i < str.length() && (str[i] == ' ' || str[i] == '\n')) { i++; }
            if (i < str.length() && str[i] == '\"')      { i++; while (i < str.length() && str[i] != '\"') { i++; } i++; }
            else if (i < str.length() && str[i] == '\'') { i++; while (i < str.length() && str[i] != '\'') { i++; } i++; }
            else { while (i < str.length() && (str[i] != ' ' && str[i] != '\n')) { i++; } }

            return i;
        }

        inline std::wstring getArg(std::wstring const& str, int n) {
            if (n < 0) return L"";

            std::wstring res = L"";

            int i = 0;
            int argN = 0;
            while (i < str.length() && argN < n) { i = skipNextArg(str, i); argN++; }
            res = nextArg(str, i).res;

            return res;
        }

        inline std::vector<std::wstring> toArgs(std::wstring const& str) {
            std::vector<std::wstring> args;

            argAdv i = { L"", 0 };
            while (i.i < str.length()) { i = nextArg(str, i.i); if (i.res.length() > 0) args.push_back(i.res); }

            return args;
        }
        inline std::vector<std::wstring> toArgs(std::wstring const& str, int start, int end) {
            if (start < 0 || end < 0) {
                int argSize = 0; int i = 0; while (i < str.length()) { int oldI = i; i = skipNextArg(str, i); if (oldI == i) break; argSize++; }
                //std::cout << "argsize " << argSize << "\n";

                if (start == 1 && end == -1) end = argSize + 1;

                if (start >= argSize) start = argSize;
                if (end >= argSize) end = argSize;

                if (start < 0) start = argSize + 1 + start;
                if (end < 0) end = argSize + 1 + end;
            }
            if (start > end) { int t = start; start = end; end = t; }
            if (start == end) return {};

            std::vector<std::wstring> args;

            argAdv i = { L"", 0 };
            int argN = 0;
            while (i.i < str.length() && argN < start) { i.i = skipNextArg(str, i.i); argN++; }
            while (i.i < str.length() && argN >= start && argN < end) { i = nextArg(str, i.i); if (i.res.length() > 0) args.push_back(i.res); argN++; }

            return args;
        }
        inline std::vector<std::wstring> toArgs(std::wstring const& str, std::vector<int> argWanted) {
            std::vector<std::wstring> args;

            for (int i = 0; i < argWanted.size(); i++) args.push_back(getArg(str, argWanted[i]));

            return args;
        }
        inline std::wstring fromArgs(std::vector<std::wstring> const& args) {
            std::wstring res;
            for (int i = 0; i < args.size(); i++) { 
                bool found = args[i].find(L' ') != std::wstring::npos || args[i].find(L'\n') != std::wstring::npos;
                if (found) res += L"\"";
                res += args[i];
                if (found) res += L"\"";
                if (i < args.size() - 1) res += L" ";
            }
            return res;
        }

        inline std::wstring replace(std::wstring subject, std::wstring const& search,
            std::wstring const& replace) {
            size_t pos = 0;
            while ((pos = subject.find(search, pos)) != std::wstring::npos) {
                subject.replace(pos, search.length(), replace);
                pos += replace.length();
            }
            return subject;
        }

        struct path {
            std::vector<std::wstring> els;
            bool isDirectory = false;

            bool isSlash(wchar_t c) const { return c == '\\' || c == '/'; }

            path(std::vector<std::wstring> els) { this->els = els; isDirectory = get(-1).find(L".") == std::wstring::npos; }
            path(std::wstring inPath) {
                if (inPath.length() <= 0) return;
                if (inPath.length() >= 2 && inPath[0] == L'\"' && inPath[inPath.length() - 1] == L'\"') inPath = inPath.substr(1, inPath.length() - 2);

                int i = isSlash(inPath[0]) ? 1 : 0;

                while (i < inPath.length()) {
                    std::wstring el = L"";
                    while (i < inPath.length() && !isSlash(inPath[i])) {
                        el += inPath[i];
                        i++;
                    }
                    if (el.length() > 0) els.push_back(el);

                    i++;
                }
                isDirectory = get(-1).find(L".") == std::wstring::npos;

                // for(int i=0; i<els.size(); i++) prw("el[",i,"]: ", els[i], "\n");
                // prw("is directory: ", isDirectory, "\n");
            }

            std::wstring get(int i) const { if (i < 0) i = els.size() + i; return els[i]; }
            path subStart(std::wstring name) const {
                std::vector<std::wstring> subpath;

                int istart = 0;
                for (int i = 0; i < els.size(); i++) if (els[i].compare(name) == 0) istart = i;
                for (int i = istart; i < els.size(); i++) subpath.push_back(els[i]);

                return path(subpath);
            }
            path subEnd(std::wstring name) const {
                std::vector<std::wstring> subpath;

                for (int i = 0; i < els.size(); i++) { subpath.push_back(els[i]); if (els[i].compare(name) == 0) break; }

                return path(subpath);
            }
            path sub(int start, int end) const {
                if (start < 0) start = els.size() + start;
                if (end < 0) end = els.size() + end;
                if (start > end) { int t = start; start = end; end = t; }
                end = end < els.size() ? end : els.size();

                std::vector<std::wstring> subpath;

                for (int i = start; i < end; i++) { subpath.push_back(els[i]); }

                return subpath;
            }

            std::wstring operator[] (int i) const { return get(i); }

            path operator() (int start, int end) const {
                return sub(start, end);
            }

            path operator[] (std::wstring const& start) const {
                if (start.length() > 0) {
                    wchar_t sym = start[0];
                    if (sym == '<') return subEnd(start.substr(2, start.length() - 2));
                    else if (sym == '>') return subStart(start.substr(2, start.length() - 2));
                    else return subStart(start);
                }
                else return subStart(start);
            }

            std::wstring toStringw() const {
                std::wstring str = L"";
                for (int i = 0; i < els.size(); i++) str += els[i] + L"\\";
                if (!isDirectory) str.erase(str.end() - 1);
                return str;
            }
            std::string toString() const { return tostr(toStringw()); }

            operator std::wstring() const { return toStringw(); }
            operator std::string() const { return toString(); }
        };

        inline std::vector<std::wstring> split(std::wstring const& str, std::wstring const& pattern) {
            std::vector<std::wstring> strs;

            size_t oldOff = 0;
            size_t off = 0;
            while ((off = str.find(pattern, oldOff)) != std::wstring::npos) {
                strs.push_back(str.substr(oldOff, off - oldOff));
                oldOff = off + 1;
            }
            strs.push_back(str.substr(oldOff, str.length()));

            return strs;
        }
	}

#ifndef PARRLIB_NAMESPACE_SHORTEN
	namespace stru = stringutils;
#endif

}
