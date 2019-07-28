// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "NativeShutdown.h"

BOOL NativeShutdown::SetShutdownPrivilege()
{
	HANDLE hToken;

	if
	(
		!OpenProcessToken
		(
			GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
			&hToken
		)
	)
		return FALSE;

	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if
	(
		!LookupPrivilegeValue
		(
			(LPCWSTR)NULL,
			SE_SHUTDOWN_NAME,
			&(tp.Privileges[0].Luid)
		)
	)
	{
		CloseHandle(hToken);
		return FALSE;
	}

	if
	(
		!AdjustTokenPrivileges
		(
			hToken,
			FALSE,
			&tp,
			sizeof(TOKEN_PRIVILEGES),
			(PTOKEN_PRIVILEGES)NULL,
			(PDWORD)NULL
		)
	)
	{
		CloseHandle(hToken);
		return FALSE;
	}

	const BOOL result = (GetLastError() != ERROR_NOT_ALL_ASSIGNED);

	CloseHandle(hToken);
	return result;
}

BOOL NativeShutdown::ShowShutdownDialog()
{
	{
		HRESULT hRes = CoInitializeEx
		(
			NULL,
			COINIT_APARTMENTTHREADED
		);

		if
		(
			(hRes != S_OK) && (hRes != S_FALSE)
		)
			return FALSE;
	}

	CComPtr<IShellDispatch> ShellDispatch;

	if
	(
		!SUCCEEDED
		(
			ShellDispatch.CoCreateInstance
			(
				CLSID_Shell,
				NULL,
				CLSCTX_INPROC_SERVER
			)
		)
	)
	{
		CoUninitialize();
		return FALSE;
	}

	const BOOL result = SUCCEEDED
	(
		ShellDispatch->ShutdownWindows()
	);

	CoUninitialize();
	return result;
}