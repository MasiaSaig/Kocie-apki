#pragma once

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window {
public:
	Window();
	Window(const Window&) = delete;
	Window& operator =(const Window*) = delete;
	~Window();

	bool processMessages();

	inline HWND get_hWnd() const {
		return _hWnd;
	}
	inline HINSTANCE get_hInstance() const {
		return _hInstance;
	}
private:
	HINSTANCE _hInstance;
	HWND _hWnd;
};