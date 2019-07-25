#pragma once

namespace NativeShutdown
{

	BOOL SetShutdownPrivilege(LPCWSTR lpSystemName, BOOL EnablePrivilege);
	HRESULT ShowShutdownDialog();

	inline static BOOL StartShutdown(BOOL bRebootAfterShutdown)
	{

		if
		(
			!NativeShutdown::SetShutdownPrivilege((LPCWSTR)NULL, true)
		)
			return FALSE;

		return InitiateSystemShutdownEx
		(
			(LPWSTR)NULL,
			(LPWSTR)NULL,
			(DWORD)0,
			FALSE,
			bRebootAfterShutdown,
			SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED
		);

	};

	inline static BOOL StopShutdown(LPWSTR lpMachineName)
	{
		if
		(
			!NativeShutdown::SetShutdownPrivilege
			(
				lpMachineName,
				true
			)
		)
			return FALSE;

		return AbortSystemShutdown(lpMachineName);
	};

	inline static BOOL UserLogOff()
	{
		return ExitWindowsEx
		(
			EWX_LOGOFF,
			SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED
		);
	};

	inline static BOOL UserLock()
	{
		return LockWorkStation();
	};

}