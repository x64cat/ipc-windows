#include <iostream>
#include <WS2tcpip.h>
#include "../include/net.hpp"
#include <string>
#include <thread>
#include "../include/server.hpp"

#pragma comment (lib, "ws2_32.lib")

int g_port = 1337;
void clientThread(SOCKET client)
{
	net::Send("ServerSynced", client, true);
	while (true)
	{
		std::string buffer;
		bool res = net::Get(buffer, client);
		if (res)
		{
			std::cout << client << ":" << buffer << std::endl;
		}
		else
		{
			std::cout << "Lost connection from " << client << std::endl;
			closesocket(client);
			return;
		}
	}
}

void startServer()
{
	std::cout << "Starting server..." << std::endl;
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);

	if (wsok != 0) {
		exit(1);
	}
	// creating socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		exit(1);
	}



	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(g_port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));


	if (listen(listening, SOMAXCONN) != 0)
		exit(1);
	std::cout << "Started listening." << std::endl;
	while (true)
	{
		SOCKET client = accept(listening, nullptr, nullptr);
		std::thread newClient(clientThread, client);
		newClient.detach();
	}
	closesocket(listening);
	WSACleanup();
	return;
}