#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include "../include/net.hpp"

bool net::Send(std::string buffer, SOCKET &sock, bool echo)
{
	if (send(sock, buffer.c_str(), buffer.size() + 1, 0) == SOCKET_ERROR)
		return false;
	if (echo)
		std::cout << "Sent to " << sock << " " << buffer << std::endl;
	return true;
}

bool net::Get(std::string &buffer, SOCKET &sock)
{
	char buf[4096];
	int received = recv(sock, buf, 4096, 0);
	if (received == SOCKET_ERROR || received == 0)
		return false;
	buffer = buf;
	return true;
}