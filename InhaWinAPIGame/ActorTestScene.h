#pragma once

#include "Scene.h"
#include "Camera.h"

class ActorTestScene : public Scene
{
public:
	ActorTestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct );

	void Update( float dt, class Game& game ) override;
	void Draw( HDC hdc ) override;

private:

};