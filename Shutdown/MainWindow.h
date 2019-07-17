#pragma once

namespace MainWindow
{

	BOOL RegisterMainWindowClass(HINSTANCE hInstance);
	BOOL CreateMainWindow();
	BOOL AddComputerName(LPCWSTR computer_name);

}