#pragma once
#include "PlayerX.h"

class PlayerX::Dash : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	static constexpr float dashStartSpeed = 0.05f;
	static constexpr float dashLoopSpeed = 0.05f;
	static constexpr float dashMoveSpeed = 300.0f;
	static constexpr float dashMaxTime = 0.5f;

	float dashTime = 0.0f;
	bool isDashRight = false;
};

