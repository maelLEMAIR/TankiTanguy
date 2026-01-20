#pragma once

class PhysicsSystem 
{
public:
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;
    void PhysicsUpdate(float deltaTime);
    void CollisionCheck();
};