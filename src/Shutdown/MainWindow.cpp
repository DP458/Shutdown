// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Resources.h"
#include "MainWindow.h"
#include "AddComputersDialog.h"

namespace MainWindow
{

	// Private

	MainWindow::__MainWindow::__MainWindow(HINSTANCE hInstance) :
		hMainInstance(hInstance), hMainWindow(nullptr), hActionsComboBox(nullptr), hExecActionButton(nullptr), hComputersListBox(nullptr),
		hAddComputersButton(nullptr), hEditComputersButton(nullptr), hRemoveComputersButton(nullptr), hTimerEdit(nullptr), hDefTimerButton(nullptr),
		hForceCheckBox(nullptr), hPlannedCheckBox(nullptr), hMessageEdit(nullptr), hStatusBar(nullptr)
	{
		this->guiFont = std::unique_ptr<std::remove_pointer<HFONT>::type, Shutdown::ObjectDeleter<std::remove_pointer<HFONT>::type>>
		(
			Shutdown::MainStaticObject::GetSystemFont(),
			Shutdown::ObjectDeleter<std::remove_pointer<HFONT>::type>(Shutdown::DeleterTypes::DeleteObjectCall)
		);

		//

		WNDCLASSEX wcex = { 0 };

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MainWindow::WndProc;
		wcex.hInstance = hInstance;
		wcex.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCE(IDI_MENU);
		wcex.lpszClassName = STR_APP_CLASS;
		wcex.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR| LR_DEFAULTSIZE | LR_SHARED);

		RegisterClassEx(&wcex);
	}

	/*
	Function GetTimerValue()
	Returns DWORD value from Timer edit box
	Returns ULONG_MAX if error has occurred
	*/
	DWORD MainWindow::__MainWindow::GetTimerValue()
	{
		auto psTimer = std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>>
		(
			Shutdown::MainStaticObject::GetWndText(this->hTimerEdit),
			Shutdown::ObjectDeleter<wchar_t>(Shutdown::DeleterTypes::DefaultArray)
		);

		if (!psTimer.get() || psTimer[0] == L'\0')
			return ULONG_MAX;

		return wcstoul(psTimer.get(), (wchar_t**)NULL, 10);
	}

	BOOL MainWindow::__MainWindow::StartShutdown(int listbox_index, BOOL bRebootAfterShutdown)
	{
		std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>> psComputerName;

		if (listbox_index >= 0)
		{
			psComputerName = std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>>
			(
				Shutdown::MainStaticObject::GetListBoxText(this->hComputersListBox, listbox_index),
				Shutdown::ObjectDeleter<wchar_t>(Shutdown::DeleterTypes::DefaultArray)
			);

			if (!psComputerName.get())
				return FALSE;
		}

		if
		(
			!Shutdown::MainStaticObject::SetShutdownPrivilege(psComputerName.get())
		)
			return FALSE;

		auto psMessage = std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>>
		(
			Shutdown::MainStaticObject::GetWndText(this->hMessageEdit),
			Shutdown::ObjectDeleter<wchar_t>(Shutdown::DeleterTypes::DefaultArray)
		);

		if (!psMessage.get())
			return FALSE;

		const DWORD cdTimerValue = this->GetTimerValue();

		if (cdTimerValue == ULONG_MAX)
			return FALSE;

		DWORD dwReason = SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER;

		if
		(
			Button_GetCheck(this->hPlannedCheckBox)
		)
			dwReason |= SHTDN_REASON_FLAG_PLANNED;

		return InitiateSystemShutdownEx
		(
			psComputerName.get(),
			(psMessage[0] == L'\0') ? (LPWSTR)NULL : psMessage.get(),
			cdTimerValue,
			Button_GetCheck(this->hForceCheckBox),
			bRebootAfterShutdown,
			dwReason
		);
	}

	BOOL MainWindow::__MainWindow::StopShutdown(int listbox_index)
	{
		std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>> psComputerName;

		if (listbox_index >= 0)
		{
			psComputerName = std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>>
			(
				Shutdown::MainStaticObject::GetListBoxText
				(
					this->hComputersListBox,
					listbox_index
				),
				Shutdown::ObjectDeleter<wchar_t>(Shutdown::DeleterTypes::DefaultArray)
			);

			if (!psComputerName.get())
				return FALSE;
		}

		if
		(
			!Shutdown::MainStaticObject::SetShutdownPrivilege(psComputerName.get())
		)
			return FALSE;

		return AbortSystemShutdown(psComputerName.get());
	}

	BOOL MainWindow::__MainWindow::StartComputersShutdown(BOOL bRebootAfterShutdown)
	{
		const int count = ListBox_GetCount(this->hComputersListBox);

		if (count < 0)
			return FALSE;

		if (count == 0)
			return this->StartShutdown
			(
				-1,
				bRebootAfterShutdown
			);

		bool result = true;

		for (int i = 0; i < count; i++)
			result = result && this->StartShutdown
			(
				i,
				FALSE
			);

		return result;
	}

	BOOL MainWindow::__MainWindow::StopComputersShutdown()
	{
		const int count = ListBox_GetCount(this->hComputersListBox);

		if (count < 0)
			return FALSE;

		if (count == 0)
			return this->StopShutdown(-1);

		bool result = true;

		for (int i = 0; i < count; i++)
			result = result && this->StopShutdown(i);

		return result;
	}

	void MainWindow::__MainWindow::LoadComputerNamesFromFile()
	{
		std::wfstream fs;

		if
		(
			!Shutdown::MainStaticObject::OpenTextFileThroughDialog(this->hMainWindow, Shutdown::FileDialogType::Open, fs)
		)
		{
			TaskDialog
			(
				this->hMainWindow,
				this->hMainInstance,
				L"Error",
				L"Failed to open text file",
				(PCWSTR)NULL,
				TDCBF_OK_BUTTON,
				TD_ERROR_ICON,
				(int*)NULL
			);
			return;
		}

		if (!fs.is_open())
			return;

		while
		(
			(!fs.eof()) && (!fs.fail())
		)
		{
			std::wstring str;
			std::getline(fs, str);

			if (str.length() == 0)
				continue;

			this->AddComputerName(str.c_str());
		}

		fs.close();
	}

	void MainWindow::__MainWindow::SaveComputerNamesToFile()
	{
		if (ListBox_GetCount(this->hComputersListBox) <= 0)
			return;

		std::wfstream fs;

		if
		(
			!Shutdown::MainStaticObject::OpenTextFileThroughDialog(this->hMainWindow, Shutdown::FileDialogType::Save, fs)
		)
		{
			TaskDialog
			(
				this->hMainWindow,
				this->hMainInstance,
				L"Error",
				L"Failed to open text file",
				(PCWSTR)NULL,
				TDCBF_OK_BUTTON,
				TD_ERROR_ICON,
				(int*)NULL
			);
			return;
		}

		if (!fs.is_open())
			return;

		if
		(
			!Shutdown::MainStaticObject::SaveListBoxStringsToFile(this->hComputersListBox, fs)
		)
			TaskDialog
			(
				this->hMainWindow,
				this->hMainInstance,
				L"Error",
				L"Failed to save some computer name",
				L"Possibly this string has invalid characters",
				TDCBF_OK_BUTTON,
				TD_ERROR_ICON,
				(int*)NULL
			);
	}

	void MainWindow::__MainWindow::ExecActionButtonClick()
	{
		switch (ComboBox_GetCurSel(this->hActionsComboBox))
		{

		case Shutdown::ActionTypes::Shutdown:
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

			if (!this->StartComputersShutdown(FALSE))
			{
				SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
				this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
				TaskDialog
				(
					this->hMainWindow,
					this->hMainInstance,
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
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

		case Shutdown::ActionTypes::Reboot:
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

			if (!this->StartComputersShutdown(TRUE))
			{
				SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
				this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
				TaskDialog
				(
					this->hMainWindow,
					this->hMainInstance,
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
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

		case Shutdown::ActionTypes::LogOff:
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

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
				this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
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
				break;
			}

			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

		case Shutdown::ActionTypes::Lock:
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

			if (!LockWorkStation())
			{
				SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
				this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
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
				break;
			}

			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
			this->CloseWindow();
		return;

		case Shutdown::ActionTypes::Cancel:
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

			if (!this->StopComputersShutdown())
			{
				SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
				this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
				TaskDialog
				(
					this->hMainWindow,
					this->hMainInstance,
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
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

		case Shutdown::ActionTypes::Suspend:
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

			{
				auto func = [this]() -> BOOL
				{
					if
						(
							!Shutdown::MainStaticObject::SetShutdownPrivilege((LPWSTR)NULL)
							)
						return FALSE;

					BOOL result = SetSuspendState((BOOLEAN)FALSE, (BOOLEAN)FALSE, (BOOLEAN)FALSE);

					if (result)
						this->CloseWindow();

					return result;
				};

				if (!func())
				{
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
					this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
					TaskDialog
					(
						this->hMainWindow,
						this->hMainInstance,
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
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
		break;

		case Shutdown::ActionTypes::Hibernate:
			SetCursor(LoadCursor((HINSTANCE)NULL, IDC_WAIT));
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Processing);

			{
				auto func = [this]() -> BOOL
				{
					if
						(
							!Shutdown::MainStaticObject::SetShutdownPrivilege((LPWSTR)NULL)
							)
						return FALSE;

					BOOL result = SetSuspendState((BOOLEAN)TRUE, (BOOLEAN)FALSE, (BOOLEAN)FALSE);

					if (result)
						this->CloseWindow();

					return result;
				};

				if (!func())
				{
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_ARROW));
					this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Fail);
					TaskDialog
					(
						this->hMainWindow,
						this->hMainInstance,
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
			this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Successful);
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

	void MainWindow::__MainWindow::UpdateStatusBarCaption(Shutdown::ShutdownStatus status)
	{
		switch(status)
		{

		case Shutdown::ShutdownStatus::Successful:
			SendMessage
			(
				this->hStatusBar,
				SB_SETTEXT,
				(WPARAM)1,
				(LPARAM)L"  Status:  Successful"
			);
		break;

		case Shutdown::ShutdownStatus::Fail:
			SendMessage
			(
				this->hStatusBar,
				SB_SETTEXT,
				(WPARAM)1,
				(LPARAM)L"  Status:  Failed"
			);
		break;

		case Shutdown::ShutdownStatus::Processing:
			SendMessage
			(
				this->hStatusBar,
				SB_SETTEXT,
				(WPARAM)1,
				(LPARAM)L"  Status:  Processing"
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
		if (ListBox_GetCount(this->hComputersListBox) <= 0)
			return;

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
		this->UpdateStatusBarCaption(ListBox_GetCount(this->hComputersListBox));
	}

	void MainWindow::__MainWindow::ClearMessage()
	{
		const int cTextLength = GetWindowTextLength(this->hMessageEdit);

		if (cTextLength <= 0)
			return;
		
		int result = 0;

		TaskDialog
		(
			this->hMainWindow,
			this->hMainInstance,
			L"Clear message",
			L"Are you sure you want to clear message?",
			(PCWSTR)NULL,
			TDCBF_YES_BUTTON | TDCBF_NO_BUTTON,
			TD_WARNING_ICON,
			&result
		);

		if (result != IDYES)
			return;

		SetWindowText(this->hMessageEdit, nullptr);
	}

	void MainWindow::__MainWindow::ShowAddComputersDialog()
	{
		DialogBoxParam(this->hMainInstance, MAKEINTRESOURCE(IDI_DIALOG_ADD_COMOUTERS), this->hMainWindow, AddComputersDialogInternals::DlgProc, (LPARAM)NULL);
	}

	void MainWindow::__MainWindow::ShowEditComputersDialog()
	{
		const int index = ListBox_GetCurSel(this->hComputersListBox);

		if (index < 0)
			return;

		DialogBoxParam(this->hMainInstance, MAKEINTRESOURCE(IDI_DIALOG_EDIT_COMOUTERS), this->hMainWindow, AddComputersDialogInternals::DlgProc, (LPARAM)1);
	}

	void MainWindow::__MainWindow::ShowAboutDialog()
	{
		std::wstringstream ws_stream;
		ws_stream << L"Version: " << STR_APP_VERSION << std::endl;
		ws_stream << L"Developer: " << L"DP458" << std::endl;
		ws_stream << L"Description: " << STR_APP_DESCRIPTION;

		TaskDialog
		(
			this->hMainWindow,
			this->hMainInstance,
			STR_ABOUT_DIALOG_TITLE,
			STR_APP_TITLE,
			ws_stream.str().c_str(),
			TDCBF_OK_BUTTON,
			TD_INFORMATION_ICON,
			(int*)NULL
		);
	}

	void MainWindow::__MainWindow::CloseWindow()
	{
		PostMessage(this->hMainWindow, WM_CLOSE, (WPARAM)NULL, (LPARAM)NULL);
	}

	void  MainWindow::__MainWindow::ActionComboBoxSelectionChanged()
	{
		switch (ComboBox_GetCurSel(this->hActionsComboBox))
		{

		case Shutdown::ActionTypes::Shutdown: case Shutdown::ActionTypes::Reboot:

			EnableMenuItem(GetMenu(this->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_MESSAGE_EDIT_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			ListBox_SetCurSel(this->hComputersListBox, -1);
			EnableWindow(this->hComputersListBox, TRUE);
			EnableWindow(this->hAddComputersButton, TRUE);
			EnableWindow(this->hEditComputersButton, TRUE);
			EnableWindow(this->hRemoveComputersButton, TRUE);
			EnableWindow(this->hTimerEdit, TRUE);
			EnableWindow(this->hDefTimerButton, TRUE);
			EnableWindow(this->hForceCheckBox, TRUE);
			EnableWindow(this->hPlannedCheckBox, TRUE);
			EnableWindow(this->hMessageEdit, TRUE);

			break;

		case Shutdown::ActionTypes::LogOff: case Shutdown::ActionTypes::Lock: case Shutdown::ActionTypes::Suspend: case Shutdown::ActionTypes::Hibernate:

			EnableMenuItem(GetMenu(this->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_MESSAGE_EDIT_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			ListBox_SetCurSel(this->hComputersListBox, -1);
			EnableWindow(this->hComputersListBox, FALSE);
			EnableWindow(this->hAddComputersButton, FALSE);
			EnableWindow(this->hEditComputersButton, FALSE);
			EnableWindow(this->hRemoveComputersButton, FALSE);
			EnableWindow(this->hTimerEdit, FALSE);
			EnableWindow(this->hDefTimerButton, FALSE);
			EnableWindow(this->hForceCheckBox, FALSE);
			EnableWindow(this->hPlannedCheckBox, FALSE);
			EnableWindow(this->hMessageEdit, FALSE);

			break;

		case Shutdown::ActionTypes::Cancel:

			EnableMenuItem(GetMenu(this->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_MESSAGE_EDIT_POPUP_ITEM, MF_BYCOMMAND | MF_ENABLED);
			ListBox_SetCurSel(this->hComputersListBox, -1);
			EnableWindow(this->hComputersListBox, TRUE);
			EnableWindow(this->hAddComputersButton, TRUE);
			EnableWindow(this->hEditComputersButton, TRUE);
			EnableWindow(this->hRemoveComputersButton, TRUE);
			EnableWindow(this->hTimerEdit, FALSE);
			EnableWindow(this->hDefTimerButton, FALSE);
			EnableWindow(this->hForceCheckBox, FALSE);
			EnableWindow(this->hPlannedCheckBox, FALSE);
			EnableWindow(this->hMessageEdit, FALSE);

			break;

		default:
			
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_NEW_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_OPEN_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_SAVE_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(GetMenu(this->hMainWindow), IDS_RESET_MESSAGE_EDIT_POPUP_ITEM, MF_BYCOMMAND | MF_GRAYED);
			ListBox_SetCurSel(this->hComputersListBox, -1);
			EnableWindow(this->hComputersListBox, FALSE);
			EnableWindow(this->hAddComputersButton, FALSE);
			EnableWindow(this->hEditComputersButton, FALSE);
			EnableWindow(this->hRemoveComputersButton, FALSE);
			EnableWindow(this->hTimerEdit, FALSE);
			EnableWindow(this->hDefTimerButton, FALSE);
			EnableWindow(this->hForceCheckBox, FALSE);
			EnableWindow(this->hPlannedCheckBox, FALSE);
			EnableWindow(this->hMessageEdit, FALSE);

			break;
		}
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

				switch(nmhdr->idFrom)
				{

				case IDS_EXECACTION_BUTTON_TITLE:
				{
					UINT uflags = MF_STRING;

					if (ComboBox_GetCurSel(MainWindow::pWndObj->hActionsComboBox) < 0)
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
					MainWindow::pWndObj->ActionComboBoxSelectionChanged();
				break;

				}

			break;

			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				//
				case IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM:
					Shutdown::MainStaticObject::ComboBoxSelectItem(MainWindow::pWndObj->hActionsComboBox, -1);
					break;

				case IDS_OPEN_SYSTEM_DIALOG_POPUP_ITEM:
					if (Shutdown::MainStaticObject::ShowShutdownDialog())
						MainWindow::pWndObj->CloseWindow();
					return 0;

				case IDS_ADD_COMPUTERS_FROM_FILE_POPUP_ITEM:
					MainWindow::pWndObj->LoadComputerNamesFromFile();
					break;

				case IDS_SAVE_COMPUTERS_TO_FILE_POPUP_ITEM:
					MainWindow::pWndObj->SaveComputerNamesToFile();
					break;

				case IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM:
					MainWindow::pWndObj->ClearComputerNames();
					break;
				//

				case IDS_EXECACTION_BUTTON_TITLE:
					MainWindow::pWndObj->ExecActionButtonClick();
				break;

				case IDS_ADDCOMPUTERS_BUTTON_TITLE:
					MainWindow::pWndObj->ShowAddComputersDialog();
				break;

				case IDS_EDITCOMPUTERS_BUTTON_TITLE:
					MainWindow::pWndObj->ShowEditComputersDialog();
				break;

				case IDS_REMOVECOMPUTERS_BUTTON_TITLE:
				{
					int index = ListBox_GetCurSel(MainWindow::pWndObj->hComputersListBox);
					if (index >= 0 && (ListBox_DeleteString(MainWindow::pWndObj->hComputersListBox, index) != LB_ERR))
						ListBox_SetCurSel(MainWindow::pWndObj->hComputersListBox, (--index) > 0 ? index : 0);
				}

					MainWindow::pWndObj->UpdateStatusBarCaption
					(
						ListBox_GetCount(MainWindow::pWndObj->hComputersListBox)
					);
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
				MainWindow::pWndObj->LoadComputerNamesFromFile();
			break;

			case IDS_SAVE_POPUP_ITEM:
				MainWindow::pWndObj->SaveComputerNamesToFile();
			break;

			case IDS_SYSTEMDIALOG_POPUP_ITEM:
				if (Shutdown::MainStaticObject::ShowShutdownDialog())
					MainWindow::pWndObj->CloseWindow();
			return 0;

			case IDS_EXIT_POPUP_ITEM:
				MainWindow::pWndObj->CloseWindow();
			return 0;

			case IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM:
				Shutdown::MainStaticObject::ComboBoxSelectItem(MainWindow::pWndObj->hActionsComboBox, -1);
			break;

			case IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM:
				MainWindow::pWndObj->ClearComputerNames();
			break;

			case IDS_RESET_MESSAGE_EDIT_POPUP_ITEM:
				MainWindow::pWndObj->ClearMessage();
			break;

			case IDS_SOURCE_AT_GITHUB_POPUP_ITEM:
				ShellExecute(NULL, NULL, L"https://github.com/DP458/Shutdown", NULL, NULL, SW_SHOW);
			break;

			case IDS_ABOUT_POPUP_ITEM:
				MainWindow::pWndObj->ShowAboutDialog();
			break;

			}

		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);

	}

	// Public

	BOOL MainWindow::__MainWindow::AddComputerName(LPCWSTR computer_name)
	{
		const auto result = ListBox_AddString(this->hComputersListBox, computer_name);

		this->UpdateStatusBarCaption
		(
			ListBox_GetCount(this->hComputersListBox)
		);

		int count = ListBox_GetCount(this->hComputersListBox);
		if (count > 0)
			ListBox_SetCurSel(this->hComputersListBox, count - 1);

		return (result >= 0);
	}

	wchar_t* MainWindow::__MainWindow::GetSelectedComputerName()
	{
		const int index = ListBox_GetCurSel(this->hComputersListBox);

		if (index < 0)
			return nullptr;

		return Shutdown::MainStaticObject::GetListBoxText(this->hComputersListBox, index);
	}

	BOOL MainWindow::__MainWindow::ReplaceSelectedComputerName(LPCWSTR computer_name)
	{
		const int index = ListBox_GetCurSel(this->hComputersListBox);

		if (index < 0)
			return FALSE;

		auto result = ListBox_DeleteString(this->hComputersListBox, index);

		if (result >= 0)
			result = ListBox_InsertString(this->hComputersListBox, index, computer_name);

		ListBox_SetCurSel(this->hComputersListBox, index);
		return (result >= 0);
	}

	BOOL MainWindow::__MainWindow::ShowDialog(HINSTANCE hInstance)
	{
		static __MainWindow WndInternalObj(hInstance);

		MainWindow::pWndObj = &WndInternalObj;

		CREATESTRUCT mainWindowCreateStruct = { 0 };
		mainWindowCreateStruct.lpszClass = STR_APP_CLASS;
		mainWindowCreateStruct.lpszName = STR_APP_TITLE;
		mainWindowCreateStruct.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
		mainWindowCreateStruct.x = CW_USEDEFAULT;
		mainWindowCreateStruct.y = CW_USEDEFAULT;
		mainWindowCreateStruct.cx = 326;
		mainWindowCreateStruct.cy = 547;
		mainWindowCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hMainWindow = Shutdown::MainStaticObject::CreateWindowFromStruct(mainWindowCreateStruct);

		if (WndInternalObj.hMainWindow)
			SetWindowFont(WndInternalObj.hMainWindow, WndInternalObj.guiFont.get(), TRUE);

		if (!WndInternalObj.hMainWindow)
			return FALSE;

		{
			HMENU hMainMenu = GetMenu(WndInternalObj.hMainWindow);

			MENUINFO info = { 0 };
			info.cbSize = sizeof(info);
			GetMenuInfo(hMainMenu, &info);

			info.fMask = info.fMask | MIM_STYLE;
			info.dwStyle = info.dwStyle | MNS_NOTIFYBYPOS;
			SetMenuInfo(hMainMenu, &info);
		}

		CREATESTRUCT actionStaticCreateStruct = { 0 };
		actionStaticCreateStruct.lpszClass = WC_STATIC;
		actionStaticCreateStruct.lpszName = STR_ACTION_STATIC_TITLE;
		actionStaticCreateStruct.style = WS_VISIBLE | WS_CHILD | SS_SIMPLE;
		actionStaticCreateStruct.x = 10;
		actionStaticCreateStruct.y = 5;
		actionStaticCreateStruct.cx = 180;
		actionStaticCreateStruct.cy = 20;
		actionStaticCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		actionStaticCreateStruct.hMenu = (HMENU)IDS_ACTION_STATIC_TITLE;
		actionStaticCreateStruct.hInstance = WndInternalObj.hMainInstance;
		Shutdown::MainStaticObject::CreateWindowFromStruct(actionStaticCreateStruct);

		CREATESTRUCT actionsComboBoxCreateStruct = { 0 };
		actionsComboBoxCreateStruct.lpszClass = WC_COMBOBOX;
		actionsComboBoxCreateStruct.lpszName = STR_ACTIONS_COMBOBOX_TITLE;
		actionsComboBoxCreateStruct.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS;
		actionsComboBoxCreateStruct.x = 10;
		actionsComboBoxCreateStruct.y = 28;
		actionsComboBoxCreateStruct.cx = 200;
		actionsComboBoxCreateStruct.cy = 40;
		actionsComboBoxCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		actionsComboBoxCreateStruct.hMenu = (HMENU)IDS_ACTIONS_COMBOBOX_TITLE;
		actionsComboBoxCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hActionsComboBox = Shutdown::MainStaticObject::CreateWindowFromStruct(actionsComboBoxCreateStruct);

		ComboBox_SetExtendedUI(WndInternalObj.hActionsComboBox, 1);
		ComboBox_InsertString(WndInternalObj.hActionsComboBox, Shutdown::ActionTypes::Shutdown, ACTION_SHUTDOWN);
		ComboBox_InsertString(WndInternalObj.hActionsComboBox, Shutdown::ActionTypes::Reboot, ACTION_REBOOT);
		ComboBox_InsertString(WndInternalObj.hActionsComboBox, Shutdown::ActionTypes::LogOff, ACTION_LOGOFF);
		ComboBox_InsertString(WndInternalObj.hActionsComboBox, Shutdown::ActionTypes::Lock, ACTION_LOCK);
		ComboBox_InsertString(WndInternalObj.hActionsComboBox, Shutdown::ActionTypes::Cancel, ACTION_CANCEL);
		ComboBox_InsertString(WndInternalObj.hActionsComboBox, Shutdown::ActionTypes::Suspend, ACTION_SUSPEND);
		ComboBox_InsertString(WndInternalObj.hActionsComboBox, Shutdown::ActionTypes::Hibernate, ACTION_HIBERNATE);

		CREATESTRUCT execActionButtonCreateStruct = { 0 };
		execActionButtonCreateStruct.lpszClass = WC_BUTTON;
		execActionButtonCreateStruct.lpszName = STR_EXECACTION_BUTTON_TITLE;
		execActionButtonCreateStruct.style = WS_VISIBLE | WS_CHILD | BS_SPLITBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		execActionButtonCreateStruct.x = 220;
		execActionButtonCreateStruct.y = 25;
		execActionButtonCreateStruct.cx = 80;
		execActionButtonCreateStruct.cy = 30;
		execActionButtonCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		execActionButtonCreateStruct.hMenu = (HMENU)IDS_EXECACTION_BUTTON_TITLE;
		execActionButtonCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hExecActionButton = Shutdown::MainStaticObject::CreateWindowFromStruct(execActionButtonCreateStruct);

		{
			BUTTON_SPLITINFO splitInfo = { 0 };
			splitInfo.mask = BCSIF_STYLE;
			splitInfo.uSplitStyle = BCSS_STRETCH;
			Button_SetSplitInfo(WndInternalObj.hExecActionButton, &splitInfo);
		}

		CREATESTRUCT computersStaticCreateStruct = { 0 };
		computersStaticCreateStruct.lpszClass = WC_STATIC;
		computersStaticCreateStruct.lpszName = STR_COMPUTERS_STATIC_TITLE;
		computersStaticCreateStruct.style = WS_VISIBLE | WS_CHILD | SS_SIMPLE;
		computersStaticCreateStruct.x = 10;
		computersStaticCreateStruct.y = 65;
		computersStaticCreateStruct.cx = 180;
		computersStaticCreateStruct.cy = 20;
		computersStaticCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		computersStaticCreateStruct.hMenu = (HMENU)IDS_COMPUTERS_STATIC_TITLE;
		computersStaticCreateStruct.hInstance = WndInternalObj.hMainInstance;
		Shutdown::MainStaticObject::CreateWindowFromStruct(computersStaticCreateStruct);

		CREATESTRUCT computersEditCreateStruct = { 0 };
		computersEditCreateStruct.dwExStyle = WS_EX_CLIENTEDGE;
		computersEditCreateStruct.lpszClass = WC_LISTBOX;
		computersEditCreateStruct.lpszName = STR_COMPUTERS_EDIT_TITLE;
		computersEditCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_DISABLED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY;
		computersEditCreateStruct.x = 10;
		computersEditCreateStruct.y = 85;
		computersEditCreateStruct.cx = 200;
		computersEditCreateStruct.cy = 100;
		computersEditCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		computersEditCreateStruct.hMenu = (HMENU)IDS_COMPUTERS_EDIT_TITLE;
		computersEditCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hComputersListBox = Shutdown::MainStaticObject::CreateWindowFromStruct(computersEditCreateStruct);

		CREATESTRUCT addComputersButtonCreateStruct = { 0 };
		addComputersButtonCreateStruct.lpszClass = WC_BUTTON;
		addComputersButtonCreateStruct.lpszName = STR_ADDCOMPUTERS_BUTTON_TITLE;
		addComputersButtonCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_SPLITBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		addComputersButtonCreateStruct.x = 220;
		addComputersButtonCreateStruct.y = 85;
		addComputersButtonCreateStruct.cx = 80;
		addComputersButtonCreateStruct.cy = 30;
		addComputersButtonCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		addComputersButtonCreateStruct.hMenu = (HMENU)IDS_ADDCOMPUTERS_BUTTON_TITLE;
		addComputersButtonCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hAddComputersButton = Shutdown::MainStaticObject::CreateWindowFromStruct(addComputersButtonCreateStruct);

		{
			BUTTON_SPLITINFO splitInfo = { 0 };
			splitInfo.mask = BCSIF_STYLE;
			splitInfo.uSplitStyle = BCSS_STRETCH;
			Button_SetSplitInfo(WndInternalObj.hAddComputersButton, &splitInfo);
		}

		CREATESTRUCT editComputersButtonCreateStruct = { 0 };
		editComputersButtonCreateStruct.lpszClass = WC_BUTTON;
		editComputersButtonCreateStruct.lpszName = STR_EDITCOMPUTERS_BUTTON_TITLE;
		editComputersButtonCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		editComputersButtonCreateStruct.x = 220;
		editComputersButtonCreateStruct.y = 120;
		editComputersButtonCreateStruct.cx = 80;
		editComputersButtonCreateStruct.cy = 30;
		editComputersButtonCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		editComputersButtonCreateStruct.hMenu = (HMENU)IDS_EDITCOMPUTERS_BUTTON_TITLE;
		editComputersButtonCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hEditComputersButton = Shutdown::MainStaticObject::CreateWindowFromStruct(editComputersButtonCreateStruct);

		CREATESTRUCT removeComputersButtonCreateStruct = { 0 };
		removeComputersButtonCreateStruct.lpszClass = WC_BUTTON;
		removeComputersButtonCreateStruct.lpszName = STR_REMOVECOMPUTERS_BUTTON_TITLE;
		removeComputersButtonCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_SPLITBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		removeComputersButtonCreateStruct.x = 220;
		removeComputersButtonCreateStruct.y = 155;
		removeComputersButtonCreateStruct.cx = 80;
		removeComputersButtonCreateStruct.cy = 30;
		removeComputersButtonCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		removeComputersButtonCreateStruct.hMenu = (HMENU)IDS_REMOVECOMPUTERS_BUTTON_TITLE;
		removeComputersButtonCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hRemoveComputersButton = Shutdown::MainStaticObject::CreateWindowFromStruct(removeComputersButtonCreateStruct);

		{
			BUTTON_SPLITINFO splitInfo = { 0 };
			splitInfo.mask = BCSIF_STYLE;
			splitInfo.uSplitStyle = BCSS_STRETCH;
			Button_SetSplitInfo(WndInternalObj.hRemoveComputersButton, &splitInfo);
		}

		CREATESTRUCT timerStaticCreateStruct = { 0 };
		timerStaticCreateStruct.lpszClass = WC_STATIC;
		timerStaticCreateStruct.lpszName = STR_TIMER_STATIC_TITLE;
		timerStaticCreateStruct.style = WS_VISIBLE | WS_CHILD | SS_SIMPLE;
		timerStaticCreateStruct.x = 10;
		timerStaticCreateStruct.y = 195;
		timerStaticCreateStruct.cx = 180;
		timerStaticCreateStruct.cy = 20;
		timerStaticCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		timerStaticCreateStruct.hMenu = (HMENU)IDS_TIMER_STATIC_TITLE;
		timerStaticCreateStruct.hInstance = WndInternalObj.hMainInstance;
		Shutdown::MainStaticObject::CreateWindowFromStruct(timerStaticCreateStruct);

		CREATESTRUCT timerEditCreateStruct = { 0 };
		timerEditCreateStruct.dwExStyle = WS_EX_CLIENTEDGE;
		timerEditCreateStruct.lpszClass = WC_EDIT;
		timerEditCreateStruct.lpszName = STR_TIMER_EDIT_TITLE;
		timerEditCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_DISABLED | ES_LEFT | ES_NUMBER | ES_NOHIDESEL;
		timerEditCreateStruct.x = 10;
		timerEditCreateStruct.y = 220;
		timerEditCreateStruct.cx = 40;
		timerEditCreateStruct.cy = 20;
		timerEditCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		timerEditCreateStruct.hMenu = (HMENU)IDS_TIMER_EDIT_TITLE;
		timerEditCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hTimerEdit = Shutdown::MainStaticObject::CreateWindowFromStruct(timerEditCreateStruct);

		Edit_LimitText(WndInternalObj.hTimerEdit, 4);

		CREATESTRUCT defTimersButtonCreateStruct = { 0 };
		defTimersButtonCreateStruct.lpszClass = WC_BUTTON;
		defTimersButtonCreateStruct.lpszName = STR_TIMER_DEF_BUTTON_TITLE;
		defTimersButtonCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		defTimersButtonCreateStruct.x = 60;
		defTimersButtonCreateStruct.y = 215;
		defTimersButtonCreateStruct.cx = 80;
		defTimersButtonCreateStruct.cy = 30;
		defTimersButtonCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		defTimersButtonCreateStruct.hMenu = (HMENU)IDS_TIMER_DEF_BUTTON_TITLE;
		defTimersButtonCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hDefTimerButton = Shutdown::MainStaticObject::CreateWindowFromStruct(defTimersButtonCreateStruct);

		CREATESTRUCT forceCheckBoxCreateStruct = { 0 };
		forceCheckBoxCreateStruct.lpszClass = WC_BUTTON;
		forceCheckBoxCreateStruct.lpszName = STR_FORCE_CHECKBOX_TITLE;
		forceCheckBoxCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX;
		forceCheckBoxCreateStruct.x = 10;
		forceCheckBoxCreateStruct.y = 255;
		forceCheckBoxCreateStruct.cx = 160;
		forceCheckBoxCreateStruct.cy = 20;
		forceCheckBoxCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		forceCheckBoxCreateStruct.hMenu = (HMENU)IDS_FORCE_CHECKBOX_TITLE;
		forceCheckBoxCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hForceCheckBox = Shutdown::MainStaticObject::CreateWindowFromStruct(forceCheckBoxCreateStruct);

		CREATESTRUCT plannedCheckBoxCreateStruct = { 0 };
		plannedCheckBoxCreateStruct.lpszClass = WC_BUTTON;
		plannedCheckBoxCreateStruct.lpszName = STR_PLANNED_CHECKBOX_TITLE;
		plannedCheckBoxCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX;
		plannedCheckBoxCreateStruct.x = 175;
		plannedCheckBoxCreateStruct.y = 255;
		plannedCheckBoxCreateStruct.cx = 160;
		plannedCheckBoxCreateStruct.cy = 20;
		plannedCheckBoxCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		plannedCheckBoxCreateStruct.hMenu = (HMENU)IDS_PLANNED_CHECKBOX_TITLE;
		plannedCheckBoxCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hPlannedCheckBox = Shutdown::MainStaticObject::CreateWindowFromStruct(plannedCheckBoxCreateStruct);

		Button_SetCheck(WndInternalObj.hPlannedCheckBox, TRUE);

		CREATESTRUCT messageStaticCreateStruct = { 0 };
		messageStaticCreateStruct.lpszClass = WC_STATIC;
		messageStaticCreateStruct.lpszName = STR_MESSAGE_STATIC_TITLE;
		messageStaticCreateStruct.style = WS_VISIBLE | WS_CHILD | SS_SIMPLE;
		messageStaticCreateStruct.x = 10;
		messageStaticCreateStruct.y = 285;
		messageStaticCreateStruct.cx = 180;
		messageStaticCreateStruct.cy = 20;
		messageStaticCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		messageStaticCreateStruct.hMenu = (HMENU)IDS_MESSAGE_STATIC_TITLE;
		messageStaticCreateStruct.hInstance = WndInternalObj.hMainInstance;
		Shutdown::MainStaticObject::CreateWindowFromStruct(messageStaticCreateStruct);

		CREATESTRUCT messageEditCreateStruct = { 0 };
		messageEditCreateStruct.dwExStyle = WS_EX_CLIENTEDGE;
		messageEditCreateStruct.lpszClass = WC_EDIT;
		messageEditCreateStruct.lpszName = STR_MESSAGE_EDIT_TITLE;
		messageEditCreateStruct.style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_DISABLED | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_NOHIDESEL;
		messageEditCreateStruct.x = 10;
		messageEditCreateStruct.y = 305;
		messageEditCreateStruct.cx = 290;
		messageEditCreateStruct.cy = 150;
		messageEditCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		messageEditCreateStruct.hMenu = (HMENU)IDS_MESSAGE_EDIT_TITLE;
		messageEditCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hMessageEdit = Shutdown::MainStaticObject::CreateWindowFromStruct(messageEditCreateStruct);

		Edit_LimitText(WndInternalObj.hMessageEdit, 511);

		CREATESTRUCT statusBarCreateStruct = { 0 };
		statusBarCreateStruct.lpszClass = STATUSCLASSNAME;
		statusBarCreateStruct.lpszName = STR_STATUS_BAR_TITLE;
		statusBarCreateStruct.style = WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP;
		statusBarCreateStruct.x = CW_USEDEFAULT;
		statusBarCreateStruct.y = CW_USEDEFAULT;
		statusBarCreateStruct.cx = CW_USEDEFAULT;
		statusBarCreateStruct.cy = CW_USEDEFAULT;
		statusBarCreateStruct.hwndParent = WndInternalObj.hMainWindow;
		statusBarCreateStruct.hMenu = (HMENU)IDS_STATUS_BAR_TITLE;
		statusBarCreateStruct.hInstance = WndInternalObj.hMainInstance;
		WndInternalObj.hStatusBar = Shutdown::MainStaticObject::CreateWindowFromStruct(statusBarCreateStruct);

		SendMessage(WndInternalObj.hStatusBar, SB_SIMPLE, FALSE, (LPARAM)NULL);

		{
			const int parts_count = 2;
			const int parts[parts_count] = { 120, 260 };
			SendMessage(WndInternalObj.hStatusBar, SB_SETPARTS, (WPARAM)parts_count, (LPARAM)&parts);
		}

		WndInternalObj.UpdateStatusBarCaption(0);
		WndInternalObj.UpdateStatusBarCaption(Shutdown::ShutdownStatus::Ready);

		Shutdown::MainStaticObject::ComboBoxSelectItem(WndInternalObj.hActionsComboBox, Shutdown::ActionTypes::Shutdown);
		const auto lpEnumFunc = [](HWND hwnd, LPARAM lParam)->BOOL
		{
			SetWindowFont(hwnd, lParam, FALSE);
			return TRUE;
		};
		EnumChildWindows(WndInternalObj.hMainWindow, lpEnumFunc, (LPARAM)WndInternalObj.guiFont.get());
		return TRUE;
	}

	__MainWindow* MainWindow::__MainWindow::GetInstance()
	{
		return MainWindow::pWndObj;
	}

}