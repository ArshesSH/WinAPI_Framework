#pragma once

#include "PlayerX.h"

class PlayerX::WalkStart : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	float walkStartSpeed = 0.05f;
};

class PlayerX::WalkLoop : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	static constexpr float walkLoopSpeed = 0.05f;
	static constexpr float walkMoveSpeed = 200.0f;
};

class PlayerX::Walk : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	void Move( PlayerX& playerX, Scene& scene, float dt );

private:
	static constexpr float animStartSpeed = 0.05f;
	static constexpr float animLoopSpeed = 0.05f;
	static constexpr float moveSpeed = 200.0f;
};