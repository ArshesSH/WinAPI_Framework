#pragma once
#include "PlayerX.h"
class PlayerX::Idle : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
	void SetRandomAnimation( PlayerX& playerX );

private:
	static constexpr float idleSpeed = 0.3f;
	static constexpr float shootSpeed = 0.08f;
	static constexpr float idleBlinkSpeed = 0.11f;
	static constexpr int idleMaxCount = 3;

	int idlePlayCnt = 0;
};

