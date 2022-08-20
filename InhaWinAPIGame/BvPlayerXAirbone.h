#pragma once

#include "PlayerX.h"

class PlayerX::Airbone : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	static constexpr float moveSpeed = 200.0f;
};