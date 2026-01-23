#include "Server.h"
#include "Gameplay/Player.h"
#include "Gameplay/Core/GameManager.h"

Server::Server()
{
	s_pInstance = this;
}

Server::~Server()
{
	Exit();
}

void Server::MainLoop()
{
	while (m_running)
	{
		if (m_toStart && m_pGM == nullptr)
		{
			m_pGM->ServerSideStart();
			m_toStart = false;
		}

		Update();
	}
}

void Server::Start()
{
	InitializeSocket();

	ClientInfo* client1 = new ClientInfo();
	ClientInfo* client2 = new ClientInfo();
	client1->playerId = 1;
	client2->playerId = 2;
	m_vClientsInfo.push_back(client1);
	m_vClientsInfo.push_back(client2);

	m_running = true;
	m_pGM = &GameManager::Get();
	m_pGM->ServerSideStart();
	m_listenThread = CreateThread(NULL, 0, Listen, this, 0, NULL);
}

void Server::Update()
{
	m_pGM->ServerSideUpdate();
	Sleep(16); // ~60 FPS
	SendServerSnapshotsToClients();
}

void Server::Exit()
{
	m_running = false;

	// fermer le socket réveille recvfrom
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	for (int i = 0; i < m_vClientsInfo.size(); ++i)
	{
		delete m_vClientsInfo[i];
	}	

	if (m_listenThread)
	{
		WaitForSingleObject(m_listenThread, INFINITE);
		CloseHandle(m_listenThread);
		m_listenThread = nullptr;
	}

	if (m_pGM)
	{
		//m_pGM->ServerSideStop();
	}
}

void Server::InitializeSocket()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(1888);

	if (bind(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
	}
}

DWORD Server::Listen(LPVOID lpParam)
{
	Server* pServer = static_cast<Server*>(lpParam);
	std::vector<uint8_t> buffer(MAX_UDP_PACKET_SIZE);

	while (pServer->m_running)
	{
		socklen_t clientSocketLen = sizeof(sockaddr_in);

		int ret = recvfrom(
			pServer->m_socket,
			reinterpret_cast<char*>(buffer.data()),
			static_cast<int>(buffer.size()),
			0,
			reinterpret_cast<sockaddr*>(&pServer->m_lastClientAddr),
			&clientSocketLen
		);

		if (ret > 0)
		{
			BufferReader reader { buffer.data(), static_cast<size_t>(ret), 0 };

			PacketHeader header;
			if (DeserializePacketHeader(reader, header) == false) 
				continue;	// paquet invalide -> ignore

			pServer->HandleOnPacketType(reader, header);
		}
		else if (ret == SOCKET_ERROR)
		{
			int err = WSAGetLastError();

			// Si on a demandé l'arrêt, c'est normal de sortir
			if (!pServer->m_running) 
				break;

			// Cas très fréquent en UDP Windows (ICMP port unreachable)
			if (err == WSAECONNRESET) 
			{
				continue; // ignore
			}

			// Si socket fermé pendant l'écoute
			if (err == WSAENOTSOCK || err == WSAEINVAL) 
			{
				break;
			}

			// Évite le spam console en boucle
			std::cerr << "recvfrom failed, WSA error: " << err << "\n";
		}
	}

	return 0;
}

void Server::SendPacket(uint8_t clientID, const char* data, int length)
{
	ClientInfo* pClient = m_vClientsInfo[clientID - 1];

	if (pClient == nullptr || !pClient->isConnected)
	{
		std::cerr << "Attempted to send packet to invalid or disconnected client ID: " << (int)clientID << "\n";
		return;
	}
	sockaddr_in clientAddr = pClient->address.sockaddr;
	int sentBytes = sendto(
		m_socket,
		data,
		length,
		0,
		(sockaddr*)&clientAddr,
		sizeof(clientAddr)
	);

	if (sentBytes == SOCKET_ERROR)
		std::cerr << "sendto failed with error: " << WSAGetLastError() << "\n";
}

void Server::UnpackHeader(BufferReader& reader, PacketHeader& header)
{
	bool headerOk = DeserializePacketHeader(reader, header);

	if (headerOk)
		std::cout << "Packet Type: " << (int)header.packetType << "\n";
	else
		std::cout << "Header invalide reçu\n";
}

void Server::HandleOnPacketType(BufferReader& reader, PacketHeader& header)
{
	switch (header.packetType)
	{
	case PacketType::ClientHello:
		HandleClientHello(reader);
		break;
	case PacketType::ClientInput:
		HandleClientInput(reader, header);
		break;
	case PacketType::Ping:
		HandlePing(reader, header);
		break;
	case PacketType::Pong:
		HandlePong(reader, header);
		break;
	case PacketType::Disconnect:
		HandleDisconnect(reader, header);
		break;
	default:
		std::cout << "Type de message inconnu reçu: " << (int)header.packetType << "\n";
		break;
	}
}

void Server::HandleClientHello(BufferReader& reader)
{
	MsgClientHello decoded;
	bool msgOk = DeserializeClientHello(reader, decoded);

	int playerID = 0;
	if (m_vClientsInfo[0]->playerName == "default")
	{
		SetClientInfo(m_vClientsInfo[0], (SocketAddress)m_lastClientAddr, decoded);
		playerID = 1;
		std::cout << "Player 1 connected: " << decoded.playerName << "\n";
	}
	else if (m_vClientsInfo[0]->playerName == decoded.playerName)
	{
		SetClientInfo(m_vClientsInfo[0], (SocketAddress)m_lastClientAddr, decoded);
		playerID = 1;
		std::cout << "Player 1 reconnected: " << decoded.playerName << "\n";
	}
	else if (m_vClientsInfo[1]->playerName == "default")
	{
		SetClientInfo(m_vClientsInfo[1], (SocketAddress)m_lastClientAddr, decoded);
		playerID = 2;
		std::cout << "Player 2 connected: " << decoded.playerName << "\n";
	}
	else
	{
		SetClientInfo(m_vClientsInfo[1], (SocketAddress)m_lastClientAddr, decoded);
		playerID = 2;
		std::cout << "Player 2 reconnected: " << decoded.playerName << "\n";
	}

	if (m_vClientsInfo[0]->isConnected && m_vClientsInfo[1]->isConnected)
	{
		std::cout << "Both players connected! Starting match...\n" << std::endl;
		m_toStart = true;
	}

	PacketHeader header;
	header.packetType = PacketType::ServerWelcome;

	MsgServerWelcome welcomeMsg;
	welcomeMsg.playerId = playerID;
	welcomeMsg.serverTickRate = 60;
	welcomeMsg.serverTimeMs = 0;

	BufferWriter writer;
	SerializePacketHeader(writer, header);
	SerializeServerWelcome(writer, welcomeMsg);
	SendPacket(welcomeMsg.playerId, (const char*)writer.data.data(), (int)writer.data.size());
}

void Server::HandleClientInput(BufferReader& reader, PacketHeader& header)
{
	MsgInputList decoded;
	bool msgOk = DeserializeInputList(reader, decoded);
	if (msgOk == false) return;

	int playerIndex = header.playerId - 1;
	
	for (const InputCmd& cmd : decoded.inputsList)
	{
		SetPlayerInput(playerIndex, cmd.buttons);
	}
}

void Server::HandlePing(BufferReader& reader, PacketHeader& header)
{
}

void Server::HandlePong(BufferReader& reader, PacketHeader& header)
{
}

void Server::HandleDisconnect(BufferReader& reader, PacketHeader& header)
{
}

void Server::SendServerSnapshotsToClients()
{
	//PacketHeader header;
	//header.packetType = PacketType::ServerSnapshot;

	//Player* player = m_pGM->GetMatchState().players[0].player;

	//PlayerNetState playerState;
	//playerState.posX = player->GetPos().x;
	//playerState.posY = player->GetPos().z;
	//playerState.angle = player->GetAngle();
	//playerState.hp = 100;

	//MsgSnapshot snapshotMsg;
	//snapshotMsg.state = playerState;

	//BufferWriter writer;
	//SerializePacketHeader(writer, header);
	//SerializeServerSnapshot(writer, snapshotMsg);
	//SendPacket(1, (const char*)writer.data.data(), (int)writer.data.size());
}

void Server::SetPlayerInput(int playerIndex, uint16_t buttons)
{
	//GameManager::Get()->GetMatchState().players[playerIndex].inputs.push_back(buttons);
	//std::cout << "Received input from Player " << (playerIndex + 1) << ": " << buttons << "\n";
}

void Server::SetClientInfo(ClientInfo* pInfo, const SocketAddress& addr, const MsgClientHello& msg)
{
	pInfo->playerName = msg.playerName;
	pInfo->address = addr;
	pInfo->isConnected = true;
}
