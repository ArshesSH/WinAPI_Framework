#pragma once

#include "framework.h"

#include "FrameTimer.h"
#include "DrawManager.h"
#include "Surface.h"
#include "Image.h"
#include "Camera.h"
#include "Gravity.h"
#include "Collider.h"
#include "CollisionManager.h"

// Test Things
//#include "TestTriangulationScene.h"

class Game
{
public:
	// For Scene Change
	enum class SceneType
	{
		SceneStart,
		SceneStage,
		SceneTest,
		SceneResult
	};
public:
	Game();
	void ComposeFrame( HDC hdc );
	void UpdateModel();

	void RefreshScreen();
	void SetScreenSize( HWND hWnd )
	{
		GetClientRect( hWnd, &screenRect );
	}

	void SetClientSize( HWND hWnd, int width = screenWidth, int height = screenHeight );

public:
	RECT screenRect;
private:
	static constexpr int screenWidth = 1600;
	static constexpr int screenHeight = 900;

	// For Check ScreenSize
	RECT oldScreenSize = screenRect;
	FrameTimer ft;
	DrawManager drawManager;

	bool isScreenChanged = true;
	SceneType sceneType = SceneType::SceneStage;

	//Test
	Surface<float> surf;
	Surface<float> surf2;
	CoordinateTransformer ct;
	Camera cam;

	Vec2<float> dudePos = { 300.0f, 0.0f };
	Vec2<float> dudeVel = { 0.0f, 0.0f };
	const Vec2<float> dudeSize = { 100.0f, 100.0f };
	Gravity dudeGravity;
	ConvexCollider<float> dudeCollider = { dudePos, 100,100 };
	//CircleCollider<float> dudeCollider = { dudePos, 50 };
	//LineCollider<float> dudeCollider = { dudePos, dudePos + Vec2<float>{50,50} };

	const Vec2<float> dirLeft = { -1.0f, 0.0f };
	const Vec2<float> dirUp = { 0.0f, 1.0f };
	const Vec2<float> dirRight = { 1.0f, 0.0f };
	const Vec2<float> dirDown = { 0.0f, -1.0f };

	Image::ImageGDI<float> imageTest;
	Image::ImageGDIPlus<float> imageTest2;

	RectF testRect = { -40, -20, -1000, 1000 };
	std::vector<Vec2<float>> testPoly;
	ConvexCollider<float> testCollider;
	LineCollider<float> testLineCollider = { {0,0,},{100,200} };
	CircleCollider<float> testCircleCollider = { {-100, 200}, 50 };
	bool isCollided = false;
	CollisionManager<float> collisionManager;

	std::vector<Collider<float>*> pColliders;
};