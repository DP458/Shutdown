// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <windows.h>
#include <shldisp.h>
#include <atlbase.h>
#include "NativeShutdown.h"

// Private

BOOL SetPrivilege(LPCWSTR lpSystemName, HANDLE hToken, LPCWSTR lpszPrivilege, BOOL EnablePrivilege)
{

	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1UL;
	tp.Privileges[0].Attributes = EnablePrivilege ? SE_PRIVILEGE_ENABLED : 0UL;

	BOOL result = LookupPrivilegeValue
	(
		lpSystemName,
		lpszPrivilege,
		&(tp.Privileges[0].Luid)
	);

	if (!result)
		return result;

	result = AdjustTokenPrivileges
	(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL
	);

	if (!result)
		return result;

	result = (GetLastError() == ERROR_NOT_ALL_ASSIGNED);

	return !result;

}

// Public

BOOL NativeShutdown::SetShutdownPrivilege(LPCWSTR lpSystemName, BOOL EnablePrivilege)
{

	HANDLE hToken;

	BOOL result = OpenProcessToken
	(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken
	);

	if (!result)
		return result;

	result = SetPrivilege
	(
		lpSystemName,
		hToken,
		SE_SHUTDOWN_NAME,
		EnablePrivilege
	);

	CloseHandle(hToken);
	return result;

}

HRESULT NativeShutdown::ShowShutdownDialog()
{

	HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	if
	(
		(hRes != S_OK) && (hRes != S_FALSE)
	)
		return hRes;

	CComPtr<IShellDispatch> ShellDispatch;

	hRes = ShellDispatch.CoCreateInstance
	(
		CLSID_Shell,
		NULL,
		CLSCTX_INPROC_SERVER
	);

	if (hRes != S_OK)
		return hRes;

	hRes = ShellDispatch->ShutdownWindows();

	CoUninitialize();

	return hRes;

}

BOOL NativeShutdown::StartShutdown(BOOL bRebootAfterShutdown)
{

	BOOL result = NativeShutdown::SetShutdownPrivilege
	(
		(LPCWSTR)NULL,
		true
	);

	if (!result)
		return result;

	result = InitiateSystemShutdownEx
	(
		(LPWSTR)NULL,
		(LPWSTR)NULL,
		(DWORD) 0,
		FALSE,
		bRebootAfterShutdown,
		SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED
	);

	return result;

}

BOOL NativeShutdown::StopShutdown()
{

	BOOL result = NativeShutdown::SetShutdownPrivilege
	(
		(LPCWSTR)NULL,
		true
	);

	if (!result)
		return result;

	result = AbortSystemShutdown
	(
		(LPWSTR)NULL
	);

	return result;

};