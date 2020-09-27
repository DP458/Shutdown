#include "stdafx.h"

HWND Shutdown::MainStaticObject::CreateWindowFromStruct(const CREATESTRUCT& createStruct)
{
    return CreateWindowEx
    (
        createStruct.dwExStyle,
        createStruct.lpszClass,
        createStruct.lpszName,
        createStruct.style,
        createStruct.x,
        createStruct.y,
        createStruct.cx,
        createStruct.cy,
        createStruct.hwndParent,
        createStruct.hMenu,
        createStruct.hInstance,
        createStruct.lpCreateParams
    );
};

BOOL Shutdown::MainStaticObject::SetShutdownPrivilege(LPWSTR lpMachineName)
{
	auto openProcessToken = []()->HANDLE
	{
		HANDLE hToken = nullptr;
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		return hToken;
	};

	auto hToken = std::unique_ptr<std::remove_pointer<HANDLE>::type, ObjectDeleter<std::remove_pointer<HANDLE>::type>>
	(
		openProcessToken(),
		ObjectDeleter<std::remove_pointer<HANDLE>::type>(DeleterTypes::CloseHandleCall)
	);

	if (!hToken.get())
		return FALSE;

	TOKEN_PRIVILEGES tp = { 0 };
	tp.PrivilegeCount = ANYSIZE_ARRAY;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if
	(
		!LookupPrivilegeValue
		(
			lpMachineName,
			SE_SHUTDOWN_NAME,
			&(tp.Privileges[0].Luid)
		)
	)
		return FALSE;

	if
	(
		!AdjustTokenPrivileges
		(
			hToken.get(),
			FALSE,
			&tp,
			sizeof(TOKEN_PRIVILEGES),
			(PTOKEN_PRIVILEGES)NULL,
			(PDWORD)NULL
		)
	)
		return FALSE;

	return (GetLastError() != ERROR_NOT_ALL_ASSIGNED);
}

wchar_t* Shutdown::MainStaticObject::GetWndText(HWND hWnd)
{
	const int cTextLength = GetWindowTextLength(hWnd);

	if (cTextLength < 0)
		return nullptr;

	wchar_t* psText = nullptr;

	try
	{
		psText = new wchar_t[static_cast<size_t>(cTextLength) + 1];
		psText[0] = L'\0';
	}
	catch (...)
	{
		return nullptr;
	}

	GetWindowText
	(
		hWnd,
		psText,
		cTextLength + 1
	);

	return psText;
}

wchar_t* Shutdown::MainStaticObject::GetListBoxText(HWND hListBox, int listbox_index)
{
	const int cTextLength = ListBox_GetTextLen
	(
		hListBox,
		listbox_index
	);

	if (cTextLength < 0)
		return nullptr;

	wchar_t* psText = nullptr;

	try
	{
		psText = new wchar_t[static_cast<size_t>(cTextLength) + 1];
		psText[0] = L'\0';
	}
	catch (...)
	{
		return nullptr;
	}

	ListBox_GetText
	(
		hListBox,
		listbox_index,
		psText
	);

	return psText;
}

BOOL Shutdown::MainStaticObject::SaveListBoxStringsToFile(HWND hListBox, std::wfstream& fs)
{
	const int cStringCount = ListBox_GetCount(hListBox);

	if (cStringCount < 0)
		return FALSE;

	for (int i = 0; i < cStringCount; i++)
	{
		auto psText = std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>>
		(
			Shutdown::MainStaticObject::GetListBoxText(hListBox, i),
			Shutdown::ObjectDeleter<wchar_t>(Shutdown::DeleterTypes::DefaultArray)
		);

		fs << psText.get();

		if (fs.fail())
			break;

		if ((i + 1) < cStringCount)
			fs << L"\n";
	}

	const BOOL result = !fs.fail();

	fs.close();
	return result;
}

inline IShellDispatch* CreateShellDispatchObject()
{
	{
		const HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

		if
		(
			(hRes != S_OK) && (hRes != S_FALSE)
		)
			return nullptr;
	}

	IShellDispatch* pIShellDispatch = nullptr;

	CoCreateInstance
	(
		CLSID_Shell,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IShellDispatch,
		(LPVOID*)&pIShellDispatch
	);

	return pIShellDispatch;
}

BOOL Shutdown::MainStaticObject::ShowShutdownDialog()
{
	auto pIShellDispatch = std::unique_ptr<IShellDispatch, ComObjectDeleter>
	(
		CreateShellDispatchObject(),
		ComObjectDeleter(true)
	);

	if (!pIShellDispatch.get())
		return FALSE;

	return SUCCEEDED
	(
		pIShellDispatch->ShutdownWindows()
	);
}

inline IFileDialog* CreateFileDialogObject(Shutdown::FileDialogType type)
{
	{
		const HRESULT hRes = CoInitializeEx
		(
			NULL,
			COINIT_APARTMENTTHREADED
		);

		if
		(
			(hRes != S_OK) && (hRes != S_FALSE)
		)
			return nullptr;
	}

	IFileDialog* pIFileDialog = nullptr;

	switch (type)
	{

	case Shutdown::FileDialogType::Open:
		CoCreateInstance
		(
			CLSID_FileOpenDialog,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IFileOpenDialog,
			(LPVOID*)&pIFileDialog
		);
		break;

	case Shutdown::FileDialogType::Save:
		CoCreateInstance
		(
			CLSID_FileSaveDialog,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IFileSaveDialog,
			(LPVOID*)&pIFileDialog
		);
		break;

	}

	return pIFileDialog;
}

BOOL Shutdown::MainStaticObject::OpenTextFileThroughDialog(HWND hOwner, Shutdown::FileDialogType type, std::wfstream& fs)
{
	auto pIFileDialog = std::unique_ptr<IFileDialog, ComObjectDeleter>
	(
		CreateFileDialogObject(type),
		ComObjectDeleter(true)
	);

	if (!pIFileDialog.get())
		return FALSE;

	{
		COMDLG_FILTERSPEC file_type;
		file_type.pszName = L"Plain text";
		file_type.pszSpec = L"*.txt";

		pIFileDialog->SetFileTypes(1, &file_type);
		pIFileDialog->SetFileTypeIndex(1);
	}

	pIFileDialog->SetDefaultExtension(L"txt");

	if
	(
		SUCCEEDED(pIFileDialog->Show(hOwner))
	)
	{
		auto pIShellItem = std::unique_ptr<IShellItem, ComObjectDeleter>
		(
			[](IFileDialog* pIFileDialog)->IShellItem*
			{
				if (!pIFileDialog)
					return nullptr;

				IShellItem* pIShellItem = nullptr;
				pIFileDialog->GetResult(&pIShellItem);

				return pIShellItem;
			}(pIFileDialog.get()),

			ComObjectDeleter(false)
		);

		if (!pIShellItem.get())
			return FALSE;

		auto psFilePath = std::unique_ptr<std::remove_pointer<LPWSTR>::type, void(*)(LPWSTR)>
		(
			[](IShellItem* pIShellItem)->LPWSTR
			{
				if (!pIShellItem)
					return nullptr;

				LPWSTR psFilePath = nullptr;

				pIShellItem->GetDisplayName
				(
					SIGDN_FILESYSPATH,
					&psFilePath
				);

				return psFilePath;
			}(pIShellItem.get()),

				[](LPWSTR psFilePath)->void
			{
				CoTaskMemFree(psFilePath);
			}
		);

		if (!psFilePath.get())
			return FALSE;

		switch (type)
		{

		case Shutdown::FileDialogType::Open:
			fs.open
			(
				psFilePath.get(),
				std::ios_base::in
			);
			break;

		case Shutdown::FileDialogType::Save:
			fs.open
			(
				psFilePath.get(),
				std::ios_base::out
			);
			break;

		}

		return !fs.fail();
	}

	return TRUE;
}

HFONT Shutdown::MainStaticObject::GetSystemFont()
{
	NONCLIENTMETRICS metrics = { 0 };
	metrics.cbSize = sizeof(metrics);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
	return CreateFontIndirect(&metrics.lfMessageFont);
}

void Shutdown::MainStaticObject::ComboBoxSelectItem(HWND hComboBox, int index)
{
	ComboBox_SetCurSel(hComboBox, index);
	SNDMSG((GetParent(hComboBox)), WM_COMMAND, MAKEWPARAM(GetMenu(hComboBox), CBN_SELCHANGE), (LPARAM)hComboBox);
}

void Shutdown::MainStaticObject::CloseWindow(HWND hWnd)
{
	PostMessage(hWnd, WM_CLOSE, (WPARAM)NULL, (LPARAM)NULL);
}