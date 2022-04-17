#include "stringutils.h"

#include <filesystem>

#include "otherutil.h"
//#include "../debug.h"


std::wostream& operator<<(std::wostream& wstr, const char* str) {
	wstr << prb::stru::towstr(str);
	return wstr;
}

std::wostream& operator<<(std::wostream& wstr, std::string const& str) {
	wstr << str.c_str();
	return wstr;
}

std::wostream& operator<<(std::wostream& wstr, std::string& str) {
	wstr << str.c_str();
	return wstr;
}


namespace prb {
	namespace stringutils {

		//std::wstringstream composewss;
		//std::wstring compose(std::vector<std::any> objs) {
		//	for (int i = 0; i < objs.size(); i++) {
		//		auto val = objs[i];
		//		composewss << std::any_cast<float>(val);
		//	}

		//	std::wstring res = composewss.str();
		//	composewss.clear(); composewss.str(L"");
		//	return res;
		//}

		std::wstring reversew(std::wstring str) {
			std::wstring res = str;
			for (int i = 0; i < str.length(); i++) {
				res[i] = str[str.length() - 1 - i];
			}
			return res;
		}

		std::wstring getFolderw(std::wstring const& path) {
			if (path.find('.') == std::wstring::npos || path.find('\\') == std::wstring::npos) return path;

			std::wstring rev = reversew(path);
			return reversew(rev.substr(rev.find('\\'), rev.length()));
		}

		std::wstring getExtw(std::wstring file) {
			std::wstring inv = reversew(file);
			int point = inv.find('.');
			return file.substr(file.length() - point, file.length() - (file.length() - point));
		}

		bool foundSlashw(std::wstring const& s) { return s.find('/') != std::wstring::npos || s.find('\\') != std::wstring::npos; }

		std::wstring getFileNamew(std::wstring const& path) {
			if (!foundSlashw(path)) return path;

			size_t idx = path.find_last_of(L"/\\"); //find last either '/' or '\'
			return path.substr(idx + 1);
		}

		std::vector<std::wstring> splitFilePathw(std::wstring const& path) {
			std::vector<std::wstring> res;

			std::wstring fileName = path;

			size_t idx = fileName.find_last_of(L"/\\");
			if (idx != std::wstring::npos) {
				res.push_back(fileName.substr(0, idx + 1));
				fileName = fileName.substr(idx + 1);
			}
			else res.push_back(L"");

			idx = fileName.find_last_of(L".");
			if (idx != std::wstring::npos) {
				res.push_back(fileName.substr(0, idx));
				res.push_back(fileName.substr(idx + 1));
			}
			else {
				res.push_back(fileName);
				res.push_back(L"");
			}

			return res;
		}


		std::string reverse(std::string str)							{ return tostr(reversew(towstr(str))); }
		std::string getExt(std::string file)							{ return tostr(getExtw(towstr(file))); }
		std::string getFileName(std::string const& path)				{ return tostr(getFileNamew(towstr(path))); }
		std::string getFolder(std::string const& path)					{ return tostr(getFolderw(towstr(path))); }
		std::vector<std::string> splitFilePath(std::string const& path) { std::vector<std::wstring> resw = splitFilePathw(towstr(path)); std::vector<std::string> res; for (int i = 0; i < resw.size(); i++) res.push_back(tostr(resw[i])); return res; }

		bool isAbsolute(std::string const& str) {
			std::filesystem::path path(str); return path.is_absolute();
		}

		std::string correctExeLocIfAbsolute(std::string const& str) {
			if (isAbsolute(str))	return str;
			else					return outl::getExeFolder() + str;
		}
		
		std::wstring fallbackPath(std::wstring const& prefix, std::wstring const& path) {
			std::wstring res = L"";

			std::ifstream f(prefix + path);
			if (f.good()) { f.close(); return prefix + path; }
			if (!f.good()) {
				//deb::outStrw( L"tested: " + prefix + path + L" and couldn't find file\n");
				for (int i = 0; i < 5 && !f.good(); i++) {
					//std::string fname = stru::getFileName(path);

					std::wstring backStr = L"../";
					for (int j = 0; j < i; j++) backStr += L"../";

					std::wstring fpath = backStr + L"parrlibassets/" + path;
					f = std::ifstream(prefix + fpath, std::ios::in);
					if (f.good()) res = fpath;

					//deb::outStrw(prefix + fpath + L" (" + path + L")" + L"\n");
				}
			}

			//deb::outStrw(L"res: " + prefix + res + L" (" + path + L")" + L"\n");

			if (f.good()) f.close();
			else res = path;

			return prefix + res;
		}
		std::wstring fallbackPath(std::wstring const& path) { return fallbackPath(L"", path); }

		std::string fallbackPath(std::string const& prefix, std::string const& path) { return tostr(fallbackPath(towstr(prefix), towstr(path))); }
		std::string fallbackPath(std::string const& path) { return fallbackPath("", path); }

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

				if (decimals <= 0 || allZero) {
					number = number.substr(0, dotIdx);
				}
			}

			return number;
		}

		std::string byteStr(long long numBytes, int decimals) {
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

		namespace ts {
			std::string fromTime(double inputTime, int decimals, timeScale inputTimeScale) {
				bool smallerThanOne = inputTime < 1.f;
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

			std::string fromNano(double inputTime, int decimals) {
				return fromTime(inputTime, decimals, timeScale::NANO);
			}

			std::string fromMicro(double inputTime, int decimals) {
				return fromTime(inputTime, decimals, timeScale::MICRO);
			}

			std::string fromMilli(double inputTime, int decimals) {
				return fromTime(inputTime, decimals, timeScale::MILLI);
			}

			std::string fromSec(double inputTime, int decimals) {
				return fromTime(inputTime, decimals, timeScale::SEC);
			}

			std::string to_time_digit(int t) {
				if (t < 10)  return "0" + std::to_string(t);
				else        return std::to_string(t);
			}

			std::string toTs(unsigned long long sec) {
				if (sec == 0) return "0:00";

				unsigned long long tsec = sec % 60;
				unsigned long long tmins = (sec / 60) % 60;
				unsigned long long thours = (sec / 60 / 60) % 24;
				unsigned long long tdays = (sec / 60 / 60 / 24) % 365;
				unsigned long long tyears = sec / 60 / 60 / 24 / 365;

				std::vector<unsigned long long> iar = { tsec, tmins, thours, tdays, tyears };

				std::stringstream ss;
				bool started = false;
				for (int i = iar.size() - 1; i >= 0; i--) {
					if (!started) {
						if (iar[i] > 0) {
							ss << (i == 0 ? ((iar[i] < 10ULL ? "0:0" : "0:")) : "")
								<< iar[i] << (i == 0 ? "" : ":");
							started = true;
						}
					}
					else ss << to_time_digit(iar[i]) << (i == 0 ? "" : ":");
				}
				return ss.str();
			}
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
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring towstr(const char* str) {
			return converter.from_bytes(str); 
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

		//std::wstringstream composeWss;

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

		float getf(std::stringstream& ss) {
			std::string str; ss >> str;
			return atof(str.c_str());
		}
		double getd(std::stringstream& ss) {
			std::string str; ss >> str;
			return std::stod(str.c_str());
		}
		int geti(std::stringstream& ss) {
			std::string str; ss >> str;
			return atoi(str.c_str());
		}

		float getf(std::ifstream& f) {
			std::string str; f >> str;
			return atof(str.c_str());
		}
		double getd(std::ifstream& f) {
			std::string str; f >> str;
			return std::stod(str.c_str());
		}
		int geti(std::ifstream& f) {
			std::string str; f >> str;
			return atoi(str.c_str());
		}

		std::vector<std::wstring> toLines(std::wstring const& str) {
			std::vector<std::wstring> res;

			int lastLine = 0;
			for (int i = 0; i < str.length() && lastLine < str.length(); i++) {
				if (str[i] == L'\n') { res.push_back(str.substr(lastLine, i - lastLine)); lastLine = i + 1; }
			}
			res.push_back(str.substr(lastLine, str.length() - lastLine));

			//str = stru::replace(str, L"\t", L"    ");

			//std::vector<std::wstring> lines = stru::split(str, L"\n");
			//if (strs.size() <= 0) strs.push_back(L""); if (lines.size() > 0) strs.back() += lines[0];
			//for (int i = 1; i < lines.size(); i++) strs.push_back(lines[i]);

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
}
