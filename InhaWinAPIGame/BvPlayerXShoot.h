#pragma once

#include "PlayerX.h"

class PlayerX::Shoot : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	static constexpr float animSpeed = 0.08f;
};

class ShootEnd : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	static constexpr float animSpeed = 0.08f;
};