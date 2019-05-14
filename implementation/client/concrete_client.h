#pragma once

#include <memory>
#include <vector>
#include <string>

#include <clsocket/ActiveSocket.h>

namespace sherry::networking
{
	class ConcreteClient
	{
	public:
		ConcreteClient(std::string_view host, uint16_t port);
		bool Send(std::string_view message);
		std::string Read(uint16_t length);
		bool IsValid();
		~ConcreteClient();

	private:
		CActiveSocket mSocket;
	};
}
