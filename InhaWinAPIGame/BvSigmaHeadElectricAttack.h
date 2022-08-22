#pragma once

#include "SigmaHead.h"

class SigmaHead::ElectricAttack : public SigmaHead::Behavior
{
public:
	void Activate( SigmaHead& sigma, class Scene& scene ) override;
	Behavior* Update( SigmaHead& sigma, class Scene& scene, float dt ) override;

private:
	static constexpr float animSpeed = 0.1f;
};