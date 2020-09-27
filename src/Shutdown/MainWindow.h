#pragma once

namespace MainWindow
{
	class __MainWindow
	{
		class MainWindowProc
		{
		private:
			inline MainWindowProc();
			static void ActionComboBoxSelectionChanged(MainWindow::__MainWindow* pWndObj);
			static void ExecActionButtonClick(MainWindow::__MainWindow* pWndObj);

		public:
			static __MainWindow* GetInstance(HWND hWnd);
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		};

		class ComputersDialogProc
		{
		private:
			inline ComputersDialogProc();
			static __MainWindow* GetMainWindowClassInstance(HWND hwndDlg);

		public:
			static INT_PTR CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
		};

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

		DWORD GetTimerValue();
		BOOL StartShutdown(int listbox_index, BOOL bRebootAfterShutdown);
		BOOL StopShutdown(int listbox_index);
		BOOL StartComputersShutdown(BOOL bRebootAfterShutdown);
		BOOL StopComputersShutdown();
		void LoadComputerNamesFromFile();
		void SaveComputerNamesToFile();
		void UpdateStatusBarCaption(int count);
		void UpdateStatusBarCaption(Shutdown::ShutdownStatus status);
		void ClearComputerNames();
		void ClearMessage();
		void ShowAddComputersDialog();
		void ShowEditComputersDialog();
		void ShowAboutDialog();

	public:
		__MainWindow(HINSTANCE hInstance);
		BOOL AddComputerName(LPCWSTR computer_name);
		wchar_t* GetSelectedComputerName();
		BOOL ReplaceSelectedComputerName(LPCWSTR computer_name);
		BOOL ShowDialog();
	};
}