#pragma once

namespace NativeShutdown
{

	BOOL SetShutdownPrivilege(BOOL EnablePrivilege);
	HRESULT ShowShutdownDialog();
	BOOL StartShutdown(BOOL bRebootAfterShutdown);
	BOOL StopShutdown();

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