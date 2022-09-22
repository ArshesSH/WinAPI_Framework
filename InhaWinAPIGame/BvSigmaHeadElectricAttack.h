#pragma once

#include "SigmaHead.h"

class SigmaHead::ElectricAttack : public SigmaHead::Behavior
{
public:
	void Activate( SigmaHead& sigma, class Scene& scene ) override;
	Behavior* Update( SigmaHead& sigma, class Scene& scene, float dt ) override;

	Vec2<float> GetDirToPlayer( SigmaHead& sigma, class Scene& scene ) const;
	float GetLengthSqFromPlayer( SigmaHead& sigma, class Scene& scene ) const;
private:
	static constexpr float animSpeed = 0.1f;
	static constexpr float ballRelPosX1 = -180.0f;
	static constexpr float ballRelPosX2 = -150.0f;
	static constexpr float ballRelPosY1 = 100.0f;
	static constexpr float ballRelPosY2 = -50.0f;
	static constexpr float attackSpeed = 600.0f;
	static constexpr float chaseEndDistance = 300.0f * 300.0f;

	float time = 0.0f;

};