#pragma once

namespace com_api
{
	enum FileDialogType
	{
		Open,
		Save
	};

	BOOL ShowShutdownDialog();
	BOOL OpenTextFileThroughDialog(HWND hOwner, FileDialogType type, std::wfstream& fs);
}
