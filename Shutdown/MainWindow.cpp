// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include "Resources.h"
#include "NativeShutdown.h"
#include "MainWindow.h"

namespace MainWindow
{

	// Private

	BOOL bHasShutdownPrivilege = FALSE;
	ATOM hMainWindowClass;
	HWND hMainWindow;
	HWND hActionsComboBox;
	HWND hExecActionButton;
	HWND hTimerEdit;
	HWND hDefTimerButton;
	HWND hForceCheckBox;
	HWND hMessageEdit;

	BOOL StartShutdown(BOOL bRebootAfterShutdown)
	{

		LPWSTR message = { 0 };

		const int msg_length = GetWindowTextLength(MainWindow::hMessageEdit);

		if (msg_length > 0)
		{
			message = new WCHAR[msg_length + 1];
			GetWindowText(MainWindow::hMessageEdit, message, msg_length + 1);
		}

		DWORD timer_value;

		{

			const int timer_length = GetWindowTextLength(MainWindow::hTimerEdit);
			LPWSTR timer = new WCHAR[timer_length + 1];
			GetWindowText(MainWindow::hTimerEdit, timer, timer_length + 1);

			timer_value = wcstoul
			(
				timer,
				(wchar_t**)NULL,
				10
			);

			delete[] timer;

		}

		BOOL result = InitiateSystemShutdownEx
		(
			(LPWSTR)NULL,
			message,
			timer_value,
			Button_GetCheck(MainWindow::hForceCheckBox),
			bRebootAfterShutdown,
			SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED
		);

		if (msg_length > 0)
			delete[] message;

		return result;

	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		switch (message)
		{

		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_COMMAND:

			switch (HIWORD(wParam))
			{

			case CBN_SELCHANGE:

				switch (LOWORD(wParam))
				{

				case IDS_ACTIONS_COMBOBOX_TITLE:

					switch (ComboBox_GetCurSel(MainWindow::hActionsComboBox))
					{

					case ID_ACTION_SHUTDOWN: case ID_ACTION_REBOOT:

						EnableWindow(MainWindow::hExecActionButton, MainWindow::bHasShutdownPrivilege);
						EnableWindow(MainWindow::hTimerEdit, MainWindow::bHasShutdownPrivilege);
						EnableWindow(MainWindow::hDefTimerButton, MainWindow::bHasShutdownPrivilege);
						EnableWindow(MainWindow::hForceCheckBox, MainWindow::bHasShutdownPrivilege);
						EnableWindow(MainWindow::hMessageEdit, MainWindow::bHasShutdownPrivilege);

					break;

					case ID_ACTION_LOCK: case ID_ACTION_LOGOFF:

						EnableWindow(MainWindow::hExecActionButton, TRUE);
						EnableWindow(MainWindow::hTimerEdit, FALSE);
						EnableWindow(MainWindow::hDefTimerButton, FALSE);
						EnableWindow(MainWindow::hForceCheckBox, FALSE);
						EnableWindow(MainWindow::hMessageEdit, FALSE);

					break;

					case ID_ACTION_CANCEL:

						EnableWindow(MainWindow::hExecActionButton, MainWindow::bHasShutdownPrivilege);
						EnableWindow(MainWindow::hTimerEdit, FALSE);
						EnableWindow(MainWindow::hDefTimerButton, FALSE);
						EnableWindow(MainWindow::hForceCheckBox, FALSE);
						EnableWindow(MainWindow::hMessageEdit, FALSE);

					break;

					}

				break;

				}

			break;

			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				case IDS_EXECACTION_BUTTON_TITLE:

					switch (ComboBox_GetCurSel(MainWindow::hActionsComboBox))
					{

					case ID_ACTION_SHUTDOWN:

						if (!MainWindow::StartShutdown(FALSE))
							MessageBox
							(
								hWnd,
								L"Failed to shutdown current system",
								(LPCWSTR)NULL,
								MB_ICONSTOP | MB_OK
							);

					break;

					case ID_ACTION_REBOOT:

						if (!MainWindow::StartShutdown(TRUE))
							MessageBox
							(
								hWnd,
								L"Failed to reboot current system",
								(LPCWSTR)NULL,
								MB_ICONSTOP | MB_OK
							);

					break;

					case ID_ACTION_LOGOFF:

						if (!NativeShutdown::UserLogOff())
							MessageBox
							(
								hWnd,
								L"Failed to log off current user",
								(LPCWSTR)NULL,
								MB_ICONSTOP | MB_OK
							);

					break;

					case ID_ACTION_LOCK:

						if (NativeShutdown::UserLock())
						{

							PostMessage
							(
								hWnd,
								WM_CLOSE,
								(WPARAM)NULL,
								(LPARAM)NULL
							);

							return 0;
						}

						MessageBox
						(
							hWnd,
							L"Failed to lock current user",
							(LPCWSTR)NULL,
							MB_ICONSTOP | MB_OK
						);

					break;

					case ID_ACTION_CANCEL:

						AbortSystemShutdown
						(
							(LPWSTR)NULL
						);

					break;

					}

				break;

				case IDS_TIMER_DEF_BUTTON_TITLE:

					SetWindowText(MainWindow::hTimerEdit, STR_TIMER_EDIT_TITLE);
					Edit_SetSel(MainWindow::hTimerEdit, 2, 2);
					SetFocus(MainWindow::hTimerEdit);

				break;

				//

				case IDS_EXIT_POPUP_ITEM:

					PostMessage
					(
						hWnd,
						WM_CLOSE,
						(WPARAM)NULL,
						(LPARAM)NULL
					);

				return 0;

				case IDS_ABOUT_POPUP_ITEM:

					MessageBox
					(
						hWnd,
						L"An application  for shutting down the system",
						L"About",
						MB_ICONINFORMATION | MB_OK
					);

				break;

				}

			break;

			case EN_CHANGE:

				switch (LOWORD(wParam))
				{

				case IDS_TIMER_EDIT_TITLE:

					if (GetWindowTextLength(MainWindow::hTimerEdit) <= 0)
					{
						SetWindowText(MainWindow::hTimerEdit, STR_TIMER_EDIT_TITLE);
						Edit_SetSel(MainWindow::hTimerEdit, 2, 2);
					}

				break;

				}

			break;

			}

		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	}

	// Public

	BOOL MainWindow::HasShutdownPrivileg()
	{
		return MainWindow::bHasShutdownPrivilege;
	}

	HWND MainWindow::GetMainWindowHandle()
	{
		return MainWindow::hMainWindow;
	}

	BOOL MainWindow::CreateMainWindow(HINSTANCE hInstance)
	{

		{

			WNDCLASSEX wcex = { 0 };

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
			wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = STR_APP_CLASS;
			wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

			MainWindow::hMainWindowClass = RegisterClassEx(&wcex);

			if (!MainWindow::hMainWindowClass)
				return FALSE;

		}

		HMENU hFilePopupMenu = CreatePopupMenu();
		AppendMenu(hFilePopupMenu, MF_STRING, IDS_EXIT_POPUP_ITEM, STR_EXIT_POPUP_ITEM);

		HMENU hHelpPopupMenu = CreatePopupMenu();
		AppendMenu(hHelpPopupMenu, MF_STRING, IDS_ABOUT_POPUP_ITEM, STR_ABOUT_POPUP_ITEM);

		HMENU hMainMenu = CreateMenu();
		AppendMenu(hMainMenu, MF_POPUP | MF_STRING, (UINT_PTR)hFilePopupMenu, STR_FILE_POPUP_MENU);
		AppendMenu(hMainMenu, MF_POPUP | MF_STRING, (UINT_PTR)hHelpPopupMenu, STR_HELP_POPUP_MENU);

		MainWindow::hMainWindow = CreateWindowEx
		(
			(DWORD)NULL,
			STR_APP_CLASS,
			STR_APP_TITLE,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			326,
			394,
			(HWND)NULL,
			hMainMenu,
			hInstance,
			(LPVOID)NULL
		);

		if (!MainWindow::hMainWindow)
			return FALSE;

		CreateWindowEx
		(
			(DWORD)NULL,
			WC_STATIC,
			STR_ACTION_STATIC_TITLE,
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			10,
			5,
			180,
			20,
			MainWindow::hMainWindow,
			(HMENU)IDS_ACTION_STATIC_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		MainWindow::hActionsComboBox = CreateWindowEx
		(
			(DWORD)NULL,
			WC_COMBOBOX,
			STR_ACTIONS_COMBOBOX_TITLE,
			WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
			10,
			28,
			200,
			40,
			MainWindow::hMainWindow,
			(HMENU)IDS_ACTIONS_COMBOBOX_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		ComboBox_InsertString(MainWindow::hActionsComboBox, ID_ACTION_SHUTDOWN, ACTION_SHUTDOWN);
		ComboBox_InsertString(MainWindow::hActionsComboBox, ID_ACTION_REBOOT, ACTION_REBOOT);
		ComboBox_InsertString(MainWindow::hActionsComboBox, ID_ACTION_LOGOFF, ACTION_LOGOFF);
		ComboBox_InsertString(MainWindow::hActionsComboBox, ID_ACTION_LOCK, ACTION_LOCK);
		ComboBox_InsertString(MainWindow::hActionsComboBox, ID_ACTION_CANCEL, ACTION_CANCEL);

		MainWindow::hExecActionButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_EXECACTION_BUTTON_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			220,
			25,
			80,
			30,
			MainWindow::hMainWindow,
			(HMENU)IDS_EXECACTION_BUTTON_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		CreateWindowEx
		(
			(DWORD)NULL,
			WC_STATIC,
			STR_TIMER_STATIC_TITLE,
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			10,
			65,
			180,
			20,
			MainWindow::hMainWindow,
			(HMENU)IDS_TIMER_STATIC_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		MainWindow::hTimerEdit = CreateWindowEx
		(
			WS_EX_CLIENTEDGE,
			WC_EDIT,
			STR_TIMER_EDIT_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | ES_LEFT | ES_NUMBER | ES_NOHIDESEL,
			10,
			90,
			40,
			20,
			MainWindow::hMainWindow,
			(HMENU)IDS_TIMER_EDIT_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		Edit_LimitText(MainWindow::hTimerEdit, 4);

		MainWindow::hDefTimerButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_TIMER_DEF_BUTTON_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			60,
			85,
			80,
			30,
			MainWindow::hMainWindow,
			(HMENU)IDS_TIMER_DEF_BUTTON_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		MainWindow::hForceCheckBox = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_FORCE_CHECKBOX_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX,
			10,
			125,
			160,
			20,
			MainWindow::hMainWindow,
			(HMENU)IDS_FORCE_CHECKBOX_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		CreateWindowEx
		(
			(DWORD)NULL,
			WC_STATIC,
			STR_MESSAGE_STATIC_TITLE,
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			10,
			155,
			180,
			20,
			MainWindow::hMainWindow,
			(HMENU)IDS_MESSAGE_STATIC_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		MainWindow::hMessageEdit = CreateWindowEx
		(
			WS_EX_CLIENTEDGE,
			WC_EDIT,
			STR_MESSAGE_EDIT_TITLE,
			WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_DISABLED | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_NOHIDESEL,
			10,
			175,
			290,
			150,
			MainWindow::hMainWindow,
			(HMENU)IDS_MESSAGE_EDIT_TITLE,
			hInstance,
			(LPVOID)NULL
		);

		Edit_LimitText(MainWindow::hMessageEdit, 511);

		bHasShutdownPrivilege = NativeShutdown::SetShutdownPrivilege(true);

		return TRUE;

	}

	BOOL MainWindow::ShowMainWindow()
	{
		return ShowWindow(MainWindow::hMainWindow, SW_SHOW);
	}

}