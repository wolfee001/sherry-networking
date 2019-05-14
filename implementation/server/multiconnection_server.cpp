#include "../../interface/multiconnection_server.h"
#include "concrete_multiconnection_server.h"

namespace sherry::networking
{
	MulticonnectionServer::MulticonnectionServer(std::string_view host, uint16_t port)
	{
		mConcreteServer = std::make_unique<ConcreteMulticonnectionServer>(host, port);
	}

	void MulticonnectionServer::WaitForConnections(uint8_t count)
	{
		mConcreteServer->WaitForConnections(count);
	}

	bool MulticonnectionServer::SendToConnection(uint8_t connection, std::string_view message)
	{
		return mConcreteServer->SendToConnection(connection, message);
	}

	std::string MulticonnectionServer::ReadFromConnection(uint8_t connection, uint16_t length/* = std::numeric_limits<uint16_t>::max()*/)
	{
		return mConcreteServer->ReadFromConnection(connection, length);
	}

	bool MulticonnectionServer::IsConnectionValid(uint8_t connection)
	{
		return mConcreteServer->IsConnectionValid(connection);
	}

	MulticonnectionServer::~MulticonnectionServer()
	{
		mConcreteServer.reset();
	}
}
