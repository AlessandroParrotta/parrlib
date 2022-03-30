#pragma once

#include <stdio.h>
#include <io.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <Windows.h>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <condition_variable>

#include "TCPClientConnection.h"

#define synchronized(m) \
    for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())

//#ifndef TCPSERVERCONNECTION_NO_DEFINE_SYNC
//	#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())
//#endif
namespace prb {
	class TCPServer {
	public:
		SOCKET lisSocket = INVALID_SOCKET;

		std::thread serverThread;
		std::unordered_map<unsigned short, TCPClientConnection*> clients;
		std::unordered_map<unsigned short, std::thread*> threads;

		struct sockaddr_in server;

		std::thread threadDeleter;
		std::vector<unsigned short> deleteQueue;

		//int is the port, to identify the connection
		std::function<void(int)> clientFunc;

		unsigned short port = 6789;

		bool closed = true;

		TCPServer();
		TCPServer(unsigned short port, std::function<void(unsigned short, TCPClientConnection*)> clientFunc);

		void close();
	};

}
