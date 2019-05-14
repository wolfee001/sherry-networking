#include "concrete_client.h"

namespace sherry::networking
{
	ConcreteClient::ConcreteClient(std::string_view host, uint16_t port)
	{
		mSocket.Initialize();
		mSocket.Open(host.data(), port);
	}

	bool ConcreteClient::Send(std::string_view message)
	{
		if (!IsValid())
			return false;

		int32_t retVal = mSocket.Send(reinterpret_cast<const uint8*>(message.data()), message.size());
		return retVal != -1 && retVal != 0;
	}

	std::string ConcreteClient::Read(uint16_t length)
	{
		if (!IsValid())
			return "";

		int32_t retVal = mSocket.Receive(length);

		if (retVal == -1 || retVal == 0 || mSocket.GetSocketError() != CSimpleSocket::SocketSuccess)
		{
			return "";
		}
		return std::string(reinterpret_cast<const char*>(mSocket.GetData()), mSocket.GetBytesReceived());
	}

	bool ConcreteClient::IsValid()
	{
		return mSocket.GetSocketError() == CSimpleSocket::SocketSuccess;
	}

	ConcreteClient::~ConcreteClient()
	{
		mSocket.Close();
	}
}
