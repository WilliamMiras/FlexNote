#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <tchar.h>
#include <vector>
#include <objbase.h>
#include <rtscom.h> // RealTimeStylus
#include <comdef.h> // For _com_smartptr
using namespace std;

// Main window class name
static TCHAR szWindowClass[] = _T("FlexNote");

// Application title bar
static TCHAR szTitle[] = _T("A Better Note Taking App");

// Stored instance handle for use in Win32 API calls
HINSTANCE hInst;

// COM smart pointer for RealTimeStylus
_COM_SMARTPTR_TYPEDEF(IRealTimeStylus, __uuidof(IRealTimeStylus));
_COM_SMARTPTR_TYPEDEF(IStylusSyncPlugin, __uuidof(IStylusSyncPlugin));
_COM_SMARTPTR_TYPEDEF(IDynamicRenderer, __uuidof(IStylusSyncPlugin));

// Storing RealTimeStylus Strokes

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain entry point
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    // Contains information about the window, such as the application icon, the background color of the window, the name to display in the title bar, among other things.
    // Also contains function pointer to window procedue that handles messages 
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("My first window application!"),
            NULL);

        return 1;
    }

    EnableMouseInPointer(TRUE);

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    // This function returns an HWND, which is a handle to a window. Windows uses it to keep track of the windows you create
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (!hWnd) {
        MessageBox(NULL,
            _T("Call to CreateWindowEx Failed!"),
            _T("My first window application!"),
            NULL);

        return 1;
    }

    // Telling Windows to Make the Window Visible
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Message Loop to listen for messages that Windows sends
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Handle messages that the application receives from Windows when events occur. For example, if a user chooses an OK button in your application, Windows sends a message to you
LRESULT CALLBACK WndProc(
    _In_ HWND hWnd,
    _In_ UINT message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Welcome! This is going to be a great project :)");
    UINT32 pointerId;

    switch (message) {
    case WM_POINTERDOWN:
        pointerId = GET_POINTERID_WPARAM(wParam);
        POINTER_INFO pointerInfo;
        if (GetPointerInfo(pointerId, &pointerInfo)) {
            if (pointerInfo.pointerType == PT_PEN) {
                POINT pt = pointerInfo.ptPixelLocation;
                std::wstringstream buffer;
                buffer << _T("Stylus click at (") << pt.x << _T(", ") << pt.y << _T(")");
                MessageBox(hWnd, buffer.str().c_str(), _T("Stylus Event"), MB_OK);
            }
        }
        break;
  
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            // Here is application laid out
            //TextOut func. displays the text at the specified location in the window
            TextOut(hdc, 5, 5, greeting, _tcslen(greeting));

            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
    }

    return 0;
}