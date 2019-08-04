#pragma once

namespace win_api
{
	BOOL SetShutdownPrivilege(LPWSTR lpMachineName);
	wchar_t* GetWndText(HWND hWnd);
	wchar_t* GetListBoxText(HWND hListBox, int listbox_index);
	BOOL SaveListBoxStringsToFile(HWND hListBox, std::wfstream& fs);
}