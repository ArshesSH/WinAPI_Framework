#pragma once

#include "PlayerX.h"

class PlayerX::Walk : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	void DoWalk( PlayerX& playerX, Scene& scene, float dt );
	void ChangeToShootAnim( PlayerX& playerX, Scene& scene, PlayerXBullet::Type type );

private:
	static constexpr float animStartSpeed = 0.05f;
	static constexpr float animLoopSpeed = 0.05f;
	static constexpr float walkResetTime = 0.5f;

	AnimationState oldAnimState = AnimationState::WalkStart;
	bool isStartResetAnimation = false;
	float time = 0.0f;
};