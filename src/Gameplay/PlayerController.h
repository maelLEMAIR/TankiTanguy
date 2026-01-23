#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <vector>
#include "Network/Network.hpp"

class Player;

class PlayerController
{
public:
    PlayerController() = default;
    ~PlayerController() = default;

    void Init(Player& _player) { m_pPlayer = &_player; };

    int HandleInput();
    void ApplyInput(int input);

    void UpdateInputs();
    void UpdateWithSnapshot();

    bool ShouldSend() const;
protected:

private:
    Player* m_pPlayer;

    std::vector<InputCmd> pendingInputs;
};

#endif // !PLAYER_CONTROLLER_H