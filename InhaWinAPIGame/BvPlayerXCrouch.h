#pragma once

#include "PlayerX.h"

class PlayerX::Crouch : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	static constexpr float animSpeed = 0.1f;
	static constexpr float animResetTime = 0.3f;

	bool isStartResetAnimation = false;
	float time = 0.0f;
};

