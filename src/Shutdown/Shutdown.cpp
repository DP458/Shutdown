// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "win_api.h"
#include "com_api.h"
#include "MainWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{

	// Processing of command line args

	switch (lpCmdLine[0])
	{

	// Canceling shutdown or rebooting

	case L'A':
		if
		(
			!win_api::SetShutdownPrivilege((LPWSTR)NULL)
		)
			return TRUE;

	return !AbortSystemShutdown((LPWSTR)NULL);

	// Shutdown Windows Dialog

	case L'D':
	return !com_api::ShowShutdownDialog();

	// Shutdown

	case L'S':
		if
		(
			!win_api::SetShutdownPrivilege((LPWSTR)NULL)
		)
			return TRUE;

	return !InitiateSystemShutdownEx
	(
		(LPWSTR)NULL,
		(LPWSTR)NULL,
		(DWORD)0,
		FALSE,
		FALSE,
		SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED
	);

	// Rebooting

	case L'R':
		if
		(
			!win_api::SetShutdownPrivilege((LPWSTR)NULL)
		)
			return TRUE;

	return !InitiateSystemShutdownEx
	(
		(LPWSTR)NULL,
		(LPWSTR)NULL,
		(DWORD)0,
		FALSE,
		TRUE,
		SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED
	);

	// Logging off

	case L'L':
	return !ExitWindowsEx
	(
		EWX_LOGOFF,
		SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED
	);

	// Locking

	case L'l':
	return !LockWorkStation();

	}

	//

	if (!MainWindow::__MainWindow::ShowDialog(hInstance))
	{
		TaskDialog
		(
			(HWND)NULL,
			hInstance,
			L"Error",
			L"Failed to show main window",
			(PCWSTR)NULL,
			TDCBF_OK_BUTTON,
			TD_ERROR_ICON,
			(int*)NULL
		);
		return TRUE;
	}

	BOOL bRet = FALSE;
	MSG msg = { 0 };

	while
	(
		bRet = GetMessage
		(
			&msg,
			(HWND)NULL,
			0U, 
			0U
		)
	)
	{
		if (bRet == -1)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}