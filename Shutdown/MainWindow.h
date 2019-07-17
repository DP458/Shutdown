#pragma once

namespace MainWindow
{

	BOOL RegisterMainWindowClass(HINSTANCE hInstance);
	BOOL CreateMainWindow();
	LRESULT AddComputerName(LPWSTR computer_name);

}