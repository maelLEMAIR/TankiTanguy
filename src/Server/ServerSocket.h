#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "Network.hpp"
#include <iostream>

struct PlayerInfo
{
	uint8_t persistentId = 0;
	uint8_t playerId = 0;
	std::string playerName;
	SocketAddress address;
	bool isConnected = false;
};

class ServerSocket
{
public:
	ServerSocket();
	~ServerSocket();

	void RecvPacket(std::vector<uint8_t> buffer, int length);
	void SendPacket(uint8_t clientID, const char* data, int length);

	void UnpackHeader(BufferReader& reader, PacketHeader& header);
	void HandleOnPacketType(BufferReader& reader, PacketHeader& header);

	//Logique de traitement des messages
	void HandleClientHello(BufferReader& reader);
	void HandleClientInput(BufferReader& reader);
	void HandlePing(BufferReader& reader);
	void HandlePong(BufferReader& reader);
	void HandleDisconnect(BufferReader& reader);

	void InitClientInfo(PlayerInfo* pInfo, const SocketAddress& addr, const MsgClientHello& msg);

private:
	SOCKET m_socket;
	sockaddr_in m_lastClientAddr;
	std::vector<PlayerInfo*> m_vClientsInfo;
};

#endif // !SERVER_SOCKET_H