#include <iostream>
#include <WS2tcpip.h>
#include "../include/net.hpp"
#include <string>
#include "../include/server.hpp"
#include "../include/client.hpp"

#pragma comment (lib, "ws2_32.lib")
std::string input(const char* msg)
{
    std::cout << msg;
    std::string str;
    getline(std::cin, str);
    if (!str.empty());
    return str;
}

void loopCmd(SOCKET &server)
{
    while (true)
    {
        //бесконечнный цикл, повторяет получение параметров из консоли
        net::Send(input("Command to server: "), server, true);
    }
}
void startClient()
{
    std::cout << "Starting client..." << std::endl;
    std::string ipAddress = "127.0.0.1"; //using localhost

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
    net::Send("CONNECTEDCLIENT", sock, true);
    // создаем новый поток для получения комманд для отправки, рассинхрон
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)loopCmd, &sock, 0, NULL);
    while (true)
    {
        std::string buffer;
        bool res = net::Get(buffer, sock);
        if (res)
        {
            std::cout << buffer << std::endl;
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