#pragma once

#include "PlayerX.h"

class PlayerX::Hover : public PlayerX::Behavior
{
public:
	void Activate( PlayerX& playerX, class Scene& scene ) override;
	Behavior* Update( PlayerX& playerX, class Scene& scene, float dt ) override;

private:
	bool IsOtherKeyInputed( PlayerX& playerX );
	void HoverEffect( PlayerX& playerX, class Scene& scene, float dt );
	void ChangeAnimation( PlayerX& playerX );
private:
	static constexpr float animSpeed = 0.05f;
	static constexpr float hoverEffectMaxSpeed = 60.0f;
	static constexpr float hoverEffectMaxTime = 0.15f;
	static constexpr float hoverMaxTime = 3.0f;

	float hoverTime = 0.0f;
	float hoverEffectTime = 0.0f;
	int hoverEffectCount = 0;

	bool checkReinput = false;
};

