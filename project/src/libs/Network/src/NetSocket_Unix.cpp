
#ifdef __unix__

#include "OsSocket.hpp"

namespace Engine {
	namespace Network {

		/*

		ServerSocket

		*/

		ServerSocket::ServerSocket(uint16_t port) {
			port = 0;
		}

		ServerSocket::~ServerSocket() {

		}

		void ServerSocket::listen() {
			
		}

		INetSocket* ServerSocket::accept() {

			ClientSocket* cliSock = nullptr;

			return cliSock;
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
			addr = { 0,0,0,0 };
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
			}

			return socketInitialized;
		}

		void cleanupSocketLib() {
			if (true == socketInitialized) {

				socketInitialized = false;
			}

		}

	}
}

#endif