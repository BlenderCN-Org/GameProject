
#include <Network/NetSocket.hpp>

#include <cstdio>
#include <cstring>

class HelloMessage : public Engine::Network::INetMessage {

	virtual void* getData() {
		return "Hello world!";
	}

	virtual uint32_t getDataSize() {
		return sizeof("Hello world!");
	}

};

class Response : public Engine::Network::INetMessage {

public:

	Response(uint32_t maxSize) {
		buf = new char[maxSize];
		memset(buf, 0, maxSize);
		bufSize = maxSize;
	}

	virtual ~Response() {

	}

	virtual void* getData() {
		return buf;
	}

	virtual uint32_t getDataSize() {
		return bufSize;
	}

private:
	char* buf;
	uint32_t bufSize;
};

int main() {

	if (!Engine::Network::initSocketLib()) {
		printf("Socket initialization failed!\n");
	}

	Engine::Network::INetServerSocket* server = Engine::Network::createServerSocket(6116);

	printf("Listening!\n");
	server->listen();

	printf("Waiting for connection!\n");
	Engine::Network::INetSocket* client = server->accept();

	HelloMessage hello;

	Response rsp(20);

	client->write(&hello);
	printf("Data sent!\n");

	client->read(&rsp);

	printf("Data recived! %s\n", static_cast<char*>(rsp.getData()) );

	Engine::Network::cleanupSocketLib();

	return 0;
}