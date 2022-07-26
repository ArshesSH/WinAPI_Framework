#include "Scene.h"

#include "Game.h"

void Scene::UpdateSceneRect( Game& game )
{
	sceneTopLeft = { (game.screenRect.right - sceneWidth) / 2, (game.screenRect.bottom - sceneHeight) / 2 };
	sceneBottomRight = { sceneTopLeft.x + sceneWidth, sceneTopLeft.y + sceneHeight };
	sceneRect = { (long)sceneTopLeft.x, (long)sceneTopLeft.y, (long)sceneBottomRight.x, (long)sceneBottomRight.y };
}
