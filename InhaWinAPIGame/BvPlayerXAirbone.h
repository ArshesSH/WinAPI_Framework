#pragma once

#include "PlayerX.h"

class PlayerX::Airbone : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	void ChangeToShootAnim( PlayerX& playerX, Scene& scene, PlayerXBullet::Type type, PlayerX::AnimationState targetAnim );
	
private:
	const float animSpeed = 0.0f;
	static constexpr float animResetTime = 0.5f;
	bool isStartResetAnimation = false;
	float time = 0.0f;
};