#include <windows.h>


#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        
        case WM_TIMER:
       
            MessageBoxW(NULL, L"Час відірватися від екрану і зробити зарядку для очей!", L"Нагадування", MB_OK | MB_ICONINFORMATION);
            break;

        case WM_TRAYICON: {
      
            if (lParam == WM_RBUTTONUP) { 
                HMENU hMenu = CreatePopupMenu();
                AppendMenuW(hMenu, MF_STRING, ID_TRAY_EXIT, L"Вихід");

                POINT pt;
                GetCursorPos(&pt);

                SetForegroundWindow(hwnd);
                TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
                DestroyMenu(hMenu);
            }
            break;
        }

        case WM_COMMAND: {
            
            if (LOWORD(wParam) == ID_TRAY_EXIT) {
                DestroyWindow(hwnd); 
            }
            break;
        }

        case WM_DESTROY: {
            
            NOTIFYICONDATAW nid = {0};
            nid.cbSize = sizeof(NOTIFYICONDATAW);
            nid.hWnd = hwnd;
            nid.uID = 1;
            Shell_NotifyIconW(NIM_DELETE, &nid); 
            
            PostQuitMessage(0); 
            break;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    WNDCLASSW wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"EyeTimerClass";
    RegisterClassW(&wc);


    HWND hwnd = CreateWindowExW(
        0, L"EyeTimerClass", L"Eye Timer", 0,
        0, 0, 0, 0,
        HWND_MESSAGE, NULL, hInstance, NULL
    );

    SetTimer(hwnd, 1, 1200000, NULL);

    
    NOTIFYICONDATAW nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATAW);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpyW(nid.szTip, L"Eye Timer");
    Shell_NotifyIconW(NIM_ADD, &nid);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}