#pragma once

namespace MainWindow
{

	void Initialize(HINSTANCE hInstance);
	BOOL HasShutdownPrivileg();
	HWND GetMainWindowHandle();
	BOOL RegisterMainWindowClass();
	BOOL CreateMainWindow();
	BOOL ShowMainWindow();

}