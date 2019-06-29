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

	HINSTANCE hMainInstance;
	ATOM hDialogWindowClass;
	HWND hOwnerWindow;
	HWND hDialogWindow;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Public

	BOOL AddComputersDialog::CreateDialogWindow(HINSTANCE hInstance, HWND owner)
	{
		
		AddComputersDialog::hMainInstance = hInstance;
		AddComputersDialog::hOwnerWindow = owner;

		/*{

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

			AddComputersDialog::hDialogWindowClass = RegisterClassEx(&wcex);

			if (!AddComputersDialog::hDialogWindowClass)
				return FALSE;

		}*/

		AddComputersDialog::hDialogWindow = CreateWindowEx
		(
			(DWORD)NULL,
			L"#32770",
			STR_APP_TITLE,
			DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			326,
			498,
			AddComputersDialog::hOwnerWindow,
			(HMENU)NULL,
			AddComputersDialog::hMainInstance,
			(LPVOID)NULL
		);

		if (AddComputersDialog::hDialogWindow)
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
		break;

		/*case WM_NCDESTROY:
			UnregisterClass(L"sdjidsf", AddComputersDialog::hMainInstance);
		break;*/

		case WM_CTLCOLORSTATIC:
		return (LRESULT)(COLOR_WINDOW + 1);

		case WM_COMMAND:
		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	}

}