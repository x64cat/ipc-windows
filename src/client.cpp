#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include "../include/server.hpp"
#include "../include/client.hpp"

#pragma comment (lib, "ws2_32.lib")
void startClient()
{
    std::cout << "Starting client..." << std::endl;
    std::string ipAddress = "127.0.0.1";

    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    //socker = sock;
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(g_port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        closesocket(sock);
        WSACleanup();
        return;
    }
    std::string userInput = "CONNECTEDCLIENT";
    send(sock, userInput.c_str(), userInput.size() + 1, 0);
    while (true)
    {
        char buf[4096];
        ZeroMemory(buf, 4096);
        int byteReceived = recv(sock, buf, 4096, 0);
        if (byteReceived > 0) 
        {
            std::cout << std::string(buf) << std::endl;
        }
        else
        {
            std::cout << "Server has been died! Quiting..." << std::endl;
            closesocket(sock);
            WSACleanup();
            return;
        }
    }

}