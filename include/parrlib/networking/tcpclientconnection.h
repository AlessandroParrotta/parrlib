#pragma once

#include <iostream>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <locale>
#include <codecvt>
#include <vector>

#include "WinsockUtil.h"

class TCPClientConnection{
	public:
		std::string ip;
		unsigned short port;

		SOCKET conSocket = INVALID_SOCKET;
		struct sockaddr_in server;

		bool closed = true;

		std::vector<int> errors;

		TCPClientConnection();
		TCPClientConnection(std::string ip, unsigned short port);
		TCPClientConnection(SOCKET socket);

		bool hasError();
		int getLastError();
		std::string getLastErrorString();
		int popError();
		std::string popErrorString();

		bool isValid();

		int sendMessage(std::string message);
		std::string receiveMessage();

		unsigned short getLocalPort();

		int shutDown();
		int close();
};

