#include <thread>
#include <conio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include "../include/server.hpp"
#include "../include/client.hpp"

wchar_t* cs2wc(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}


int main(int argc, char* argv[])
{
	//Choosing if this app is server or client
	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "Client")
		{
			startClient();
			return 0;
		}
	}
	std::thread server(startServer);
	server.detach();
	while (true)
	{
		char pressedKey = _getch();
		if (pressedKey == '1')
		{

			wchar_t szAppPath[MAX_PATH];
			GetModuleFileName(0, szAppPath, MAX_PATH);
			DWORD czCreationFlags[] = { CREATE_NEW_CONSOLE };
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			memset(&si, 0, sizeof(si));
			si.cb = sizeof(si);
			if (!CreateProcess(szAppPath, cs2wc("Client"), NULL, NULL, FALSE, *czCreationFlags, NULL, NULL, &si, &pi))
				return 2;
		}
	}
	return 1;
}