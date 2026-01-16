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
}

ServerSocket::~ServerSocket()
{
	closesocket(m_socket);
}

void ServerSocket::RecvPacket(std::vector<uint8_t> buffer, int length)
{
	sockaddr_in clientSocket;
	socklen_t clientSocketLen = sizeof(sockaddr_in);
	int ret = recvfrom(m_socket, (char*)buffer.data(), length, 0, (sockaddr*)&clientSocket, &clientSocketLen);

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

void ServerSocket::SendPacket(const char* data, int length)
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
	std::cout << "Nonce: " << std::hex << decoded.clientNonce << std::dec << "\n";
	std::cout << "Name : " << decoded.playerName << "\n";
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
