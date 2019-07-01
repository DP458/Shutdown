// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <memory>
#include "Resources.h"
#include "AddComputersDialog.h"
#include "MainWindow.h"

namespace AddComputersDialog
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
			wcex.hIcon = (HICON)NULL;
			wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = STR_ADDCOMPUTERS_DIALOG_CLASS;
			wcex.hIconSm = (HICON)NULL;

			AddComputersDialog::bIsClassRegistered = RegisterClassEx(&wcex);

			if (!AddComputersDialog::bIsClassRegistered)
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
				AddComputersDialog::hOwnerWindow,
				(HMENU)NULL,
				AddComputersDialog::hMainInstance,
				(LPVOID)NULL
			)
		)
		{
			EnableWindow(AddComputersDialog::hOwnerWindow, FALSE);
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
				AddComputersDialog::hMainInstance,
				(LPVOID)NULL
			);


			AddComputersDialog::hComputerNameEdit = CreateWindowEx
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
				AddComputersDialog::hMainInstance,
				(LPVOID)NULL
			);

			Edit_LimitText(AddComputersDialog::hComputerNameEdit, 30);
			SetFocus(hComputerNameEdit);


			AddComputersDialog::hAddButton = CreateWindowEx
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
				AddComputersDialog::hMainInstance,
				(LPVOID)NULL
			);

			AddComputersDialog::hCancelButton = CreateWindowEx
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
				AddComputersDialog::hMainInstance,
				(LPVOID)NULL
			);

		break;

		case WM_DESTROY:
			EnableWindow(AddComputersDialog::hOwnerWindow, TRUE);
			SetActiveWindow(AddComputersDialog::hOwnerWindow);
		break;

		case WM_COMMAND:

			switch (HIWORD(wParam))
			{

			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				case IDS_ADDCOMPUTERS_ADD_BUTTON:

				{

					const int text_length = GetWindowTextLength(AddComputersDialog::hComputerNameEdit);

					if (text_length <= 0)
						break;

					wchar_t* computer_name = new wchar_t[text_length + 1];

					GetWindowText(AddComputersDialog::hComputerNameEdit, computer_name, text_length + 1);

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