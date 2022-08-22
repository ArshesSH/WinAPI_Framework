#pragma once
#include "PlayerX.h"

class PlayerX::Dash : public PlayerX::Behavior
{
public:
	virtual ~Dash() = default;
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

protected:
	void DoDash( PlayerX& playerX, Scene& scene, float dt );
	bool IsDashEnd( PlayerX& playerX, Scene& scene, float dt );
	void ChangeToShootAnim( PlayerX& playerX, Scene& scene, PlayerXBullet::Type type );

private:
	static constexpr float animStartSpeed = 0.05f;
	static constexpr float animLoopSpeed = 0.05f;
	static constexpr float dashMaxTime = 0.7f;
	static constexpr float animEndSpeed = 0.1f;

	float dashTime = 0.0f;
	float dashEndTime = 0.0f;
	bool isDashEndStart = false;
};


class PlayerX::AirDash : public PlayerX::Dash
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;
private:
	static constexpr float animStartSpeed = 0.05f;
	static constexpr float animLoopSpeed = 0.05f;
};