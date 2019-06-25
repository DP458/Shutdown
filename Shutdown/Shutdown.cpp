// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <WinSDKVer.h>

#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "NativeShutdown.h"
#include "MainWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{

	switch (lpCmdLine[0])
	{

	// Canceling shutdown or rebooting

	case 'A':
		return NativeShutdown::StopShutdown() ? 0 : -1;

	// Shutdown Windows Dialog

	case L'D':
		return (NativeShutdown::ShowShutdownDialog() != S_OK) ? -1 : 0;

	// Shutdown

	case L'S':
		return NativeShutdown::StartShutdown(FALSE) ? 0 : -1;

	// Rebooting

	case L'R':
		return NativeShutdown::StartShutdown(TRUE) ? 0 : -1;

	// Logging off

	case L'L':
		return NativeShutdown::UserLogOff() ? 0 : -1;

	// Locking

	case L'l':
		return NativeShutdown::UserLock() ? 0 : -1;

	}

	//

	if (!MainWindow::CreateMainWindow(hInstance))
		return -1;

	MainWindow::ShowMainWindow();

	if (!MainWindow::HasShutdownPrivileg())
		MessageBox
		(
			MainWindow::GetMainWindowHandle(),
			L"Failed to get shutdown privilege for current process",
			(LPCWSTR)NULL,
			MB_ICONSTOP | MB_OK
		);

	//

	BOOL bRet = FALSE;
	MSG msg = { 0 };

	while (bRet = GetMessage(&msg, (HWND)NULL, 0U, 0U))
	{

		if (bRet == -1)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;

}