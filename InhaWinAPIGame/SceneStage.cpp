#include "SceneStage.h"

#include "Game.h"
#include "UtilSH.h"

SceneStage::SceneStage()
	:
	Scene((int)worldWidth, (int)worldHeight),
	player( { 0,0 },  20, 20),
	playerArea( { sceneRect.left, sceneRect.right, (int)worldWidth, (int)worldHeight} )
{
}

void SceneStage::Update( float dt, Game& game )
{
	UpdateSceneRect( game );

	playerArea.Update( dt, *this );
	player.Update( dt, *this );
	player.ControlPlayer( dt, playerArea );
}

void SceneStage::Draw( HDC hdc )
{
	Gdiplus::Graphics graphics( hdc );
	Surface::DrawImageNonChroma( graphics, pBackImage.get(), sceneTopLeft, sceneBottomRight, { 0,0 }, imageEnd );
	Surface::DrawRect( graphics, Gdiplus::Color( 255, 255, 0, 255 ), worldBorderThick,
		{ sceneRect.left - worldBorderThick * 0.5f,
		sceneRect.top - worldBorderThick * 0.5f,
		worldWidth + worldBorderThick,
		worldHeight + worldBorderThick }
	);
	playerArea.Draw( graphics );

	player.Draw( graphics );
}