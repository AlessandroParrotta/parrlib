#pragma once

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

#include "WinsockUtil.h"

namespace prb {
	class UDPConnection {
	public:

		std::string ip;
		unsigned short port;

		SOCKET s;

		struct sockaddr_in saddr;
		int slen;

		std::vector<int> errors;

		bool isServer = false;
		bool closed = true;

		UDPConnection();

		UDPConnection(std::string ip, unsigned short port);
		UDPConnection(unsigned short port);

		int sendMessage(std::string message);
		int sendMessage(std::string ip, unsigned short port, std::string message);

		class Packet {
		public:
			bool isNull = true;
			std::string message;
			struct sockaddr_in saddr;
			int slen;

			std::string ip;
			unsigned short port;

			Packet() {}
			Packet(std::string message, struct sockaddr_in saddr, int slen) {
				isNull = false;
				this->message = message;
				this->saddr = saddr;
				this->slen = slen;

				WCHAR* str = new WCHAR[20];
				for (int i = 0; i < 20; i++) str[i] = L' ';
				PCWSTR pcw = InetNtopW(AF_INET, &saddr.sin_addr, str, 20);
				if (pcw == NULL) { std::cout << "error: inetNtop client " << WSAGetLastError() << "\n"; return; }

				std::wstring wstr = std::wstring(str);
				//ip = std::string(wstr.begin(), wstr.end());

				using convert_type = std::codecvt_utf8<wchar_t>;
				std::wstring_convert<convert_type, wchar_t> converter;

				ip = converter.to_bytes(wstr);
				port = saddr.sin_port;
			}
		};
		Packet receivePacket();
		std::string receiveMessage();

		bool hasError();
		int getLastError();
		std::string getLastErrorString();
		int popError();
		std::string popErrorString();

		int close();
	};
}
