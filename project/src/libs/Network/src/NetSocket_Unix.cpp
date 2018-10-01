
#ifdef __unix__

#include "OsSocket.hpp"

namespace Engine {
	namespace Network {

		/*

		ServerSocket

		*/

		ServerSocket::ServerSocket(uint16_t port) {

		}

		ServerSocket::~ServerSocket() {

		}

		uint32_t ServerSocket::read(void* data, uint32_t dataSize) {

		}

		void ServerSocket::write(void* data, uint32_t dataSize) {

		}

		/*

		ClientSocket

		*/

		ClientSocket::ClientSocket(IpAddress addr, uint16_t port) {

		}

		ClientSocket::~ClientSocket() {

		}

		uint32_t ServerSocket::read(void* data, uint32_t dataSize) {

		}

		void ServerSocket::write(void* data, uint32_t dataSize) {

		}

	}
}

#endif