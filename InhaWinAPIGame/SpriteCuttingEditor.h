#pragma once

#include "framework.h"
#include "EditorFrame.h"
#include "DrawManager.h"
#include "FrameTimer.h"
#include "Surface.h"
#include "Vec2.h"

#include "CoordinateTransformer.h"
#include "Camera.h"

class SpriteCuttingEditor : public EditorFrame
{
public:
public:
    using ImagePlus = Image::ImageGDIPlus<float>;

    SpriteCuttingEditor( HWND& hWnd, HINSTANCE hInst, UINT dialogNum, DLGPROC dialogFunc )
        :
        EditorFrame( hWnd, hInst, dialogNum, dialogFunc ),
        cam( ct )
    {
    }

    void Update() override
    {
        float dt = ft.Mark();

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
		}
		else if ( GetAsyncKeyState( 'E' ) & 0x8001 )
		{
			cam.SetScale( cam.GetScale() + (2.0f * dt) );
		}

        const auto camPos = cam.GetPos();
        const auto camZoom = cam.GetScale();
        camPosStr = L"CamPos : (" + std::to_wstring(camPos.x) + L", " + std::to_wstring( camPos.y ) + L")";
        camZoomStr = L"Zoom : " +  std::to_wstring( camZoom );
    }

    void Draw( HDC hdc ) override
    {
        DrawMainWnd( hdc );
    }

    void FileOpen() override
    {
        EditorFrame::FileOpen();
        pSprite = std::make_unique<ImagePlus>( this->fileName );
        pSp2 = std::make_unique<Image::ImageGDI<float>>( L"Images/awsom.bmp" );
    }
    void FileSave() override
    {
        EditorFrame::FileSave();
    }

    void DrawMainWnd( HDC hdc )
    {
        if ( pSprite )
        {
            drawManager.DrawMain( hdc, this->clientRect, isClientSizeChanged,
                [this](HDC hdc)
                {
                    Gdiplus::Graphics gfx( hdc );
                    cam.Draw( hdc, { 0.0f, float( mainWndSize.cy ) },
                        [&]( HDC hdc, const Mat3<float>& camTransform )
                        {
                            surf.SetTransformation( camTransform );
                            //surf.DrawImageNonChromaPlus( gfx, pSprite->GetImagePtr(), { 0.0f,0.0f }, pSprite->GetImageSize(), { 0.0f,0.0f }, pSprite->GetImageSize() );
                            surf.DrawImageNonChromaGDI( hdc, pSp2->GetHBitmap(), { 0.0f, 0.0f }, pSp2->GetImageSize(), { 0.0f, 0.0f }, pSp2->GetImageSize() );
                        }
                    );
                    
                    screenSurf.DrawStringGDI( hdc, { 0,0 }, camPosStr );
                    screenSurf.DrawStringGDI( hdc, { 0,20 }, camZoomStr );
                }
            );
        }
    }


	
private:
    DrawManager drawManager;
    bool isClientSizeChanged = false;
    FrameTimer ft;

    CoordinateTransformer ct;
    Camera cam;
    std::wstring camPosStr;
    std::wstring camZoomStr;

	const Vec2<float> dirLeft = { -1.0f, 0.0f };
	const Vec2<float> dirUp = { 0.0f, 1.0f };
	const Vec2<float> dirRight = { 1.0f, 0.0f };
	const Vec2<float> dirDown = { 0.0f, -1.0f };

    std::unique_ptr<ImagePlus> pSprite;
    std::unique_ptr<Image::ImageGDI<float>> pSp2;
    
    Surface<float> surf;
    Surface<float> screenSurf;
};