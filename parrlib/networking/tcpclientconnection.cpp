#include "TCPClientConnection.h"

namespace prb {
	TCPClientConnection::TCPClientConnection() {}

	TCPClientConnection::TCPClientConnection(std::string ip, unsigned short port) {
		if (ip.compare("localhost") == 0) ip = "127.0.0.1";
		this->ip = ip;
		this->port = port;

		if ((conSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) { std::cout << "error TCP: socket: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return; }

		if (inet_pton(AF_INET, ip.c_str(), &server.sin_addr) != 1) { std::cout << "error TCP: inet_pton: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return; }
		server.sin_family = AF_INET;
		server.sin_port = htons(port);

		if (connect(conSocket, (struct sockaddr*)&server, sizeof(server)) < 0) { std::cout << "error TCP: connect: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return; }

		if (conSocket == INVALID_SOCKET) { std::cout << "error: TCP could not connect to server " << ip << ":" << port << "\n"; return; }

		closed = false;
	}

	TCPClientConnection::TCPClientConnection(SOCKET socket) {
		conSocket = socket;

		struct sockaddr_in address = { 0 };
		int addressLength = sizeof(sockaddr_in);

		if (getpeername(socket, (struct sockaddr*)&address, &addressLength) == SOCKET_ERROR) { std::cout << "error: socket getpeername client: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return; }

		WCHAR* str = new WCHAR[20];
		for (int i = 0; i < 20; i++) str[i] = ' ';
		PCWSTR pcw = InetNtopW(AF_INET, &address.sin_addr, str, 20);
		if (pcw == NULL) { std::cout << "error TCP: inetNtop client " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return; }

		std::wstring wstr = std::wstring(str);
		//ip = std::string(wstr.begin(), wstr.end());

		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		ip = converter.to_bytes(wstr);
		port = address.sin_port;

		//std::cout << "client ip converted " << ip << "\n";

		closed = false;
	}

	bool TCPClientConnection::hasError() {
		return errors.size() > 0;
	}

	int TCPClientConnection::getLastError() {
		if (errors.size() <= 0) return -1;
		return errors.back();
	}

	std::string TCPClientConnection::getLastErrorString() {
		if (errors.size() <= 0) return "";
		return WinsockUtil::errorToString(errors.back());
	}

	int TCPClientConnection::popError() {
		int err = errors.back();
		errors.pop_back();
		return err;
	}

	std::string TCPClientConnection::popErrorString() {
		int err = errors.back();
		errors.pop_back();
		return WinsockUtil::errorToString(err);
	}

	bool TCPClientConnection::isValid() {
		return conSocket != INVALID_SOCKET;
	}

	int TCPClientConnection::sendMessage(std::string message) {
		int res = 0;
		if ((res = send(conSocket, message.c_str(), message.length(), 0)) == SOCKET_ERROR) { std::cout << "error TCP sending message" << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); }
		return res;
	}

	std::string TCPClientConnection::receiveMessage() {
		char recvbuf[512];
		for (int i = 0; i < 512; i++) recvbuf[i] = ' ';

		int res = 1;

		res = recv(conSocket, recvbuf, 512, 0);
		if (res > 0) {
			std::cout << "message received, size " << res << "\n";
		}
		else if (res == 0) { std::cout << "error TCP: connection closed\n"; errors.push_back(-1); close(); }
		else { std::cout << "error TCP: recv failed: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); }

		return std::string(recvbuf).substr(0, res);
	}

	unsigned short TCPClientConnection::getLocalPort() {
		struct sockaddr_in sin;
		int addrlen = sizeof(sin);
		if (getsockname(conSocket, (struct sockaddr*)&sin, &addrlen) == SOCKET_ERROR) { std::cout << "error: getsockname: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return 0; }

		return sin.sin_port;
	}

	int TCPClientConnection::shutDown() {
		int res = 0;
		if ((res = shutdown(conSocket, SD_BOTH)) == SOCKET_ERROR) { std::cout << "error TCP shutting down both send and receive socket: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); }
		return res;
	}

	int TCPClientConnection::close() {
		closed = true;

		int res = shutDown();
		if ((res = closesocket(conSocket)) == SOCKET_ERROR) { std::cout << "error TCP closing socket " << res << ": " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); }
		return res;
	}
}
