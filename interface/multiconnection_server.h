#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <memory>

namespace sherry::networking
{
	class ConcreteMulticonnectionServer;

	class MulticonnectionServer final
	{
	public:
		MulticonnectionServer(std::string_view host, uint16_t port);
		void WaitForConnections(uint8_t count);
		bool SendToConnection(uint8_t connection, std::string_view message);
		std::string ReadFromConnection(uint8_t connection, uint16_t length = UINT16_MAX);
		bool IsConnectionValid(uint8_t connection);
		~MulticonnectionServer();

	private:
		std::unique_ptr<ConcreteMulticonnectionServer> mConcreteServer;
	};
}
