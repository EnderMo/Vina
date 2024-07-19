#pragma once
#include <string>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include "Window.h"
#pragma comment(lib, "Version.lib")
namespace vui
{
    constexpr int WM_TRAYMSG = WM_USER + 312;
    class Tray
    {
    protected:
        virtual LPARAM WndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) = 0;
        NOTIFYICONDATAW nid{0};

    private:
        class TrayDummyWindow : public vui::Window
        {
        public:
            Tray *pnt;
            TrayDummyWindow(Tray *parent)
            {
                pnt = parent;
                CreateControl();
            }
            virtual LPARAM WndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) override
            {
                if (msg == WM_TRAYMSG)
                    return pnt->WndProc(h, LOWORD(lp), HIWORD(lp), wp);
                return vui::Window::WndProc(h, msg, wp, lp);
            }
        };
        TrayDummyWindow win{this};

    public:
        Tray()
        {
            nid.hWnd = win.handle();
            nid.cbSize = sizeof(nid);
            nid.uID = rand();
            wchar_t *filename;
            _get_wpgmptr(&filename);
            // wchar_t rawexename[512];
            DWORD verlen = GetFileVersionInfoSizeW(filename, 0);
            LPVOID verbuf = new char[verlen];
            GetFileVersionInfoW(filename, 0, verlen, verbuf);
            DWORD *localname;
            UINT _;
            VerQueryValueW(verbuf, L"\\VarFileInfo\\Translation", (LPVOID *)&localname, &_);
            // std::wstring pnk = L"\\StringFileInfo\\00000000\\ProductName";
            std::wostringstream wss;
            wss << "\\StringFileInfo\\" << std::hex << std::setw(8) << std::setfill(L'0') << MAKELONG(HIWORD(*localname),LOWORD(*localname)) << "\\ProductName";
            auto str = wss.str();
            wchar_t *productname;
            UINT productnamesize;
            VerQueryValueW(verbuf, str.c_str(), (LPVOID *)&productname, &productnamesize);
            std::copy(productname, productname + productnamesize, nid.szTip);
            delete[] verbuf;
            // delete[] filename;
            //  std::copy(nid.szTip);
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
            nid.uCallbackMessage = WM_TRAYMSG;
            nid.uVersion = 4;
            SetIcon();
        }
        ~Tray()
        {
            Destory();
        }
        void SetIcon(HICON ico = 0)
        {
            if (ico == 0)
            {
                wchar_t filename[512];
                GetModuleFileNameW(GetModuleHandleW(NULL), filename, sizeof(filename));
                nid.hIcon = ExtractIconW((HINSTANCE)GetModuleHandleW(NULL), filename, 0);
                nid.hIcon = nid.hIcon == 0 ? LoadIcon(NULL, IDI_APPLICATION) : nid.hIcon;
                nid.hBalloonIcon = nid.hIcon;
            }
            else
            {
                nid.hIcon = ico;
                nid.hBalloonIcon = nid.hIcon;
            }
        }
        void Show()
        {
            Shell_NotifyIconW(NIM_ADD, &nid);
        }
        void ShowBoolean(const std::wstring &content, const std::wstring &title,int infoflag = 0,unsigned int timeout = 5000)
        {
            if (title.size() > sizeof(nid.szInfoTitle))
            {
                throw;
            }
            std::copy(title.begin(),title.end(),nid.szInfoTitle);
            if(content.size() > sizeof(nid.szInfo))
            {
                throw;
            }
            std::copy(content.begin(), content.end(), nid.szInfo);
            nid.uTimeout = timeout;
            nid.dwInfoFlags = infoflag;
            Shell_NotifyIconW(NIM_MODIFY, &nid);
        }
        void Destory()
        {
            Shell_NotifyIconW(NIM_DELETE, &nid);
        }
    };
}