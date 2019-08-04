// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "win_api.h"

BOOL win_api::SetShutdownPrivilege(LPWSTR lpMachineName)
{
	auto hToken = std::unique_ptr
	<
		std::remove_pointer<HANDLE>::type, 
		void(*)(HANDLE)
	>
	(
		[]()->HANDLE
		{
			HANDLE hToken = { 0 };

			OpenProcessToken
			(
				GetCurrentProcess(),
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
				&hToken
			);

			return hToken;
		}(),

		[](HANDLE handle)->void
		{ 
			CloseHandle(handle);
		}
	);

	if (!hToken.get())
		return FALSE;

	TOKEN_PRIVILEGES tp;
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

wchar_t* win_api::GetWndText(HWND hWnd)
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

wchar_t* win_api::GetListBoxText(HWND hListBox, int listbox_index)
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

BOOL win_api::SaveListBoxStringsToFile(HWND hListBox, std::wfstream& fs)
{
	const int cStringCount = ListBox_GetCount(hListBox);

	if (cStringCount < 0)
		return FALSE;

	for (int i = 0; i < cStringCount; i++)
	{
		auto psText = std::unique_ptr<wchar_t[]>
		(
			win_api::GetListBoxText
			(
				hListBox,
				i
			)
		);

		fs << psText.get();

		if(fs.fail())
			break;

		if ((i + 1) < cStringCount)
			fs << L"\n";
	}

	const BOOL result = !fs.fail();

	fs.close();
	return result;
}