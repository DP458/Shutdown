// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Resources.h"
#include "MainWindow.h"
#include "AddComputersDialog.h"

namespace AddComputersDialogInternals
{

	INT_PTR CALLBACK AddComputersDialogInternals::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{

		case WM_INITDIALOG:
		{
			HWND hComputerNameEdit = GetDlgItem(hwndDlg, IDS_ADDCOMPUTERS_COMPUTERNAME_EDIT);

			if (!hComputerNameEdit)
				break;

			SetFocus(hComputerNameEdit);

			if (lParam && lParam == 1)
			{
				auto psText = std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>>
				(
					MainWindow::__MainWindow::GetInstance()->GetSelectedComputerName(),
					Shutdown::ObjectDeleter<wchar_t>(Shutdown::DeleterTypes::DefaultArray)
				);

				if (!psText.get())
					break;

				SetWindowText(hComputerNameEdit, psText.get());

				const int cTextLength = GetWindowTextLength(hComputerNameEdit);

				if (cTextLength < 0)
					break;

				Edit_SetSel(hComputerNameEdit, cTextLength, cTextLength);
			}
		}
		break;

		case WM_COMMAND:

			switch (HIWORD(wParam))
			{
			case BN_CLICKED:

				switch (LOWORD(wParam))
				{

				case IDS_ADDCOMPUTERS_ADD_BUTTON: case IDS_EDITCOMPUTERS_EDIT_BUTTON:
				{
					HWND hComputerNameEdit = GetDlgItem(hwndDlg, IDS_ADDCOMPUTERS_COMPUTERNAME_EDIT);

					if (!hComputerNameEdit)
						return TRUE;

					auto psComputerName = std::unique_ptr<wchar_t[], Shutdown::ObjectDeleter<wchar_t>>
					(
						Shutdown::MainStaticObject::GetWndText(hComputerNameEdit),
						Shutdown::ObjectDeleter<wchar_t>(Shutdown::DeleterTypes::DefaultArray)
					);

					if
					(
						!psComputerName.get() || psComputerName[0] == L'\0'
						|| LOWORD(wParam) == IDS_ADDCOMPUTERS_ADD_BUTTON && !MainWindow::__MainWindow::GetInstance()->AddComputerName(psComputerName.get())
						|| LOWORD(wParam) == IDS_EDITCOMPUTERS_EDIT_BUTTON && !MainWindow::__MainWindow::GetInstance()->ReplaceSelectedComputerName(psComputerName.get())
					)
					{
						SetFocus(hComputerNameEdit);
						return TRUE;
					}

					EndDialog(hwndDlg, wParam);
					return TRUE;
				}

				case IDOK: case IDCANCEL: case IDS_ADDCOMPUTERS_CANCEL_BUTTON:
					EndDialog(hwndDlg, wParam);
					return TRUE;

				}
				break;

			}
			break;

		}

		return FALSE;
	}
}