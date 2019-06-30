#pragma once

namespace MainWindow
{

	BOOL RegisterMainWindowClass(HINSTANCE hInstance);
	BOOL CreateMainWindow();
	HWND GetHostListBoxHandle();

}