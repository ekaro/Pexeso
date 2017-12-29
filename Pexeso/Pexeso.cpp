#include <stdlib.h>
#include <Windowsx.h>
#include "Deck.h"

// Forward declaration of functions included in this module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Window properties
int WindowWidth = 600;
int WindowHeight = 700;

// Card location properties
int xPos;
int yPos;
int Card;

// Button properties
HWND RestartButton;
int FontHeight;
HFONT ButtonFont;
LOGFONT logFont;

// Deck creation
Deck NewDeck;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const wchar_t szWindowClass[] = L"WinApp";
	static TCHAR szTitle[] = _T("Pexeso");

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));  // wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
		_T("Call to RegisterClassEx failed!"),
		_T("Windows Desktop Guided Tour"),
		NULL);

		return 1;
	}

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WindowWidth, WindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (IsWindow(hWnd))
	{
		DWORD dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		DWORD dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		HMENU menu = GetMenu(hWnd);

		RECT rc = { 0, 0, WindowWidth, WindowHeight };

		AdjustWindowRectEx(&rc, dwStyle, menu ? TRUE : FALSE, dwExStyle);

		SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
	}

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


#define ID_BUTTON 1

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR display_msg[] = _T("Message in window");
		 
	switch (message)
	{
	case WM_CREATE:

		RestartButton = CreateWindow(
			L"BUTTON",  
			L"New Game",    
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
			0,         
			0,         
			WindowWidth / 5,        
			WindowHeight / 7,        
			hWnd,     
			(HMENU) ID_BUTTON,       
			NULL,
			NULL
		);

	case WM_COMMAND:

		if (LOWORD(wParam) == ID_BUTTON)
		{
			NewDeck.NewGame();
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		}
		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		NewDeck.DrawDeck(hdc, hWnd);

		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:

		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		if (yPos > NewDeck.GetOffset())
		{
			Card = NewDeck.GetCardIndex(hWnd, xPos, yPos);

			NewDeck.CompareCards(hWnd, Card);

			NewDeck.Clicked(hWnd, Card);
		}	
		break;

	case WM_SIZE:

		NewDeck.ResizeText(hWnd);

		SetWindowPos(RestartButton, NULL, 0, 0, NewDeck.GetClientDimensions(hWnd).first / 5, NewDeck.GetClientDimensions(hWnd).second / 7, SWP_NOZORDER | SWP_NOMOVE);

		FontHeight = (NewDeck.GetClientDimensions(hWnd).first + NewDeck.GetClientDimensions(hWnd).second) / 60;
		DeleteObject(ButtonFont);   // delete previous font (GDI object) to prevent memory leak
		
		memset(&logFont, 0, sizeof(logFont));
		logFont.lfHeight = FontHeight;
		ButtonFont = CreateFontIndirect(&logFont);  // or ButtonFont = CreateFont(FontHeight, 0, 0, 0, 300, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial"));
		SendMessage(RestartButton, WM_SETFONT, WPARAM(ButtonFont), TRUE);

		// cards correctly resized after window is maximized
		if (wParam == SIZE_MAXIMIZED)
		{
			NewDeck.ResizeDeck(hWnd);
		}

		if (wParam == SIZE_RESTORED)
		{
			NewDeck.ResizeDeck(hWnd);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}