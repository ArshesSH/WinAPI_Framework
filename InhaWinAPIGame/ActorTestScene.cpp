#include "ActorTestScene.h"

#include "Game.h"

ActorTestScene::ActorTestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct )
	:
	Scene( sceneWidth, sceneHeight ),
	cam( ct )
{}

void ActorTestScene::Update( float dt, Game & game )
{
}

void ActorTestScene::Draw( HDC hdc )
{
}
