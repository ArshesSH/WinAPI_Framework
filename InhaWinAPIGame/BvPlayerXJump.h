#pragma once

#include "PlayerX.h"

class PlayerX::Jump : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	static constexpr float jumpMinTime = 0.1f;
	static constexpr float jumpMaxTime = 1.0f;
};