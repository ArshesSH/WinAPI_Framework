#pragma once

#include "framework.h"
#include "Vec2.h"
#include <vector>
#include "Actor.h"
#include "Wall.h"
#include "CollisionManager.h"
#include "Camera.h"

class Scene
{
public:
	Scene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct )
		:
		sceneWidth( sceneWidth ),
		sceneHeight( sceneHeight ),
		sceneTopLeft( 0, 0 ),
		sceneBottomRight( sceneWidth, sceneHeight ),
		cam( ct )
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
	const Camera& AccessCamera() const
	{
		return cam;
	}

	const std::vector<std::unique_ptr<Wall>>& GetWallPtrs() const
	{
		return wallPtrs;
	}

	std::vector<Actor*> FindByTag(ActorTag tag)
	{
		std::vector<Actor*> targets;
		for ( const auto& pActor : actorPtrs )
		{
			if ( pActor->IsTagSameWith( tag ) )
			{
				targets.push_back( pActor.get() );
			}
		}
		return targets;
	}
	CollisionManager<float> GetCollisionManager() const 
	{
		return cm;
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

	Camera cam;
	CollisionManager<float> cm;
	std::vector<std::unique_ptr<Actor>> actorPtrs;
	std::vector<std::unique_ptr<Wall>> wallPtrs;
};

