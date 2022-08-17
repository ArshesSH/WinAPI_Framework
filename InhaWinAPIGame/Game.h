#pragma once

#include "framework.h"

#include <memory>
#include <vector>

#include "FrameTimer.h"
#include "DrawManager.h"
#include "CoordinateTransformer.h"
#include "Scene.h"
#include "TestScene.h"
#include "ActorTestScene.h"

class Game
{
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

private:
	void CycleScenes();
	void ReverseCycleScenes();

public:
	RECT screenRect;
private:
	static constexpr int screenWidth = 1600;
	static constexpr int screenHeight = 900;

	RECT oldScreenSize = screenRect;
	FrameTimer ft;
	DrawManager drawManager;
	CoordinateTransformer ct;

	bool isScreenChanged = true;

	std::vector<std::unique_ptr<Scene>> pScenes;
	std::vector<std::unique_ptr<Scene>>::iterator curScene;
};