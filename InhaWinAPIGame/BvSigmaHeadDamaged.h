#pragma once


#include "SigmaHead.h"

class SigmaHead::Damaged : public SigmaHead::Behavior
{
public:
	void Activate( SigmaHead& sigma, class Scene& scene ) override;
	Behavior* Update( SigmaHead& sigma, class Scene& scene, float dt ) override;

private:
	static constexpr float animSpeed = 0.1f;
	static constexpr float playTime = 1.0f;

	float time = 0.0f;
};