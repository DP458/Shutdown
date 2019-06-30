// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include "Resources.h"
#include "AddComputersDialog.h"
#include "MainWindow.h"

namespace AddComputersDialog
{

	// Private

	BOOL bIsClassRegistered = FALSE;
	HINSTANCE hMainInstance;
	WNDPROC PrevWndProc;
	HWND hOwnerWindow;
	HWND hHostNameEdit;
	HWND hAddButton;

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
			wcex.lpszClassName = L"#32770";
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
				L"#32770",
				L"Add computers",
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
				L"Enter computer name",
				WS_VISIBLE | WS_CHILD | SS_SIMPLE,
				10,
				5,
				180,
				20,
				hWnd,
				(HMENU)IDS_ACTION_STATIC_TITLE,
				AddComputersDialog::hMainInstance,
				(LPVOID)NULL
			);


			AddComputersDialog::hHostNameEdit = CreateWindowEx
			(
				WS_EX_CLIENTEDGE,
				WC_EDIT,
				STR_MESSAGE_EDIT_TITLE,
				WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_NOHIDESEL,
				10,
				28,
				250,
				40,
				hWnd,
				(HMENU)IDS_MESSAGE_EDIT_TITLE,
				AddComputersDialog::hMainInstance,
				(LPVOID)NULL
			);

			Edit_LimitText(AddComputersDialog::hHostNameEdit, 30);
			SetFocus(hHostNameEdit);


			AddComputersDialog::hAddButton = CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				L"Add",
				WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
				86,
				78,
				80,
				30,
				hWnd,
				(HMENU)IDS_EXECACTION_BUTTON_TITLE,
				AddComputersDialog::hMainInstance,
				(LPVOID)NULL
			);

			CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				L"Cancel",
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
				176,
				78,
				80,
				30,
				hWnd,
				(HMENU)IDS_EXECACTION_BUTTON_TITLE,
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

			case EN_CHANGE:

				switch (LOWORD(wParam))
				{
					
				case IDS_MESSAGE_EDIT_TITLE:
					EnableWindow(AddComputersDialog::hAddButton, GetWindowTextLength((HWND)lParam));
				break;

				}

			break;

			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				case IDS_EXECACTION_BUTTON_TITLE:

				{

					const int text_length = GetWindowTextLength(AddComputersDialog::hHostNameEdit);

					wchar_t* host_name = new wchar_t[text_length + 1];

					GetWindowText(AddComputersDialog::hHostNameEdit, host_name, text_length + 1);

					SendMessage
					(
						MainWindow::GetHostListBoxHandle(),
						LB_ADDSTRING,
						(WPARAM)NULL,
						(LPARAM)host_name
					);

					delete[] host_name;

				}

				PostMessage
				(
					hWnd,
					WM_CLOSE,
					(WPARAM)NULL,
					(LPARAM)NULL
				);

				break;

				}

			break;

			}

		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	}

}