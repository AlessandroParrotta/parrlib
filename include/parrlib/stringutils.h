#pragma once

#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>
#include <sstream>

#include "Timer.h"

namespace stringutils {

	std::string reverse(std::string str);
	std::string getExt(std::string file);
	std::string getFileName(std::string const& path);
	std::vector<std::string> splitFilePath(std::string const& path);

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

	int getHexVal(char c);

	std::wstring toWString(std::string const& str);
	std::wstring towstr(std::string const& str);
	std::wstring towstr(float i);
	std::wstring towstr(double i);
	std::wstring towstr(int i);
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


	std::string toString(std::wstring const& wstr);
	std::string tostr(std::wstring const& wstr);
	std::string tostr(float i);
	std::string tostr(double i);
	std::string tostr(int i);

	float tof(std::string const& str);
	double tod(std::string const& str);
	int toi(std::string const& str);

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
}

#ifndef PARRLIB_NAMESPACE_SHORTEN
namespace stru = stringutils;
#endif

