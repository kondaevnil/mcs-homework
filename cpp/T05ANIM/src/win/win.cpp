/* FILENAME:    win.cpp
 * LAST UPDATE: 21.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     WinAPI classes realisation.
 */

#include "win.h"

namespace degl
{
  const char wnd_class_name[] = "asd";

  /* The win class constructor.
   * ARGUMENTS:
   *   - instance handler:
   *       HISNTANCE hInst;
   * RETURNS: None.
   */
  win::win( HINSTANCE hInst ) : hInstance( hInst ), W(0), H(0), IsActive(FALSE),
                                IsFullScreen(FALSE), MouseWheel(0), hWnd(nullptr)
  {
    WNDCLASS wc;

    wc.style = CS_VREDRAW | CS_HREDRAW; // redraw after changing window size
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(win *);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.lpszMenuName = nullptr;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = win::WinFunc;
    wc.lpszClassName = wnd_class_name;

    ShowCursor(FALSE);

    // !!Check it later
    if (!RegisterClass(&wc))
    {
      MessageBoxA(NULL, "Error register window class", "ERROR", MB_OK);
      return;
    }

    win::hWnd = CreateWindow(wnd_class_name, "asd asf",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      nullptr, nullptr, hInstance, reinterpret_cast<VOID *>(this));

    ShowWindow(win::hWnd, SW_SHOWNORMAL);
    UpdateWindow(win::hWnd);
  }

  LRESULT CALLBACK win::WinFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
  {
    win *Win;
    HDC hDC;
    PAINTSTRUCT ps;

    switch (Msg)
    {
    case WM_CREATE:
      SetWindowLongPtr( hWnd, 0, (UINT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams );
    default:
      Win = reinterpret_cast<win *>(GetWindowLongPtr( hWnd, 0 ));
      if (Win != nullptr)
        switch (Msg)
        {
        case WM_GETMINMAXINFO:
          ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
            GetSystemMetrics( SM_CYMAXTRACK ) +
            GetSystemMetrics( SM_CYCAPTION ) +
            GetSystemMetrics( SM_CYMENU ) +
            GetSystemMetrics( SM_CYBORDER ) * 2;
          return 0;
        case WM_CREATE:
          Win->hWnd = hWnd;
          return Win->OnCreate( (CREATESTRUCT *)lParam ) ? 0 : -1;
        case WM_SIZE:
          Win->OnSize( wParam, LOWORD( lParam ), HIWORD( lParam ) );
          return 0;
        case WM_ERASEBKGND:
          return (LRESULT)Win->OnEraseBackground( (HDC)wParam );
        case WM_PAINT:
          hDC = BeginPaint( hWnd, &ps );
          Win->OnPaint( hDC, &ps );
          EndPaint( hWnd, &ps );
          return 0;
        case WM_TIMER:
          Win->OnTimer( (UINT)wParam );
          return 0;

        case WM_MOUSEWHEEL:
          Win->OnMouseWheel( (INT)(SHORT)LOWORD( lParam ),
            (INT)(SHORT)HIWORD( lParam ),
            (INT)(SHORT)HIWORD( wParam ),
            (UINT)(SHORT)LOWORD( wParam ) );
          return 0;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
          Win->OnLButtonDown( FALSE,
            (INT)(SHORT)LOWORD( lParam ),
            (INT)(SHORT)HIWORD( lParam ),
            (UINT)(SHORT)LOWORD( wParam ) );
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
          Win->OnLButtonUp( (INT)(SHORT)LOWORD( lParam ),
            (INT)(SHORT)HIWORD( lParam ),
            (UINT)(SHORT)LOWORD( wParam ) );
          return 0;
        case WM_DESTROY:
          Win->OnDestroy();
          PostQuitMessage( 30 );
        }
    }
    return DefWindowProc( hWnd, Msg, wParam, lParam );
  }

  /* The win class deconstructor
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  win::~win( VOID )
  {
    // TODO
  } /* End of 'win::~win' function */

  /* Message loop function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::Run( VOID )
  {
    MSG msg;

    while (TRUE)
    {
      /* Check message at window message queue */
      if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
        if (msg.message == WM_QUIT)
          break;
        else
        {
          /* Displatch message to window */
          TranslateMessage( &msg );
          DispatchMessage( &msg );
        }
      else
      {
        /* Idle... */
        if (IsInit)
          Idle();
      }
    }
    //return msg.wParam;
  } /* End of 'win::Run' function */

  /* Initialization system callback function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::Init( VOID )
  {
  } /* End of 'win::Init' function */

  /* Deinitialization system callback function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::Close( VOID )
  {
  } /* End of 'win::Close' function */

  /* Resize window callback function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::Resize( VOID )
  {
  } /* End of 'win::Resize' function */

  /* Timer callback function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::Timer( VOID )
  {
  } /* End of 'win::Timer' function */

  /* Window message callback function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::Activate( VOID )
  {
  } /* End of 'win::Activate' function */

  /* ??? callback function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::Idle( VOID )
  {
  } /* End of 'win::Idle' function */

  /* On erase backgroung callback function
   * ARGUMENTS:
   *   - descriptor handler:
   *       HDC hDC;
   * RETURNS: None.
   */
  VOID win::Erase( HDC hDC )
  {
  } /* End of 'win::Erase' function */

  /* On paint callback function
   * ARGUMENTS:
   *   - descriptor handler:
   *       HDC hDC;
   * RETURNS: None.
   */
  VOID win::Paint( HDC hDC )
  {
  } /* End of 'win::Paint' function */

}

/* END OF 'win.cpp' FILE */