#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include "Network/Network.hpp"
#include "Core/GameManager.h"
#include "../Client/ClientSocket.h"
#include "Player.hpp"

class PlayerController
{
public:
    PlayerController() = default;
    ~PlayerController() = default;

    void Init(Player& _player) { m_pPlayer = &_player; };

    int HandleInput();
    void HandleInput(int input);

    void UpdateInputs();
    void UpdateWithSnapshot();

    bool ShouldSend() const;
protected:

private:
    Player* m_pPlayer;

    std::vector<InputCmd> pendingInputs;
};

inline int PlayerController::HandleInput()
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
}

inline void PlayerController::HandleInput(int input)
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

inline void PlayerController::UpdateInputs()
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

inline void PlayerController::UpdateWithSnapshot()
{
	GameManager* gm = GameManager::Get();
	ClientSocket* clientSocket = &ClientSocket::GetInstance(); 
	PlayerState& playerState = gm->GetMatchState().players[clientSocket->GetServerInfo().playerId - 1];

    m_pPlayer->GetOwner()->m_pEntity->transform.pos = playerState.player->GetPos();
    m_pPlayer->SetAngle(playerState.player->GetAngle());
}

inline bool PlayerController::ShouldSend() const
{
    static DWORD lastSendTime = 0;
    DWORD currentTime = GetTickCount64();
    if (currentTime - lastSendTime >= 50) // envoi toutes les 50 ms
    {
        lastSendTime = currentTime;
        return true;
    }
    return false;
}

#endif // !PLAYER_CONTROLLER_HPP