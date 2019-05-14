#include "concrete_multiconnection_server.h"

namespace sherry::networking
{
	ConcreteMulticonnectionServer::ConcreteMulticonnectionServer(std::string_view host, uint16_t port)
	{
		mServerSocket.Initialize();
		mServerSocket.Listen(host.data(), port);
	}

	void ConcreteMulticonnectionServer::WaitForConnections(uint8_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			CActiveSocket* clientSocket = mServerSocket.Accept();
			if (clientSocket != nullptr)
			{
				mClientSockets.emplace_back(clientSocket);
			}
		}
	}

	bool ConcreteMulticonnectionServer::SendToConnection(uint8_t connection, std::string_view message)
	{
		if (!IsConnectionValid(connection))
			return false;

		int32_t retVal = mClientSockets[connection]->Send(reinterpret_cast<const uint8*>(message.data()), message.size());
		return retVal != -1 && retVal != 0;
	}

	std::string ConcreteMulticonnectionServer::ReadFromConnection(uint8_t connection, uint16_t length)
	{
		if (!IsConnectionValid(connection))
			return "";

		std::unique_ptr<CActiveSocket>& socket = mClientSockets[connection];
		int32_t retVal = socket->Receive(length);

		if (retVal == -1 || retVal == 0 || socket->GetSocketError() != CSimpleSocket::SocketSuccess)
		{
			socket.reset();
			return "";
		}

		return std::string(reinterpret_cast<const char*>(socket->GetData()), socket->GetBytesReceived());
	}

	bool ConcreteMulticonnectionServer::IsConnectionValid(uint8_t connection)
	{
		if (connection >= mClientSockets.size())
			return false;

		std::unique_ptr<CActiveSocket>& socket = mClientSockets[connection];
		if (!socket)
			return false;

		return socket->GetSocketError() == CSimpleSocket::SocketSuccess;
	}

	ConcreteMulticonnectionServer::~ConcreteMulticonnectionServer()
	{
		mServerSocket.Close();
		for (size_t connection = 0; connection < mClientSockets.size(); ++connection)
		{
			if (IsConnectionValid(connection))
			{
				mClientSockets[connection]->Close();
			}
		}
	}

}
