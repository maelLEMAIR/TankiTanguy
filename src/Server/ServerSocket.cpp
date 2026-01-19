#include "ServerSocket.h"

ServerSocket::ServerSocket()
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

	PlayerInfo* player1 = new PlayerInfo();
	PlayerInfo* player2 = new PlayerInfo();
	player1->playerId = 1;
	player2->playerId = 2;
	m_vClientsInfo.push_back(player1);
	m_vClientsInfo.push_back(player2);
}

ServerSocket::~ServerSocket()
{
	closesocket(m_socket);
}

void ServerSocket::RecvPacket(std::vector<uint8_t> buffer, int length)
{
	socklen_t clientSocketLen = sizeof(sockaddr_in);
	int ret = recvfrom(m_socket, (char*)buffer.data(), length, 0, (sockaddr*)&m_lastClientAddr, &clientSocketLen);

	if (ret > 0)
	{
		// Init d'un BufferReader pour lire les données reçues
		BufferReader reader{ buffer.data(), buffer.size(), 0 };

		PacketHeader header;
		UnpackHeader(reader, header);
		HandleOnPacketType(reader, header);
	}
	else
	{
		std::cout << "Erreur reception : " << WSAGetLastError() << std::endl;
	}
}

void ServerSocket::SendPacket(uint8_t clientID, const char* data, int length)
{
}

void ServerSocket::UnpackHeader(BufferReader& reader, PacketHeader& header)
{
	bool headerOk = DeserializePacketHeader(reader, header);

	if (headerOk)
		std::cout << "Packet Type: " << (int)header.packetType << "\n";
	else
		std::cout << "Header invalide reçu\n";
}

void ServerSocket::HandleOnPacketType(BufferReader& reader, PacketHeader& header)
{
	switch (header.packetType)
	{
	case PacketType::ClientHello:
		HandleClientHello(reader);
		break;
	case PacketType::ClientInput:
		HandleClientInput(reader);
		break;
	case PacketType::Ping:
		HandlePing(reader);
		break;
	case PacketType::Pong:
		HandlePong(reader);
		break;
	case PacketType::Disconnect:
		HandleDisconnect(reader);
		break;
	default:
		std::cout << "Type de message inconnu reçu: " << (int)header.packetType << "\n";
		break;
	}
}

void ServerSocket::HandleClientHello(BufferReader& reader)
{
	MsgClientHello decoded;
	bool msgOk = DeserializeClientHello(reader, decoded);
	std::cout << "persistentID: " << (int)decoded.persistentId << "\n";

	int playerID = 0;
	if (m_vClientsInfo[0]->persistentId == 0)
	{
		InitClientInfo(m_vClientsInfo[0], (SocketAddress)m_lastClientAddr, decoded);
		std::cout << "Player 1 connected: " << decoded.playerName << "\n";
	}
	else if (m_vClientsInfo[0]->persistentId == decoded.persistentId)
	{
		InitClientInfo(m_vClientsInfo[0], (SocketAddress)m_lastClientAddr, decoded);
		std::cout << "Player 1 reconnected: " << decoded.playerName << "\n";
	}
	else if (m_vClientsInfo[1]->persistentId == 0)
	{
		InitClientInfo(m_vClientsInfo[1], (SocketAddress)m_lastClientAddr, decoded);
		std::cout << "Player 2 connected: " << decoded.playerName << "\n";
	}
	else
	{
		InitClientInfo(m_vClientsInfo[1], (SocketAddress)m_lastClientAddr, decoded);
		std::cout << "Player 2 reconnected: " << decoded.playerName << "\n";
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

void ServerSocket::HandleClientInput(BufferReader& reader)
{
}

void ServerSocket::HandlePing(BufferReader& reader)
{
}

void ServerSocket::HandlePong(BufferReader& reader)
{
}

void ServerSocket::HandleDisconnect(BufferReader& reader)
{
}

void ServerSocket::InitClientInfo(PlayerInfo* pInfo, const SocketAddress& addr, const MsgClientHello& msg)
{
	pInfo->persistentId = msg.persistentId;
	pInfo->playerName = msg.playerName;
	pInfo->address = addr;
	pInfo->isConnected = true;
}
