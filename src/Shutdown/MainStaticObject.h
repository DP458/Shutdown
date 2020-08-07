#pragma once

namespace Shutdown
{
    /// <summary>
    /// Types of deleters for std::unique_ptr
    /// </summary>
    enum DeleterTypes
    {
        /// <summary>
        /// Default deleter
        /// </summary>
        Default = 1,

        /// <summary>
        /// Default deleter for arrays
        /// </summary>
        DefaultArray = 2,

        /// <summary>
        /// Deleter of WIN API objects by using DeleteObject()
        /// </summary>
        DeleteObjectCall = 3,

        /// <summary>
        /// Deleter of WIN API objects by using CloseHandle()
        /// </summary>
        CloseHandleCall = 4
    };

    /// <summary>
    /// Deleter of objects for std::unique_ptr
    /// </summary>
    template <class T> class ObjectDeleter
    {
    private:
        DeleterTypes _type;

    public:
        inline ObjectDeleter(DeleterTypes type = DeleterTypes::Default) : _type(type)
        {
        }

        inline void operator()(T* pObj)const
        {
            if (!pObj)
                return;

            switch (_type)
            {

            case DeleterTypes::Default:
                delete pObj;
                break;

            case DeleterTypes::DefaultArray:
                delete[] pObj;
                break;

            case DeleterTypes::DeleteObjectCall:
                DeleteObject(pObj);
                break;

            case DeleterTypes::CloseHandleCall:
                CloseHandle(pObj);
                break;

            default:
                MessageBox(nullptr, L"The object have not been deleted", L"Warning", MB_OK | MB_ICONWARNING);
                break;

            }
        }
    };

    /// <summary>
    /// Deleter of COM objects for std::unique_ptr
    /// </summary>
    class ComObjectDeleter
    {
    private:
        bool _uninitialize;

    public:
        inline ComObjectDeleter(bool uninitialize = false) : _uninitialize(uninitialize)
        {
        }

        inline void operator()(IUnknown* pIUnknown)const
        {
            if (!pIUnknown)
                return;
            pIUnknown->Release();
            if (_uninitialize)
                CoUninitialize();
        }
    };

    enum FileDialogType
    {
        Open = 1,
        Save = 2
    };

    /*struct WindowStruct
    {
    };*/

    class MainStaticObject
    {
    public:
        static HWND CreateWindowFromStruct(const CREATESTRUCT& createStruct);
        static BOOL SetShutdownPrivilege(LPWSTR lpMachineName);
        static wchar_t* GetWndText(HWND hWnd);
        static wchar_t* GetListBoxText(HWND hListBox, int listbox_index);
        static BOOL SaveListBoxStringsToFile(HWND hListBox, std::wfstream& fs);
        static BOOL ShowShutdownDialog();
        static BOOL OpenTextFileThroughDialog(HWND hOwner, FileDialogType type, std::wfstream& fs);
        //static WindowStruct* GetWindowStruct(HWND hwnd);
        static HFONT GetSystemFont();
        static void ComboBoxSelectItem(HWND hComboBox, int index);

    private:
        inline MainStaticObject();
    };
}