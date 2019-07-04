// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Resources.h"
#include "AboutDialog.h"
#include "MainWindow.h"

namespace AboutDialog
{

	// Private

	BOOL bIsClassRegistered = FALSE;
	HINSTANCE hMainInstance;
	HWND hOwnerWindow;
	HWND hComputerNameEdit;
	HWND hAddButton;
	HWND hCancelButton;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Public

	BOOL AboutDialog::CreateDialogWindow(HINSTANCE hInstance, HWND owner)
	{
		
		AboutDialog::hMainInstance = hInstance;
		AboutDialog::hOwnerWindow = owner;

		if(!bIsClassRegistered)
		{

			WNDCLASSEX wcex = { 0 };

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = DLGWINDOWEXTRA;
			wcex.hInstance = AboutDialog::hMainInstance;
			wcex.hIcon = (HICON)NULL;
			wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = STR_ADDCOMPUTERS_DIALOG_CLASS;
			wcex.hIconSm = (HICON)NULL;

			AboutDialog::bIsClassRegistered = RegisterClassEx(&wcex);

			if (!AboutDialog::bIsClassRegistered)
				return FALSE;

		}

		if
		(
			CreateWindowEx
			(
				WS_EX_DLGMODALFRAME,
				STR_ADDCOMPUTERS_DIALOG_CLASS,
				STR_ADDCOMPUTERS_DIALOG_TITLE,
				WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				285,
				158,
				AboutDialog::hOwnerWindow,
				(HMENU)NULL,
				AboutDialog::hMainInstance,
				(LPVOID)NULL
			)
		)
		{
			EnableWindow(AboutDialog::hOwnerWindow, FALSE);
			return TRUE;
		}
		else return FALSE;

	}

	// Private

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		switch (message)
		{

		case WM_CREATE:

			CreateWindowEx
			(
				(DWORD)NULL,
				WC_STATIC,
				STR_ADDCOMPUTERS_COMPUTERNAME_STATIC,
				WS_VISIBLE | WS_CHILD | SS_SIMPLE,
				10,
				5,
				180,
				20,
				hWnd,
				(HMENU)IDS_ADDCOMPUTERS_COMPUTERNAME_STATIC,
				AboutDialog::hMainInstance,
				(LPVOID)NULL
			);


			AboutDialog::hComputerNameEdit = CreateWindowEx
			(
				WS_EX_CLIENTEDGE,
				WC_EDIT,
				STR_ADDCOMPUTERS_COMPUTERNAME_EDIT,
				WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_NOHIDESEL,
				10,
				28,
				250,
				40,
				hWnd,
				(HMENU)IDS_ADDCOMPUTERS_COMPUTERNAME_EDIT,
				AboutDialog::hMainInstance,
				(LPVOID)NULL
			);

			Edit_LimitText(AboutDialog::hComputerNameEdit, 30);
			SetFocus(AboutDialog::hComputerNameEdit);


			AboutDialog::hAddButton = CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				STR_ADDCOMPUTERS_ADD_BUTTON,
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
				86,
				78,
				80,
				30,
				hWnd,
				(HMENU)IDS_ADDCOMPUTERS_ADD_BUTTON,
				AboutDialog::hMainInstance,
				(LPVOID)NULL
			);

			AboutDialog::hCancelButton = CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				STR_ADDCOMPUTERS_CANCEL_BUTTON,
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
				176,
				78,
				80,
				30,
				hWnd,
				(HMENU)IDS_ADDCOMPUTERS_CANCEL_BUTTON,
				AboutDialog::hMainInstance,
				(LPVOID)NULL
			);

		break;

		case WM_DESTROY:
			EnableWindow(AboutDialog::hOwnerWindow, TRUE);
			SetActiveWindow(AboutDialog::hOwnerWindow);
		break;

		case WM_COMMAND:

			switch (HIWORD(wParam))
			{

			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				case IDS_ADDCOMPUTERS_ADD_BUTTON:

				{

					const int text_length = GetWindowTextLength(AboutDialog::hComputerNameEdit);

					if (text_length <= 0)
						break;

					wchar_t* computer_name = new wchar_t[text_length + 1];

					GetWindowText(AboutDialog::hComputerNameEdit, computer_name, text_length + 1);

					SendMessage
					(
						MainWindow::GetHostListBoxHandle(),
						LB_ADDSTRING,
						(WPARAM)NULL,
						(LPARAM)computer_name
					);

					delete[] computer_name;

				}
				
					PostMessage
					(
						hWnd,
						WM_CLOSE,
						(WPARAM)NULL,
						(LPARAM)NULL
					);

				return 0;

				case IDS_ADDCOMPUTERS_CANCEL_BUTTON:
					PostMessage
					(
						hWnd,
						WM_CLOSE,
						(WPARAM)NULL,
						(LPARAM)NULL
					);
				return 0;

				}

			break;

			}

		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	}

}