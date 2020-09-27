#include "stdafx.h"
#include "Application.h"

int Shutdown::Application::Run()
{
	BOOL bRet = FALSE;
	MSG msg = { 0 };

	while(bRet = GetMessage(&msg, (HWND)NULL, NULL, NULL))
	{
		if (bRet == -1)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}