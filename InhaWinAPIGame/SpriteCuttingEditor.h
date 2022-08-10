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
        //ct(Mat3<float>::Identity()),
        cam( ct ),
        halfMainWndSize( (float)(mainWndSize.cx / 2), (float)(mainWndSize.cy / 2) )
    {
    }

    void Update() override
    {
        float dt = ft.Mark();


        /*************************/
        /*      Update Camera    */
        /*************************/
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
        transMousePosStr = L"ImageMousePos : (" + std::to_wstring( imageBasedMousePos.x ) + L", " + std::to_wstring( imageBasedMousePos.y ) + L")";

        /*************************/
        /*  Update Mouse Click   */
        /*************************/
        if ( isSelectStart )
        {
            selectEndPos = mousePos;
            selectRect = { selectStartPos, selectEndPos };
            selectImageRect = { (coordMatI * selectStartPos).AddToY( imageSize.y ), (coordMatI * selectEndPos).AddToY( imageSize.y ) };
        }

        /*************************/
        /*      Update Image     */
        /*************************/
        if ( isImageSet )
        {
            switch ( mode )
            {
            case SpriteCuttingEditor::Mode::GDI:
                imageSize = pSpriteGdi->GetImageSize();
                break;
            case SpriteCuttingEditor::Mode::GDIPlus:
                imageSize = pSpriteGdiPlus->GetImageSize();
                break;
            }
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

                mousePos = Vec2<int>(
                    (coordMatF * (Vec2<float>( mousePos ) - halfMainWndSize)) / cam.GetScale() + cam.GetPos()
                    );

                imageBasedMousePos = (coordMatI * mousePos).AddToY( imageSize.y );

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
                            surf.DrawRectGDI( hdc, selectRect, RGB( 0, 255, 0 ), 1 );
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
                    screenSurf.DrawStringGDI( hdc, { 0,60 }, transMousePosStr );
                }
            );
        }
    }

private:
    void FindMinRect(HDC hSrcImageDC, COLORREF chroma)
    {
        selectImageRect.top = FindTopPos( hSrcImageDC, chroma );
        selectImageRect.left = FindLeftPos( hSrcImageDC, chroma );
        selectImageRect.right = FindRightPos( hSrcImageDC, chroma ) + 1;
        selectImageRect.bottom = FindBottomPos( hSrcImageDC, chroma ) + 1;

        const Vec2<int> topLeft = coordMatI * Vec2<int>( selectImageRect.left, selectImageRect.top ).AddToY(-imageSize.y);
        selectRect.left = topLeft.x;
        selectRect.top = topLeft.y;

        const Vec2<int> bottomRight = coordMatI * Vec2<int>( selectImageRect.right, selectImageRect.bottom ).AddToY( -imageSize.y );
        selectRect.right = bottomRight.x;
        selectRect.bottom = bottomRight.y;

        shouldCompactRect = false;
    }

    int FindTopPos(HDC hSrcImageDC, COLORREF chroma ) const 
    {
        for ( int y = selectImageRect.top; y < selectImageRect.bottom; ++y )
        {
            for ( int x = selectImageRect.left; x < selectImageRect.right; ++x )
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
        return selectImageRect.top;
    }
    int FindBottomPos( HDC hSrcImageDC, COLORREF chroma ) const
    {
        for ( int y = selectImageRect.bottom; y > selectImageRect.top; --y )
        {
            for ( int x = selectImageRect.left; x < selectImageRect.right; ++x )
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
        return selectImageRect.bottom;
    }
    int FindLeftPos( HDC hSrcImageDC, COLORREF chroma ) const
    {
        for ( int x = selectImageRect.left; x < selectImageRect.right; ++x )
        {
            for ( int y = selectImageRect.top; y < selectImageRect.bottom; ++y )
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
        return selectImageRect.left;
    }
    int FindRightPos( HDC hSrcImageDC, COLORREF chroma ) const
    {
        for ( int x = selectImageRect.right; x > selectImageRect.left; --x )
        {
            for ( int y = selectImageRect.top; y < selectImageRect.bottom; ++y )
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
        return selectImageRect.right;
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
    const Vec2<float> dirLeft = { -1.0f, 0.0f };
    const Vec2<float> dirUp = { 0.0f, 1.0f };
    const Vec2<float> dirRight = { 1.0f, 0.0f };
    const Vec2<float> dirDown = { 0.0f, -1.0f };
    const Vec2<float> halfMainWndSize;
    const Mat3<int> coordMatI = Mat3<int>::ScaleIndependent( 1, -1 );
    const Mat3<float> coordMatF = Mat3<float>::ScaleIndependent( 1.0f, -1.0f );

    DrawManager drawManagerMain;
    DrawManager drawManagerSub;
    bool isClientSizeChanged = false;
    FrameTimer ft;
    Mode mode = Mode::GDI;
    bool isImageSet = false;

    // Camera
    CoordinateTransformer ct;
    Camera cam;
    std::wstring camPosStr;
    std::wstring camZoomStr;
    
    // Mouse
    Vec2<int> mousePos;
    Vec2<int> imageBasedMousePos;
    std::wstring mousePosStr;
    std::wstring transMousePosStr;

    // Select Rect
    bool isSelectStart = false;
    bool shouldCompactRect = false;
    Vec2<int> selectStartPos;
    Vec2<int> selectEndPos;
    RectI selectRect;
    RectI selectImageRect;

    // Sprite
    std::unique_ptr<ImagePlus> pSpriteGdiPlus;
    std::unique_ptr<ImageGDI> pSpriteGdi;
    Vec2<int> imageSize = { 0,0 };

    // Surface
    Surface<int> surf;
    Surface<float> screenSurf;
};