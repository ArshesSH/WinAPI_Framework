#pragma once

#include "PlayerX.h"

class PlayerX::WalkStart : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	float walkStartSpeed = 0.2f;
};

class PlayerX::WalkLoop : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	static constexpr float walkLoopSpeed = 0.05f;
};