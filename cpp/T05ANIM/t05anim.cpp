/* FILENAME:   win.cpp
 * PROGRAMMER: DK6
 * DATE:       17.07.2021
 * PURPOSE:    WinAPI windowed application sample.
 */

#include <Windows.h>

const char WND_CLASS_NAME[] = "My window";

// LPCWSTR

/* Link to function handler */
LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg,
	WPARAM wParam, LPARAM lParam);

/* The Main program function.
 * ARGUMENTS:
 *   - instance handler:
 *		 HISNTANCE hInstance;
 *	 - previous instance handler:
 *       HINSTANCE hPrevInstance;
 *	 - command line:
 *		 CHAR *CmdLine;
 *	 - window vision flag:
 *		 INT ShowCmd;
 * RETURNS:
 *   (INT) system return code.
 */
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR* CmdLine, INT ShowCmd)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;

	wc.style = CS_VREDRAW | CS_HREDRAW; // redraw after changing window size
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = MyWindowFunc;
	wc.lpszClassName = (wchar_t*)WND_CLASS_NAME;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, L"Error register window class", L"ERROR", MB_OK);
		return 0;
	}

	hWnd = CreateWindow((wchar_t*)WND_CLASS_NAME,
		L"Window title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
} /* End of 'WinMain' function */

/* Window messages handler function
 * ARGUMENTS:
 *   - window handler:
 *		 HWND hWnd;
 *   - message number:
 *		 UINT Msg;
 *   - message parameter:
 *       WPARAM wParam;
 *   - message parameter:
 *		 LPARAM lParam;
 * RETURNS:
 */
LRESULT CALLBACK MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'win.cpp' FILE */
