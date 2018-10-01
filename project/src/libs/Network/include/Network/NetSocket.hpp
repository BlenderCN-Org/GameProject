#ifndef NET_SOCKET_HPP
#define NET_SOCKET_HPP

#include <cstdint>

namespace Engine {
	namespace Network {

		enum class eSocketType {
			E_SOCKET_SERVER,
			E_SOCKET_CLIENT,
		};

		struct IpAddress {
			uint8_t addr[4];
		};

		class INetMessage {
		public:
			virtual ~INetMessage() {};

			virtual void* getData() = 0;
			virtual uint32_t getDataSize() = 0;

		};

		class INetSocket {

		public:

			virtual ~INetSocket() {};

			virtual eSocketType getSocketType() = 0;

			virtual uint32_t read(INetMessage* msg) = 0;
			virtual void write(INetMessage* msg) = 0;

		};

		class INetServerSocket : public INetSocket {

		public:

			virtual void listen() = 0;
			virtual INetSocket* accept() = 0;
			// INetSocket
			virtual eSocketType getSocketType() = 0;

			virtual uint32_t read(INetMessage* msg) = 0;
			virtual void write(INetMessage* msg) = 0;


		};

		INetServerSocket* createServerSocket(uint16_t port);
		INetSocket* createClientSocket(IpAddress addr, uint16_t port);

		bool initSocketLib();
		void cleanupSocketLib();

	};
}

#endif