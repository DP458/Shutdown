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

	MainWindow::__MainWindow::__MainWindow(HINSTANCE hInstance) :
		hMainInstance(hInstance), bIsClassRegistered(FALSE), hMainWindow(NULL), hActionsComboBox(NULL), hExecActionButton(NULL),
		hComputersListBox(NULL), hAddComputersButton(NULL), hRemoveComputersButton(NULL), hClearComputersButton(NULL), hTimerEdit(NULL),
		hDefTimerButton(NULL), hForceCheckBox(NULL), hPlannedCheckBox(NULL), hMessageEdit(NULL), hStatusBar(NULL)
	{
	}

	void MainWindow::__MainWindow::InitMainWindow()
	{
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
			this->hMainWindow,
			(HMENU)IDS_ACTION_STATIC_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hActionsComboBox = CreateWindowEx
		(
			(DWORD)NULL,
			WC_COMBOBOX,
			STR_ACTIONS_COMBOBOX_TITLE,
			WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
			10,
			28,
			200,
			40,
			this->hMainWindow,
			(HMENU)IDS_ACTIONS_COMBOBOX_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		ComboBox_InsertString(this->hActionsComboBox, ID_ACTION_SHUTDOWN, ACTION_SHUTDOWN);
		ComboBox_InsertString(this->hActionsComboBox, ID_ACTION_REBOOT, ACTION_REBOOT);
		ComboBox_InsertString(this->hActionsComboBox, ID_ACTION_LOGOFF, ACTION_LOGOFF);
		ComboBox_InsertString(this->hActionsComboBox, ID_ACTION_LOCK, ACTION_LOCK);
		ComboBox_InsertString(this->hActionsComboBox, ID_ACTION_CANCEL, ACTION_CANCEL);

		this->hExecActionButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_EXECACTION_BUTTON_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			220,
			25,
			80,
			30,
			this->hMainWindow,
			(HMENU)IDS_EXECACTION_BUTTON_TITLE,
			this->hMainInstance,
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
			this->hMainWindow,
			(HMENU)IDS_COMPUTERS_STATIC_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hComputersListBox = CreateWindowEx
		(
			WS_EX_CLIENTEDGE,
			WC_LISTBOX,
			STR_COMPUTERS_EDIT_TITLE,
			WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_DISABLED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY,
			10,
			85,
			200,
			100,
			this->hMainWindow,
			(HMENU)IDS_COMPUTERS_EDIT_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hAddComputersButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_ADDCOMPUTERS_BUTTON_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			220,
			85,
			80,
			30,
			this->hMainWindow,
			(HMENU)IDS_ADDCOMPUTERS_BUTTON_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hRemoveComputersButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_REMOVECOMPUTERS_BUTTON_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			220,
			120,
			80,
			30,
			this->hMainWindow,
			(HMENU)IDS_REMOVECOMPUTERS_BUTTON_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hClearComputersButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_CLEARCOMPUTERS_BUTTON_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			220,
			155,
			80,
			30,
			this->hMainWindow,
			(HMENU)IDS_CLEARCOMPUTERS_BUTTON_TITLE,
			this->hMainInstance,
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
			this->hMainWindow,
			(HMENU)IDS_TIMER_STATIC_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hTimerEdit = CreateWindowEx
		(
			WS_EX_CLIENTEDGE,
			WC_EDIT,
			STR_TIMER_EDIT_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | ES_LEFT | ES_NUMBER | ES_NOHIDESEL,
			10,
			220,
			40,
			20,
			this->hMainWindow,
			(HMENU)IDS_TIMER_EDIT_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		Edit_LimitText(this->hTimerEdit, 4);

		this->hDefTimerButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_TIMER_DEF_BUTTON_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			60,
			215,
			80,
			30,
			this->hMainWindow,
			(HMENU)IDS_TIMER_DEF_BUTTON_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hForceCheckBox = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_FORCE_CHECKBOX_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX,
			10,
			255,
			160,
			20,
			this->hMainWindow,
			(HMENU)IDS_FORCE_CHECKBOX_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hPlannedCheckBox = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_PLANNED_CHECKBOX_TITLE,
			WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX,
			175,
			255,
			160,
			20,
			this->hMainWindow,
			(HMENU)IDS_PLANNED_CHECKBOX_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		Button_SetCheck(this->hPlannedCheckBox, TRUE);

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
			this->hMainWindow,
			(HMENU)IDS_MESSAGE_STATIC_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hMessageEdit = CreateWindowEx
		(
			WS_EX_CLIENTEDGE,
			WC_EDIT,
			STR_MESSAGE_EDIT_TITLE,
			WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_DISABLED | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_NOHIDESEL,
			10,
			305,
			290,
			150,
			this->hMainWindow,
			(HMENU)IDS_MESSAGE_EDIT_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		Edit_LimitText(this->hMessageEdit, 511);

		this->hStatusBar = CreateWindowEx
		(
			(DWORD)NULL,
			STATUSCLASSNAME,
			STR_STATUS_BAR_TITLE,
			WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			this->hMainWindow,
			(HMENU)IDS_STATUS_BAR_TITLE,
			this->hMainInstance,
			(LPVOID)NULL
		);

		SendMessage(this->hStatusBar, SB_SIMPLE, FALSE, (LPARAM)NULL);

		{
			const int parts_count = 2;
			const int parts[parts_count] = { 120, 260 };

			SendMessage
			(
				this->hStatusBar,
				SB_SETPARTS,
				(WPARAM)parts_count,
				(LPARAM)&parts
			);
		}

		this->UpdateStatusBarCaption(0);
		this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Ready);
	}

	/*
	Function GetTimerValue()
	Returns DWORD value from Timer edit box
	Returns ULONG_MAX if error has occurred
	*/
	DWORD MainWindow::__MainWindow::GetTimerValue()
	{
		const int timer_length = GetWindowTextLength(this->hTimerEdit);

		if (timer_length <= 0)
			return ULONG_MAX;

		auto psTimer = std::make_unique<wchar_t[]>(timer_length + 1);

		GetWindowText
		(
			this->hTimerEdit,
			psTimer.get(),
			timer_length + 1
		);

		const DWORD timer_value = wcstoul
		(
			psTimer.get(),
			(wchar_t**)NULL,
			10
		);

		return timer_value;
	}

	BOOL MainWindow::__MainWindow::StartShutdown(int listbox_index, BOOL bRebootAfterShutdown)
	{
		LPWSTR computer_name = nullptr;

		if (listbox_index >= 0)
		{
			const int computer_name_length = ListBox_GetTextLen(this->hComputersListBox, listbox_index);

			if (computer_name_length < 0)
				return FALSE;

			try
			{
				computer_name = new WCHAR[computer_name_length + 1];
			}
			catch (...)
			{
				return FALSE;
			}

			ListBox_GetText
			(
				this->hComputersListBox,
				listbox_index,
				computer_name
			);
		}

		if
		(
			!NativeShutdown::SetShutdownPrivilege
			(
				computer_name,
				true
			)
		)
		{	
			delete[] computer_name;
			return FALSE;
		}

		LPWSTR message = nullptr;

		{
			const int msg_length = GetWindowTextLength(this->hMessageEdit);

			if (msg_length > 0)
			{
				try
				{
					message = new WCHAR[msg_length + 1];
				}
				catch (...)
				{
					delete[] computer_name;
					return FALSE;
				}

				GetWindowText
				(
					this->hMessageEdit,
					message,
					msg_length + 1
				);
			}
		}

		const DWORD timer_value = this->GetTimerValue();

		if (timer_value == ULONG_MAX)
		{
			delete[] message;
			delete[] computer_name;
			return FALSE;
		}

		DWORD dwReason = SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER;

		if
		(
			Button_GetCheck(this->hPlannedCheckBox)
		)
			dwReason |= SHTDN_REASON_FLAG_PLANNED;

		const BOOL result = InitiateSystemShutdownEx
		(
			computer_name,
			message,
			timer_value,
			Button_GetCheck(this->hForceCheckBox),
			bRebootAfterShutdown,
			dwReason
		);

		delete[] message;
		delete[] computer_name;
		return result;
	}

	BOOL MainWindow::__MainWindow::StopShutdown(int listbox_index)
	{	
		if (listbox_index < 0)
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

			return AbortSystemShutdown((LPWSTR)NULL);
		}

		const int computer_name_length = ListBox_GetTextLen
		(
			this->hComputersListBox,
			listbox_index
		);

		if (computer_name_length < 0)
			return FALSE;

		auto psComputerName = std::make_unique<wchar_t[]>(computer_name_length + 1);

		ListBox_GetText
		(
			this->hComputersListBox,
			listbox_index,
			psComputerName.get()
		);

		if
		(
			!NativeShutdown::SetShutdownPrivilege
			(
				psComputerName.get(),
				true
			)
		)
			return FALSE;

		return AbortSystemShutdown(psComputerName.get());
	}

	void MainWindow::__MainWindow::ExecActionButtonClick()
	{

		const int item_id = ComboBox_GetCurSel(this->hActionsComboBox);

		switch (item_id)
		{

		case ID_ACTION_LOGOFF:
			if
			(
				!NativeShutdown::UserLogOff()
			)
			{
				TaskDialog
				(
					this->hMainWindow,
					this->hMainInstance,
					L"Error",
					L"Failed to log off current user",
					(PCWSTR)NULL,
					TDCBF_OK_BUTTON,
					TD_ERROR_ICON,
					(int*)NULL
				);
				this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Fail);
				return;
			}

			this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Successful);
		return;

		case ID_ACTION_LOCK:
			if
			(
				!NativeShutdown::UserLock()
			)
			{
				TaskDialog
				(
					this->hMainWindow,
					this->hMainInstance,
					L"Error",
					L"Failed to lock current user",
					(PCWSTR)NULL,
					TDCBF_OK_BUTTON,
					TD_ERROR_ICON,
					(int*)NULL
				);
				this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Fail);
				return;
			}

			this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Successful);
			this->CloseWindow();
		return;

		}

		const int count = ListBox_GetCount(this->hComputersListBox);

		switch (item_id)
		{

		case ID_ACTION_SHUTDOWN:
			if (count == 0)
			{
				if
				(
					!this->StartShutdown(-1, FALSE)
				)
				{
					TaskDialog
					(
						this->hMainWindow,
						this->hMainInstance,
						L"Error",
						L"Failed to shutdown current system",
						L"Possibly this action has been restricted by administrator",
						TDCBF_OK_BUTTON,
						TD_ERROR_ICON,
						(int*)NULL
					);
					this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Fail);
					break;
				}

				this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Successful);
				break;
			}

			for (int i = 0; i < count; i++)
				this->StartShutdown(i, FALSE);
			break;

		case ID_ACTION_REBOOT:
			if (count == 0)
			{
				if
				(
					!this->StartShutdown(-1, TRUE)
				)
				{
					TaskDialog
					(
						this->hMainWindow,
						this->hMainInstance,
						L"Error",
						L"Failed to reboot current system",
						L"Possibly this action has been restricted by administrator",
						TDCBF_OK_BUTTON,
						TD_ERROR_ICON,
						(int*)NULL
					);
					this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Fail);
					break;
				}

				this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Successful);
				break;
			}

			for (int i = 0; i < count; i++)
				this->StartShutdown(i, TRUE);
			break;

		case ID_ACTION_CANCEL:
			if (count == 0)
			{
				if
				(
					!this->StopShutdown(-1)
				)
				{
					TaskDialog
					(
						this->hMainWindow,
						this->hMainInstance,
						L"Error",
						L"Failed to reboot current system",
						L"Possibly this action has been restricted by administrator",
						TDCBF_OK_BUTTON,
						TD_ERROR_ICON,
						(int*)NULL
					);
					this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Fail);
					break;
				}

				this->UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus::Successful);
				break;
			}

			for (int i = 0; i < count; i++)
				this->StopShutdown(i);
			break;

		}

	}

	void MainWindow::__MainWindow::UpdateStatusBarCaption(int count)
	{
		std::wstringstream ws_stream;
		ws_stream << L"  Computers:  " << count;

		SendMessage
		(
			this->hStatusBar,
			SB_SETTEXT,
			(WPARAM)0,
			(LPARAM)ws_stream.str().c_str()
		);
	}

	void MainWindow::__MainWindow::UpdateStatusBarCaption(MainWindow::__MainWindow::ShutdownStatus status)
	{
		switch(status)
		{

		case ShutdownStatus::Successful:
			SendMessage
			(
				this->hStatusBar,
				SB_SETTEXT,
				(WPARAM)1,
				(LPARAM)L"  Status:  Successful"
			);
		break;

		case ShutdownStatus::Fail:
			SendMessage
			(
				this->hStatusBar,
				SB_SETTEXT,
				(WPARAM)1,
				(LPARAM)L"  Status:  Fail"
			);
		break;

		default:
			SendMessage
			(
				this->hStatusBar,
				SB_SETTEXT,
				(WPARAM)1,
				(LPARAM)L"  Status:  Ready"
			);
		break;

		}
	}

	void MainWindow::__MainWindow::ClearComputerNames()
	{

		if
		(
			ListBox_GetCount(this->hComputersListBox) > 0
		)
		{

			int result = 0;

			TaskDialog
			(
				this->hMainWindow,
				this->hMainInstance,
				L"Clear list of computer names",
				L"Are you sure you want to clear all computer names from list?",
				(PCWSTR)NULL,
				TDCBF_YES_BUTTON | TDCBF_NO_BUTTON,
				TD_WARNING_ICON,
				&result
			);

			if (result != IDYES)
				return;

			ListBox_SetCurSel(this->hComputersListBox, -1);
			ListBox_ResetContent(this->hComputersListBox);

			this->UpdateStatusBarCaption
			(
				ListBox_GetCount(this->hComputersListBox)
			);

		}

	}

	void MainWindow::__MainWindow::CloseWindow()
	{
		PostMessage
		(
			this->hMainWindow,
			WM_CLOSE,
			(WPARAM)NULL,
			(LPARAM)NULL
		);
	}

	// Local

	static MainWindow::__MainWindow* pWndObj = nullptr;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		switch (message)
		{

		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_CTLCOLORSTATIC:

		{
			HWND hWnd = (HWND)lParam;

			if
			(
				(hWnd == MainWindow::pWndObj->hTimerEdit) || (hWnd == MainWindow::pWndObj->hMessageEdit)
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

					switch (ComboBox_GetCurSel(MainWindow::pWndObj->hActionsComboBox))
					{

					case ID_ACTION_SHUTDOWN: case ID_ACTION_REBOOT:

						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
						EnableWindow(MainWindow::pWndObj->hExecActionButton, TRUE);
						SendMessage(MainWindow::pWndObj->hComputersListBox, LB_SETCURSEL, -1, NULL);
						EnableWindow(MainWindow::pWndObj->hComputersListBox, TRUE);
						EnableWindow(MainWindow::pWndObj->hAddComputersButton, TRUE);
						EnableWindow(MainWindow::pWndObj->hRemoveComputersButton, TRUE);
						EnableWindow(MainWindow::pWndObj->hClearComputersButton, TRUE);
						EnableWindow(MainWindow::pWndObj->hTimerEdit, TRUE);
						EnableWindow(MainWindow::pWndObj->hDefTimerButton, TRUE);
						EnableWindow(MainWindow::pWndObj->hForceCheckBox, TRUE);
						EnableWindow(MainWindow::pWndObj->hPlannedCheckBox, TRUE);
						EnableWindow(MainWindow::pWndObj->hMessageEdit, TRUE);

					break;

					case ID_ACTION_LOGOFF: case ID_ACTION_LOCK:

						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
						EnableWindow(MainWindow::pWndObj->hExecActionButton, TRUE);
						SendMessage(MainWindow::pWndObj->hComputersListBox, LB_SETCURSEL, -1, NULL);
						EnableWindow(MainWindow::pWndObj->hComputersListBox, FALSE);
						EnableWindow(MainWindow::pWndObj->hAddComputersButton, FALSE);
						EnableWindow(MainWindow::pWndObj->hRemoveComputersButton, FALSE);
						EnableWindow(MainWindow::pWndObj->hClearComputersButton, FALSE);
						EnableWindow(MainWindow::pWndObj->hTimerEdit, FALSE);
						EnableWindow(MainWindow::pWndObj->hDefTimerButton, FALSE);
						EnableWindow(MainWindow::pWndObj->hForceCheckBox, FALSE);
						EnableWindow(MainWindow::pWndObj->hPlannedCheckBox, FALSE);
						EnableWindow(MainWindow::pWndObj->hMessageEdit, FALSE);

					break;

					case ID_ACTION_CANCEL:

						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
						EnableMenuItem(GetMenu(MainWindow::pWndObj->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
						EnableWindow(MainWindow::pWndObj->hExecActionButton, TRUE);
						SendMessage(MainWindow::pWndObj->hComputersListBox, LB_SETCURSEL, -1, NULL);
						EnableWindow(MainWindow::pWndObj->hComputersListBox, TRUE);
						EnableWindow(MainWindow::pWndObj->hAddComputersButton, TRUE);
						EnableWindow(MainWindow::pWndObj->hRemoveComputersButton, TRUE);
						EnableWindow(MainWindow::pWndObj->hClearComputersButton, TRUE);
						EnableWindow(MainWindow::pWndObj->hTimerEdit, FALSE);
						EnableWindow(MainWindow::pWndObj->hDefTimerButton, FALSE);
						EnableWindow(MainWindow::pWndObj->hForceCheckBox, FALSE);
						EnableWindow(MainWindow::pWndObj->hPlannedCheckBox, FALSE);
						EnableWindow(MainWindow::pWndObj->hMessageEdit, FALSE);

					break;

					}

				break;

				}

			break;

			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				case IDS_EXECACTION_BUTTON_TITLE:
					MainWindow::pWndObj->ExecActionButtonClick();
				break;

				case IDS_ADDCOMPUTERS_BUTTON_TITLE:
					AddComputersDialogInternals::__AddComputersDialog::ShowDialog
					(
						MainWindow::pWndObj->hMainInstance,
						hWnd
					);
				break;

				case IDS_REMOVECOMPUTERS_BUTTON_TITLE:
				{

					const auto index = ListBox_GetCurSel(MainWindow::pWndObj->hComputersListBox);

					if (index != LB_ERR)
						ListBox_DeleteString(MainWindow::pWndObj->hComputersListBox, index);

					MainWindow::pWndObj->UpdateStatusBarCaption
					(
						ListBox_GetCount(MainWindow::pWndObj->hComputersListBox)
					);

				}
				break;

				case IDS_CLEARCOMPUTERS_BUTTON_TITLE:
					MainWindow::pWndObj->ClearComputerNames();
				break;

				case IDS_TIMER_DEF_BUTTON_TITLE:

					SetWindowText(MainWindow::pWndObj->hTimerEdit, STR_TIMER_EDIT_TITLE);
					Edit_SetSel(MainWindow::pWndObj->hTimerEdit, 3, 3);
					SetFocus(MainWindow::pWndObj->hTimerEdit);

				break;

				}

			break;

			case EN_CHANGE:

				switch (LOWORD(wParam))
				{

				case IDS_TIMER_EDIT_TITLE:

					if (GetWindowTextLength(MainWindow::pWndObj->hTimerEdit) <= 0)
					{
						SetWindowText(MainWindow::pWndObj->hTimerEdit, STR_TIMER_EDIT_TITLE);
						Edit_SetSel(MainWindow::pWndObj->hTimerEdit, 3, 3);
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

			case IDS_NEW_POPUP_ITEM:
				MainWindow::pWndObj->ClearComputerNames();
			break;

			case IDS_OPEN_POPUP_ITEM:
			{
				CComPtr<IFileOpenDialog> FileOpenDialog;

				HRESULT hRes = FileOpenDialog.CoCreateInstance
				(
					CLSID_FileOpenDialog,
					NULL,
					CLSCTX_INPROC_SERVER
				);

				if (!SUCCEEDED(hRes))
					break;

				{
					const UINT count = 2U;

					COMDLG_FILTERSPEC file_types[count];
					file_types[0].pszName = L"Plain text";
					file_types[0].pszSpec = L"*.txt";
					file_types[1].pszName = L"Plain text";
					file_types[1].pszSpec = L"*.*";

					FileOpenDialog->SetFileTypes
					(
						count,
						file_types
					);

					FileOpenDialog->SetFileTypeIndex(count - 1U);
				}

				hRes = FileOpenDialog->Show(MainWindow::pWndObj->hMainWindow);

				if (!SUCCEEDED(hRes))
					break;

				std::wifstream ifs;

				{
					CComPtr<IShellItem> item;
					FileOpenDialog->GetResult(&item);

					PWSTR file_path;

					item->GetDisplayName
					(
						SIGDN_FILESYSPATH,
						&file_path
					);

					ifs.open(file_path);

					CoTaskMemFree(file_path);
				}

				while (!ifs.eof())
				{
					std::wstring str;
					ifs >> str;

					if (ifs.fail())
						break;

					MainWindow::pWndObj->AddComputerName(str.c_str());
				}

				ifs.close();
			}
			break;

			case IDS_SAVE_POPUP_ITEM:
			{
				CComPtr<IFileSaveDialog> FileSaveDialog;

				HRESULT hRes = FileSaveDialog.CoCreateInstance
				(
					CLSID_FileSaveDialog,
					NULL,
					CLSCTX_INPROC_SERVER
				);

				if (!SUCCEEDED(hRes))
					break;

				{
					const UINT count = 2U;

					COMDLG_FILTERSPEC file_types[count];
					file_types[0].pszName = L"Plain text";
					file_types[0].pszSpec = L"*.txt";
					file_types[1].pszName = L"Plain text";
					file_types[1].pszSpec = L"*.*";

					FileSaveDialog->SetFileTypes
					(
						count,
						file_types
					);

					FileSaveDialog->SetFileTypeIndex(count - 1U);
				}

				hRes = FileSaveDialog->Show(MainWindow::pWndObj->hMainWindow);

				if (!SUCCEEDED(hRes))
					break;
			}
			break;

			case IDS_SYSTEMDIALOG_POPUP_ITEM:
			{
				CComPtr<IShellDispatch> ShellDispatch;

				HRESULT hRes = ShellDispatch.CoCreateInstance
				(
					CLSID_Shell,
					NULL,
					CLSCTX_INPROC_SERVER
				);

				if (!SUCCEEDED(hRes))
					break;

				hRes = ShellDispatch->ShutdownWindows();

				if (!SUCCEEDED(hRes))
					break;
			}

				MainWindow::pWndObj->CloseWindow();
			return 0;

			case IDS_EXIT_POPUP_ITEM:
				MainWindow::pWndObj->CloseWindow();
			return 0;

			case IDS_ABOUT_POPUP_ITEM:
			{
				std::wstringstream ws_stream;
				ws_stream << L"Version: " << STR_APP_VERSION << std::endl;
				ws_stream << L"Developer: " << L"DP458" << std::endl;
				ws_stream << L"Description: " << STR_APP_DESCRIPTION;

				TaskDialog
				(
					hWnd,
					MainWindow::pWndObj->hMainInstance,
					STR_ABOUT_POPUP_ITEM,
					STR_APP_TITLE,
					ws_stream.str().c_str(),
					TDCBF_OK_BUTTON,
					TD_INFORMATION_ICON,
					(int*)NULL
				);
			}
			break;

			}

		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	}

	// Public

	BOOL MainWindow::__MainWindow::AddComputerName(LPCWSTR computer_name)
	{
		const auto result = ListBox_AddString
		(
			this->hComputersListBox,
			computer_name
		);

		this->UpdateStatusBarCaption
		(
			ListBox_GetCount(this->hComputersListBox)
		);

		return (result >= 0);
	}

	BOOL MainWindow::__MainWindow::ShowDialog(HINSTANCE hInstance)
	{

		static __MainWindow WndInternalObj(hInstance);

		MainWindow::pWndObj = &WndInternalObj;

		if (!WndInternalObj.bIsClassRegistered)
		{

			WNDCLASSEX wcex = { 0 };

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = MainWindow::WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
			wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = STR_APP_CLASS;
			wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

			WndInternalObj.bIsClassRegistered = RegisterClassEx(&wcex);

			if (!WndInternalObj.bIsClassRegistered)
				return FALSE;

		}

		HMENU hFilePopupMenu = CreatePopupMenu();

		AppendMenu
		(
			hFilePopupMenu,
			MF_STRING | MF_GRAYED,
			IDS_NEW_POPUP_ITEM,
			STR_NEW_POPUP_ITEM
		);
		AppendMenu
		(
			hFilePopupMenu,
			MF_STRING | MF_GRAYED,
			IDS_OPEN_POPUP_ITEM,
			STR_OPEN_POPUP_ITEM
		);
		AppendMenu
		(
			hFilePopupMenu,
			MF_STRING | MF_GRAYED,
			IDS_SAVE_POPUP_ITEM,
			STR_SAVE_POPUP_ITEM
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

		WndInternalObj.hMainWindow = CreateWindowEx
		(
			(DWORD)NULL,
			STR_APP_CLASS,
			STR_APP_TITLE,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			326,
			547,
			(HWND)NULL,
			hMainMenu,
			WndInternalObj.hMainInstance,
			(LPVOID)NULL
		);

		if (!WndInternalObj.hMainWindow)
		{
			DestroyMenu(hMainMenu);
			return FALSE;
		}

		WndInternalObj.InitMainWindow();
		return TRUE;
	}

	__MainWindow* MainWindow::__MainWindow::GetInstance()
	{
		return MainWindow::pWndObj;
	}

}