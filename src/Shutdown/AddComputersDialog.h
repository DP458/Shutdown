#pragma once

namespace AddComputersDialogInternals
{

	class __AddComputersDialog
	{

	private:
		HINSTANCE hMainInstance;
		HWND hOwnerWindow;
		BOOL bIsClassRegistered;
		HWND hDialogWindow;
		HWND hComputerNameEdit;
		HWND hAddButton;
		HWND hCancelButton;

		__AddComputersDialog(HINSTANCE hInstance, HWND hOwnerWnd);
		void InitDialogWindow();
		BOOL AddComputerName();
		void CloseWindow();
		friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		static BOOL ShowDialog(HINSTANCE hInstance, HWND hOwnerWnd);

	};

}