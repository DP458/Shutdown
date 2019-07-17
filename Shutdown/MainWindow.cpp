// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Resources.h"
#include "NativeShutdown.h"
#include "MainWindow.h"
#include "AddComputersDialog.h"

namespace MainWindow
{

	// Private

	HINSTANCE hMainInstance;
	HWND hActionsComboBox;
	HWND hExecActionButton;
	HWND hComputersListBox;
	HWND hAddComputersButton;
	HWND hRemoveComputersButton;
	HWND hClearComputersButton;
	HWND hTimerEdit;
	HWND hDefTimerButton;
	HWND hForceCheckBox;
	HWND hPlannedCheckBox;
	HWND hMessageEdit;

	BOOL StartShutdown(BOOL bRebootAfterShutdown);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Public

	BOOL MainWindow::RegisterMainWindowClass(HINSTANCE hInstance)
	{

		MainWindow::hMainInstance = hInstance;

		WNDCLASSEX wcex = { 0 };

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MainWindow::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = MainWindow::hMainInstance;
		wcex.hIcon = LoadIcon(MainWindow::hMainInstance, MAKEINTRESOURCE(IDI_ICON1));
		wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = STR_APP_CLASS;
		wcex.hIconSm = LoadIcon(MainWindow::hMainInstance, MAKEINTRESOURCE(IDI_ICON1));

		return RegisterClassEx(&wcex) ? TRUE : FALSE;

	}

	BOOL MainWindow::CreateMainWindow()
	{

		HMENU hFilePopupMenu = CreatePopupMenu();

		AppendMenu
		(
			hFilePopupMenu,
			MF_STRING,
			IDS_OPEN_POPUP_ITEM,
			STR_OPEN_POPUP_ITEM
		);
		AppendMenu
		(
			hFilePopupMenu,
			MF_SEPARATOR,
			(UINT_PTR)NULL,
			(LPCWSTR)NULL
		);
		AppendMenu
		(
			hFilePopupMenu,
			MF_STRING,
			IDS_SYSTEMDIALOG_POPUP_ITEM,
			STR_SYSTEMDIALOG_POPUP_ITEM
		);
		AppendMenu
		(
			hFilePopupMenu,
			MF_SEPARATOR,
			(UINT_PTR)NULL,
			(LPCWSTR)NULL
		);
		AppendMenu
		(
			hFilePopupMenu,
			MF_STRING,
			IDS_EXIT_POPUP_ITEM,
			STR_EXIT_POPUP_ITEM
		);

		HMENU hHelpPopupMenu = CreatePopupMenu();
		AppendMenu
		(
			hHelpPopupMenu,
			MF_STRING,
			IDS_ABOUT_POPUP_ITEM,
			STR_ABOUT_POPUP_ITEM
		);

		HMENU hMainMenu = CreateMenu();
		AppendMenu
		(
			hMainMenu,
			MF_POPUP | MF_STRING,
			(UINT_PTR)hFilePopupMenu,
			STR_FILE_POPUP_MENU
		);
		AppendMenu
		(
			hMainMenu,
			MF_POPUP | MF_STRING,
			(UINT_PTR)hHelpPopupMenu,
			STR_HELP_POPUP_MENU
		);

		{

			MENUINFO info = { 0 };
			info.cbSize = sizeof(info);
			GetMenuInfo(hMainMenu, &info);

			info.fMask = info.fMask | MIM_STYLE;
			info.dwStyle = info.dwStyle | MNS_NOTIFYBYPOS;
			SetMenuInfo(hMainMenu, &info);

		}

		if
		(
			!CreateWindowEx
			(
				(DWORD)NULL,
				STR_APP_CLASS,
				STR_APP_TITLE,
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				326,
				523,
				(HWND)NULL,
				hMainMenu,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			)
		)
		{
			DestroyMenu(hMainMenu);
			return FALSE;
		}

		return TRUE;

	}

	LRESULT MainWindow::AddComputerName(LPWSTR computer_name)
	{
		return SendMessage
		(
			MainWindow::hComputersListBox,
			LB_ADDSTRING,
			(WPARAM)NULL,
			(LPARAM)computer_name
		);
	}

	// Private

	BOOL StartShutdown(BOOL bRebootAfterShutdown)
	{

		if
		(
			!NativeShutdown::SetShutdownPrivilege
			(
				(LPCWSTR)NULL,
				true
			)
		)
			return FALSE;

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

		DWORD  dwReason = SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER;

		if (Button_GetCheck(MainWindow::hPlannedCheckBox))
			dwReason = dwReason | SHTDN_REASON_FLAG_PLANNED;

		const BOOL result = InitiateSystemShutdownEx
		(
			(LPWSTR)NULL,
			message,
			timer_value,
			Button_GetCheck(MainWindow::hForceCheckBox),
			bRebootAfterShutdown,
			dwReason
		);

		if (msg_length > 0)
			delete[] message;

		return result;

	}

	BOOL StopShutdown()
	{

		if
		(
			!NativeShutdown::SetShutdownPrivilege
			(
				(LPCWSTR)NULL,
				true
			)
		)
			return FALSE;

		return AbortSystemShutdown
		(
			(LPWSTR)NULL
		);

	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		switch (message)
		{

		case WM_CREATE:

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
				hWnd,
				(HMENU)IDS_ACTION_STATIC_TITLE,
				MainWindow::hMainInstance,
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
				hWnd,
				(HMENU)IDS_ACTIONS_COMBOBOX_TITLE,
				MainWindow::hMainInstance,
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
				hWnd,
				(HMENU)IDS_EXECACTION_BUTTON_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			CreateWindowEx
			(
				(DWORD)NULL,
				WC_STATIC,
				STR_COMPUTERS_STATIC_TITLE,
				WS_VISIBLE | WS_CHILD | SS_SIMPLE,
				10,
				65,
				180,
				20,
				hWnd,
				(HMENU)IDS_COMPUTERS_STATIC_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			MainWindow::hComputersListBox = CreateWindowEx
			(
				WS_EX_CLIENTEDGE,
				WC_LISTBOX,
				STR_COMPUTERS_EDIT_TITLE,
				WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_DISABLED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY,
				10,
				85,
				200,
				100,
				hWnd,
				(HMENU)IDS_COMPUTERS_EDIT_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			MainWindow::hAddComputersButton = CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				STR_ADDCOMPUTERS_BUTTON_TITLE,
				WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
				220,
				85,
				80,
				30,
				hWnd,
				(HMENU)IDS_ADDCOMPUTERS_BUTTON_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			MainWindow::hRemoveComputersButton = CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				STR_REMOVECOMPUTERS_BUTTON_TITLE,
				WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
				220,
				120,
				80,
				30,
				hWnd,
				(HMENU)IDS_REMOVECOMPUTERS_BUTTON_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			MainWindow::hClearComputersButton = CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				STR_CLEARCOMPUTERS_BUTTON_TITLE,
				WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
				220,
				155,
				80,
				30,
				hWnd,
				(HMENU)IDS_CLEARCOMPUTERS_BUTTON_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			CreateWindowEx
			(
				(DWORD)NULL,
				WC_STATIC,
				STR_TIMER_STATIC_TITLE,
				WS_VISIBLE | WS_CHILD | SS_SIMPLE,
				10,
				195,
				180,
				20,
				hWnd,
				(HMENU)IDS_TIMER_STATIC_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			MainWindow::hTimerEdit = CreateWindowEx
			(
				WS_EX_CLIENTEDGE,
				WC_EDIT,
				STR_TIMER_EDIT_TITLE,
				WS_VISIBLE | WS_CHILD | WS_DISABLED | ES_LEFT | ES_NUMBER | ES_NOHIDESEL,
				10,
				220,
				40,
				20,
				hWnd,
				(HMENU)IDS_TIMER_EDIT_TITLE,
				MainWindow::hMainInstance,
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
				215,
				80,
				30,
				hWnd,
				(HMENU)IDS_TIMER_DEF_BUTTON_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			MainWindow::hForceCheckBox = CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				STR_FORCE_CHECKBOX_TITLE,
				WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX,
				10,
				255,
				160,
				20,
				hWnd,
				(HMENU)IDS_FORCE_CHECKBOX_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			MainWindow::hPlannedCheckBox = CreateWindowEx
			(
				(DWORD)NULL,
				WC_BUTTON,
				STR_PLANNED_CHECKBOX_TITLE,
				WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX,
				175,
				255,
				160,
				20,
				hWnd,
				(HMENU)IDS_PLANNED_CHECKBOX_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			Button_SetCheck(MainWindow::hPlannedCheckBox, TRUE);

			CreateWindowEx
			(
				(DWORD)NULL,
				WC_STATIC,
				STR_MESSAGE_STATIC_TITLE,
				WS_VISIBLE | WS_CHILD | SS_SIMPLE,
				10,
				285,
				180,
				20,
				hWnd,
				(HMENU)IDS_MESSAGE_STATIC_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			MainWindow::hMessageEdit = CreateWindowEx
			(
				WS_EX_CLIENTEDGE,
				WC_EDIT,
				STR_MESSAGE_EDIT_TITLE,
				WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_DISABLED | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_NOHIDESEL,
				10,
				305,
				290,
				150,
				hWnd,
				(HMENU)IDS_MESSAGE_EDIT_TITLE,
				MainWindow::hMainInstance,
				(LPVOID)NULL
			);

			Edit_LimitText(MainWindow::hMessageEdit, 511);

		break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_CTLCOLORSTATIC:

		{
			HWND hWnd = (HWND)lParam;

			if
			(
				(hWnd == MainWindow::hTimerEdit) || (hWnd == MainWindow::hMessageEdit)
			)
				break;
		}

		return (LRESULT)(COLOR_WINDOW + 1);

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

						EnableWindow(MainWindow::hExecActionButton, TRUE);
						SendMessage(MainWindow::hComputersListBox, LB_SETCURSEL, -1, NULL);
						EnableWindow(MainWindow::hComputersListBox, TRUE);
						EnableWindow(MainWindow::hAddComputersButton, TRUE);
						EnableWindow(MainWindow::hRemoveComputersButton, TRUE);
						EnableWindow(MainWindow::hClearComputersButton, TRUE);
						EnableWindow(MainWindow::hTimerEdit, TRUE);
						EnableWindow(MainWindow::hDefTimerButton, TRUE);
						EnableWindow(MainWindow::hForceCheckBox, TRUE);
						EnableWindow(MainWindow::hPlannedCheckBox, TRUE);
						EnableWindow(MainWindow::hMessageEdit, TRUE);

					break;

					case ID_ACTION_LOGOFF: case ID_ACTION_LOCK:

						EnableWindow(MainWindow::hExecActionButton, TRUE);
						SendMessage(MainWindow::hComputersListBox, LB_SETCURSEL, -1, NULL);
						EnableWindow(MainWindow::hComputersListBox, FALSE);
						EnableWindow(MainWindow::hAddComputersButton, FALSE);
						EnableWindow(MainWindow::hRemoveComputersButton, FALSE);
						EnableWindow(MainWindow::hClearComputersButton, FALSE);
						EnableWindow(MainWindow::hTimerEdit, FALSE);
						EnableWindow(MainWindow::hDefTimerButton, FALSE);
						EnableWindow(MainWindow::hForceCheckBox, FALSE);
						EnableWindow(MainWindow::hPlannedCheckBox, FALSE);
						EnableWindow(MainWindow::hMessageEdit, FALSE);

					break;

					case ID_ACTION_CANCEL:

						EnableWindow(MainWindow::hExecActionButton, TRUE);
						SendMessage(MainWindow::hComputersListBox, LB_SETCURSEL, -1, NULL);
						EnableWindow(MainWindow::hComputersListBox, TRUE);
						EnableWindow(MainWindow::hAddComputersButton, TRUE);
						EnableWindow(MainWindow::hRemoveComputersButton, TRUE);
						EnableWindow(MainWindow::hClearComputersButton, TRUE);
						EnableWindow(MainWindow::hTimerEdit, FALSE);
						EnableWindow(MainWindow::hDefTimerButton, FALSE);
						EnableWindow(MainWindow::hForceCheckBox, FALSE);
						EnableWindow(MainWindow::hPlannedCheckBox, FALSE);
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
							TaskDialog
							(
								hWnd,
								MainWindow::hMainInstance,
								L"Error",
								L"Failed to shutdown current system",
								L"Possibly this action has been restricted by administrator",
								TDCBF_OK_BUTTON,
								TD_ERROR_ICON,
								(int*)NULL
							);
					break;

					case ID_ACTION_REBOOT:
						if (!MainWindow::StartShutdown(TRUE))
							TaskDialog
							(
								hWnd,
								MainWindow::hMainInstance,
								L"Error",
								L"Failed to reboot current system",
								L"Possibly this action has been restricted by administrator",
								TDCBF_OK_BUTTON,
								TD_ERROR_ICON,
								(int*)NULL
							);
					break;

					case ID_ACTION_LOGOFF:

						if (!NativeShutdown::UserLogOff())
							TaskDialog
							(
								hWnd,
								MainWindow::hMainInstance,
								L"Error",
								L"Failed to log off current user",
								(PCWSTR)NULL,
								TDCBF_OK_BUTTON,
								TD_ERROR_ICON,
								(int*)NULL
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

						TaskDialog
						(
							hWnd,
							MainWindow::hMainInstance,
							L"Error",
							L"Failed to lock current user",
							(PCWSTR)NULL,
							TDCBF_OK_BUTTON,
							TD_ERROR_ICON,
							(int*)NULL
						);

					break;

					case ID_ACTION_CANCEL:
						if (!MainWindow::StopShutdown())
							TaskDialog
							(
								hWnd,
								MainWindow::hMainInstance,
								L"Error",
								L"Failed to cancel shutdown",
								L"Possibly shutdown process has not been begun or shutdown privilege has been restricted by administrator",
								TDCBF_OK_BUTTON,
								TD_ERROR_ICON,
								(int*)NULL
							);
					break;

					}

				break;

				case IDS_ADDCOMPUTERS_BUTTON_TITLE:

					AddComputersDialogInternals::__AddComputersDialog::ShowDialog
					(
						MainWindow::hMainInstance,
						hWnd
					);

				break;

				case IDS_REMOVECOMPUTERS_BUTTON_TITLE:

				{

					const auto index = SendMessage(MainWindow::hComputersListBox, LB_GETCURSEL, NULL, NULL);

					if (index != LB_ERR)
						SendMessage(MainWindow::hComputersListBox, LB_DELETESTRING, (WPARAM)index, NULL);

				}

				break;

				case IDS_CLEARCOMPUTERS_BUTTON_TITLE:

				if
				(
					SendMessage(MainWindow::hComputersListBox, LB_GETCOUNT,NULL, NULL) > 0
				)
				{

					int result = 0;

					TaskDialog
					(
						hWnd,
						MainWindow::hMainInstance,
						L"Clear list of computer names",
						L"Are you sure you want to clear all computer names from list?",
						(PCWSTR)NULL,
						TDCBF_YES_BUTTON | TDCBF_NO_BUTTON,
						TD_WARNING_ICON,
						&result
					);

					if (result != IDYES)
						break;

					SendMessage(MainWindow::hComputersListBox, LB_SETCURSEL, -1, NULL);
					SendMessage(MainWindow::hComputersListBox, LB_RESETCONTENT, NULL, NULL);

				}

				break;

				case IDS_TIMER_DEF_BUTTON_TITLE:

					SetWindowText(MainWindow::hTimerEdit, STR_TIMER_EDIT_TITLE);
					Edit_SetSel(MainWindow::hTimerEdit, 3, 3);
					SetFocus(MainWindow::hTimerEdit);

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
						Edit_SetSel(MainWindow::hTimerEdit, 3, 3);
					}

				break;

				}

			break;

			}

		break;

		case WM_MENUCOMMAND:

			switch
			(
				GetMenuItemID((HMENU)lParam, (int)wParam)
			)
			{

			case IDS_OPEN_POPUP_ITEM:
				TaskDialog
				(
					hWnd,
					MainWindow::hMainInstance,
					STR_ABOUT_POPUP_ITEM,
					STR_APP_TITLE,
					STR_APP_DESCRIPTION,
					TDCBF_OK_BUTTON,
					TD_INFORMATION_ICON,
					(int*)NULL
				);
			break;

			case IDS_SYSTEMDIALOG_POPUP_ITEM:

				if (NativeShutdown::ShowShutdownDialog() != S_OK)
					break;

				PostMessage
				(
					hWnd,
					WM_CLOSE,
					(WPARAM)NULL,
					(LPARAM)NULL
				);

			return 0;

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
				TaskDialog
				(
					hWnd,
					MainWindow::hMainInstance,
					STR_ABOUT_POPUP_ITEM,
					STR_APP_TITLE,
					STR_APP_DESCRIPTION,
					TDCBF_OK_BUTTON,
					TD_INFORMATION_ICON,
					(int*)NULL
				);
			break;

			}

		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	}

}