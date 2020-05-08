#include "stringutils.h"

namespace stringutils {
	

	std::string reverse(std::string str) {
		std::string res = str;
		for (int i = 0; i < str.length(); i++) {
			res[i] = str[str.length() - 1 - i];
		}
		return res;
	}

	std::string getExt(std::string file) {
		std::string inv = reverse(file);
		int point = inv.find('.');
		return file.substr(file.length() - point, file.length() - (file.length() - point));
	}

	bool foundSlash(std::string const& s) { return s.find('/') != std::string::npos || s.find('\\') != std::string::npos; }

	std::string getFileName(std::string const& path) {
		if (!foundSlash(path)) return path;

		size_t idx = path.find_last_of("/\\"); //find last either '/' or '\'
		return path.substr(idx+1);
	}

	std::vector<std::string> splitFilePath(std::string const& path){
		std::vector<std::string> res;

		std::string fileName = path;

		size_t idx = fileName.find_last_of("/\\");
		if (idx != std::string::npos) {
			res.push_back(fileName.substr(0, idx+1));
			fileName = fileName.substr(idx + 1);
		}
		else res.push_back("");
		
		idx = fileName.find_last_of(".");
		if (idx != std::string::npos) {
			res.push_back(fileName.substr(0, idx));
			res.push_back(fileName.substr(idx+1));
		}
		else {
			res.push_back(fileName);
			res.push_back("");
		}

		return res;
	}


	std::string fallbackPath(std::string const& path) {
		std::string res = "";

		std::ifstream f(path);
		if (f.good()) { f.close(); return path; }
		if (!f.good()) {
			for (int i = 0; i < 5 && !f.good(); i++) {
				//std::string fname = stru::getFileName(path);

				std::string backStr = "../";
				for (int j = 0; j < i; j++) backStr += "../";

				std::string fpath = backStr + "parrlibassets/" + path;
				f = std::ifstream(fpath, std::ios::in);
				if (f.good()) res = fpath;
			}
		}

		if(f.good()) f.close();
		return res;
	}


	std::string cutDecimals(std::string number, int decimals) {
		int dotIdx = number.find('.');
		if (dotIdx != std::string::npos) {
			bool allZero = true;
			if (decimals > 0) {
				for (int i = dotIdx + 1; i < number.length() && allZero; i++) {
					allZero = number[i] == '0';
				}

				if (!allZero) {
					number = number.substr(0, dotIdx + 1 + decimals);
				}
			}
			
			if(decimals <= 0 || allZero){
				number = number.substr(0, dotIdx);
			}
		}

		return number;
	}

	std::string getByteSizeFormatted(long long numBytes, int decimals) {
		int type = 0;
		double bytesCalc = (double)numBytes;
		double finalBytes = bytesCalc;
		while (bytesCalc > 1.f) {
			finalBytes = bytesCalc;
			bytesCalc /= (double)1e03;
			type++;
		}
		type--;

		std::string finalString = cutDecimals(tostr(finalBytes), decimals);

		switch (type) {
		case 1: finalString += 'K'; break;
		case 2: finalString += 'M'; break;
		case 3: finalString += 'G'; break;
		case 4: finalString += 'T'; break;
		case 5: finalString += 'P'; break;
		case 6: finalString += 'E'; break;
		case 7: finalString += 'Z'; break;
		case 8: finalString += 'Y'; break;
		case 9: finalString += 'B'; break;
		}

		finalString += "B";

		return finalString;
	}

	std::string getTimeFormatted(double inputTime, int decimals, TimeScale inputTimeScale) {
		bool smallerThanOne = inputTime<1.f;
		int normalizedScale = inputTimeScale;
		//std::cout << inputTime << " " << normalizedScale << "\n";

		double prevInputTime = inputTime;
		while (smallerThanOne && normalizedScale > 0 && inputTime < 1.) {
			prevInputTime = inputTime;
			inputTime *= 1e03;
			normalizedScale--;
			//std::cout << inputTime << " " << normalizedScale << "\n";
		}
		/*if (smallerThanOne) {
			normalizedScale++;
		}*/

		while (!smallerThanOne && normalizedScale <= 3 && inputTime > 1.) {
			prevInputTime = inputTime;
			inputTime /= 1e03;
			normalizedScale++;
			//std::cout << inputTime << " " << normalizedScale << "\n";
		}
		if (!smallerThanOne) {
			inputTime = prevInputTime;
			normalizedScale--;
		}

		std::string finalString = cutDecimals(std::to_string(inputTime), decimals);

		switch (normalizedScale) {
		case 0: finalString += "ns"; break;
		case 1: finalString += "us"; break;
		case 2: finalString += "ms"; break;
		case 3: finalString += "s"; break;
		}

		return finalString;
	}

	std::string getTimeFormattedNano(double inputTime, int decimals) {
		return getTimeFormatted(inputTime, decimals, TimeScale::NANO);
	}

	std::string getTimeFormattedMicro(double inputTime, int decimals) {
		return getTimeFormatted(inputTime, decimals, TimeScale::MICRO);
	}

	std::string getTimeFormattedMilli(double inputTime, int decimals) {
		return getTimeFormatted(inputTime, decimals, TimeScale::MILLI);
	}

	std::string getTimeFormattedSec(double inputTime, int decimals) {
		return getTimeFormatted(inputTime,decimals, TimeScale::SEC);
	}

	int getHexVal(char c) {
		switch (c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'A': case 'a': return 10;
		case 'B': case 'b':return 11;
		case 'C': case 'c':return 12;
		case 'D': case 'd':return 13;
		case 'E': case 'e':return 14;
		case 'F': case 'f':return 15;
		}

		return -1;
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> widec;
	std::wstring toWString(std::string const& str) {
		std::wstring wide = widec.from_bytes(str);
		return wide;
	}
	std::wstring towstr(std::string const& str) { return toWString(str); }

	std::wstringstream wss;
	std::wstring towstr(float f) {
		wss << f;
		std::wstring res;
		wss >> res;
		wss.clear(); wss.str(L"");
		return res;
	}
	std::wstring towstr(double d) {
		wss << d;
		std::wstring res;
		wss >> res;
		wss.clear(); wss.str(L"");
		return res;
	}
	std::wstring towstr(int i) {
		wss << i;
		std::wstring res;
		wss >> res;
		wss.clear(); wss.str(L"");
		return res;
	}
	float tof(std::wstring const& wstr) {
		wss << wstr;
		float f;
		wss >> f;
		wss.clear(); wss.str(L"");
		return f;
	}
	double tod(std::wstring const& wstr) {
		wss << wstr;
		double d;
		wss >> d;
		wss.clear(); wss.str(L"");
		return d;
	}
	int toi(std::wstring const& wstr) {
		wss << wstr;
		float i;
		wss >> i;
		wss.clear(); wss.str(L"");
		return i;
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> narrowc;
	std::string toString(std::wstring const& wstr) {
		std::string narrow = narrowc.to_bytes(wstr);
		return narrow;
	}
	std::string tostr(std::wstring const& wstr) { return toString(wstr); }

	std::stringstream ss;
	std::string tostr(float f) {
		ss << f;
		std::string res;
		ss >> res;
		ss.clear(); ss.str("");
		return res;
	}
	std::string tostr(double d) {
		ss << d;
		std::string res;
		ss >> res;
		ss.clear(); ss.str("");
		return res;
	}
	std::string tostr(int i) {
		ss << i;
		std::string res;
		ss >> res;
		ss.clear(); ss.str("");
		return res;
	}

	float tof(std::string const& str) {
		ss << str;
		float f; ss >> f;
		return f;
	}
	double tod(std::string const& str) {
		ss << str;
		double d; ss >> d;
		return d;
	}
	int toi(std::string const& str) {
		ss << str;
		int i; ss >> i;
		return i;
	}

	std::vector<std::wstring> toLines(std::wstring const& str) {
		std::vector<std::wstring> res;

		int lastLine = 0;
		for (int i = 0; i < str.length() && lastLine < str.length(); i++) {
			if (str[i] == L'\n') { res.push_back(str.substr(lastLine, i - lastLine)); lastLine = i + 1; }
		}
		res.push_back(str.substr(lastLine, str.length() - lastLine));

		return res;
	}
	
	std::vector<std::string> toLines(std::string const& str) {
		std::vector<std::string> res;

		int lastLine = 0;
		for (int i = 0; i < str.length() && lastLine < str.length(); i++) {
			if (str[i] == L'\n') { res.push_back(str.substr(lastLine, i - lastLine)); lastLine = i + 1; }
		}
		res.push_back(str.substr(lastLine, str.length() - lastLine));

		return res;
	}
}
