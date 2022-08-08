#pragma once

#include "framework.h"
#include <commdlg.h>
#include <string>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

class EditorFrame
{
public:
    EditorFrame( HWND& hWnd, HINSTANCE hInst, UINT dialogNum, DLGPROC dialogFunc )
    {
        wndSize.cx = GetSystemMetrics( SM_CXSCREEN );
        wndSize.cy = GetSystemMetrics( SM_CYSCREEN );
        mainWndSize.cx = LONG(wndSize.cx * 0.75f);
        mainWndSize.cy = LONG((wndSize.cy - 30) * 0.75f);
        bottomWndSize.cx = mainWndSize.cx;
        bottomWndSize.cy = LONG( (wndSize.cy - 30) * 0.25f );

        MoveWindow( hWnd, 0, 0, mainWndSize.cx, mainWndSize.cy, TRUE );

        hBottomWnd = CreateWindow( L"BottomWnd", L"View2", WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE,
            0, mainWndSize.cy, bottomWndSize.cx, bottomWndSize.cy, hWnd, NULL, hInst, NULL );

        clientRect = { 0,0,mainWndSize.cx, mainWndSize.cy };
        AdjustWindowRect( &clientRect, WS_OVERLAPPEDWINDOW, TRUE );
        GetWindowRect( hMenuWnd, &clientRect );

        hMenuWnd = CreateDialog( hInst, MAKEINTRESOURCE( dialogNum ), hWnd, dialogFunc );
        MoveWindow( hMenuWnd, mainWndSize.cx, 0, menuDlgWidth, wndSize.cy, TRUE );
    }

    virtual ~EditorFrame()
    {
        DestroyWindow( hMenuWnd );
        DestroyWindow( hBottomWnd );
    }

    virtual void Update() = 0;
    virtual void Draw( HDC hdc )
    {

    }

    void FileOpenFrame( HWND& hWnd )
    {
        ZeroMemory( &ofn, sizeof( ofn ) );
        ofn.lStructSize = sizeof( ofn );
        ofn.hwndOwner = hWnd;
        ofn.lpstrTitle = L"������ �����ϼ���";
        ofn.lpstrFile = fileName;
        ofn.lpstrFilter = L"��� ����(*.*)\0*.*\0";
        ofn.nMaxFile = MAX_PATH;
        ofn.nMaxFileTitle = MAX_PATH;

        if ( GetOpenFileName( &ofn ) != 0 )
        {
            switch ( ofn.nFilterIndex )
            {
            case 1:
                {
                    if ( PathRelativePathTo( fileName, L"D:\\Project\\WindowAPIs\\WinAPI_Framework\\InhaWinAPIGame\\Image",
                        FILE_ATTRIBUTE_DIRECTORY, fileName, FILE_ATTRIBUTE_NORMAL ) )
                    {
                        std::wstring path = fileName;
                        fileRelativePath = (path.substr( 3, path.size() ));
                    }
                    else
                    {
                        fileRelativePath = fileName;
                        MessageBox( 0, L"File Path convert failed", L"error", 0 );
                    }
                    FileOpen();
                }
                break;
            default:
                break;
            }
        }
    }
    void FileSaveFrame( HWND& hWnd )
    {
        ZeroMemory( &ofn, sizeof( ofn ) );
        ofn.lStructSize = sizeof( ofn );
        ofn.hwndOwner = hWnd;
        ofn.lpstrTitle = L"������ ��ġ�� �����ϼ���";
        ofn.lpstrFile = fileName;
        ofn.lpstrFilter = L"��� ����(*.*)\0*.*\0";
        ofn.nMaxFile = MAX_PATH;
        ofn.nMaxFileTitle = MAX_PATH;
        if ( GetSaveFileName( &ofn ) != 0 )
        {
            switch ( ofn.nFilterIndex )
            {
            case 1:
                {
                    FileSave();
                }
                break;
            }
        }
    }

    void MoveEditor(HWND& hWnd)
    {
        GetWindowRect( hWnd, &clientRect );
        MoveWindow( hWnd, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom, TRUE );
        MoveWindow( hMenuWnd, clientRect.right, clientRect.top, menuDlgWidth, wndSize.cy, TRUE );
        MoveWindow( hBottomWnd, clientRect.left, clientRect.bottom, bottomWndSize.cx, bottomWndSize.cy, TRUE );
    }

protected:
    virtual void FileOpen()
    {
        MessageBox( 0, &fileRelativePath[0], L"��� ����", 0 );
    }
    virtual void FileSave()
    {
        MessageBox( 0, &fileName[0], L"��� ����", 0 );
    }

protected:
    OPENFILENAME ofn;
    //std::wstring fileName;
    TCHAR fileName[MAX_PATH];
    std::wstring fileRelativePath;

    HWND hMenuWnd;
    HWND hBottomWnd;

    SIZE wndSize;
    SIZE mainWndSize;
    SIZE bottomWndSize;
    static constexpr int menuDlgWidth = 300;
    RECT clientRect;
};