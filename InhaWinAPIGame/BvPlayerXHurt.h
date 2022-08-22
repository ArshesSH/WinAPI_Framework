#pragma once

#include "PlayerX.h"

class PlayerX::Hurt : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	static constexpr float animStartSpeed = 0.03f;
	static constexpr float hurtResetTime = 0.5f;

	float time = 0.0f;
};