#pragma once

#include "Scene.h"


#include <vector>
#include "Rect.h"
#include "Vec2.h"

#include "CoordinateTransformer.h"
#include "Camera.h"
#include "Gravity.h"
#include "Collider.h"
#include "CollisionManager.h"

#include "TestCharacter.h"

class TestScene : public Scene
{
public:
	TestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct);
	void Update( float dt, class Game& game ) override;
	void Draw( HDC hdc ) override;
private:
	Surface<float> surf;
	Surface<float> surf2;
	Camera cam;

	Vec2<float> dudePos = { 300.0f, 0.0f };
	Vec2<float> dudeVel = { 0.0f, 0.0f };
	const Vec2<float> dudeSize = { 100.0f, 100.0f };
	Gravity dudeGravity;
	ConvexCollider<float> dudeCollider = { dudePos, 100,100 };
	//CircleCollider<float> dudeCollider = { dudePos, 50 };
	//LineCollider<float> dudeCollider = { dudePos, dudePos + Vec2<float>{100,100} };

	const Vec2<float> dirLeft = { -1.0f, 0.0f };
	const Vec2<float> dirUp = { 0.0f, 1.0f };
	const Vec2<float> dirRight = { 1.0f, 0.0f };
	const Vec2<float> dirDown = { 0.0f, -1.0f };

	Image::ImageGDI<float> imageTest;
	Image::ImageGDIPlus<float> imageTest2;

	RectF testRect = { -40, -20, -1000, 1000 };
	std::vector<Vec2<float>> testPoly;
	ConvexCollider<float> testCollider;
	LineCollider<float> testLineCollider = { {-200,0,},{200,100} };
	CircleCollider<float> testCircleCollider = { {-100, 200}, 50 };
	bool isCollided = false;
	CollisionManager<float> collisionManager;
	std::vector<Collider<float>*> pColliders;
	float delta;
};