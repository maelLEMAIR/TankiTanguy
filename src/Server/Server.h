#ifndef SERVER_H
#define SERVER_H

#include "Gameplay/Core/GameManager.h"
#include "Network.hpp"
#include <iostream>

constexpr size_t MAX_UDP_PACKET_SIZE = 1500;

struct ClientInfo
{
	uint8_t playerId = 0;
	std::string playerName = "default";
	SocketAddress address;
	bool isConnected = false;
};

class GameManager;

class Server
{
public:
	Server();
	~Server();

	static Server& GetInstance() { return *s_pServer; }

	void MainLoop();

	void Start();
	void Update();
	void Exit();

	void InitializeSocket();

	static DWORD WINAPI Listen(LPVOID lpParam);
	void SendPacket(uint8_t clientID, const char* data, int length);

	void UnpackHeader(BufferReader& reader, PacketHeader& header);
	void HandleOnPacketType(BufferReader& reader, PacketHeader& header);

	//Logique de traitement des messages
	void HandleClientHello(BufferReader& reader);
	void HandleClientInput(BufferReader& reader);
	void HandlePing(BufferReader& reader);
	void HandlePong(BufferReader& reader);
	void HandleDisconnect(BufferReader& reader);

	void SetClientInfo(ClientInfo* pInfo, const SocketAddress& addr, const MsgClientHello& msg);

	sockaddr_in m_lastClientAddr;
	SOCKET m_socket;

private:
	inline static Server* s_pServer = nullptr;

	HANDLE m_listenThread = nullptr;
	
	GameManager* m_pGM = nullptr;
	bool m_toStart = false;
	bool m_running = false;
	std::vector<ClientInfo*> m_vClientsInfo;
};

#endif // !SERVER_SOCKET_H
