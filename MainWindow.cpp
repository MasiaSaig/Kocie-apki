#include "MainWindow.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_PAINT: 
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc, 80, 100, TEXT("Milej zabawy :D"), strlen("Milej zabawy :D"));

		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
	}	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


Window::Window() : 
	_hInstance{GetModuleHandle(nullptr)}
{
	const wchar_t* CLASS_NAME = L"Kocielapki";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = _hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WndProc;

	RegisterClass(&wndClass);
	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	int width = 250, height = 250;

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false);

	_hWnd = CreateWindowEx(
		NULL,
		CLASS_NAME,
		L"Kocielapki",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		_hInstance,
		NULL
	);

	if (!_hWnd){
		int nResult = GetLastError();
		MessageBox(NULL,
			TEXT("Window creation failed"),
			TEXT("Window Failed"),
			MB_ICONERROR);
	}
	else {
		// SW_HIDE
		ShowWindow(_hWnd, SW_SHOW);
	}

	
}

Window::~Window()
{
	const wchar_t* CLASS_NAME = L"Kocielapki";
	UnregisterClass(CLASS_NAME, _hInstance);
}

bool Window::processMessages() {
	MSG msg{};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

