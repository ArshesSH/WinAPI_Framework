#pragma once
#include "PlayerX.h"

class PlayerX::Dash : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	void DoDash( PlayerX& playerX, Scene& scene, float dt );

private:
	static constexpr float animStartSpeed = 0.05f;
	static constexpr float animLoopSpeed = 0.05f;

	static constexpr float moveSpeed = 500.0f;
	static constexpr float dashMaxTime = 0.5f;

	float dashTime = 0.0f;
};

class DashEnd : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	static constexpr float animEndSpeed = 0.1f;
};