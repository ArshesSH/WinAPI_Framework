#pragma once

#include "PlayerX.h"

class PlayerX::Jump : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	static constexpr float animSpeed = 0.1f;
	static constexpr float jumpMinTime = 0.1f;
	static constexpr float jumpMaxTime = 0.3f;
	static constexpr float jumpSpeed = 300.0f;
	static constexpr float moveSpeed = 200.0f;

	float jumpTime;
};