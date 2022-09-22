#pragma once

#include "SigmaHead.h"
#include <random>



class SigmaHead::Decide : public SigmaHead::Behavior
{
public:
	enum class AttackType
	{
		ElectricBall,
		Ball,
		Layser
	};

public:
	Decide(std::mt19937& rng)
		:
		rng(rng),
		attackRandomGen((int)AttackType::ElectricBall, (int)AttackType::Ball),
		lessHPRandomGen( (int)AttackType::ElectricBall, (int)AttackType::Layser )
	{}
	void Activate( SigmaHead& sigma, class Scene& scene ) override;
	Behavior* Update( SigmaHead& sigma, class Scene& scene, float dt ) override;

private:
	static constexpr float decideTime = 1.0f;
	static constexpr int decideChangeHP = 10;

	std::mt19937& rng;
	std::uniform_int_distribution<int> attackRandomGen;
	std::uniform_int_distribution<int> lessHPRandomGen;

	float time = 0.0f;
	

};