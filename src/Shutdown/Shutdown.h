#pragma once

namespace Shutdown
{
	enum ActionTypes
	{
		 Shutdown  = 0
		,Reboot    = 1
		,LogOff    = 2
		,Lock      = 3
		,Cancel    = 4
		,Suspend   = 5
		,Hibernate = 6
	};

	enum ShutdownStatus
	{
		Ready,
		Successful,
		Fail,
		Processing
	};
}