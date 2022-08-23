#pragma once

#include "framework.h"
#include "Vec2.h"
#include <vector>
#include "Actor.h"
#include "Wall.h"
#include "Ground.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "Bullet.h"
#include "Keyboard.h"
#include <random>

class Scene
{
public:
	Scene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct )
		:
		sceneWidth( sceneWidth ),
		sceneHeight( sceneHeight ),
		sceneTopLeft( 0, 0 ),
		sceneBottomRight( sceneWidth, sceneHeight ),
		cam( ct ),
		rng(rd())
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
	const std::vector<std::unique_ptr<Ground>>& GetGroundPtrs() const
	{
		return groundPtrs;
	}

	std::vector<Actor*> FindActorByTag(ActorTag tag) const
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

	Actor* FindPlayer() const
	{
		for ( const auto& pActor : actorPtrs )
		{
			if ( pActor->IsTagSameWith( ActorTag::Player ) )
			{
				return pActor.get();
			}
		}
		return nullptr;
	}


	const std::unique_ptr<Actor>& FindPlayerPtr() const
	{
		for ( const auto& pActor : actorPtrs )
		{
			if ( pActor->IsTagSameWith( ActorTag::Player ) )
			{
				return pActor;
			}
		}
		return nullptr;
	}

	CollisionManager<float> GetCollisionManager() const 
	{
		return cm;
	}

	std::vector <std::unique_ptr<Bullet>>& AccessBulletPtrs()
	{
		return bulletPtrs;
	}

	void CollectingObjects()
	{
		UtilSH::remove_erase_if( actorPtrs, []( const auto& pActor ) {return pActor->ShouldDestroy(); } );
		UtilSH::remove_erase_if( bulletPtrs, []( const auto& pBullet ) {return pBullet->ShouldDestroy(); } );
	}

	std::mt19937& GetRandomEngine()
	{
		return rng;
	}

	int GetPlayerLife() const
	{
		return playerLife;
	}

	void DecreasePlayerLife()
	{
		if ( playerLife >= 0 )
		{
			--playerLife;
		}
	}

protected:
	void UpdateSceneRect( class Game& game );
protected:
	bool isSceneFinish = false;

	int sceneWidth;
	int sceneHeight;
	int playerLife = 02;
	
	RECT sceneRect = {0,0,0,0};
	Vec2<int> sceneTopLeft;
	Vec2<int> sceneBottomRight;


	Camera cam;
	CollisionManager<float> cm;

	std::random_device rd;
	std::mt19937 rng;

	std::vector<std::unique_ptr<Actor>> actorPtrs;
	std::vector<std::unique_ptr<Wall>> wallPtrs;
	std::vector<std::unique_ptr<Bullet>> bulletPtrs;
	std::vector<std::unique_ptr<Ground>> groundPtrs;
};

