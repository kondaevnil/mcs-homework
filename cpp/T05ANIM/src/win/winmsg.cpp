/* FILENAME:    winmsg.cpp
 * LAST UPDATE: 21.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     WinAPI classes realisation.
 */

#include "win.h"

static const INT InitTimer = 30;
static const INT RefreshTimer = 102; 

namespace degl
{
  /* WM_CREATE window message handle function.
   * ARGUMENTS:
   *   - structure with creation data:
   *       CREATESTRUCT *CS;
   * RETURNS:
   *   (BOOL) TRUE to continue creation window, FALSE to terminate.
   */
  BOOL win::OnCreate( CREATESTRUCT *CS )
  {
    SetTimer(hWnd, InitTimer, 10, nullptr);
    return TRUE;
  } /* End of 'win::OnCreate' function */

  /* WM_DESTROY window message handle function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::OnDestroy( VOID )
  {
    if (IsInit)
    {
      Close();
      KillTimer(hWnd, RefreshTimer);
    }
    else ;
      KillTimer(hWnd, InitTimer);
  } /* End of 'win::OnDestroy' function */


  /* WM_SIZE window message handle function.
   * ARGUMENTS:
   *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
   *       UINT State;
   *   - new width and height of client area:
   *       INT W, H;
   * RETURNS: None.
   */
  VOID win::OnSize( UINT State, INT W, INT H )
  {
    this->W = W;
    this->H = H;

    if (IsInit)
      Resize();
  } /* End of 'win::OnSize' function */

  /* WM_ERASEBKGND window message handle function.
   * ARGUMENTS:
   *   - device context of client area:
   *       HDC hDC;
   * RETURNS:
   *   (BOOL) TRUE if background is erased, FALSE otherwise.
   */
  BOOL win::OnEraseBackground( HDC hDC )
  {
    if (IsInit)
      Erase(hDC);
    return TRUE;
  } /* End of 'win::OnEraseBkgnd' function */

  /* WM_PAINT window message handle function.
   * ARGUMENTS:
   *   - window device context:
   *       HDC hDC;
   *   - paint message structure pointer:
   *       PAINTSTRUCT *PS;
   * RETURNS: None.
   */
  VOID win::OnPaint( HDC hDC, PAINTSTRUCT *PS )
  {
    if (IsInit)
      Paint(hDC);
  } /* End of 'win::OnPaint' function */

  /* WM_ACTIVATE window message handle function.
   * ARGUMENTS:
   *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
   *       UINT Reason;
   *   - handle of active window:
   *       HWND hWndActDeact;
   *   - minimized flag:
   *       BOOL IsMinimized;
   * RETURNS: None.
   */
  VOID win::OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
  {
    // IsActive = Reason == WA_CLICKACTIVE || Reason == WA_ACTIVE;
    if (IsInit)
      Activate();
  } /* End of 'win::OnActivate' function */

  /* WM_TIMER window message handle function.
   * ARGUMENTS:
   *   - specified the timer identifier.
   *       INT Id;
   * RETURNS: None.
   */
  VOID win::OnTimer( INT Id )
  {
    if (!IsInit)
    {
      KillTimer(hWnd, InitTimer);
      SetTimer(hWnd, RefreshTimer, 10, nullptr);
      IsInit = TRUE;
      Init();
      Resize();
    }
    Timer();
  } /* End of 'win::OnTimer' function */

  /* WM_MOUSEWHEEL window message handle function.
   * ARGUMENTS:
   *   - mouse window position:
   *       INT X, Y;
   *   - mouse wheel relative delta value:
   *       INT Z;
   *   - mouse keys bits (see MK_*** bits constants):
   *       UINT Keys;
   * RETURNS: None.
   */
  VOID win::OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
  {
    MouseWheel += Z;
  } /* End of 'win::OnMouseWheel' function */

  // TODO
  VOID win::OnClose( VOID )
  {
  }

  VOID win::OnLButtonUp( INT X, INT Y, UINT Keys )
  {
  }

  VOID win::OnLButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys )
  {
  }

  VOID win::OnGetMinMaxInfo( MINMAXINFO *MinMax )
  {
  }

  VOID win::FlipFullScreen( VOID )
  {
  }
}
 
 /* END OF 'winmsg.cpp' FILE */