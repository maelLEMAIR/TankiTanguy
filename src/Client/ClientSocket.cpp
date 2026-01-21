#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
	}
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::ConnectToServer(const char* ipAddress, int port)
{
	inet_pton(AF_INET, ipAddress, &m_serverAddr.sin_addr.s_addr);
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(port);

	PacketHeader header;
	header.packetType = PacketType::ClientHello;

	MsgClientHello hello;
	hello.playerName = "Alice";

	BufferWriter writer;
	SerializePacketHeader(writer, header);
	SerializeClientHello(writer, hello);

	SendPacket(writer.data, (int)writer.data.size());
}

void ClientSocket::RecvPacket(std::vector<uint8_t> buffer, int length)
{
	sockaddr_in serverSocket;
	socklen_t serverSocketLen = sizeof(sockaddr_in);
	int ret = recvfrom(m_socket, (char*)buffer.data(), length, 0, (sockaddr*)&serverSocket, &serverSocketLen);

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

void ClientSocket::SendPacket(std::vector<uint8_t> data, int length)
{
	int ret = sendto(m_socket, (const char*)data.data(), length, 0, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr));
	if (ret == SOCKET_ERROR)
	{
		std::cout << "Erreur envoi : " << WSAGetLastError() << std::endl;
	}
}

void ClientSocket::UnpackHeader(BufferReader& reader, PacketHeader& header)
{
	bool headerOk = DeserializePacketHeader(reader, header);
	if (headerOk)
		std::cout << "Packet Type: " << (int)header.packetType << "\n";
	else
		std::cout << "Header invalide reçu\n";
}

void ClientSocket::HandleOnPacketType(BufferReader& reader, PacketHeader& header)
{
	switch (header.packetType)
	{
	case PacketType::ServerWelcome:
		HandleServerWelcome(reader);
		break;
	case PacketType::ServerSnapshot:
		HandleServerSnapshot(reader);
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
		std::cout << "Packet type inconnu reçu: " << (int)header.packetType << "\n";
		break;
	}
}

void ClientSocket::HandleServerWelcome(BufferReader& reader)
{
	std::cout << "Message ServerWelcome reçu\n";

	MsgServerWelcome decoded;
	bool msgOk = DeserializeServerWelcome(reader, decoded);

	m_serverInfo.playerId = decoded.playerId;
	m_serverInfo.serverTickRate = decoded.serverTickRate;
	m_serverInfo.serverTimeMs = decoded.serverTimeMs;
}

void ClientSocket::HandleServerSnapshot(BufferReader& reader)
{
	std::cout << "Message ServerSnapshot reçu\n";

	MsgSnapshot decoded;
	bool msgOk = DeserializeServerSnapshot(reader, decoded);
}

void ClientSocket::HandlePing(BufferReader& reader)
{
	std::cout << "Message Ping reçu\n";
}

void ClientSocket::HandlePong(BufferReader& reader)
{
	std::cout << "Message Pong reçu\n";
}

void ClientSocket::HandleDisconnect(BufferReader& reader)
{
	std::cout << "Message Disconnect reçu\n";
}