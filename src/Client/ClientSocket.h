#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "Network.hpp"
#include <iostream>

struct ServerInfo
{
	uint8_t  playerId;
	uint8_t  serverTickRate;
	uint32_t serverTimeMs;
};

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	void ConnectToServer(const char* serverIp, int serverPort);

	void RecvPacket(std::vector<uint8_t> buffer, int length);
	void SendPacket(std::vector<uint8_t> data, int length);

	void UnpackHeader(BufferReader& reader, PacketHeader& header);
	void HandleOnPacketType(BufferReader& reader, PacketHeader& header);

	//Logique de traitement des messages
	void HandleServerWelcome(BufferReader& reader);
	void HandleServerSnapshot(BufferReader& reader);
	void HandlePing(BufferReader& reader);
	void HandlePong(BufferReader& reader);
	void HandleDisconnect(BufferReader& reader);

private:
	SOCKET m_socket;
	ServerInfo m_serverInfo;
	sockaddr_in m_serverAddr;
};

#endif // !CLIENT_SOCKET_H



