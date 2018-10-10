#ifndef OS_SOCKETS_HPP
#define OS_SOCKETS_HPP

#include "Network/NetSocket.hpp"

#ifdef WIN32
#include <WinSock2.h>
#elif __unix__

#endif

namespace Engine {
	namespace Network {


		struct OsSocket {
#ifdef WIN32
			SOCKET socket;
#elif __unix__

#endif
		};

		class ServerSocket : public INetServerSocket {
		public:

			ServerSocket(uint16_t port);
			virtual ~ServerSocket();

			// INetServerSocket
			virtual void listen();
			virtual INetSocket* accept();
			// INetSocket
			virtual eSocketType getSocketType() { return eSocketType::E_SOCKET_SERVER; };
			
			virtual uint32_t read(INetMessage* msg);
			virtual void write(INetMessage* msg);

		private:
			OsSocket listenSocket;
		};

		class ClientSocket : public INetSocket {
		public:
			ClientSocket(IpAddress addr, uint16_t port);
			ClientSocket(OsSocket createdClient);

			virtual ~ClientSocket();

			virtual eSocketType getSocketType() { return eSocketType::E_SOCKET_CLIENT; };

			virtual uint32_t read(INetMessage* msg);
			virtual void write(INetMessage* msg);

		private:
			OsSocket dataSocket;
		};

	}
}
#endif