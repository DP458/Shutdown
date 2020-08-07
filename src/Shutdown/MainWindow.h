#pragma once

namespace MainWindow
{

	class __MainWindow
	{

	private:
		HINSTANCE hMainInstance;
		std::unique_ptr<std::remove_pointer<HFONT>::type, Shutdown::ObjectDeleter<std::remove_pointer<HFONT>::type>> guiFont;
		HWND hMainWindow;
		HWND hActionsComboBox;
		HWND hExecActionButton;
		HWND hComputersListBox;
		HWND hAddComputersButton;
		HWND hEditComputersButton;
		HWND hRemoveComputersButton;
		HWND hTimerEdit;
		HWND hDefTimerButton;
		HWND hForceCheckBox;
		HWND hPlannedCheckBox;
		HWND hMessageEdit;
		HWND hStatusBar;

		__MainWindow(HINSTANCE hInstance);
		DWORD GetTimerValue();
		BOOL StartShutdown(int listbox_index, BOOL bRebootAfterShutdown);
		BOOL StopShutdown(int listbox_index);
		BOOL StartComputersShutdown(BOOL bRebootAfterShutdown);
		BOOL StopComputersShutdown();
		void LoadComputerNamesFromFile();
		void ExecActionButtonClick();
		void UpdateStatusBarCaption(int count);
		void UpdateStatusBarCaption(Shutdown::ShutdownStatus status);
		void ClearComputerNames();
		void ClearMessage();
		void ShowAddComputersDialog();
		void ShowEditComputersDialog();
		void ShowAboutDialog();
		void CloseWindow();
		void ActionComboBoxSelectionChanged();
		friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		BOOL AddComputerName(LPCWSTR computer_name);
		wchar_t* GetSelectedComputerName();
		BOOL ReplaceSelectedComputerName(LPCWSTR computer_name);
		static BOOL ShowDialog(HINSTANCE hInstance);
		static __MainWindow* GetInstance();

	};

}