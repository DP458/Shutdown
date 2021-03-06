#pragma once

#define IDI_ICON1 1
#define IDI_ICON2 2
#define IDI_ICON3 3
#define IDI_ICON4 4
#define IDI_ICON5 5
#define IDI_ICON6 6
#define IDI_ICON7 7

//#define IDI_BMP_ADD 1

#define IDI_MENU 1

#define IDI_DIALOG_ADD_COMOUTERS 1
#define IDI_DIALOG_EDIT_COMOUTERS 2

#define APP_VERSION 8,0,3,0
#define STR_APP_VERSION L"8.0.3.0"
#define STR_APP_DEVELOPER L"DP458"
#define STR_APP_DESCRIPTION L"Graphical app to shutdown local host or remote ones"
#define STR_APP_NAME L"Shutdown Windows"
#define STR_APP_EXE_NAME L"Shutdown.exe"

// Main Window

#define ID_ACTION_SHUTDOWN 0
#define ID_ACTION_REBOOT 1
#define ID_ACTION_LOGOFF 2
#define ID_ACTION_LOCK 3
#define ID_ACTION_CANCEL 4
#define ID_ACTION_SUSPEND 5
#define ID_ACTION_HIBERNATE 6

#define ACTION_SHUTDOWN L"Shutdown"
#define ACTION_REBOOT L"Rebooting"
#define ACTION_LOGOFF L"Logging off"
#define ACTION_LOCK L"Locking"
#define ACTION_CANCEL L"Canceling"
#define ACTION_SUSPEND L"Suspending"
#define ACTION_HIBERNATE L"Hibernation"

#define IDC_APP_CLASS 100
#define IDS_APP_TITLE 101
#define IDS_ACTION_STATIC_TITLE 102
#define IDS_ACTIONS_COMBOBOX_TITLE 103
#define IDS_EXECACTION_BUTTON_TITLE 104
#define IDS_COMPUTERS_STATIC_TITLE 105
#define IDS_COMPUTERS_EDIT_TITLE 106
#define IDS_ADDCOMPUTERS_BUTTON_TITLE 107
#define IDS_EDITCOMPUTERS_BUTTON_TITLE 108
#define IDS_REMOVECOMPUTERS_BUTTON_TITLE 109
#define IDS_TIMER_STATIC_TITLE 110
#define IDS_TIMER_EDIT_TITLE 111
#define IDS_TIMER_DEF_BUTTON_TITLE 112
#define IDS_FORCE_CHECKBOX_TITLE 113
#define IDS_PLANNED_CHECKBOX_TITLE 114
#define IDS_MESSAGE_STATIC_TITLE 115
#define IDS_MESSAGE_EDIT_TITLE 116
#define IDS_STATUS_BAR_TITLE 117

#define STR_APP_CLASS L"ShutdownClassName"
#define STR_APP_TITLE L"Shutdown Windows"
#define STR_ACTION_STATIC_TITLE L"Select action"
#define STR_ACTIONS_COMBOBOX_TITLE NULL
#define STR_EXECACTION_BUTTON_TITLE L"Execute"
#define STR_COMPUTERS_STATIC_TITLE L"List of computers"
#define STR_ADDCOMPUTERS_BUTTON_TITLE L"Add"
#define STR_EDITCOMPUTERS_BUTTON_TITLE L"Edit"
#define STR_REMOVECOMPUTERS_BUTTON_TITLE L"Remove"
#define STR_COMPUTERS_EDIT_TITLE NULL
#define STR_TIMER_STATIC_TITLE L"Time-out period in seconds"
#define STR_TIMER_EDIT_TITLE L"120"
#define STR_TIMER_DEF_BUTTON_TITLE L"Default"
#define STR_FORCE_CHECKBOX_TITLE L"Force apps to close"
#define STR_PLANNED_CHECKBOX_TITLE L"Planned break"
#define STR_MESSAGE_STATIC_TITLE L"Warning message"
#define STR_MESSAGE_EDIT_TITLE NULL
#define STR_STATUS_BAR_TITLE L"Status Bar"

#define IDS_FILE_POPUP_MENU 200
#define IDS_NEW_POPUP_ITEM 205
#define IDS_OPEN_POPUP_ITEM 206
#define IDS_SAVE_POPUP_ITEM 207
#define IDS_SYSTEMDIALOG_POPUP_ITEM 208
#define IDS_EXIT_POPUP_ITEM 209

#define IDS_EDIT_POPUP_MENU 210
#define IDS_EXECUTE_ACTION_POPUP_ITEM 216
#define IDS_RESET_ACTIONS_COMBOBOX_POPUP_ITEM 217
#define IDS_RESET_COMPUTERS_LISTBOX_POPUP_ITEM 218
#define IDS_RESET_MESSAGE_EDIT_POPUP_ITEM 219

#define IDS_ADD_COMPUTERS_FROM_FILE_POPUP_ITEM 280
#define IDS_SAVE_COMPUTERS_TO_FILE_POPUP_ITEM 281
#define IDS_OPEN_SYSTEM_DIALOG_POPUP_ITEM 282

#define IDS_HELP_POPUP_MENU 290
#define IDS_SOURCE_AT_GITHUB_POPUP_ITEM 297
#define IDS_VIEW_WIKI_POPUP_ITEM 298
#define IDS_ABOUT_POPUP_ITEM 299

#define STR_FILE_POPUP_MENU L"&File"
#define STR_NEW_POPUP_ITEM L"&New"
#define STR_OPEN_POPUP_ITEM L"&Open"
#define STR_SAVE_POPUP_ITEM L"&Save"
#define STR_SYSTEMDIALOG_POPUP_ITEM L"&System Dialog"
#define STR_EXIT_POPUP_ITEM L"&Exit"

#define STR_EDIT_POPUP_MENU L"&Edit"
#define STR_EXECUTE_ACTION_POPUP_ITEM L"&Execute action"
#define STR_RESET_ACTIONS_COMBOBOX_POPUP_ITEM L"&Reset actions"
#define STR_RESET_COMPUTERS_LISTBOX_POPUP_ITEM L"&Clear computers"
#define STR_RESET_MESSAGE_EDIT_POPUP_ITEM L"&Clear message"

#define STR_ADD_COMPUTERS_FROM_FILE_POPUP_ITEM L"&Add from file"
#define STR_SAVE_COMPUTERS_TO_FILE_POPUP_ITEM L"&Save to file"
#define STR_OPEN_SYSTEM_DIALOG_POPUP_ITEM L"&Open system dialog"

#define STR_HELP_POPUP_MENU L"&Help"
#define STR_SOURCE_AT_GITHUB_POPUP_ITEM L"&Source at GitHub"
#define STR_VIEW_WIKI_POPUP_ITEM L"&View Wiki"
#define STR_ABOUT_POPUP_ITEM L"&About"

// About Dialog

#define IDS_ABOUT_DIALOG_TITLE 800
#define IDS_ABOUT_DIALOG_VERSION_CAPTION 801
#define IDS_ABOUT_DIALOG_DEVELOPER_CAPTION 802
#define IDS_ABOUT_DIALOG_DESCRIPTION_CAPTION 803

#define STR_ABOUT_DIALOG_TITLE L"About"
#define STR_ABOUT_DIALOG_VERSION_CAPTION L"Version: "
#define STR_ABOUT_DIALOG_DEVELOPER_CAPTION L"Developer: "
#define STR_ABOUT_DIALOG_DESCRIPTION_CAPTION L"Description: "

// Add Computers Dialog

#define IDS_ADDCOMPUTERS_DIALOG_TITLE 900
#define IDS_EDITCOMPUTERS_DIALOG_TITLE 901
#define IDS_ADDCOMPUTERS_COMPUTERNAME_STATIC 902
#define IDS_ADDCOMPUTERS_COMPUTERNAME_EDIT 903
#define IDS_ADDCOMPUTERS_ADD_BUTTON 904
#define IDS_EDITCOMPUTERS_EDIT_BUTTON 905
#define IDS_ADDCOMPUTERS_CANCEL_BUTTON 906

#define STR_ADDCOMPUTERS_DIALOG_TITLE L"Add computer"
#define STR_EDITCOMPUTERS_DIALOG_TITLE L"Edit computer"
#define STR_ADDCOMPUTERS_COMPUTERNAME_STATIC L"Enter computer name"
#define STR_ADDCOMPUTERS_COMPUTERNAME_EDIT NULL
#define STR_ADDCOMPUTERS_ADD_BUTTON L"Add"
#define STR_EDITCOMPUTERS_EDIT_BUTTON L"Edit"
#define STR_ADDCOMPUTERS_CANCEL_BUTTON L"Cancel"
