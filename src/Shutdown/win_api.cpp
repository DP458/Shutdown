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