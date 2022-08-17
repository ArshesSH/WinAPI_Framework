#include "ActorTestScene.h"

#include "Game.h"
#include "PlayerX.h"

ActorTestScene::ActorTestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct )
	:
	Scene( sceneWidth, sceneHeight ),
	cam( ct )
{
	actorPtrs.emplace_back( std::make_unique<PlayerX>( Vec2<float>{ 0.0f, 0.0f }, Vec2<float>{ 30.0f, 100.0f } ) );
}

void ActorTestScene::Update( float dt, Game & game )
{
	for ( auto& pActor : actorPtrs )
	{
		pActor->Update( dt, *this );
	}
}

void ActorTestScene::Draw( HDC hdc )
{
	for ( auto& pActor : actorPtrs )
	{
		pActor->Draw( hdc );
	}
}
