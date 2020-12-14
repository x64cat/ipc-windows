#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include "../include/server.hpp"
#pragma comment (lib, "ws2_32.lib")

void clientThread(SOCKET client)
{
	std::string welcomemsg = "ReadyServer";
	send(client, welcomemsg.c_str(), welcomemsg.size() + 1, 0);
	while (true)
	{
		char buf[4096];
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(client, buf, 4096, 0);
		if (bytesReceived > 0)
		{
			std::cout << client << ":" << std::string(buf) << std::endl;
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
	// creating winsocket
	if (wsok != 0) {
		// Can't init win sock
		return;
	}
	// creating socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		// Can't create socket
		return;
	}

	// Bind ip and port to socket

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	std::string str = "1321";
	hint.sin_port = htons(atoi(str.c_str()));
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));
	// tell winsock the sock is for listening

	listen(listening, SOMAXCONN);
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