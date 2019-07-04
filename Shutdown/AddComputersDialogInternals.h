#pragma once

namespace AddComputersDialogInternals
{

	struct __AddComputersDialog
	{

		BOOL bIsClassRegistered;
		HINSTANCE hMainInstance;
		HWND hOwnerWindow;
		HWND hComputerNameEdit;
		HWND hAddButton;
		HWND hCancelButton;

		static __AddComputersDialog* GetObjectRef();

	};

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

}