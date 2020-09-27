#include "stdafx.h"
#include "Resources.h"
#include "MainWindow.h"

// Private

void MainWindow::__MainWindow::MainWindowProc::ActionComboBoxSelectionChanged(MainWindow::__MainWindow* pWndObj)
{
	switch (ComboBox_GetCurSel(pWndObj->hActionsComboBox))
	{

	case Shutdown::ActionTypes::Shutdown: case Shutdown::ActionTypes::Reboot:

		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_EXECUTE_ACTION_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_MESSAGE_EDIT_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		ListBox_SetCurSel(pWndObj->hComputersListBox, -1);
		EnableWindow(pWndObj->hComputersListBox, TRUE);
		EnableWindow(pWndObj->hAddComputersButton, TRUE);
		EnableWindow(pWndObj->hEditComputersButton, TRUE);
		EnableWindow(pWndObj->hRemoveComputersButton, TRUE);
		EnableWindow(pWndObj->hTimerEdit, TRUE);
		EnableWindow(pWndObj->hDefTimerButton, TRUE);
		EnableWindow(pWndObj->hForceCheckBox, TRUE);
		EnableWindow(pWndObj->hPlannedCheckBox, TRUE);
		EnableWindow(pWndObj->hMessageEdit, TRUE);

		break;

	case Shutdown::ActionTypes::LogOff: case Shutdown::ActionTypes::Lock: case Shutdown::ActionTypes::Suspend: case Shutdown::ActionTypes::Hibernate:

		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_EXECUTE_ACTION_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_MESSAGE_EDIT_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		ListBox_SetCurSel(pWndObj->hComputersListBox, -1);
		EnableWindow(pWndObj->hComputersListBox, FALSE);
		EnableWindow(pWndObj->hAddComputersButton, FALSE);
		EnableWindow(pWndObj->hEditComputersButton, FALSE);
		EnableWindow(pWndObj->hRemoveComputersButton, FALSE);
		EnableWindow(pWndObj->hTimerEdit, FALSE);
		EnableWindow(pWndObj->hDefTimerButton, FALSE);
		EnableWindow(pWndObj->hForceCheckBox, FALSE);
		EnableWindow(pWndObj->hPlannedCheckBox, FALSE);
		EnableWindow(pWndObj->hMessageEdit, FALSE);

		break;

	case Shutdown::ActionTypes::Cancel:

		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_EXECUTE_ACTION_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_MESSAGE_EDIT_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
		ListBox_SetCurSel(pWndObj->hComputersListBox, -1);
		EnableWindow(pWndObj->hComputersListBox, TRUE);
		EnableWindow(pWndObj->hAddComputersButton, TRUE);
		EnableWindow(pWndObj->hEditComputersButton, TRUE);
		EnableWindow(pWndObj->hRemoveComputersButton, TRUE);
		EnableWindow(pWndObj->hTimerEdit, FALSE);
		EnableWindow(pWndObj->hDefTimerButton, FALSE);
		EnableWindow(pWndObj->hForceCheckBox, FALSE);
		EnableWindow(pWndObj->hPlannedCheckBox, FALSE);
		EnableWindow(pWndObj->hMessageEdit, FALSE);

		break;

	default:

		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_EXECUTE_ACTION_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(GetMenu(pWndObj->hMainWindow), IDS_RESET_MESSAGE_EDIT_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
		ListBox_SetCurSel(pWndObj->hComputersListBox, -1);
		EnableWindow(pWndObj->hComputersListBox, FALSE);
		EnableWindow(pWndObj->hAddComputersButton, FALSE);
		EnableWindow(pWndObj->hEditComputersButton, FALSE);
		EnableWindow(pWndObj->hRemoveComputersButton, FALSE);
		EnableWindow(pWndObj->hTimerEdit, FALSE);
		EnableWindow(pWndObj->hDefTimerButton, FALSE);
		EnableWindow(pWndObj->hForceCheckBox, FALSE);
		EnableWindow(pWndObj->hPlannedCheckBox, FALSE);
		EnableWindow(pWndObj->hMessageEdit, FALSE);

		break;
	}
}

void MainWindow::__MainWindow::MainWindowProc::ExecActionButtonClick(MainWindow::__MainWindow* pWndObj)
{
	switch (ComboBox_GetCurSel(pWndObj->hActionsComboBox))
	{

	case Shutdown::ActionTypes::Shutdown:
		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

		if (!pWndObj->StartComputersShutdown(FALSE))
		{
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
			pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
			TaskDialog
			(
				pWndObj->hMainWindow,
				pWndObj->hMainInstance,
				L"Error",
				L"Failed to shutdown one computer or more ones",
				L"Possibly these computers are unavailable or this action has been restricted by administrator",
				TDCBF_OK_BUTTON,
				TD_ERROR_ICON,
				(int*)NULL
			);
			break;
		}

		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

	case Shutdown::ActionTypes::Reboot:
		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

		if (!pWndObj->StartComputersShutdown(TRUE))
		{
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
			pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
			TaskDialog
			(
				pWndObj->hMainWindow,
				pWndObj->hMainInstance,
				L"Error",
				L"Failed to reboot one computer or more ones",
				L"Possibly these computers are unavailable or this action has been restricted by administrator",
				TDCBF_OK_BUTTON,
				TD_ERROR_ICON,
				(int*)NULL
			);
			break;
		}

		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

	case Shutdown::ActionTypes::LogOff:
		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

		if
			(
				!ExitWindowsEx
				(
					EWX_LOGOFF,
					SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED
				)
				)
		{
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
			pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
			TaskDialog
			(
				pWndObj->hMainWindow,
				pWndObj->hMainInstance,
				L"Error",
				L"Failed to log off current user",
				(PCWSTR)NULL,
				TDCBF_OK_BUTTON,
				TD_ERROR_ICON,
				(int*)NULL
			);
			break;
		}

		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

	case Shutdown::ActionTypes::Lock:
		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

		if (!LockWorkStation())
		{
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
			pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
			TaskDialog
			(
				pWndObj->hMainWindow,
				pWndObj->hMainInstance,
				L"Error",
				L"Failed to lock current user",
				(PCWSTR)NULL,
				TDCBF_OK_BUTTON,
				TD_ERROR_ICON,
				(int*)NULL
			);
			break;
		}

		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		Shutdown::MainStaticObject::CloseWindow(pWndObj->hMainWindow);
		return;

	case Shutdown::ActionTypes::Cancel:
		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

		if (!pWndObj->StopComputersShutdown())
		{
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
			pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
			TaskDialog
			(
				pWndObj->hMainWindow,
				pWndObj->hMainInstance,
				L"Error",
				L"Failed to cancel shutdown on one computer or more ones",
				L"Possibly these computers are unavailable or this action has been restricted by administrator",
				TDCBF_OK_BUTTON,
				TD_ERROR_ICON,
				(int*)NULL
			);
			break;
		}

		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

	case Shutdown::ActionTypes::Suspend:
		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

		{
			auto func = [pWndObj]() -> BOOL
			{
				if
					(
						!Shutdown::MainStaticObject::SetShutdownPrivilege((LPWSTR)NULL)
						)
					return FALSE;

				BOOL result = SetSuspendState((BOOLEAN)FALSE, (BOOLEAN)FALSE, (BOOLEAN)FALSE);

				if (result)
					Shutdown::MainStaticObject::CloseWindow(pWndObj->hMainWindow);

				return result;
			};

			if (!func())
			{
				SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
				pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
				TaskDialog
				(
					pWndObj->hMainWindow,
					pWndObj->hMainInstance,
					L"Error",
					L"Failed to suspend current computer",
					(PCWSTR)NULL,
					TDCBF_OK_BUTTON,
					TD_ERROR_ICON,
					(int*)NULL
				);
				break;
			}
		}

		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

	case Shutdown::ActionTypes::Hibernate:
		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

		{
			auto func = [pWndObj]() -> BOOL
			{
				if
				(
					!Shutdown::MainStaticObject::SetShutdownPrivilege((LPWSTR)NULL)
				)
					return FALSE;

				BOOL result = SetSuspendState((BOOLEAN)TRUE, (BOOLEAN)FALSE, (BOOLEAN)FALSE);

				if (result)
					Shutdown::MainStaticObject::CloseWindow(pWndObj->hMainWindow);

				return result;
			};

			if (!func())
			{
				SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
				pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
				TaskDialog
				(
					pWndObj->hMainWindow,
					pWndObj->hMainInstance,
					L"Error",
					L"Failed to hibernate current computer",
					(PCWSTR)NULL,
					TDCBF_OK_BUTTON,
					TD_ERROR_ICON,
					(int*)NULL
				);
				break;
			}
		}

		SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
		pWndObj->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

	}
}

// Public

MainWindow::__MainWindow* MainWindow::__MainWindow::MainWindowProc::GetInstance(HWND hWnd)
{
	return reinterpret_cast<MainWindow::__MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

LRESULT MainWindow::__MainWindow::MainWindowProc::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CTLCOLORSTATIC:

	{
		HWND hWndChild = (HWND)lParam;
		__MainWindow* pWndObj = MainWindow::__MainWindow::MainWindowProc::GetInstance(hWnd);

		if
		(
			(hWndChild == pWndObj->hTimerEdit) || (hWndChild == pWndObj->hMessageEdit)
		)
			break;
	}

	return (LRESULT)(COLOR_WINDOW + 1);

	case WM_NOTIFY:
	{
		LPNMHDR nmhdr = (LPNMHDR)lParam;

		if (nmhdr->code == BCN_DROPDOWN)
		{
			NMBCDROPDOWN* pDropDown = (NMBCDROPDOWN*)lParam;

			POINT pt = { 0 };
			pt.x = pDropDown->rcButton.left;
			pt.y = pDropDown->rcButton.bottom;
			ClientToScreen(nmhdr->hwndFrom, &pt);

			HMENU splitPopupMenu = CreatePopupMenu();

			switch (nmhdr->idFrom)
			{

			case IDS_EXECACTION_BUTTON_TITLE:
			{
				UINT uflags = MF_STRING;
				__MainWindow* pWndObj = MainWindow::__MainWindow::MainWindowProc::GetInstance(hWnd);

				if (ComboBox_GetCurSel(pWndObj->hActionsComboBox) < 0)
					uflags |= MF_GRAYED;

				AppendMenu
				(
					splitPopupMenu,
					uflags,
					IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM,
					STR_RESET_ACTIONS_COMBOBOX_POPUP_ITEM
				);
			}
			AppendMenu
			(
				splitPopupMenu,
				MF_SEPARATOR,
				(UINT_PTR)NULL,
				(LPCWSTR)NULL
			);
			AppendMenu
			(
				splitPopupMenu,
				MF_STRING,
				IDS_OPEN_SYSTEM_DIALOG_POPUP_ITEM,
				STR_OPEN_SYSTEM_DIALOG_POPUP_ITEM
			);
			break;

			case IDS_ADDCOMPUTERS_BUTTON_TITLE:
				AppendMenu
				(
					splitPopupMenu,
					MF_STRING,
					IDS_ADD_COMPUTERS_FROM_FILE_POPUP_ITEM,
					STR_ADD_COMPUTERS_FROM_FILE_POPUP_ITEM
				);
				AppendMenu
				(
					splitPopupMenu,
					MF_STRING,
					IDS_SAVE_COMPUTERS_TO_FILE_POPUP_ITEM,
					STR_SAVE_COMPUTERS_TO_FILE_POPUP_ITEM
				);
				break;

			case IDS_REMOVECOMPUTERS_BUTTON_TITLE:
				AppendMenu
				(
					splitPopupMenu,
					MF_STRING,
					IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM,
					STR_RESET_COMPUTERS_LISTBOX_POPUP_ITEM
				);
				break;

			}

			if (splitPopupMenu)
			{
				TrackPopupMenu(splitPopupMenu, TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, hWnd, NULL);
				DestroyMenu(splitPopupMenu);
			}
		}
	}
	break;

	case WM_COMMAND:

		switch (HIWORD(wParam))
		{

		case CBN_SELCHANGE:

			switch (LOWORD(wParam))
			{

			case IDS_ACTIONS_COMBOBOX_TITLE:
			{
				__MainWindow* pWndObj = MainWindow::__MainWindow::MainWindowProc::GetInstance(hWnd);
				ActionComboBoxSelectionChanged(pWndObj);
			}
				break;

			}

			break;

		case BN_CLICKED:
		{
			__MainWindow* pWndObj = MainWindow::__MainWindow::MainWindowProc::GetInstance(hWnd);
			switch (LOWORD(wParam))
			{

				//
			case IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM:
				Shutdown::MainStaticObject::ComboBoxSelectItem(pWndObj->hActionsComboBox, -1);
				break;

			case IDS_OPEN_SYSTEM_DIALOG_POPUP_ITEM:
				if (Shutdown::MainStaticObject::ShowShutdownDialog())
					Shutdown::MainStaticObject::CloseWindow(pWndObj->hMainWindow);
				return 0;

			case IDS_ADD_COMPUTERS_FROM_FILE_POPUP_ITEM:
				pWndObj->LoadComputerNamesFromFile();
				break;

			case IDS_SAVE_COMPUTERS_TO_FILE_POPUP_ITEM:
				pWndObj->SaveComputerNamesToFile();
				break;

			case IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM:
				pWndObj->ClearComputerNames();
				break;
				//

			case IDS_EXECACTION_BUTTON_TITLE:
				ExecActionButtonClick(pWndObj);
				break;

			case IDS_ADDCOMPUTERS_BUTTON_TITLE:
				pWndObj->ShowAddComputersDialog();
				break;

			case IDS_EDITCOMPUTERS_BUTTON_TITLE:
				pWndObj->ShowEditComputersDialog();
				break;

			case IDS_REMOVECOMPUTERS_BUTTON_TITLE:
			{
				int index = ListBox_GetCurSel(pWndObj->hComputersListBox);
				if (index >= 0 && (ListBox_DeleteString(pWndObj->hComputersListBox, index) != LB_ERR))
					ListBox_SetCurSel(pWndObj->hComputersListBox, (--index) > 0 ? index : 0);
			}

			pWndObj->UpdateStatusBarCaption
			(
				ListBox_GetCount(pWndObj->hComputersListBox)
			);
			break;

			case IDS_TIMER_DEF_BUTTON_TITLE:

				SetWindowText(pWndObj->hTimerEdit, STR_TIMER_EDIT_TITLE);
				Edit_SetSel(pWndObj->hTimerEdit, 3, 3);
				SetFocus(pWndObj->hTimerEdit);

				break;

			}
		}
			break;

		case EN_CHANGE:

			switch (LOWORD(wParam))
			{

			case IDS_TIMER_EDIT_TITLE:
			{
				__MainWindow* pWndObj = MainWindow::__MainWindow::MainWindowProc::GetInstance(hWnd);
				if (GetWindowTextLength(pWndObj->hTimerEdit) <= 0)
				{
					SetWindowText(pWndObj->hTimerEdit, STR_TIMER_EDIT_TITLE);
					Edit_SetSel(pWndObj->hTimerEdit, 3, 3);
				}
			}
				break;

			}

			break;

		}
		break;
	case WM_MENUCOMMAND:
	{
		__MainWindow* pWndObj = MainWindow::__MainWindow::MainWindowProc::GetInstance(hWnd);

		switch
		(
			GetMenuItemID((HMENU)lParam, (int)wParam)
		)
		{

		case IDS_NEW_POPUP_ITEM:
			pWndObj->ClearComputerNames();
			break;

		case IDS_OPEN_POPUP_ITEM:
			pWndObj->LoadComputerNamesFromFile();
			break;

		case IDS_SAVE_POPUP_ITEM:
			pWndObj->SaveComputerNamesToFile();
			break;

		case IDS_SYSTEMDIALOG_POPUP_ITEM:
			if (Shutdown::MainStaticObject::ShowShutdownDialog())
				Shutdown::MainStaticObject::CloseWindow(pWndObj->hMainWindow);
			return 0;

		case IDS_EXIT_POPUP_ITEM:
			Shutdown::MainStaticObject::CloseWindow(pWndObj->hMainWindow);
			return 0;

		case IDS_EXECUTE_ACTION_POPUP_ITEM:
			ExecActionButtonClick(pWndObj);
			break;

		case IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM:
			Shutdown::MainStaticObject::ComboBoxSelectItem(pWndObj->hActionsComboBox, -1);
			break;

		case IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM:
			pWndObj->ClearComputerNames();
			break;

		case IDS_RESET_MESSAGE_EDIT_POPUP_ITEM:
			pWndObj->ClearMessage();
			break;

		case IDS_SOURCE_AT_GITHUB_POPUP_ITEM:
			ShellExecute(NULL, NULL, L"https://github.com/DP458/Shutdown", NULL, NULL, SW_SHOW);
			break;

		case IDS_VIEW_WIKI_POPUP_ITEM:
			ShellExecute(NULL, NULL, L"https://github.com/DP458/Shutdown/wiki", NULL, NULL, SW_SHOW);
			break;

		case IDS_ABOUT_POPUP_ITEM:
			pWndObj->ShowAboutDialog();
			break;

		}
	}
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}