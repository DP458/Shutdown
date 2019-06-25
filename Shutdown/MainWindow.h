#pragma once

namespace MainWindow
{

	BOOL HasShutdownPrivileg();
	HWND GetMainWindowHandle();
	BOOL CreateMainWindow(HINSTANCE hInstance);
	BOOL ShowMainWindow();

}