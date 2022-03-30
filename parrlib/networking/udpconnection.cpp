#include "UDPConnection.h"

namespace prb {
	UDPConnection::UDPConnection() {}

	UDPConnection::UDPConnection(std::string ip, unsigned short port) {
		if (ip.compare("localhost") == 0) ip = "127.0.0.1";
		this->ip = ip;
		this->port = port;
		isServer = ip.compare("") == 0;

		if ((s = socket(AF_INET, SOCK_DGRAM, isServer ? 0 : IPPROTO_UDP)) == INVALID_SOCKET) { std::cout << "error UDP: init socket " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return; }

		memset((char*)&saddr, 0, sizeof(saddr));
		if (!isServer) {
			if (inet_pton(AF_INET, ip.c_str(), &saddr.sin_addr) != 1) { std::cout << "error UDP: inet_pton: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; return; }
		}
		else saddr.sin_addr.S_un.S_addr = INADDR_ANY;
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		slen = sizeof(saddr);

		if (isServer) if (bind(s, (struct sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR) { std::cout << "error UDP: bind " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); closesocket(s); return; }

		closed = false;
	}

	UDPConnection::UDPConnection(unsigned short port) : UDPConnection("", port) {}


	int UDPConnection::sendMessage(std::string message) {
#ifndef UDPCONNECTION_SUPPRESS_SERVER_SENDMESSAGE_BLIND
		if (isServer) std::cout << "warning UDP: sending message blindly through server UDPConnection\n";
#endif
		if (sendto(s, message.c_str(), message.length(), 0, (struct sockaddr*)&saddr, slen) == SOCKET_ERROR) { std::cout << "error UDP: send message: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return WSAGetLastError(); }

		return 0;
	}

	int UDPConnection::sendMessage(std::string ip, unsigned short port, std::string message) {
		int res = 0;

		struct sockaddr_in other;
		res = InetPtonA(AF_INET, ip.c_str(), &other.sin_addr);
		other.sin_family = AF_INET;
		other.sin_port = port;

		int slen = sizeof(other);
		if ((res = sendto(s, message.c_str(), message.length(), 0, (struct sockaddr*)&other, slen)) == SOCKET_ERROR) { std::cout << "error: sendto UDP: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return res; }

		return res;
	}

	UDPConnection::Packet UDPConnection::receivePacket() {
		char buf[512];
		//memset(&buf, (int)(' '), 512);
		for (int i = 0; i < 512; i++) buf[i] = ' ';

		struct sockaddr_in saddr;
		int slen = sizeof(saddr);

		int res = recvfrom(s, buf, 512, 0, (struct sockaddr*)&saddr, &slen);
		if (res == 0) { std::cout << "error UDP: connection closed\n"; errors.push_back(-1); return Packet(); }
		else if (res == SOCKET_ERROR) { std::cout << "error UDP recvfrom: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return Packet(); }

		return Packet(std::string(buf).substr(0, res), saddr, slen);
	}

	std::string UDPConnection::receiveMessage() {
		return receivePacket().message;
	}

	bool UDPConnection::hasError() {
		return errors.size() > 0;
	}

	int UDPConnection::getLastError() {
		if (errors.size() <= 0) return -1;
		return errors.back();
	}

	std::string UDPConnection::getLastErrorString() {
		if (errors.size() <= 0) return "";
		return WinsockUtil::errorToString(errors.back());
	}

	int UDPConnection::popError() {
		int err = errors.back();
		errors.pop_back();
		return err;
	}

	std::string UDPConnection::popErrorString() {
		int err = errors.back();
		errors.pop_back();
		return WinsockUtil::errorToString(err);
	}

	int UDPConnection::close() {
		closed = true;
		if (closesocket(s) == SOCKET_ERROR) { std::cout << "error: closesocket UDP: " << WSAGetLastError() << " " << WinsockUtil::errorToString(WSAGetLastError()) << "\n"; errors.push_back(WSAGetLastError()); return SOCKET_ERROR; }
		return 0;
	}
}
