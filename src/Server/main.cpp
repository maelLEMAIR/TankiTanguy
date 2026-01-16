#include <iostream>

#include "pch.h"
#include "ServerSocket.h"
#include "AppServer.h"

ServerSocket* server = nullptr;

DWORD WINAPI ReceiveMessage(LPVOID lpParam)
{
	constexpr size_t MAX_UDP_PACKET_SIZE = 1500;
	std::vector<uint8_t> buffer(MAX_UDP_PACKET_SIZE);

	while (true)
	{
		server->RecvPacket(buffer, (int)buffer.size());
	}

	return 0;
}

int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	server = new ServerSocket();
	CPU_RUN(512, 256);

	HANDLE UdpRecv = CreateThread(NULL, 0, ReceiveMessage, NULL, 0, NULL);
	WaitForSingleObject(UdpRecv, INFINITE);

	std::cout << "Thread termine." << std::endl;

    return 0;
}