#include "Server.h"
#include "Gameplay/Player.h"
#include "Gameplay/PlayerController.h"
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
		if (m_waitingPlayers == false)
			Update();
	}
}

void Server::Start()
{
	InitializeSocket();

	ClientInfo* client1 = new ClientInfo();
	ClientInfo* client2 = new ClientInfo();
	client1->playerId = 0;
	client2->playerId = 1;
	m_vClientsInfo.push_back(client1);
	m_vClientsInfo.push_back(client2);

	m_running = true;
	m_pGM = new GameManager(true);
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

	// fermer le socket r�veille recvfrom
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
				continue;

			pServer->HandleOnPacketType(reader, header);
		}
		else if (ret == SOCKET_ERROR)
		{
			int err = WSAGetLastError();

			if (!pServer->m_running) 
				break;

			if (err == WSAECONNRESET) 
			{
				continue; // ignore
			}

			if (err == WSAENOTSOCK || err == WSAEINVAL) 
			{
				break;
			}

			std::cerr << "recvfrom failed, WSA error: " << err << "\n";
		}
	}

	return 0;
}

void Server::SendPacket(uint8_t clientID, const char* data, int length)
{
	ClientInfo* pClient = m_vClientsInfo[clientID];

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
		std::cout << "Header invalide re�u\n";
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
		std::cout << "Type de message inconnu re�u: " << (int)header.packetType << "\n";
		break;
	}
}

void Server::HandleClientHello(BufferReader& reader)
{
	MsgClientHello decoded;
	bool msgOk = DeserializeClientHello(reader, decoded);

	int playerID = -1;
	if (m_vClientsInfo[0]->playerName == "default")
	{
		SetClientInfo(m_vClientsInfo[0], (SocketAddress)m_lastClientAddr, decoded);
		playerID = 0;
		std::cout << "Player 0 connected: " << decoded.playerName << "\n";
	}
	else if (m_vClientsInfo[0]->playerName == decoded.playerName)
	{
		SetClientInfo(m_vClientsInfo[0], (SocketAddress)m_lastClientAddr, decoded);
		playerID = 0;
		std::cout << "Player 0 reconnected: " << decoded.playerName << "\n";
	}
	else if (m_vClientsInfo[1]->playerName == "default")
	{
		SetClientInfo(m_vClientsInfo[1], (SocketAddress)m_lastClientAddr, decoded);
		playerID = 1;
		std::cout << "Player 1 connected: " << decoded.playerName << "\n";
	}
	else if (m_vClientsInfo[1]->playerName == decoded.playerName)
	{
		SetClientInfo(m_vClientsInfo[1], (SocketAddress)m_lastClientAddr, decoded);
		playerID = 1;
		std::cout << "Player 1 reconnected: " << decoded.playerName << "\n";
	}
	else
	{
		std::cout << "Player try to join..." << decoded.playerName << "\n";
	}

	if (m_vClientsInfo[0]->isConnected && m_vClientsInfo[1]->isConnected)
	{
		std::cout << "Both players connected! Starting match...\n" << std::endl;
		m_waitingPlayers = false;
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

	int playerIndex = header.playerId;
	
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
	PacketHeader header;
	header.packetType = PacketType::ServerSnapshot;

	for (int clientID = 0; clientID < m_vClientsInfo.size(); clientID++)
	{
		//Pour chaque client envoie des infos des autres players
		for (int playerID = 0; playerID < m_pGM->GetPlayers().size(); playerID++)
		{
			Player* player = m_pGM->GetPlayers()[playerID];

			if (player->IsDead())
				player->Dead(m_pGM->GetDeltaTime());
			
			//TODO
			PlayerNetState playerState;
			playerState.posX = player->GetPos().x;
			playerState.posY = player->GetPos().y;
			playerState.posZ = player->GetPos().z;
			playerState.dirX = player->GetDir().x;
			playerState.dirY = player->GetDir().y;
			playerState.dirZ = player->GetDir().z;
			playerState.hp = player->GetLife().GetCurrentLife();

			MsgSnapshot snapshotMsg;
			snapshotMsg.playerId = playerID;
			snapshotMsg.state = playerState;

			BufferWriter writer;
			SerializePacketHeader(writer, header);
			SerializeServerSnapshot(writer, snapshotMsg);
			SendPacket(clientID, (const char*)writer.data.data(), (int)writer.data.size());
		}
	}
}

void Server::SetPlayerInput(int playerIndex, uint16_t buttons)
{
	GameManager::Get().GetPlayerControllers()[playerIndex]->AddIncomingInputs(buttons);
	std::cout << "Received input from Player " << (playerIndex + 1) << ": " << buttons << "\n";
}

void Server::SetClientInfo(ClientInfo* pInfo, const SocketAddress& addr, const MsgClientHello& msg)
{
	pInfo->playerName = msg.playerName;
	pInfo->address = addr;
	pInfo->isConnected = true;
	m_pGM->GetPlayers()[pInfo->playerId]->SetPseudo(msg.playerName);
}
