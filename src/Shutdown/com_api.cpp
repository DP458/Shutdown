// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include <shobjidl.h>
#include <shldisp.h>
#include "com_api.h"

/*
   Deleter of COM objects for std::unique_ptr with CoUninitialize() calling
*/
struct CoUninitializeObjDeleter
{
	inline void operator()(IUnknown* pIUnknown)const
	{
		if (pIUnknown)
			pIUnknown->Release();

		CoUninitialize();
	}
};

/*
   Deleter of COM objects for std::unique_ptr
*/
struct COM_ObjDeleter
{
	inline void operator()(IUnknown* pIUnknown)const
	{
		if (pIUnknown)
			pIUnknown->Release();
	}
};

inline IShellDispatch* CreateShellDispatchObject()
{
	{
		const HRESULT hRes = CoInitializeEx
		(
			NULL,
			COINIT_APARTMENTTHREADED
		);

		if
		(
			(hRes != S_OK) && (hRes != S_FALSE)
		)
			return nullptr;
	}

	IShellDispatch* pIShellDispatch = nullptr;

	CoCreateInstance
	(
		CLSID_Shell,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IShellDispatch,
		(LPVOID*)&pIShellDispatch
	);

	return pIShellDispatch;
}

BOOL com_api::ShowShutdownDialog()
{
	auto pIShellDispatch = std::unique_ptr
	<
		IShellDispatch,
		CoUninitializeObjDeleter
	>
	(
		CreateShellDispatchObject(),
		CoUninitializeObjDeleter()
	);

	if (!pIShellDispatch.get())
		return FALSE;

	return SUCCEEDED
	(
		pIShellDispatch->ShutdownWindows()
	);
}

inline IFileDialog* CreateFileDialogObject(com_api::FileDialogType type)
{
	{
		const HRESULT hRes = CoInitializeEx
		(
			NULL,
			COINIT_APARTMENTTHREADED
		);

		if
		(
			(hRes != S_OK) && (hRes != S_FALSE)
		)
			return nullptr;
	}

	IFileDialog* pIFileDialog = nullptr;

	switch (type)
	{

	case com_api::FileDialogType::Open:
		CoCreateInstance
		(
			CLSID_FileOpenDialog,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IFileOpenDialog,
			(LPVOID*)& pIFileDialog
		);
	break;

	case com_api::FileDialogType::Save:
		CoCreateInstance
		(
			CLSID_FileSaveDialog,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IFileSaveDialog,
			(LPVOID*)& pIFileDialog
		);
	break;

	}

	return pIFileDialog;
}

BOOL com_api::OpenTextFileThroughDialog(HWND hOwner, com_api::FileDialogType type, std::wfstream& fs)
{
	auto pIFileDialog = std::unique_ptr
	<
		IFileDialog,
		CoUninitializeObjDeleter
	>
	(
		CreateFileDialogObject(type),
		CoUninitializeObjDeleter()
	);

	if (!pIFileDialog.get())
		return FALSE;

	{
		COMDLG_FILTERSPEC file_type;
		file_type.pszName = L"Plain text";
		file_type.pszSpec = L"*.txt";

		pIFileDialog->SetFileTypes
		(
			1,
			&file_type
		);

		pIFileDialog->SetFileTypeIndex(1);
	}

	pIFileDialog->SetDefaultExtension(L"txt");

	if
	(
		SUCCEEDED(pIFileDialog->Show(hOwner))
	)
	{
		auto pIShellItem = std::unique_ptr
		<
			IShellItem,
			COM_ObjDeleter
		>
		(
			[](IFileDialog* pIFileDialog)->IShellItem*
			{
				if (!pIFileDialog)
					return nullptr;

				IShellItem* pIShellItem = nullptr;
				pIFileDialog->GetResult(&pIShellItem);

				return pIShellItem;
			}(pIFileDialog.get()),

			COM_ObjDeleter()
		);

		if(!pIShellItem.get())
			return FALSE;

		auto psFilePath = std::unique_ptr
		<
			std::remove_pointer<LPWSTR>::type,
			void(*)(LPWSTR)
		>
		(
			[](IShellItem* pIShellItem)->LPWSTR
			{
				if (!pIShellItem)
					return nullptr;

				LPWSTR psFilePath = nullptr;

				pIShellItem->GetDisplayName
				(
					SIGDN_FILESYSPATH,
					&psFilePath
				);

				return psFilePath;
			}(pIShellItem.get()),

			[](LPWSTR psFilePath)->void
			{
				CoTaskMemFree(psFilePath);
			}
		);

		if (!psFilePath.get())
			return FALSE;

		switch (type)
		{

		case com_api::FileDialogType::Open:
			fs.open
			(
				psFilePath.get(),
				std::ios_base::in
			);
		break;

		case com_api::FileDialogType::Save:
			fs.open
			(
				psFilePath.get(),
				std::ios_base::out
			);
		break;

		}

		return !fs.fail();
	}

	return TRUE;
}