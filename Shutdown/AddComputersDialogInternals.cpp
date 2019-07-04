// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Resources.h"
#include "MainWindow.h"
#include "AddComputersDialogInternals.h"

namespace AddComputersDialogInternals
{

	AddComputersDialogInternals::__AddComputersDialog DlgInternalObj;


	AddComputersDialogInternals::__AddComputersDialog* AddComputersDialogInternals::__AddComputersDialog::GetObjectRef()
	{
		return &DlgInternalObj;
	}

	LRESULT CALLBACK AddComputersDialogInternals::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				DlgInternalObj.hMainInstance,
				(LPVOID)NULL
			);


			DlgInternalObj.hComputerNameEdit = CreateWindowEx
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
				DlgInternalObj.hMainInstance,
				(LPVOID)NULL
			);

			Edit_LimitText(DlgInternalObj.hComputerNameEdit, 30);
			SetFocus(DlgInternalObj.hComputerNameEdit);


			DlgInternalObj.hAddButton = CreateWindowEx
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
				DlgInternalObj.hMainInstance,
				(LPVOID)NULL
			);

			DlgInternalObj.hCancelButton = CreateWindowEx
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
				DlgInternalObj.hMainInstance,
				(LPVOID)NULL
			);

		break;

		case WM_DESTROY:
			EnableWindow(DlgInternalObj.hOwnerWindow, TRUE);
			SetActiveWindow(DlgInternalObj.hOwnerWindow);
		break;

		case WM_COMMAND:

			switch (HIWORD(wParam))
			{

			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				case IDS_ADDCOMPUTERS_ADD_BUTTON:

				{

					const int text_length = GetWindowTextLength(DlgInternalObj.hComputerNameEdit);

					if (text_length <= 0)
						break;

					wchar_t* computer_name = new wchar_t[text_length + 1];

					GetWindowText(DlgInternalObj.hComputerNameEdit, computer_name, text_length + 1);

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