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
	void MoveCamera( float dt )
	{
        if ( GetAsyncKeyState( 'A' ) & 0x8001 )
        {
            cam.MoveBy( dirLeft * dt * 200 );
        }
        else if ( GetAsyncKeyState( 'D' ) & 0x8001 )
        {
            cam.MoveBy( dirRight * dt * 200 );
        }
        if ( GetAsyncKeyState( 'W' ) & 0x8001 )
        {
            cam.MoveBy( dirUp * dt * 200 );
        }
        else if ( GetAsyncKeyState( 'S' ) & 0x8001 )
        {
            cam.MoveBy( dirDown * dt * 200 );
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
	}

private:
    const Vec2<float> dirLeft = { -1.0f, 0.0f };
    const Vec2<float> dirUp = { 0.0f, 1.0f };
    const Vec2<float> dirRight = { 1.0f, 0.0f };
    const Vec2<float> dirDown = { 0.0f, -1.0f };
};