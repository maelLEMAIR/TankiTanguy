#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "Network.hpp"
#include <iostream>

class ServerSocket
{
public:
	ServerSocket();
	~ServerSocket();

	void RecvPacket(std::vector<uint8_t> buffer, int length);
	void SendPacket(const char* data, int length);

	void UnpackHeader(BufferReader& reader, PacketHeader& header);
	void HandleOnPacketType(BufferReader& reader, PacketHeader& header);

	//Logique de traitement des messages
	void HandleClientHello(BufferReader& reader);
	void HandleClientInput(BufferReader& reader);
	void HandlePing(BufferReader& reader);
	void HandlePong(BufferReader& reader);
	void HandleDisconnect(BufferReader& reader);

private:
	SOCKET m_socket;
	sockaddr_in m_clientAddr;
};

#endif // !SERVER_SOCKET_H