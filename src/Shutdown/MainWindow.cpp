#include "stdafx.h"
#include "Resources.h"
#include "MainWindow.h"

namespace MainWindow
{
	// Public

	MainWindow::__MainWindow::__MainWindow(HINSTANCE hInstance) :
		hMainInstance(hInstance), hMainWindow(nullptr), hActionsComboBox(nullptr), hExecActionButton(nullptr), hComputersListBox(nullptr),
		hAddComputersButton(nullptr), hEditComputersButton(nullptr), hRemoveComputersButton(nullptr), hTimerEdit(nullptr), hDefTimerButton(nullptr),
		hForceCheckBox(nullptr), hPlannedCheckBox(nullptr), hMessageEdit(nullptr), hStatusBar(nullptr)
	{
		{
			WNDCLASSEX wcex = { 0 };

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = MainWindow::__MainWindow::MainWindowProc::WndProc;
			wcex.cbWndExtra = sizeof(MainWindow::__MainWindow*);
			wcex.hInstance = hInstance;
			wcex.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = MAKEINTRESOURCE(IDI_MENU);
			wcex.lpszClassName = STR_APP_CLASS;
			wcex.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

			if (!RegisterClassEx(&wcex))
				return;
		}

		{
			CREATESTRUCT mainWindowCreateStruct = { 0 };
			mainWindowCreateStruct.lpszClass = STR_APP_CLASS;
			mainWindowCreateStruct.lpszName = STR_APP_TITLE;
			mainWindowCreateStruct.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
			mainWindowCreateStruct.x = CW_USEDEFAULT;
			mainWindowCreateStruct.y = CW_USEDEFAULT;
			mainWindowCreateStruct.cx = 326;
			mainWindowCreateStruct.cy = 547;
			mainWindowCreateStruct.hInstance = this->hMainInstance;
			this->hMainWindow = Shutdown::MainStaticObject::CreateWindowFromStruct(mainWindowCreateStruct);
		}

		if (!this->hMainWindow)
			return;

		SetWindowLongPtr(this->hMainWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		this->guiFont = std::unique_ptr<std::remove_pointer<HFONT>::type, Shutdown::ObjectDeleter<std::remove_pointer<HFONT>::type>>
		(
			Shutdown::MainStaticObject::GetSystemFont(),
			Shutdown::ObjectDeleter<std::remove_pointer<HFONT>::type>(Shutdown::DeleterTypes::DeleteObjectCall)
		);
		SetWindowFont(this->hMainWindow, this->guiFont.get(), TRUE);

		{
			HMENU hMainMenu = GetMenu(this->hMainWindow);

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
		actionStaticCreateStruct.style = WS_CHILD | SS_SIMPLE;
		actionStaticCreateStruct.x = 10;
		actionStaticCreateStruct.y = 5;
		actionStaticCreateStruct.cx = 180;
		actionStaticCreateStruct.cy = 20;
		actionStaticCreateStruct.hwndParent = this->hMainWindow;
		actionStaticCreateStruct.hMenu = (HMENU)IDS_ACTION_STATIC_TITLE;
		actionStaticCreateStruct.hInstance = this->hMainInstance;
		Shutdown::MainStaticObject::CreateWindowFromStruct(actionStaticCreateStruct);

		CREATESTRUCT actionsComboBoxCreateStruct = { 0 };
		actionsComboBoxCreateStruct.lpszClass = WC_COMBOBOX;
		actionsComboBoxCreateStruct.lpszName = STR_ACTIONS_COMBOBOX_TITLE;
		actionsComboBoxCreateStruct.style = WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS;
		actionsComboBoxCreateStruct.x = 10;
		actionsComboBoxCreateStruct.y = 28;
		actionsComboBoxCreateStruct.cx = 200;
		actionsComboBoxCreateStruct.cy = 40;
		actionsComboBoxCreateStruct.hwndParent = this->hMainWindow;
		actionsComboBoxCreateStruct.hMenu = (HMENU)IDS_ACTIONS_COMBOBOX_TITLE;
		actionsComboBoxCreateStruct.hInstance = this->hMainInstance;
		this->hActionsComboBox = Shutdown::MainStaticObject::CreateWindowFromStruct(actionsComboBoxCreateStruct);

		ComboBox_SetExtendedUI(this->hActionsComboBox, 1);
		ComboBox_InsertString(this->hActionsComboBox, Shutdown::ActionTypes::Shutdown, ACTION_SHUTDOWN);
		ComboBox_InsertString(this->hActionsComboBox, Shutdown::ActionTypes::Reboot, ACTION_REBOOT);
		ComboBox_InsertString(this->hActionsComboBox, Shutdown::ActionTypes::LogOff, ACTION_LOGOFF);
		ComboBox_InsertString(this->hActionsComboBox, Shutdown::ActionTypes::Lock, ACTION_LOCK);
		ComboBox_InsertString(this->hActionsComboBox, Shutdown::ActionTypes::Cancel, ACTION_CANCEL);
		ComboBox_InsertString(this->hActionsComboBox, Shutdown::ActionTypes::Suspend, ACTION_SUSPEND);
		ComboBox_InsertString(this->hActionsComboBox, Shutdown::ActionTypes::Hibernate, ACTION_HIBERNATE);

		CREATESTRUCT execActionButtonCreateStruct = { 0 };
		execActionButtonCreateStruct.lpszClass = WC_BUTTON;
		execActionButtonCreateStruct.lpszName = STR_EXECACTION_BUTTON_TITLE;
		execActionButtonCreateStruct.style = WS_CHILD | BS_SPLITBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		execActionButtonCreateStruct.x = 220;
		execActionButtonCreateStruct.y = 25;
		execActionButtonCreateStruct.cx = 80;
		execActionButtonCreateStruct.cy = 30;
		execActionButtonCreateStruct.hwndParent = this->hMainWindow;
		execActionButtonCreateStruct.hMenu = (HMENU)IDS_EXECACTION_BUTTON_TITLE;
		execActionButtonCreateStruct.hInstance = this->hMainInstance;
		this->hExecActionButton = Shutdown::MainStaticObject::CreateWindowFromStruct(execActionButtonCreateStruct);

		{
			BUTTON_SPLITINFO splitInfo = { 0 };
			splitInfo.mask = BCSIF_STYLE;
			splitInfo.uSplitStyle = BCSS_STRETCH;
			Button_SetSplitInfo(this->hExecActionButton, &splitInfo);
		}

		CREATESTRUCT computersStaticCreateStruct = { 0 };
		computersStaticCreateStruct.lpszClass = WC_STATIC;
		computersStaticCreateStruct.lpszName = STR_COMPUTERS_STATIC_TITLE;
		computersStaticCreateStruct.style = WS_CHILD | SS_SIMPLE;
		computersStaticCreateStruct.x = 10;
		computersStaticCreateStruct.y = 65;
		computersStaticCreateStruct.cx = 180;
		computersStaticCreateStruct.cy = 20;
		computersStaticCreateStruct.hwndParent = this->hMainWindow;
		computersStaticCreateStruct.hMenu = (HMENU)IDS_COMPUTERS_STATIC_TITLE;
		computersStaticCreateStruct.hInstance = this->hMainInstance;
		Shutdown::MainStaticObject::CreateWindowFromStruct(computersStaticCreateStruct);

		CREATESTRUCT computersEditCreateStruct = { 0 };
		computersEditCreateStruct.dwExStyle = WS_EX_CLIENTEDGE;
		computersEditCreateStruct.lpszClass = WC_LISTBOX;
		computersEditCreateStruct.lpszName = STR_COMPUTERS_EDIT_TITLE;
		computersEditCreateStruct.style = WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_DISABLED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY;
		computersEditCreateStruct.x = 10;
		computersEditCreateStruct.y = 85;
		computersEditCreateStruct.cx = 200;
		computersEditCreateStruct.cy = 100;
		computersEditCreateStruct.hwndParent = this->hMainWindow;
		computersEditCreateStruct.hMenu = (HMENU)IDS_COMPUTERS_EDIT_TITLE;
		computersEditCreateStruct.hInstance = this->hMainInstance;
		this->hComputersListBox = Shutdown::MainStaticObject::CreateWindowFromStruct(computersEditCreateStruct);

		CREATESTRUCT addComputersButtonCreateStruct = { 0 };
		addComputersButtonCreateStruct.lpszClass = WC_BUTTON;
		addComputersButtonCreateStruct.lpszName = STR_ADDCOMPUTERS_BUTTON_TITLE;
		addComputersButtonCreateStruct.style = WS_CHILD | WS_DISABLED | BS_SPLITBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		addComputersButtonCreateStruct.x = 220;
		addComputersButtonCreateStruct.y = 85;
		addComputersButtonCreateStruct.cx = 80;
		addComputersButtonCreateStruct.cy = 30;
		addComputersButtonCreateStruct.hwndParent = this->hMainWindow;
		addComputersButtonCreateStruct.hMenu = (HMENU)IDS_ADDCOMPUTERS_BUTTON_TITLE;
		addComputersButtonCreateStruct.hInstance = this->hMainInstance;
		this->hAddComputersButton = Shutdown::MainStaticObject::CreateWindowFromStruct(addComputersButtonCreateStruct);

		{
			BUTTON_SPLITINFO splitInfo = { 0 };
			splitInfo.mask = BCSIF_STYLE;
			splitInfo.uSplitStyle = BCSS_STRETCH;
			Button_SetSplitInfo(this->hAddComputersButton, &splitInfo);
		}

		CREATESTRUCT editComputersButtonCreateStruct = { 0 };
		editComputersButtonCreateStruct.lpszClass = WC_BUTTON;
		editComputersButtonCreateStruct.lpszName = STR_EDITCOMPUTERS_BUTTON_TITLE;
		editComputersButtonCreateStruct.style = WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		editComputersButtonCreateStruct.x = 220;
		editComputersButtonCreateStruct.y = 120;
		editComputersButtonCreateStruct.cx = 80;
		editComputersButtonCreateStruct.cy = 30;
		editComputersButtonCreateStruct.hwndParent = this->hMainWindow;
		editComputersButtonCreateStruct.hMenu = (HMENU)IDS_EDITCOMPUTERS_BUTTON_TITLE;
		editComputersButtonCreateStruct.hInstance = this->hMainInstance;
		this->hEditComputersButton = Shutdown::MainStaticObject::CreateWindowFromStruct(editComputersButtonCreateStruct);

		CREATESTRUCT removeComputersButtonCreateStruct = { 0 };
		removeComputersButtonCreateStruct.lpszClass = WC_BUTTON;
		removeComputersButtonCreateStruct.lpszName = STR_REMOVECOMPUTERS_BUTTON_TITLE;
		removeComputersButtonCreateStruct.style = WS_CHILD | WS_DISABLED | BS_SPLITBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		removeComputersButtonCreateStruct.x = 220;
		removeComputersButtonCreateStruct.y = 155;
		removeComputersButtonCreateStruct.cx = 80;
		removeComputersButtonCreateStruct.cy = 30;
		removeComputersButtonCreateStruct.hwndParent = this->hMainWindow;
		removeComputersButtonCreateStruct.hMenu = (HMENU)IDS_REMOVECOMPUTERS_BUTTON_TITLE;
		removeComputersButtonCreateStruct.hInstance = this->hMainInstance;
		this->hRemoveComputersButton = Shutdown::MainStaticObject::CreateWindowFromStruct(removeComputersButtonCreateStruct);

		{
			BUTTON_SPLITINFO splitInfo = { 0 };
			splitInfo.mask = BCSIF_STYLE;
			splitInfo.uSplitStyle = BCSS_STRETCH;
			Button_SetSplitInfo(this->hRemoveComputersButton, &splitInfo);
		}

		CREATESTRUCT timerStaticCreateStruct = { 0 };
		timerStaticCreateStruct.lpszClass = WC_STATIC;
		timerStaticCreateStruct.lpszName = STR_TIMER_STATIC_TITLE;
		timerStaticCreateStruct.style = WS_CHILD | SS_SIMPLE;
		timerStaticCreateStruct.x = 10;
		timerStaticCreateStruct.y = 195;
		timerStaticCreateStruct.cx = 180;
		timerStaticCreateStruct.cy = 20;
		timerStaticCreateStruct.hwndParent = this->hMainWindow;
		timerStaticCreateStruct.hMenu = (HMENU)IDS_TIMER_STATIC_TITLE;
		timerStaticCreateStruct.hInstance = this->hMainInstance;
		Shutdown::MainStaticObject::CreateWindowFromStruct(timerStaticCreateStruct);

		CREATESTRUCT timerEditCreateStruct = { 0 };
		timerEditCreateStruct.dwExStyle = WS_EX_CLIENTEDGE;
		timerEditCreateStruct.lpszClass = WC_EDIT;
		timerEditCreateStruct.lpszName = STR_TIMER_EDIT_TITLE;
		timerEditCreateStruct.style = WS_CHILD | WS_DISABLED | ES_LEFT | ES_NUMBER | ES_NOHIDESEL;
		timerEditCreateStruct.x = 10;
		timerEditCreateStruct.y = 220;
		timerEditCreateStruct.cx = 40;
		timerEditCreateStruct.cy = 20;
		timerEditCreateStruct.hwndParent = this->hMainWindow;
		timerEditCreateStruct.hMenu = (HMENU)IDS_TIMER_EDIT_TITLE;
		timerEditCreateStruct.hInstance = this->hMainInstance;
		this->hTimerEdit = Shutdown::MainStaticObject::CreateWindowFromStruct(timerEditCreateStruct);

		Edit_LimitText(this->hTimerEdit, 4);

		CREATESTRUCT defTimersButtonCreateStruct = { 0 };
		defTimersButtonCreateStruct.lpszClass = WC_BUTTON;
		defTimersButtonCreateStruct.lpszName = STR_TIMER_DEF_BUTTON_TITLE;
		defTimersButtonCreateStruct.style = WS_CHILD | WS_DISABLED | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT;
		defTimersButtonCreateStruct.x = 60;
		defTimersButtonCreateStruct.y = 215;
		defTimersButtonCreateStruct.cx = 80;
		defTimersButtonCreateStruct.cy = 30;
		defTimersButtonCreateStruct.hwndParent = this->hMainWindow;
		defTimersButtonCreateStruct.hMenu = (HMENU)IDS_TIMER_DEF_BUTTON_TITLE;
		defTimersButtonCreateStruct.hInstance = this->hMainInstance;
		this->hDefTimerButton = Shutdown::MainStaticObject::CreateWindowFromStruct(defTimersButtonCreateStruct);

		CREATESTRUCT forceCheckBoxCreateStruct = { 0 };
		forceCheckBoxCreateStruct.lpszClass = WC_BUTTON;
		forceCheckBoxCreateStruct.lpszName = STR_FORCE_CHECKBOX_TITLE;
		forceCheckBoxCreateStruct.style = WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX;
		forceCheckBoxCreateStruct.x = 10;
		forceCheckBoxCreateStruct.y = 255;
		forceCheckBoxCreateStruct.cx = 160;
		forceCheckBoxCreateStruct.cy = 20;
		forceCheckBoxCreateStruct.hwndParent = this->hMainWindow;
		forceCheckBoxCreateStruct.hMenu = (HMENU)IDS_FORCE_CHECKBOX_TITLE;
		forceCheckBoxCreateStruct.hInstance = this->hMainInstance;
		this->hForceCheckBox = Shutdown::MainStaticObject::CreateWindowFromStruct(forceCheckBoxCreateStruct);

		CREATESTRUCT plannedCheckBoxCreateStruct = { 0 };
		plannedCheckBoxCreateStruct.lpszClass = WC_BUTTON;
		plannedCheckBoxCreateStruct.lpszName = STR_PLANNED_CHECKBOX_TITLE;
		plannedCheckBoxCreateStruct.style = WS_CHILD | WS_DISABLED | BS_AUTOCHECKBOX;
		plannedCheckBoxCreateStruct.x = 175;
		plannedCheckBoxCreateStruct.y = 255;
		plannedCheckBoxCreateStruct.cx = 160;
		plannedCheckBoxCreateStruct.cy = 20;
		plannedCheckBoxCreateStruct.hwndParent = this->hMainWindow;
		plannedCheckBoxCreateStruct.hMenu = (HMENU)IDS_PLANNED_CHECKBOX_TITLE;
		plannedCheckBoxCreateStruct.hInstance = this->hMainInstance;
		this->hPlannedCheckBox = Shutdown::MainStaticObject::CreateWindowFromStruct(plannedCheckBoxCreateStruct);

		Button_SetCheck(this->hPlannedCheckBox, TRUE);

		CREATESTRUCT messageStaticCreateStruct = { 0 };
		messageStaticCreateStruct.lpszClass = WC_STATIC;
		messageStaticCreateStruct.lpszName = STR_MESSAGE_STATIC_TITLE;
		messageStaticCreateStruct.style = WS_CHILD | SS_SIMPLE;
		messageStaticCreateStruct.x = 10;
		messageStaticCreateStruct.y = 285;
		messageStaticCreateStruct.cx = 180;
		messageStaticCreateStruct.cy = 20;
		messageStaticCreateStruct.hwndParent = this->hMainWindow;
		messageStaticCreateStruct.hMenu = (HMENU)IDS_MESSAGE_STATIC_TITLE;
		messageStaticCreateStruct.hInstance = this->hMainInstance;
		Shutdown::MainStaticObject::CreateWindowFromStruct(messageStaticCreateStruct);

		CREATESTRUCT messageEditCreateStruct = { 0 };
		messageEditCreateStruct.dwExStyle = WS_EX_CLIENTEDGE;
		messageEditCreateStruct.lpszClass = WC_EDIT;
		messageEditCreateStruct.lpszName = STR_MESSAGE_EDIT_TITLE;
		messageEditCreateStruct.style = WS_CHILD | WS_VSCROLL | WS_DISABLED | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_NOHIDESEL;
		messageEditCreateStruct.x = 10;
		messageEditCreateStruct.y = 305;
		messageEditCreateStruct.cx = 290;
		messageEditCreateStruct.cy = 150;
		messageEditCreateStruct.hwndParent = this->hMainWindow;
		messageEditCreateStruct.hMenu = (HMENU)IDS_MESSAGE_EDIT_TITLE;
		messageEditCreateStruct.hInstance = this->hMainInstance;
		this->hMessageEdit = Shutdown::MainStaticObject::CreateWindowFromStruct(messageEditCreateStruct);

		Edit_LimitText(this->hMessageEdit, 511);

		CREATESTRUCT statusBarCreateStruct = { 0 };
		statusBarCreateStruct.lpszClass = STATUSCLASSNAME;
		statusBarCreateStruct.lpszName = STR_STATUS_BAR_TITLE;
		statusBarCreateStruct.style = WS_CHILD | SBARS_SIZEGRIP;
		statusBarCreateStruct.x = CW_USEDEFAULT;
		statusBarCreateStruct.y = CW_USEDEFAULT;
		statusBarCreateStruct.cx = CW_USEDEFAULT;
		statusBarCreateStruct.cy = CW_USEDEFAULT;
		statusBarCreateStruct.hwndParent = this->hMainWindow;
		statusBarCreateStruct.hMenu = (HMENU)IDS_STATUS_BAR_TITLE;
		statusBarCreateStruct.hInstance = this->hMainInstance;
		this->hStatusBar = Shutdown::MainStaticObject::CreateWindowFromStruct(statusBarCreateStruct);

		SendMessage(this->hStatusBar, SB_SIMPLE, FALSE, (LPARAM)NULL);

		{
			const int parts_count = 2;
			const int parts[parts_count] = { 120, 260 };
			SendMessage(this->hStatusBar, SB_SETPARTS, (WPARAM)parts_count, (LPARAM)&parts);
		}

		this->UpdateStatusBarCaption(0);
		this->UpdateStatusBarCaption(Shutdown::ShutdownStatus::Ready);

		Shutdown::MainStaticObject::ComboBoxSelectItem(this->hActionsComboBox, Shutdown::ActionTypes::Shutdown);
	}

	// Private

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
		DialogBoxParam(this->hMainInstance, MAKEINTRESOURCE(IDI_DIALOG_ADD_COMOUTERS), this->hMainWindow, MainWindow::__MainWindow::ComputersDialogProc::DlgProc, (LPARAM)NULL);
	}

	void MainWindow::__MainWindow::ShowEditComputersDialog()
	{
		const int index = ListBox_GetCurSel(this->hComputersListBox);

		if (index < 0)
			return;

		DialogBoxParam(this->hMainInstance, MAKEINTRESOURCE(IDI_DIALOG_EDIT_COMOUTERS), this->hMainWindow, MainWindow::__MainWindow::ComputersDialogProc::DlgProc, (LPARAM)1);
	}

	void MainWindow::__MainWindow::ShowAboutDialog()
	{
		std::wstringstream ws_stream;
		ws_stream << STR_ABOUT_DIALOG_VERSION_CAPTION << STR_APP_VERSION << std::endl;
		ws_stream << STR_ABOUT_DIALOG_DEVELOPER_CAPTION << STR_APP_DEVELOPER << std::endl;
		ws_stream << STR_ABOUT_DIALOG_DESCRIPTION_CAPTION << STR_APP_DESCRIPTION;

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

	BOOL MainWindow::__MainWindow::ShowDialog()
	{
		if (!this->hMainWindow)
			return FALSE;

		const auto lpEnumFunc = [](HWND hwnd, LPARAM lParam)->BOOL
		{
			SetWindowFont(hwnd, lParam, FALSE);
			ShowWindow(hwnd, SW_SHOW);
			return TRUE;
		};
		EnumChildWindows(this->hMainWindow, lpEnumFunc, (LPARAM)this->guiFont.get());
		ShowWindow(hMainWindow, SW_SHOW);
		return TRUE;
	}
}