#include "pch.h"
#include "ClientSocket.h"

ClientSocket* client = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int cmdShow)
{
    Sleep(1000); // Attendre que le serveur soit prêt

    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    client = new ClientSocket();
    client->ConnectToServer("127.0.0.1", 1888);

    // AMIGA NTSC FULLSCREEN
    //CPU_RUN(320, 200, true, true);	// AMIGA PAL FULLSCREEN
    //CPU_RUN(320, 256, true, true);

    // RETRO FULLSCREEN
    CPU_RUN(512, 256, false, true);

    // MODERN
    //CPU_RUN(1024, 576);

    // FULL HD (please use release)
    //CPU_RUN(1920, 1080);

    return 0;
}
