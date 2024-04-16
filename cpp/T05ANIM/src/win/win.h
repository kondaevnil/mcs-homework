/* FILENAME:    win.h
 * LAST UPDATE: 21.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     WinAPI classes declaration.
 */

#ifndef __win_h_
#define __win_h_

//#include "../mth/mth.h"
//#include <commondf.h>

#include "../anim/timer.h"

namespace degl
{
  class win
  {
  protected:
    HWND hWnd;           // window handle
    INT W, H;            // window size
    HINSTANCE hInstance; // application handle
    INT MouseWheel = 0;

  private:
    static LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam );
    
     BOOL IsFullScreen;
     //RECT FullScreenSaveRect;
     //INT MouseWheel = 0;
     BOOL IsInit = FALSE;
     
  public:
    BOOL IsActive;

    win( HINSTANCE hInst = GetModuleHandle(nullptr) );
    ~win( VOID );
 
    VOID Run( VOID );
    VOID FlipFullScreen( VOID );

  private:
    // message cracks
    VOID OnGetMinMaxInfo( MINMAXINFO *MinMax );
    BOOL OnCreate( CREATESTRUCT *CS );
    VOID OnSize( UINT State, INT W, INT H );
    VOID OnTimer( INT Id );
    VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys );
    VOID OnLButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys );
    VOID OnLButtonUp( INT X, INT Y, UINT Keys );
    BOOL OnEraseBackground( HDC hDC );
    VOID OnPaint( HDC hDC, PAINTSTRUCT *PS );
    VOID OnClose( VOID );
    VOID OnDestroy( VOID );
    VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized );

  protected:
    // callbacks block
    virtual VOID Init( VOID );
    virtual VOID Close( VOID );
    virtual VOID Resize( VOID );
    virtual VOID Timer( VOID );
    virtual VOID Activate( VOID );
    virtual VOID Idle( VOID );
    virtual VOID Erase( HDC hDC );
    virtual VOID Paint( HDC hDC );
  };
}

#endif // __win_h_

/* END OF 'win.h' FILE */
