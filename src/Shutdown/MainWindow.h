#pragma once

namespace MainWindow
{

	class __MainWindow
	{

	private:
		HINSTANCE hMainInstance;
		HWND hMainWindow;
		HWND hActionsComboBox;
		HWND hExecActionButton;
		HWND hComputersListBox;
		HWND hAddComputersButton;
		HWND hRemoveComputersButton;
		HWND hClearComputersButton;
		HWND hTimerEdit;
		HWND hDefTimerButton;
		HWND hForceCheckBox;
		HWND hPlannedCheckBox;
		HWND hMessageEdit;
		HWND hStatusBar;

		enum ShutdownStatus
		{
			Ready,
			Successful,
			Fail,
			Processing
		};

		__MainWindow(HINSTANCE hInstance);
		void InitMainWindow();
		DWORD GetTimerValue();
		BOOL StartShutdown(int listbox_index, BOOL bRebootAfterShutdown);
		BOOL StopShutdown(int listbox_index);
		BOOL StartComputersShutdown(BOOL bRebootAfterShutdown);
		BOOL StopComputersShutdown();
		void ExecActionButtonClick();
		void UpdateStatusBarCaption(int count);
		void UpdateStatusBarCaption(ShutdownStatus status);
		void ClearComputerNames();
		void CloseWindow();
		friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		BOOL AddComputerName(LPCWSTR computer_name);
		static BOOL ShowDialog(HINSTANCE hInstance);
		static __MainWindow* GetInstance();

	};

}