// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Resources.h"
#include "AddComputersDialog.h"
#include "AddComputersDialogInternals.h"

namespace AddComputersDialog
{

	AddComputersDialogInternals::__AddComputersDialog* DlgInternalObj = AddComputersDialogInternals::__AddComputersDialog::GetObjectRef();

	BOOL AddComputersDialog::CreateDialogWindow(HINSTANCE hInstance, HWND owner)
	{
		
		DlgInternalObj->hMainInstance = hInstance;
		DlgInternalObj->hOwnerWindow = owner;

		if(!DlgInternalObj->bIsClassRegistered)
		{

			WNDCLASSEX wcex = { 0 };

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = AddComputersDialogInternals::WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = DLGWINDOWEXTRA;
			wcex.hInstance = DlgInternalObj->hMainInstance;
			wcex.hIcon = (HICON)NULL;
			wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = STR_ADDCOMPUTERS_DIALOG_CLASS;
			wcex.hIconSm = (HICON)NULL;

			DlgInternalObj->bIsClassRegistered = RegisterClassEx(&wcex);

			if (!DlgInternalObj->bIsClassRegistered)
				return FALSE;

		}

		if
		(
			CreateWindowEx
			(
				WS_EX_DLGMODALFRAME,
				STR_ADDCOMPUTERS_DIALOG_CLASS,
				STR_ADDCOMPUTERS_DIALOG_TITLE,
				WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				285,
				158,
				DlgInternalObj->hOwnerWindow,
				(HMENU)NULL,
				DlgInternalObj->hMainInstance,
				(LPVOID)NULL
			)
		)
		{
			EnableWindow(DlgInternalObj->hOwnerWindow, FALSE);
			return TRUE;
		}
		else return FALSE;

	}

}