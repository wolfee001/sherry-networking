#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include "../interface/multiconnection_server.h"
#include "../interface/client.h"

struct Logger
{
	void operator()(size_t level, const std::string& sender, const std::string& message)
	{
		std::cout << std::string(level * 4, ' ') << ' ' << sender << ": " << message << '\n';
	}
};

void TestSingleConnection()
{
	Logger log;
	log(1, "system", "testing single connection...");
	std::thread([&]() {
		sherry::networking::MulticonnectionServer server("127.0.0.1", 8888);
		server.WaitForConnections(1);
		if (!server.IsConnectionValid(0))
		{
			log(2, "SERVER", "client is dead!");
			std::exit(-1);
		}
	}).detach();

	sherry::networking::Client client("127.0.0.1", 8888);
	if (!client.IsValid())
	{
		log(2, "CLIENT", "unable to connect!");
		std::exit(-1);
	}
	log(1, "system", "single connection OK");
}

void TestMultipleConnection()
{
	Logger log;
	log(1, "system", "testing multiple connection...");
	std::thread([&]() {
		sherry::networking::MulticonnectionServer server("127.0.0.1", 8888);
		server.WaitForConnections(3);
		for (uint8_t i = 0; i < 3; ++i)
		{
			if (!server.IsConnectionValid(i))
			{
				log(2, "SERVER", "client " + std::to_string(i) + " is dead!");
				std::exit(-1);
			}
		}
	}).detach();

	std::vector<std::unique_ptr<sherry::networking::Client>> clients;
	for (size_t i = 0; i < 3; ++i)
	{
		clients.emplace_back(new sherry::networking::Client("127.0.0.1", 8888));
		if (!clients.back()->IsValid())
		{
			log(2, "CLIENT", "client " + std::to_string(i) + " unable to connect!");
			std::exit(-1);
		}
	}
	log(1, "system", "multiple connection OK");
}

void TestSingleCommunication()
{
	Logger log;
	log(1, "system", "testing single communication...");
	std::vector<std::string> clientMessages = { "lorem", "dolor", "amet" };
	std::vector<std::string> serverMessages = { "ipsum", "sit", "." };
	std::thread([&]() {
		sherry::networking::MulticonnectionServer server("127.0.0.1", 8888);
		server.WaitForConnections(1);
		if (!server.IsConnectionValid(0))
		{
			log(2, "SERVER", "client is dead!");
			std::exit(-1);
		}
		for (size_t i = 0; i < 3; ++i)
		{
			std::string msg = server.ReadFromConnection(0);
			if (msg != clientMessages[i])
			{
				log(2, "SERVER", "invalid message: got " + msg + " instead of " + clientMessages[i]);
				std::exit(-1);
			}
			if (!server.SendToConnection(0, serverMessages[i]))
			{
				log(2, "SERVER", "unable to send message to client");
				std::exit(-1);
			}
		}
	}).detach();

	sherry::networking::Client client("127.0.0.1", 8888);
	if (!client.IsValid())
	{
		log(2, "CLIENT", "unable to connect!");
		std::exit(-1);
	}
	for (size_t i = 0; i < 3; ++i)
	{
		if (!client.Send(clientMessages[i]))
		{
			log(2, "CLIENT", "unable to send message to server");
			std::exit(-1);
		}
		std::string msg = client.Read();
		if (msg != serverMessages[i])
		{
			log(2, "CLIENT", "invalid message: got " + msg + " instead of " + serverMessages[i]);
			std::exit(-1);
		}
	}
	log(1, "system", "single connection OK");
}

void TestMultipleCommunication()
{
	Logger log;
	log(1, "system", "testing multiple communication...");
	std::vector<std::string> clientMessages = { "lorem", "dolor", "amet" };
	std::vector<std::string> serverMessages = { "ipsum", "sit", "." };
	std::thread([&]() {
		sherry::networking::MulticonnectionServer server("127.0.0.1", 8888);
		server.WaitForConnections(3);
		for (uint8_t i = 0; i < 3; ++i)
		{
			if (!server.IsConnectionValid(i))
			{
				log(2, "SERVER", "client " + std::to_string(i) + " is dead!");
				std::exit(-1);
			}
			for (size_t i = 0; i < 3; ++i)
			{
				for (uint8_t connection = 0; connection < 3; ++connection)
				{
					std::string msg = server.ReadFromConnection(connection);
					if (msg != clientMessages[i])
					{
						log(2, "SERVER", "invalid message: got " + msg + " instead of " + clientMessages[i] + " from client " + std::to_string(connection));
						std::exit(-1);
					}
					if (!server.SendToConnection(connection, serverMessages[i]))
					{
						log(2, "SERVER", "unable to send message to client " + std::to_string(connection));
						std::exit(-1);
					}
				}
			}
		}
	}).detach();

	std::vector<std::unique_ptr<sherry::networking::Client>> clients;
	for (size_t i = 0; i < 3; ++i)
	{
		clients.emplace_back(new sherry::networking::Client("127.0.0.1", 8888));
		if (!clients.back()->IsValid())
		{
			log(2, "CLIENT", "client " + std::to_string(i) + " unable to connect!");
			std::exit(-1);
		}
	}

	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t client = 0; client < 3; ++client)
		{
			if (!clients[client]->Send(clientMessages[i]))
			{
				log(2, "CLIENT", "unable to send message to server from client " + std::to_string(client));
				std::exit(-1);
			}
			std::string msg = clients[client]->Read();
			if (msg != serverMessages[i])
			{
				log(2, "CLIENT", "invalid message: got " + msg + " instead of " + serverMessages[i] + " to client " + std::to_string(client));
				std::exit(-1);
			}
		}
	}

	log(1, "system", "multiple communication OK");
}

int main(int argc, char** argv)
{
	Logger log;
	log(0, "system", "sherry::networking test");
	TestSingleConnection();
	TestMultipleConnection();
	TestSingleCommunication();
	TestMultipleCommunication();
	log(0, "system", "all test ran!");

	return 0;
}
