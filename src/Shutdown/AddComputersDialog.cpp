// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Resources.h"
#include "MainWindow.h"
#include "AddComputersDialog.h"

namespace AddComputersDialogInternals
{

	// Private

	AddComputersDialogInternals::__AddComputersDialog::__AddComputersDialog(HINSTANCE hInstance, HWND hOwnerWnd) :
		hMainInstance(hInstance), hOwnerWindow(hOwnerWnd), hDialogWindow(NULL), hComputerNameEdit(NULL), hAddButton(NULL),
		hCancelButton(NULL)
	{
		WNDCLASSEX wcex = { 0 };

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = AddComputersDialogInternals::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = DLGWINDOWEXTRA;
		wcex.hInstance = hInstance;
		wcex.hIcon = (HICON)NULL;
		wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = STR_ADDCOMPUTERS_DIALOG_CLASS;
		wcex.hIconSm = (HICON)NULL;

		RegisterClassEx(&wcex);
	}

	void AddComputersDialogInternals::__AddComputersDialog::InitDialogWindow()
	{
		CreateWindowEx
		(
			(DWORD)NULL,
			WC_STATIC,
			STR_ADDCOMPUTERS_COMPUTERNAME_STATIC,
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			10,
			5,
			180,
			20,
			this->hDialogWindow,
			(HMENU)IDS_ADDCOMPUTERS_COMPUTERNAME_STATIC,
			this->hMainInstance,
			(LPVOID)NULL
		);


		this->hComputerNameEdit = CreateWindowEx
		(
			WS_EX_CLIENTEDGE,
			WC_EDIT,
			STR_ADDCOMPUTERS_COMPUTERNAME_EDIT,
			WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_NOHIDESEL,
			10,
			28,
			250,
			40,
			this->hDialogWindow,
			(HMENU)IDS_ADDCOMPUTERS_COMPUTERNAME_EDIT,
			this->hMainInstance,
			(LPVOID)NULL
		);

		Edit_LimitText(this->hComputerNameEdit, 30);
		SetFocus(this->hComputerNameEdit);


		this->hAddButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_ADDCOMPUTERS_ADD_BUTTON,
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			86,
			78,
			80,
			30,
			this->hDialogWindow,
			(HMENU)IDS_ADDCOMPUTERS_ADD_BUTTON,
			this->hMainInstance,
			(LPVOID)NULL
		);

		this->hCancelButton = CreateWindowEx
		(
			(DWORD)NULL,
			WC_BUTTON,
			STR_ADDCOMPUTERS_CANCEL_BUTTON,
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT,
			176,
			78,
			80,
			30,
			this->hDialogWindow,
			(HMENU)IDS_ADDCOMPUTERS_CANCEL_BUTTON,
			this->hMainInstance,
			(LPVOID)NULL
		);
	}

	BOOL AddComputersDialogInternals::__AddComputersDialog::AddComputerName()
	{
		const int text_length = GetWindowTextLength(this->hComputerNameEdit);

		if (text_length <= 0)
			return FALSE;

		auto psComputerName = std::make_unique<wchar_t[]>(static_cast<size_t>(text_length) + 1);

		GetWindowText
		(
			this->hComputerNameEdit,
			psComputerName.get(),
			text_length + 1
		);

		MainWindow::__MainWindow::GetInstance()->AddComputerName
		(
			psComputerName.get()
		);

		return TRUE;
	}

	void AddComputersDialogInternals::__AddComputersDialog::CloseWindow()
	{
		PostMessage
		(
			this->hDialogWindow,
			WM_CLOSE,
			(WPARAM)NULL,
			(LPARAM)NULL
		);
	}

	// Local

	static AddComputersDialogInternals::__AddComputersDialog* pDlgObj = nullptr;

	LRESULT CALLBACK AddComputersDialogInternals::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{

		case WM_DESTROY:
			EnableWindow(AddComputersDialogInternals::pDlgObj->hOwnerWindow, TRUE);
			SetActiveWindow(AddComputersDialogInternals::pDlgObj->hOwnerWindow);
		return 0;

		case WM_COMMAND:

			switch (HIWORD(wParam))
			{

			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				case IDS_ADDCOMPUTERS_ADD_BUTTON:

					if (!AddComputersDialogInternals::pDlgObj->AddComputerName())
					{
						SetFocus(AddComputersDialogInternals::pDlgObj->hComputerNameEdit);
						break;
					}

					AddComputersDialogInternals::pDlgObj->CloseWindow();

				return 0;

				case IDS_ADDCOMPUTERS_CANCEL_BUTTON:
					AddComputersDialogInternals::pDlgObj->CloseWindow();
				return 0;

				}

			break;

			}

		break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	// Public

	BOOL AddComputersDialogInternals::__AddComputersDialog::ShowDialog(HINSTANCE hInstance, HWND hOwnerWnd)
	{
		static __AddComputersDialog DlgInternalObj(hInstance, hOwnerWnd);

		AddComputersDialogInternals::pDlgObj = &DlgInternalObj;

		DlgInternalObj.hDialogWindow = CreateWindowEx
		(
			WS_EX_DLGMODALFRAME,
			STR_ADDCOMPUTERS_DIALOG_CLASS,
			STR_ADDCOMPUTERS_DIALOG_TITLE,
			WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			285,
			158,
			hOwnerWnd,
			(HMENU)NULL,
			hInstance,
			(LPVOID)NULL
		);

		if (!DlgInternalObj.hDialogWindow)
			return FALSE;

		DlgInternalObj.InitDialogWindow();
		EnableWindow(hOwnerWnd, FALSE);
		return TRUE;
	}

}