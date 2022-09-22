#pragma once
#include "PlayerX.h"

class PlayerX::WallSlide : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	void ChangeToShootAnim( PlayerX& playerX, Scene& scene, PlayerXBullet::Type type, PlayerX::AnimationState targetAnim );
private:
	bool checkReinput = false;
	static constexpr float animStartSpeed = 0.08f;
	static constexpr float wallSlideSpeed = -150.0f;
	static constexpr float animResetTime = 0.5f;
	const float animSpeed = 0.0f;
	bool isStartResetAnimation = false;
	float time = 0.0f;
};

