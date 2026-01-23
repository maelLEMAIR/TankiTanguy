#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <iostream>

#include "Network.hpp"

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

	static ClientSocket& GetInstance() { return *s_pClient; }

	void ConnectToServer(const char* serverIp, int serverPort);

	void RecvPacket(std::vector<uint8_t> buffer, int length);
	void SendPacket(const char* data, int length);

	void UnpackHeader(BufferReader& reader, PacketHeader& header);
	void HandleOnPacketType(BufferReader& reader, PacketHeader& header);

	//Logique de traitement des messages
	void HandleServerWelcome(BufferReader& reader);
	void HandleServerSnapshot(BufferReader& reader);
	void HandlePing(BufferReader& reader);
	void HandlePong(BufferReader& reader);
	void HandleDisconnect(BufferReader& reader);

	SOCKET GetSocket() const { return m_socket; }
	ServerInfo& GetServerInfo() { return m_serverInfo; }

private:
	inline static ClientSocket* s_pClient = nullptr;

	SOCKET m_socket;
	ServerInfo m_serverInfo;
	sockaddr_in m_serverAddr;
};

#endif // !CLIENT_SOCKET_H



