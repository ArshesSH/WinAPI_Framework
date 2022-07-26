#pragma once

#include "framework.h"
#include "Vec2.h"

class Scene
{
public:
	Scene(int sceneWidth, int sceneHeight)
		:
		sceneWidth(sceneWidth),
		sceneHeight(sceneHeight)
	{}
	virtual ~Scene() {}
	virtual void Update(float dt, class Game& game ) = 0;
	virtual void Draw( HDC hdc ) = 0;
	RECT GetSceneRECT() const
	{
		return sceneRect;
	}
	Vec2<int> GetSceneTopLeft() const
	{
		return sceneTopLeft;
	}
	bool isSceneFinshed() const
	{
		return isSceneFinish;
	}
	void FinishScene()
	{
		isSceneFinish = true;
	}
protected:
	void UpdateSceneRect( class Game& game );
protected:
	bool isSceneFinish = false;

	int sceneWidth;
	int sceneHeight;
	
	RECT sceneRect = {0,0,0,0};
	Vec2<int> sceneTopLeft;
	Vec2<int> sceneBottomRight;
};

