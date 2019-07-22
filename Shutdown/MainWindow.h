#pragma once

namespace MainWindow
{

	class __MainWindow
	{

	private:
		HINSTANCE hMainInstance;
		BOOL bIsClassRegistered;
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

		enum ShutdownActionType
		{
			Shutdown,
			Reboot,
			Cancel
		};

		__MainWindow(HINSTANCE hInstance);
		void InitMainWindow();
		BOOL StartShutdown(LPWSTR computer_name, BOOL bRebootAfterShutdown);
		void ShutdownComputers(ShutdownActionType type);
		void AddStatusBarCaption(int count);
		void ClearComputerNames();
		void CloseWindow();
		friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		BOOL AddComputerName(LPCWSTR computer_name);
		static BOOL ShowDialog(HINSTANCE hInstance);
		static __MainWindow* GetInstance();

	};

}