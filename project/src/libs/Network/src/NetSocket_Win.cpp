#ifdef WIN32

#include "OsSocket.hpp"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <cstdio>

namespace Engine {
	namespace Network {

		
		/*
		
		ServerSocket

		*/

		ServerSocket::ServerSocket(uint16_t port) {
			port = 0;

			listenSocket.socket = INVALID_SOCKET;

			struct addrinfo *result = NULL, hints;

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			CHAR portVal[10];
			
			sprintf_s(portVal, "%d", port);

			// Resolve the local address and port to be used by the server
			int iResult = getaddrinfo(NULL, portVal, &hints, &result);
			if (iResult != 0) {
				printf("getaddrinfo failed: %d\n", iResult);
			}

			listenSocket.socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			if (listenSocket.socket == INVALID_SOCKET) {
				printf("Error at socket(): %ld\n", WSAGetLastError());
				freeaddrinfo(result);
			}

			iResult = bind(listenSocket.socket, result->ai_addr, (int)result->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				printf("bind failed with error: %d\n", WSAGetLastError());
				freeaddrinfo(result);
				closesocket(listenSocket.socket);
			}

			freeaddrinfo(result);

		}

		ServerSocket::~ServerSocket() {

		}

		void ServerSocket::listen() {

			if (::listen(listenSocket.socket, SOMAXCONN) == SOCKET_ERROR) {
				printf("Listen failed with error: %ld\n", WSAGetLastError());
				closesocket(listenSocket.socket);
			}

		}

		INetSocket* ServerSocket::accept() {

			SOCKET sock;

			sock = INVALID_SOCKET;

			// Accept a client socket
			sock = ::accept(listenSocket.socket, NULL, NULL);
			if (sock == INVALID_SOCKET) {
				printf("accept failed: %d\n", WSAGetLastError());
				closesocket(listenSocket.socket);
			}

			return nullptr;
		}

		uint32_t ServerSocket::read(INetMessage* msg) {
			msg = 0;
			return 0;
		}

		void ServerSocket::write(INetMessage* msg) {
			msg = 0;
		}

		/*

		ClientSocket

		*/

		ClientSocket::ClientSocket(IpAddress addr, uint16_t port) {
			addr = { 0, 0,0,0 };
			port = 0;
		}

		ClientSocket::~ClientSocket() {

		}

		uint32_t ClientSocket::read(INetMessage* msg) {
			msg = 0;
			return 0;
		}

		void ClientSocket::write(INetMessage* msg) {
			msg = 0;
		}
		
		/*
		Variables 
		*/
		WSADATA wsaData = { 0 };
		bool socketInitialized = false;

		/*
		Global Functions
		*/

		INetServerSocket* createServerSocket(uint16_t port) {
			return new ServerSocket(port);
		}

		INetSocket* createClientSocket(IpAddress addr, uint16_t port) {
			return new ClientSocket(addr, port);
		}


		

		bool initSocketLib() {
			if (false == socketInitialized) {
				int iResult;

				// Initialize Winsock
				iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
				if (iResult != 0) {
					printf("WSAStartup failed: %d\n", iResult);
				} else {
					socketInitialized = true;
				}
			}

			return socketInitialized;
		}

		void cleanupSocketLib() {
			if (true == socketInitialized) {

				socketInitialized = false;
				WSACleanup();
			}

		}

	}
}

#endif