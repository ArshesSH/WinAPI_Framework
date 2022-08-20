#pragma once

#include "PlayerX.h"

class PlayerX::Hover : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	bool IsInputOthers( PlayerX& playerX );
	void HoverEffect( PlayerX& playerX, class Scene& scene, float dt );
private:
	static constexpr float animSpeed = 0.5f;
	static constexpr float hoverEffectMaxSpeed = 500.0f;
	static constexpr float hoverEffectMaxTime = 0.3f;
	static constexpr float hoverMaxTime = 1.0f;

	float hoverTime = 0.0f;
	float hoverEffectTime = 0.0f;
	int hoverEffectCount;
};

