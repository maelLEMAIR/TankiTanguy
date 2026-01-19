#include <iostream>

#include "pch.h"
#include "ServerSocket.h"
#include "AppServer.h"

constexpr size_t MAX_UDP_PACKET_SIZE = 1500;
ServerSocket* server = nullptr;

DWORD WINAPI ReceiveMessage(LPVOID lpParam)
{
	Sleep(1000); //Attendre lancement jeu

	std::vector<uint8_t> buffer(MAX_UDP_PACKET_SIZE);

	while (App::GetInstance().IsRunning())
	{
		server->RecvPacket(buffer, (int)buffer.size());
	}

	std::cout << "Thread termine." << std::endl;
	delete server;
	return 0;
}

DWORD WINAPI Game(LPVOID lpParam)
{
	CPU_RUN(512, 256);

	return 0;
}

int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	server = new ServerSocket();

	HANDLE game = CreateThread(NULL, 0, Game, NULL, 0, NULL);
	HANDLE UdpRecv = CreateThread(NULL, 0, ReceiveMessage, NULL, 0, NULL);

	WaitForSingleObject(UdpRecv, INFINITE);
	
    return 0;
}