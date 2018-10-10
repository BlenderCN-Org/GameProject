
#include <Network/NetSocket.hpp>

#include <cstdio>

int main() {

	if (!Engine::Network::initSocketLib()) {
		printf("Socket initialization failed!\n");
	}

	Engine::Network::INetServerSocket* server = Engine::Network::createServerSocket(6116);

	server->listen();
	Engine::Network::INetSocket* client = server->accept();

	client->write(nullptr);

	printf("Hello There!\n");

	Engine::Network::cleanupSocketLib();

	return 0;
}