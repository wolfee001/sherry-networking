#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <memory>

namespace sherry::networking
{
	class ConcreteClient;

	class Client final
	{
	public:
		Client(std::string_view host, uint16_t port);
		bool Send(std::string_view message);
		std::string Read(uint16_t length = UINT16_MAX);
		bool IsValid();
		~Client();

	private:
		std::unique_ptr<ConcreteClient> mConcreteClient;
	};
}
