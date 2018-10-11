
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

	}
}

#endif