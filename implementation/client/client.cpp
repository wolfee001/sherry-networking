#include "../../interface/client.h"
#include "concrete_client.h"

namespace sherry::networking
{
	Client::Client(std::string_view host, uint16_t port)
	{
		mConcreteClient = std::make_unique<ConcreteClient>(host, port);
	}

	std::string Client::Read(uint16_t length /*= UINT16_MAX*/)
	{
		return mConcreteClient->Read(length);
	}

	bool Client::IsValid()
	{
		return mConcreteClient->IsValid();
	}

	Client::~Client()
	{
		mConcreteClient.reset();
	}

	bool Client::Send(std::string_view message)
	{
		return mConcreteClient->Send(message);
	}
}
