#pragma once

#include "SigmaHead.h"

class SigmaHead::Fade : public SigmaHead::Behavior
{
public:
	void Activate( SigmaHead& sigma, class Scene& scene ) override;
	Behavior* Update( SigmaHead& sigma, class Scene& scene, float dt ) override;

private:
	static constexpr float animSpeed = 0.03f;
	static constexpr float playTime = 0.5f;
	const Vec2<float> resetPos = { 700.0f, 300.0f };
	float time = 0.0f;
	bool isFadeIn = false;
};