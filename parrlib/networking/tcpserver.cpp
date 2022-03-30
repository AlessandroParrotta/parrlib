#include "TCPServer.h"

namespace prb {
	std::recursive_mutex tcpm;

	std::mutex tcpcvmtx;
	std::condition_variable tcpcv;

	TCPServer::TCPServer() {}

	void spawnClient(TCPServer* serv, std::reference_wrapper<SOCKET> socket) {
		std::cout << "spawning client\n";



		std::cout << "spawned client\n";
	}

	TCPServer::TCPServer(unsigned short port, std::function<void(unsigned short, TCPClientConnection*)> clientFunc) {
		this->port = port;

		//struct addrinfo hints;

		//ZeroMemory(&hints, sizeof(hints));
		//hints.ai_family = AF_UNSPEC;
		//hints.ai_socktype = SOCK_STREAM;
		//hints.ai_protocol = IPPROTO_TCP;

		//struct addrinfo* result;

		//std::stringstream ss;
		//ss << port;
		//std::string sport = ss.str();
		//int res = 0;
		//if (res = getaddrinfo(NULL, sport.c_str(), &hints, &result)) { std::cout << "error: getaddrinfo " << WSAGetLastError() << "\n"; return; }

		//lisSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		lisSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (lisSocket == INVALID_SOCKET) { std::cout << "error: socket failed: " << WSAGetLastError() << "\n"; return; }

		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(port);

		//if ((res = bind(lisSocket, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR) { std::cout << "bind failed: " << WSAGetLastError() << "\n"; closesocket(lisSocket); return; }
		if (bind(lisSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) { std::cout << "bind failed: " << WSAGetLastError() << "\n"; closesocket(lisSocket); return; }

		//freeaddrinfo(result);

		closed = false;

		threadDeleter = std::thread([this] {
			std::cout << "deleter started\n";

			std::unique_lock<std::mutex> lck(tcpcvmtx);
			while (!closed) {
				std::cout << "deleter waiting\n";
				tcpcv.wait(lck);
				std::cout << "notified\n";
				synchronized(tcpm) {
					while (deleteQueue.size() > 0) {
						if (threads.find(deleteQueue.back()) != threads.end()) {
							if (threads[deleteQueue.back()]->joinable()) threads[deleteQueue.back()]->join();
							delete threads[deleteQueue.back()];
							threads.erase(deleteQueue.back());
							std::cout << "thread " << deleteQueue.back() << " deleted \n";
						}

						deleteQueue.pop_back();
					}
				}
			}

			std::cout << "END\n";
			});

		serverThread = std::thread([this, clientFunc] {
			while (!closed && lisSocket != INVALID_SOCKET) {
				std::cout << "waiting for clients...\n";
				if (listen(lisSocket, SOMAXCONN) == SOCKET_ERROR) { std::cout << "error: listening " << WSAGetLastError() << "\n"; closesocket(lisSocket); return; }

				struct sockaddr_in address = { 0 };
				int addressLength = sizeof(sockaddr_in);

				SOCKET clientSocket = INVALID_SOCKET;
				clientSocket = accept(lisSocket, (struct sockaddr*)&address, &addressLength);
				if (clientSocket == INVALID_SOCKET) { std::cout << "error: accept: " << WSAGetLastError() << "\n"; closesocket(lisSocket); continue; }

				WCHAR* str = new WCHAR[20];
				for (int i = 0; i < 20; i++) str[i] = L' ';
				if ((InetNtopW(AF_INET, &address.sin_addr, str, 20)) == NULL) { std::cout << "error: inetNtop " << WSAGetLastError() << "\n"; continue; }

				std::wcout << "client accepted (" << str << ", port " << address.sin_port << ")\n";

				clients[address.sin_port] = new TCPClientConnection(clientSocket);
				threads[address.sin_port] = new std::thread([clientFunc, address, this] {
					clientFunc(address.sin_port, clients[address.sin_port]);

					synchronized(tcpm) {
						if (!clients[address.sin_port]->closed) clients[address.sin_port]->close();
						if (clients[address.sin_port]) delete clients[address.sin_port];

						if (clients.find(address.sin_port) != clients.end()) clients.erase(address.sin_port);
						std::cout << "deleted client on port " << address.sin_port << "\n";

						deleteQueue.push_back(address.sin_port);
						tcpcv.notify_all();
					}
					});
			}
			});
	}

	void TCPServer::close() {
		closed = true;

		if (threadDeleter.joinable()) threadDeleter.join();
		if (serverThread.joinable()) serverThread.join();

		for (int i = 0; i < clients.size(); i++) {
			clients[i]->close();
			if (clients[i]) delete clients[i];
		}
		for (int i = 0; i < threads.size(); i++) {
			if (threads[i]->joinable()) threads[i]->join();
			if (threads[i]) delete threads[i];
		}

		clients.clear();
		threads.clear();

		if (lisSocket != INVALID_SOCKET) closesocket(lisSocket);
	}
}
