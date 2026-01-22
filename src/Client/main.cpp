#include "pch.h"
#include "Network.hpp"
#include "ClientSocket.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int cmdShow)
{
    // AMIGA NTSC FULLSCREEN
    // CPU_RUN(320, 200, true, true);	// AMIGA PAL FULLSCREEN
    //CPU_RUN(320, 256, true, true);

    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
    
	ClientSocket* client = new ClientSocket();

	Sleep(1000);
    client->ConnectToServer("", 1888);

    while (true)
    {
        if (cpuInput.IsKeyDown(VK_ESCAPE))
        {
            uint32_t inputSequence = 0;

            InputCmd cmd;
            cmd.sequence = inputSequence++;
            cmd.buttons = VK_ESCAPE;

            PacketHeader header;
            header.packetType = PacketType::ClientInput;
            header.playerId = 1;

            MsgInputList msg;
            msg.inputsList.push_back(cmd);

            BufferWriter writer;
            SerializePacketHeader(writer, header);
            SerializeInputList(writer, msg);
            client->SendPacket(writer.data, (int)writer.data.size());
        }
	}

    // RETRO FULLSCREEN
    //CPU_RUN(512, 256, true, true);

    // MODERN
    //CPU_RUN(1024, 576);

    // FULL HD (please use release)
    // CPU_RUN(1920, 1080, true);

    return 0;
}