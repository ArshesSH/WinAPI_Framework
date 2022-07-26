#pragma once

#include "Scene.h"
#include "Surface.h"
#include "Polygon.h"
#include <string>

class TestTriangulationScene : Scene
{
public:
	TestTriangulationScene()
		:
		Scene( worldWidth, worldHeight )
	{
		testPolygon.vertices.push_back( { 0,0 } );
		testPolygon.vertices.push_back( { 250,0 } );
		testPolygon.vertices.push_back( { 250,100 } );
		testPolygon.vertices.push_back( { 300,100 } );
		testPolygon.vertices.push_back( { 300,120 } );
		testPolygon.vertices.push_back( { 200,120 } );
		testPolygon.vertices.push_back( { 100,120 } );
		testPolygon.vertices.push_back( { 100,300 } );
		testPolygon.vertices.push_back( { 300,300 } );
		testPolygon.vertices.push_back( { 500,300 } );
		testPolygon.vertices.push_back( { 500,500 } );
		testPolygon.vertices.push_back( { 400,500 } );
		testPolygon.vertices.push_back( { 400,400 } );
		testPolygon.vertices.push_back( { 300,400 } );
		testPolygon.vertices.push_back( { 300,500 } );
		testPolygon.vertices.push_back( { 0,500 } );
		testPolygon.vertices.push_back( { 0,200 } );
		testPolygon.vertices.push_back( { 50,200 } );
		testPolygon.vertices.push_back( { 50,260 } );
		testPolygon.vertices.push_back( { 0,260 } );

		//testPolygon.vertices.push_back( { 0,0 } );
		//testPolygon.vertices.push_back( { 500,0 } );
		//testPolygon.vertices.push_back( { 500,500 } );
		//testPolygon.vertices.push_back( { 0,500 } );
	}
	void Update( float dt, class Game& game ) override
	{
		
	}
	void Draw( HDC hdc ) override
	{
		Gdiplus::Graphics graphics( hdc );
		Surface::DrawFillPolygon( graphics, { 255,255,0,255 }, testPolygon.vertices[0], int( testPolygon.size() ) );
		
		const std::wstring sizeStr = L"size = " + std::to_wstring( testPolygon.GetArea() );
		Surface::DrawString( graphics, sizeStr, { 0,0 }, { 255,255,255,255 } );
	}

private:

private:
	static const int worldWidth = 500;
	static const int worldHeight = 500;

	ArshesSH::Polygon<Gdiplus::Point> testPolygon;
};