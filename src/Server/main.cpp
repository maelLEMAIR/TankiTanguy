#include <iostream>

#include "pch.h"
#include "ServerSocket.h"
#include "AppServer.h"

constexpr size_t MAX_UDP_PACKET_SIZE = 1500;
ServerSocket* server = nullptr;

DWORD WINAPI ReceiveMessage(LPVOID lpParam)
{
	std::vector<uint8_t> buffer(MAX_UDP_PACKET_SIZE);

	while (App::GetInstance().IsRunning())
	{
		server->RecvPacket(buffer, (int)buffer.size());
	}

	std::cout << "Thread termine." << std::endl;
	delete server;
	return 0;
}

int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	server = new ServerSocket();

	HANDLE UdpRecv = CreateThread(NULL, 0, ReceiveMessage, NULL, 0, NULL);
	CPU_RUN(512, 256);

	WaitForSingleObject(UdpRecv, INFINITE);
	
    return 0;
}