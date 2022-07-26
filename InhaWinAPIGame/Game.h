#pragma once

#include "framework.h"

#include "FrameTimer.h"
#include "SceneStage.h"
#include "DrawManager.h"

// Test Things
//#include "TestTriangulationScene.h"

class Game
{
public:
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

	bool IsInitialGame() const;
	bool IsGameFinished() const;
	bool IsScreenChanged() const;

public:
	RECT screenRect;
private:
	RECT oldScreenSize = screenRect;
	FrameTimer ft;
	SceneStage stage;
	DrawManager drawManager;

	bool isScreenChanged = true;
	float time = 0.0f;
	SceneType sceneType = SceneType::SceneStage;

	bool isFinishedResult = false;




	// for Test
	//TestTriangulationScene testTriangulationScene;
};