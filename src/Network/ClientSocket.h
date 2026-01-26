#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <iostream>

#include "Network.hpp"

constexpr size_t MAX_UDP_PACKET_SIZE = 1500;

struct ServerInfo
{
	uint8_t  playerId;
	uint8_t  serverTickRate;
	uint32_t serverTimeMs;
};

class GameManager;

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	static ClientSocket& GetInstance() { return *s_pClient; }

	bool ConnectToServer(const char* serverIp, int serverPort, const char* _pseudo);

	static DWORD WINAPI Listen(LPVOID lpParam);

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

	bool m_isRunning = false;
	GameManager* m_gm;

	SOCKET m_socket;
	ServerInfo m_serverInfo;
	sockaddr_in m_serverAddr;

	HANDLE m_listenThread = nullptr;
};

#endif // !CLIENT_SOCKET_H



