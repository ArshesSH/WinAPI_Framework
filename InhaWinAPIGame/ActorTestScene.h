#pragma once

#include "Scene.h"
#include "Camera.h"
#include "PlayerXHUD.h"

class ActorTestScene : public Scene
{
public:
	ActorTestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct );

	void Update( float dt, class Game& game ) override;
	void Draw( HDC hdc ) override;

private:
	void MoveCamera( float dt )
	{
        if ( GetAsyncKeyState( 'A' ) & 0x8001 )
        {
            cam.MoveBy( dirLeft * dt * 1500 );
        }
        else if ( GetAsyncKeyState( 'D' ) & 0x8001 )
        {
            cam.MoveBy( dirRight * dt * 1500 );
        }
        if ( GetAsyncKeyState( 'W' ) & 0x8001 )
        {
            cam.MoveBy( dirUp * dt * 600 );
        }
        else if ( GetAsyncKeyState( 'S' ) & 0x8001 )
        {
            cam.MoveBy( dirDown * dt * 600 );
        }

        if ( GetAsyncKeyState( 'Q' ) & 0x8001 )
        {
            cam.SetScale( cam.GetScale() - (2.0f * dt) );
            if ( cam.GetScale() <= 0.1f )
            {
                cam.SetScale( 0.1f );
            }

        }
        else if ( GetAsyncKeyState( 'E' ) & 0x8001 )
        {
            cam.SetScale( cam.GetScale() + (2.0f * dt) );
        }

        const auto& pPlayer = FindPlayerPtr();
        if ( cam.GetScreenRect( sceneWidth, sceneHeight ).left >= 0.0f )
        {
            cam.SetPos( { pPlayer->GetPos().x, cam.GetPos().y } );
        }
        

	}

private:
    static constexpr int playerXMaxHP = 20;

    const Vec2<float> dirLeft = { -1.0f, 0.0f };
    const Vec2<float> dirUp = { 0.0f, 1.0f };
    const Vec2<float> dirRight = { 1.0f, 0.0f };
    const Vec2<float> dirDown = { 0.0f, -1.0f };

    Image::ImageGDI<int> stageImage;
    Surface<int> stageSurf;
    Vec2<int> stageSize;

    Keyboard kbd;
    PlayerXHUD xHUD;
};