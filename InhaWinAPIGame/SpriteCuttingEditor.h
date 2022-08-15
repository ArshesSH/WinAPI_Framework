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
#include "Animation.h"
#include "PivotGizmo.h"

class SpriteCuttingEditor : public EditorFrame
{
public:
    enum class SpriteType
    {
        GDI,
        GDIPlus
    };
    enum class Mode
    {
        Select,
        Chroma
    };
public:
    using ImagePlus = Image::ImageGDIPlus<int>;
    using ImageGDI = Image::ImageGDI<int>;

    SpriteCuttingEditor( HWND& hWnd, HINSTANCE hInst, UINT dialogNum, DLGPROC dialogFunc )
        :
        EditorFrame( hWnd, hInst, dialogNum, dialogFunc ),
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
            if ( cam.GetScale() <= 0.1f )
            {
                cam.SetScale( 0.1f );
            }
            
        }
        else if ( GetAsyncKeyState( 'E' ) & 0x8001 )
        {
            cam.SetScale( cam.GetScale() + (2.0f * dt) );
        }
        if ( GetAsyncKeyState( 'F' ) & 0x8001 )
        {
            if ( isImageSet )
            {
                switch ( spriteType )
                {
                case SpriteCuttingEditor::SpriteType::GDI:
                    cam.SetPos( { 0.0f, (float)pSpriteGdi->GetImageSize().y } );
                    break;
                case SpriteCuttingEditor::SpriteType::GDIPlus:
                    cam.SetPos( { 0.0f, (float)pSpriteGdiPlus->GetImageSize().y } );
                    break;
                }
            }
        }
        if ( GetAsyncKeyState( 'R' ) & 0x8001 )
        {
            if ( isImageSet )
            {
                cam.SetScale( 1.0f );
            }
        }
        if ( GetAsyncKeyState( 'T' ) & 0x0001 )
        {
            if ( isImageSet )
            {
                curFrame.pivot = CalcPivotFromNDC( curFrame.sprite );
                curPivotGizmo.SetPos( CalcPivotCoordFromNDC( selectRect ) );
                isDrawPivot = true;
            }
        }
        if ( GetAsyncKeyState( 'G' ) & 0x0001 )
        {
            if ( isImageSet )
            {
                frames.push_back( curFrame );
                listStr = L"R:" + std::to_wstring( curFrame.sprite.left ) + L"," + std::to_wstring( curFrame.sprite.top ) + L"|" + std::to_wstring( curFrame.sprite.right ) + L"," + std::to_wstring( curFrame.sprite.bottom ) + L" / " + pivotStr;
                SendMessage( hList, LB_ADDSTRING, 0, (LPARAM)listStr.c_str() );
            }
        }

        const auto camPos = cam.GetPos();
        const auto camZoom = cam.GetScale();
        camPosStr = L"CamPos : (" + std::to_wstring( camPos.x ) + L", " + std::to_wstring( camPos.y ) + L")";
        camZoomStr = L"Zoom : " + std::to_wstring( camZoom );
        mousePosStr = L"MousePos : (" + std::to_wstring( mousePos.x ) + L", " + std::to_wstring( mousePos.y ) + L")";
        transMousePosStr = L"ImageMousePos : (" + std::to_wstring( imageBasedMousePos.x ) + L", " + std::to_wstring( imageBasedMousePos.y ) + L")";
        chromaStr = L"Chroma : (" + std::to_wstring( GetRValue( chroma ) ) + L"," + std::to_wstring( GetGValue( chroma ) ) +
            L"," + std::to_wstring( GetBValue( chroma ) ) + L")";
        pivotStr = L"Pivot : (" + std::to_wstring( curFrame.pivot.x ) + L"," + std::to_wstring( curFrame.pivot.y ) + L")";
        if ( pPlayAnim )
        {
            animFrameStr = L"Frame : " + std::to_wstring( pPlayAnim->GetFrameIndex() );
        }

        /*************************/
        /*  Update Mouse Click   */
        /*************************/
        if ( isSelectRect )
        {
            selectEndPos = mousePos;
            selectRect = { selectStartPos, selectEndPos };
        }

        /*************************/
        /*      Update Image     */
        /*************************/
        if ( isImageSet )
        {
            switch ( spriteType )
            {
            case SpriteCuttingEditor::SpriteType::GDI:
                imageSize = pSpriteGdi->GetImageSize();
                break;
            case SpriteCuttingEditor::SpriteType::GDIPlus:
                imageSize = pSpriteGdiPlus->GetImageSize();
                break;
            }
        }

        /*************************/
        /*      Update Anime     */
        /*************************/
        if ( isPlayAnimation )
        {
            float dt = ft.Mark();
            pPlayAnim->Update( dt );
        }
    }

    void InitWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) override
    {
    }
    void InitBottomProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) override
    {
    }
    void InitMenuProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) override
    {
        CheckRadioButton( hWnd, IDC_RADIO_Select, IDC_RADIO_Pick, IDC_RADIO_Pick );
        hList = GetDlgItem( hWnd, IDC_LIST_Animation );
        this->isMenuInit = true;
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
                // Select Mode
                if ( mode == Mode::Select )
                {
                    isSelectRect = true;
                    selectStartPos = mousePos;
                }
                else
                {
                    isChromaPicked = true;
                    shouldChangeChromaDlg = true;
                }
            }
            break;

        case WM_LBUTTONUP:
            {
                if ( mode == Mode::Select )
                {
                    selectEndPos = mousePos;
                    isSelectRect = false;
                    shouldCompactRect = true;
                    selectRect = { selectStartPos, selectEndPos };
                    selectImageRect = { (coordMatI * selectStartPos).AddToY( imageSize.y ), (coordMatI * selectEndPos).AddToY( imageSize.y ) };
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
        if ( !this->isMenuInit )
        {
            InitMenuProc( hWnd, message, wParam, lParam );
        }

        if ( shouldChangeChromaDlg )
        {
            const std::wstring chromaR = L"R: " + std::to_wstring( GetRValue( chroma ) );
            const std::wstring chromaG = L"G: " + std::to_wstring( GetGValue( chroma ) );
            const std::wstring chromaB = L"B: " + std::to_wstring( GetBValue( chroma ) );
            SetDlgItemText( hWnd, IDC_STATIC_ChromaR, chromaR.c_str() );
            SetDlgItemText( hWnd, IDC_STATIC_ChromaG, chromaG.c_str() );
            SetDlgItemText( hWnd, IDC_STATIC_ChromaB, chromaB.c_str() );
            shouldChangeChromaDlg = false;
        }

        switch ( message )
        {
        case WM_COMMAND:
            {
                if ( isImageSet )
                {
                    switch ( LOWORD( wParam ) )
                    {
                    case IDC_RADIO_Pick:
                        mode = Mode::Chroma;
                        break;
                    case IDC_RADIO_Select:
                        mode = Mode::Select;
                        break;
                    case IDC_BUTTON_Pivot:
                        {
                            TCHAR xChar[256] = L"";
                            TCHAR yChar[256] = L"";
                            GetDlgItemText( hWnd, IDC_EDIT_PivotX, xChar, 256 );
                            GetDlgItemText( hWnd, IDC_EDIT_PivotY, yChar, 256 );

                            if ( xChar[0] == '\0' && yChar[0] == '\0' )
                            {
                                pivotNDC = { 0.5f, 1.0f };
                            }
                            else
                            {
                                pivotNDC = { float( _wtof( xChar ) ), float( _wtof( yChar ) ) };
                            }

                            curFrame.pivot = CalcPivotFromNDC( curFrame.sprite );
                            curPivotGizmo.SetPos( CalcPivotCoordFromNDC( selectRect ) );
                            isDrawPivot = true;
                        }
                        break;
                    case IDC_BUTTON_Add:
                        {
                            frames.push_back( curFrame );
                            listStr = L"R:" + std::to_wstring( curFrame.sprite.left ) + L"," + std::to_wstring( curFrame.sprite.top ) + L"|" + std::to_wstring( curFrame.sprite.right ) + L"," + std::to_wstring( curFrame.sprite.bottom ) + L" / " + pivotStr;
                            SendMessage( hList, LB_ADDSTRING, 0, (LPARAM)listStr.c_str() );
                        }
                        break;
                    case IDC_LIST_Animation:
                        {
                            if ( HIWORD( wParam ) == LBN_SELCHANGE )
                            {
                                listSelectIdx = (int)SendMessage( hList, LB_GETCURSEL, 0, 0 );
                            }
                        }
                        break;
                    case IDC_BUTTON_Delete:
                        {
                            if ( !frames.empty() && listSelectIdx <= frames.size() - 1 )
                            {
                                frames.erase( frames.begin() + listSelectIdx );
                                SendMessage( hList, LB_DELETESTRING, listSelectIdx, 0 );
                            }
                        }
                        break;
                    case IDC_BUTTON_Insert:
                        {
                            if ( listSelectIdx <= frames.size() - 1 )
                            {
                                frames.insert( frames.begin() + listSelectIdx + 1, curFrame );
                                listStr = L"R:" + std::to_wstring( curFrame.sprite.left ) + L"," + std::to_wstring( curFrame.sprite.top ) + L"|" + std::to_wstring( curFrame.sprite.right ) + L"," + std::to_wstring( curFrame.sprite.bottom ) + L" / " + pivotStr;
                                SendMessage( hList, LB_INSERTSTRING, listSelectIdx + 1, (LPARAM)listStr.c_str() );
                            }
                        }
                        break;

                    case IDC_BUTTON_Edit:
                        {
                            if ( !frames.empty() && listSelectIdx <= frames.size() - 1 )
                            {
                                TCHAR xChar[256] = L"";
                                TCHAR yChar[256] = L"";
                                GetDlgItemText( hWnd, IDC_EDIT_PivotX, xChar, 256 );
                                GetDlgItemText( hWnd, IDC_EDIT_PivotY, yChar, 256 );
                                if ( xChar[0] == '\0' && yChar[0] == '\0' )
                                {
                                    pivotNDC = { 0.5f, 1.0f };
                                }
                                else
                                {
                                    pivotNDC = { float( _wtof( xChar ) ), float( _wtof( yChar ) ) };
                                }
                                frames[listSelectIdx].pivot = CalcPivotFromNDC( frames[listSelectIdx].sprite );
                            }
                        }
                        break;

                    case IDC_BUTTON_Play:
                        {
                            switch ( spriteType )
                            {
                            case SpriteCuttingEditor::SpriteType::GDI:
                                pPlayAnim = std::make_unique<Animation<int>>( Animation<int>::SpriteType::GDI, fileName, frames, playSpeed );
                                break;
                            case SpriteCuttingEditor::SpriteType::GDIPlus:
                                pPlayAnim = std::make_unique<Animation<int>>( Animation<int>::SpriteType::GDIPlus, fileName, frames, playSpeed );
                                break;
                            }
                            isPlayAnimation = true;
                        }
                        break;

                    case IDC_BUTTON_Stop:
                        {
                            isPlayAnimation = false;
                        }
                        break;

                    case IDC_BUTTON_ApplyScale:
                        {
                            TCHAR scaleStr[256] = L"";
                            GetDlgItemText( hWnd, IDC_EDIT_PlayScale, scaleStr, 256 );
                            playScale = _wtof( scaleStr );
                            if ( playScale < 0.5f )
                            {
                                playScale = 0.5f;
                            }
                        }
                        break;

                    case IDC_BUTTON_ApplySpeed:
                        {
                            TCHAR speedStr[256] = L"";
                            GetDlgItemText( hWnd, IDC_EDIT_PlaySpeed, speedStr, 256 );
                            playSpeed = _wtof( speedStr );
                            if ( playSpeed < 0.0f )
                            {
                                playSpeed = 0.0f;
                            }
                            pPlayAnim->SetPlaySpeed( playSpeed );
                        }
                        break;
                    }
                }
            }
            break;
        }
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
            spriteType = SpriteType::GDI;
        }
        else
        {
            spriteType = SpriteType::GDIPlus;
        }

        switch ( spriteType )
        {
        case SpriteCuttingEditor::SpriteType::GDI:
            pSpriteGdi = std::make_unique<ImageGDI>( this->fileName );
            break;
        case SpriteCuttingEditor::SpriteType::GDIPlus:
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

                            switch ( spriteType )
                            {
                            case SpriteCuttingEditor::SpriteType::GDI:
                                DrawImageNonChromaGDILocal( hdc, pSpriteGdi->GetHBitmap(), { 0, 0 }, pSpriteGdi->GetImageSize(),
                                    { 0, 0 }, pSpriteGdi->GetImageSize() );
                                break;
                            case SpriteCuttingEditor::SpriteType::GDIPlus:
                                surf.DrawImageNonChromaPlus( gfx, pSpriteGdiPlus->GetImagePtr(), { 0, 0 }, pSpriteGdiPlus->GetImageSize(),
                                    { 0, 0 }, pSpriteGdiPlus->GetImageSize() );
                                break;
                            }
                
                            // Draw mouse rect
                            surf.DrawRectGDI( hdc, selectRect, RGB(0,255,0), 1 );

                            if ( isDrawPivot )
                            {
                                curPivotGizmo.SetTransform( camTransform );
                                curPivotGizmo.Draw( hdc );
                            }
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
                    screenSurf.DrawStringGDI( hdc, { 0,80 }, chromaStr );
                    screenSurf.DrawStringGDI( hdc, { 0,100 }, pivotStr );


                    if ( isPlayAnimation )
                    {
                        screenSurf.DrawStringGDI( hdc, { 0,120 }, animFrameStr );
                        switch ( spriteType )
                        {
                        case SpriteCuttingEditor::SpriteType::GDI:
                            pPlayAnim->PlayGDI( hdc, { 400,100 }, playScale, chroma );
                            break;
                        case SpriteCuttingEditor::SpriteType::GDIPlus:
                            break;
                        }
                    }

                }
            );
        }
    }

    void SetListHandle( HWND hWnd )
    {
        hList = hWnd;
    }

private:
    Vec2<int> CalcPivotFromNDC( const RectI& rect ) const
    {
        return Vec2<int>( int( rect.GetWidth() * pivotNDC.x ), int( rect.GetHeight() * pivotNDC.y ) );
    }
    Vec2<int> CalcPivotCoordFromNDC( const RectI& rect ) const
    {
        return rect.GetTopLeft() + Vec2<int>( int( rect.GetWidth() * pivotNDC.x ), int( rect.GetHeight() * pivotNDC.y ) );
    }

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

        curFrame.sprite = selectImageRect;

        shouldCompactRect = false;
    }
    int FindTopPos(HDC hSrcImageDC, COLORREF chroma ) const 
    {
        for ( int y = selectImageRect.top; y < selectImageRect.bottom; ++y )
        {
            for ( int x = selectImageRect.left; x < selectImageRect.right; ++x )
            {
                const COLORREF curColor = GetPixel( hSrcImageDC, x, y );
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
        for ( int x = selectImageRect.right - 1; x > selectImageRect.left; --x )
        {
            for ( int y = selectImageRect.top; y < selectImageRect.bottom; ++y )
            {

                const COLORREF curColor = GetPixel( hSrcImageDC, x, y );
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


        /****************************/
        /*      Do Pixel Things     */
        /****************************/

        // Compact
        if ( shouldCompactRect )
        {
            FindMinRect( hMemDC, chroma );
        }

        // Get Chroma
        if ( isChromaPicked )
        {
            chroma = GetPixel( hMemDC, imageBasedMousePos.x, imageBasedMousePos.y );
            isChromaPicked = false;
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
    SpriteType spriteType = SpriteType::GDI;
    bool isImageSet = false;

    // Camera
    CoordinateTransformer ct;
    Camera cam;
    
    // Mouse
    Vec2<int> mousePos;
    Vec2<int> imageBasedMousePos;

    // Select Rect
    bool isSelectRect = false;
    bool shouldCompactRect = false;
    Vec2<int> selectStartPos;
    Vec2<int> selectEndPos;
    RectI selectRect;
    RectI selectImageRect;

    // Select Chorma
    bool isChromaPicked = false;
    bool shouldChangeChromaDlg = false;

    // Sprite
    std::unique_ptr<ImagePlus> pSpriteGdiPlus;
    std::unique_ptr<ImageGDI> pSpriteGdi;
    Vec2<int> imageSize = { 0,0 };

    // Surface
    Surface<int> surf;
    Surface<float> screenSurf;
    
    // Animation
    Animation<int>::Frame curFrame;
    std::vector<Animation<int>::Frame> frames;
    std::unique_ptr<Animation<int>> pPlayAnim;
    float playSpeed = 0.2f;
    float playScale = 1.0f;

    // Dialog
    Mode mode = Mode::Chroma;
    Vec2<float> pivotNDC = { 0.5f, 1.0f };
    COLORREF chroma = RGB(255, 0, 255);
    PivotGizmo curPivotGizmo;
    bool isDrawPivot = false;
    HWND hList;
    int listSelectIdx = 0;
    bool isPlayAnimation = false;

    // DebugStr
    std::wstring camPosStr;
    std::wstring camZoomStr;
    std::wstring mousePosStr;
    std::wstring transMousePosStr;
    std::wstring chromaStr;
    std::wstring pivotStr;
    std::wstring listStr;
    std::wstring animFrameStr;
};