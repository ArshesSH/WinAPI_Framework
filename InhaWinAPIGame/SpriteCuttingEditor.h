#pragma once

#include "framework.h"
#include "Image.h"
#include "EditorFrame.h"
#include "DrawManager.h"
#include "FrameTimer.h"
#include "Surface.h"
#include "Vec2.h"

#include "CoordinateTransformer.h"
#include "Camera.h"
#include "Rect.h"

class SpriteCuttingEditor : public EditorFrame
{
public:
    enum class Mode
    {
        GDI,
        GDIPlus
    };
public:
    using ImagePlus = Image::ImageGDIPlus<int>;
    using ImageGDI = Image::ImageGDI<int>;

    SpriteCuttingEditor( HWND& hWnd, HINSTANCE hInst, UINT dialogNum, DLGPROC dialogFunc )
        :
        EditorFrame( hWnd, hInst, dialogNum, dialogFunc ),
        ct(Mat3<float>::Identity()),
        cam( ct ),
        halfMainWndSize( (float)(mainWndSize.cx / 2), (float)(mainWndSize.cy / 2) )
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
        if ( GetAsyncKeyState( 'R' ) & 0x8001 )
        {
            if ( isImageSet )
            {
                switch ( mode )
                {
                case SpriteCuttingEditor::Mode::GDI:
                    cam.SetPos( { 0.0f, (float)pSpriteGdi->GetImageSize().y } );
                    break;
                case SpriteCuttingEditor::Mode::GDIPlus:
                    cam.SetPos( { 0.0f, (float)pSpriteGdiPlus->GetImageSize().y } );
                    break;
                }
            }
            
        }
        const auto camPos = cam.GetPos();
        const auto camZoom = cam.GetScale();
        camPosStr = L"CamPos : (" + std::to_wstring( camPos.x ) + L", " + std::to_wstring( camPos.y ) + L")";
        camZoomStr = L"Zoom : " + std::to_wstring( camZoom );
        mousePosStr = L"MousePos : (" + std::to_wstring( mousePos.x ) + L", " + std::to_wstring( mousePos.y ) + L")";

        /*************************/
        /*  Update Mouse Click   */
        /*************************/
        if ( isSelectStart )
        {
            selectEndPos = mousePos;
            selectRect = { selectStartPos, selectEndPos };
        }
    }

    void CaptureWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) override
    {
        switch ( message )
        {
            case WM_MOUSEMOVE:
            {
                mousePos.x = LOWORD( lParam );
                mousePos.y = HIWORD( lParam );
                mousePos = Vec2<int>( (Mat3<float>::ScaleIndependent( 1.0f, -1.0f ) * (Vec2<float>( mousePos ) - halfMainWndSize) ) / cam.GetScale() + cam.GetPos()   );
            }
            break;

            case WM_LBUTTONDOWN:
            {
                if ( !isSelectStart )
                {
                    isSelectStart = true;
                    selectStartPos = mousePos;
                }
            }
            break;

            case WM_LBUTTONUP:
            {
                if ( isSelectStart )
                {
                    selectEndPos = mousePos;
                    isSelectStart = false;
                    shouldCompactRect = true;
                    selectRect = { selectStartPos, selectEndPos };
                    
                 
                }
            }
            break;

        }
    }

    void CaptureBottomWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) override
    {
    }

    void CaptureMenuProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) override
    {

    }

    void Draw( HDC hdc ) override
    {
        DrawMainWnd( hdc );
    }

    void FileOpen() override
    {
        EditorFrame::FileOpen();
        auto fileExtension = PathFindExtension( fileName );
        
        if ( wcscmp( fileExtension, L".bmp" ) == 0 )
        {
            mode = Mode::GDI;
        }
        else
        {
            mode = Mode::GDIPlus;
        }

        switch ( mode )
        {
        case SpriteCuttingEditor::Mode::GDI:
            pSpriteGdi = std::make_unique<ImageGDI>( this->fileName );
            break;
        case SpriteCuttingEditor::Mode::GDIPlus:
            pSpriteGdiPlus = std::make_unique<ImagePlus>( this->fileName );
            break;
        }
        isImageSet = true;
    }
    void FileSave() override
    {
        EditorFrame::FileSave();
    }

    void DrawMainWnd( HDC hdc )
    {
        if ( isImageSet )
        {
            drawManagerMain.DrawMain( hdc, this->clientRect, isClientSizeChanged,
                [this]( HDC hdc )
                {
                    Gdiplus::Graphics gfx( hdc );
                    cam.Draw( hdc, { halfMainWndSize.x, halfMainWndSize.y },
                        [&]( HDC hdc, const Mat3<float>& camTransform )
                        {
                            surf.SetTransformation( camTransform );

                            switch ( mode )
                            {
                            case SpriteCuttingEditor::Mode::GDI:
                                DrawImageNonChromaGDILocal( hdc, pSpriteGdi->GetHBitmap(), { 0, 0 }, pSpriteGdi->GetImageSize(),
                                    { 0, 0 }, pSpriteGdi->GetImageSize() );
                                break;
                            case SpriteCuttingEditor::Mode::GDIPlus:
                                surf.DrawImageNonChromaPlus( gfx, pSpriteGdiPlus->GetImagePtr(), { 0, 0 }, pSpriteGdiPlus->GetImageSize(),
                                    { 0, 0 }, pSpriteGdiPlus->GetImageSize() );
                                break;
                            }
                
                            // Draw mouse rect
                            //surf.DrawRectGDI( hdc, selectRect, RGB( 0, 255, 0 ), 1 );
                            surf.DrawRectPlus( gfx, selectRect, { 255,0,255,0 }, 1 );
                        }
                    );
                }
            );
        }
    }

    void DrawBottomWnd( HDC hdc ) override
    {
        if ( isImageSet )
        {
            drawManagerSub.DrawMain( hdc, { 0,0,bottomWndSize.cx, bottomWndSize.cy }, isClientSizeChanged,
                [this]( HDC hdc )
                {
                    screenSurf.DrawStringGDI( hdc, { 0,0 }, camPosStr );
                    screenSurf.DrawStringGDI( hdc, { 0,20 }, camZoomStr );
                    screenSurf.DrawStringGDI( hdc, { 0,40 }, mousePosStr );
                }
            );
        }
    }

private:
    void FindMinRect(HDC hSrcImageDC, COLORREF chroma)
    {
        //selectRect.top = FindTopPos( hSrcImageDC, chroma );
        selectRect.left = FindLeftPos( hSrcImageDC, chroma );
        //selectRect.right = FindRightPos( hSrcImageDC, chroma );
        // Find Left

        shouldCompactRect = false;
    }

    int FindTopPos(HDC hSrcImageDC, COLORREF chroma )
    {
        for ( int y = selectRect.top; y < selectRect.bottom; ++y )
        {
            for ( int x = selectRect.left; x < selectRect.right; ++x )
            {
                const COLORREF curColor = GetPixel( hSrcImageDC, x, y );
                int r = (curColor >> 16u) & 0xFFu;
                int g = (curColor >> 8u) & 0xFFu;
                int b = curColor & 0xFFu;
                if ( curColor != chroma )
                {
                    return y;
                }
            }
        }
    }
    int FindLeftPos( HDC hSrcImageDC, COLORREF chroma )
    {
  
        for ( int x = selectRect.left; x < selectRect.right; ++x )
        {
            for ( int y = selectRect.top; y < selectRect.bottom; ++y )
            {

                const COLORREF curColor = GetPixel( hSrcImageDC, x, y );
                int r = (curColor >> 16u) & 0xFFu;
                int g = (curColor >> 8u) & 0xFFu;
                int b = curColor & 0xFFu;
                if ( curColor != chroma )
                {
                    return x;
                }
            }
        }
    }
    int FindRightPos( HDC hSrcImageDC, COLORREF chroma )
    {

        for ( int x = selectRect.right; x > selectRect.left; --x )
        {
            for ( int y = selectRect.top; y < selectRect.bottom; ++y )
            {

                const COLORREF curColor = GetPixel( hSrcImageDC, x, y );
                int r = (curColor >> 16u) & 0xFFu;
                int g = (curColor >> 8u) & 0xFFu;
                int b = curColor & 0xFFu;
                if ( curColor != chroma )
                {
                    return x;
                }
            }
        }
    }

    void DrawImageNonChromaGDILocal( HDC hdc, const HBITMAP& hBitmap, const Vec2<int>& topLeft, const Vec2<int>& size,
        const Vec2<int>& imageStart, const Vec2<int>& imageEnd )
    {
        HDC hMemDC = CreateCompatibleDC( hdc );
        HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

        auto tl = surf.GetTransformation() * Vec2<float>( topLeft );
        auto br = surf.GetTransformation() * Vec2<float>( topLeft + size );
        if ( tl.y > br.y )
        {
            std::swap( tl.y, br.y );
        }
        const auto sizeT = br - tl;



        StretchBlt( hdc, (int)tl.x, (int)tl.y, (int)sizeT.x, (int)sizeT.y,
            hMemDC, (int)imageStart.x, (int)imageStart.y, (int)imageEnd.x, (int)imageEnd.y, SRCCOPY );

        if ( shouldCompactRect )
        {
            FindMinRect( hMemDC, RGB( 255, 0, 255 ) );
        }

        SelectObject( hMemDC, hOldBitmap );
        DeleteObject( hMemDC );
    }
	
private:
    DrawManager drawManagerMain;
    DrawManager drawManagerSub;
    bool isClientSizeChanged = false;
    FrameTimer ft;
    Mode mode = Mode::GDI;
    bool isImageSet = false;

    CoordinateTransformer ct;
    Camera cam;
    std::wstring camPosStr;
    std::wstring camZoomStr;
    Vec2<int> mousePos;
    std::wstring mousePosStr;

    bool isSelectStart = false;
    bool shouldCompactRect = false;
    Vec2<int> selectStartPos;
    Vec2<int> selectEndPos;
    RectI selectRect;

	const Vec2<float> dirLeft = { -1.0f, 0.0f };
	const Vec2<float> dirUp = { 0.0f, 1.0f };
	const Vec2<float> dirRight = { 1.0f, 0.0f };
	const Vec2<float> dirDown = { 0.0f, -1.0f };
    
    const Vec2<float> halfMainWndSize;

    std::unique_ptr<ImagePlus> pSpriteGdiPlus;
    std::unique_ptr<ImageGDI> pSpriteGdi;

    Surface<int> surf;
    Surface<float> screenSurf;
};