// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include "Resources.h"
#include "AddComputersDialog.h"

namespace AddComputersDialog
{

	// Private

	BOOL bIsClassRegistered = FALSE;
	HINSTANCE hMainInstance;
	HWND hOwnerWindow;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Public

	BOOL AddComputersDialog::CreateDialogWindow(HINSTANCE hInstance, HWND owner)
	{
		
		AddComputersDialog::hMainInstance = hInstance;
		AddComputersDialog::hOwnerWindow = owner;

		if(!bIsClassRegistered)
		{

			WNDCLASSEX wcex = { 0 };

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = DLGWINDOWEXTRA;
			wcex.hInstance = AddComputersDialog::hMainInstance;
			wcex.hIcon = LoadIcon(AddComputersDialog::hMainInstance, MAKEINTRESOURCE(IDI_ICON1));
			wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = L"#32770";
			wcex.hIconSm = LoadIcon(AddComputersDialog::hMainInstance, MAKEINTRESOURCE(IDI_ICON1));

			ATOM hDialogWindowClass = RegisterClassEx(&wcex);

			AddComputersDialog::bIsClassRegistered = (bool)hDialogWindowClass;

			if (!AddComputersDialog::bIsClassRegistered)
				return FALSE;

		}

		HWND hDialogWindow = CreateWindowEx
		(
			(DWORD)NULL,
			L"#32770",
			STR_APP_TITLE,
			WS_POPUP | WS_BORDER | WS_CAPTION  | WS_SYSMENU | WS_TABSTOP | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			326,
			498,
			AddComputersDialog::hOwnerWindow,
			(HMENU)NULL,
			AddComputersDialog::hMainInstance,
			(LPVOID)NULL
		);

		if (hDialogWindow)
			EnableWindow(AddComputersDialog::hOwnerWindow, FALSE);
		else return FALSE;

		return TRUE;

	}

	// Private

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		switch (message)
		{

		case WM_DESTROY:
			EnableWindow(AddComputersDialog::hOwnerWindow, TRUE);
			SetActiveWindow(AddComputersDialog::hOwnerWindow);
		break;

		case WM_CTLCOLORSTATIC:
		return (LRESULT)(COLOR_WINDOW + 1);

		case WM_COMMAND:
		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	}

}