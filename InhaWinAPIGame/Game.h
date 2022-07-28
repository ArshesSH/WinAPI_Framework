#pragma once

#include "framework.h"

#include "FrameTimer.h"
#include "DrawManager.h"
#include "Surface.h"
#include "Image.h"

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
public:
	RECT screenRect;
private:
	// For Check ScreenSize
	
	RECT oldScreenSize = screenRect;
	FrameTimer ft;
	DrawManager drawManager;

	bool isScreenChanged = true;
	SceneType sceneType = SceneType::SceneStage;

	//Test
	Surface<float> surf;
	Image::ImageGDI<float> imageTest;
	Image::ImageGDIPlus<float> imageTest2;
};