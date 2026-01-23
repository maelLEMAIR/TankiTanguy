#include "pch.h"
#include "PlayerController.h"

#include "Player.h"
#include "Network/ClientSocket.h"
#include "Core/GameManager.h"

int PlayerController::HandleInput()
{
    if (cpuInput.IsKey(VK_UP))
    {
        m_pPlayer->Move(1);
        return VK_UP;
    }
    if (cpuInput.IsKey(VK_DOWN))
    {
        m_pPlayer->Move(-1);
        return VK_DOWN;
    }
    if (cpuInput.IsKey(VK_RIGHT))
    {
        m_pPlayer->Turn(1);
        return VK_RIGHT;
    }
    if (cpuInput.IsKey(VK_LEFT))
    {
        m_pPlayer->Turn(-1);
        return VK_LEFT;
    }
    if (cpuInput.IsKey(VK_SPACE))
    {
        m_pPlayer->Shoot();
        return VK_SPACE;
    }

	return -1; // No input
}

void PlayerController::ApplyInput(int input)
{
    if (input == VK_UP) // UP
        m_pPlayer->Move(1);
    if (input == VK_DOWN) // DOWN
        m_pPlayer->Move(-1);
    if (input == VK_RIGHT) // RIGHT
        m_pPlayer->Turn(1);
    if (input == VK_LEFT) // LEFT
        m_pPlayer->Turn(-1);

    if (input == VK_SPACE) // SHOOT
        m_pPlayer->Shoot();
}

void PlayerController::UpdateInputs()
{
    InputCmd cmd;
    cmd.buttons = HandleInput();

    pendingInputs.push_back(cmd);

    // envoi toutes les X ms (ex: 50 ms)
    if (ShouldSend())
    {
        ClientSocket* clientSocket = &ClientSocket::GetInstance();

        PacketHeader header;
        header.packetType = PacketType::ClientInput;
        header.playerId = clientSocket->GetServerInfo().playerId;

        MsgInputList msg;
        msg.inputsList = pendingInputs;

        BufferWriter writer;
        SerializePacketHeader(writer, header);
        SerializeInputList(writer, msg);
        clientSocket->SendPacket((const char*)writer.data.data(), (int)writer.data.size());

        pendingInputs.clear();
    }
}

void PlayerController::UpdateWithSnapshot()
{
    //GameManager* gm = GameManager::Get();
    //ClientSocket* clientSocket = &ClientSocket::GetInstance();
    //PlayerState& playerState = gm->GetMatchState().players[clientSocket->GetServerInfo().playerId - 1];

    //m_pPlayer->GetOwner()->m_pEntity->transform.pos = playerState.player->GetPos();
    //m_pPlayer->SetAngle(playerState.player->GetAngle());
}

bool PlayerController::ShouldSend() const
{
    static DWORD lastSendTime = 0;
    DWORD currentTime = GetTickCount64();
    if (currentTime - lastSendTime >= 3) // envoi toutes les 3 ms
    {
        lastSendTime = currentTime;
        return true;
    }
    return false;
}
