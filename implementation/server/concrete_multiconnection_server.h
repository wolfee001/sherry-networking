#pragma once

#include <memory>
#include <vector>
#include <string>

#include <clsocket/PassiveSocket.h>
#include <clsocket/ActiveSocket.h>

namespace sherry::networking
{
	class ConcreteMulticonnectionServer
	{
	public:
		ConcreteMulticonnectionServer(std::string_view host, uint16_t port);

		void WaitForConnections(uint8_t count);
		bool SendToConnection(uint8_t connection, std::string_view message);
		std::string ReadFromConnection(uint8_t connection, uint16_t length);
		bool IsConnectionValid(uint8_t connection);
		~ConcreteMulticonnectionServer();

	private:
		CPassiveSocket mServerSocket;
		std::vector<std::unique_ptr<CActiveSocket>> mClientSockets;
	};
}
