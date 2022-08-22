#pragma once

#include "PlayerX.h"

class PlayerX::Jump : public PlayerX::Behavior
{
public:
	virtual ~Jump() = default;
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

protected:
	void DoJump( PlayerX& playerX, class Scene& scene, float dt );
	void SetVelXByInput( PlayerX& playerX, float speed );
	const float animSpeed = 0.04f;
private:
	static constexpr float jumpMaxTime = 0.3f;
	float jumpTime = 0.0f;
};

class PlayerX::DashJump : public PlayerX::Jump
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
};

class PlayerX::WallKick : public PlayerX::Jump
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	static constexpr float animSpeed = 0.1f;
};
