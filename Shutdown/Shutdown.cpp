// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "NativeShutdown.h"
#include "MainWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{

	switch (lpCmdLine[0])
	{

	// Canceling shutdown or rebooting

	case L'A':
		return NativeShutdown::StopShutdown((LPWSTR)NULL) ? 0 : -1;

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

	if (!MainWindow::RegisterMainWindowClass(hInstance))
	{
		TaskDialog
		(
			(HWND)NULL,
			hInstance,
			L"Error",
			L"Failed to register main window class",
			(PCWSTR)NULL,
			TDCBF_OK_BUTTON,
			TD_ERROR_ICON,
			(int*)NULL
		);
		return -1;
	}

	if (!MainWindow::CreateMainWindow())
	{
		TaskDialog
		(
			(HWND)NULL,
			hInstance,
			L"Error",
			L"Failed to create main window",
			(PCWSTR)NULL,
			TDCBF_OK_BUTTON,
			TD_ERROR_ICON,
			(int*)NULL
		);
		return -1;
	}

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