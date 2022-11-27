// window.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "window.h"
#include "windowsx.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

#pragma comment(lib,"msimg32.lib")

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDB_FAN));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDB_TB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int page = 0;
    static HINSTANCE hInst;
    HDC hdc, hdcMem;
    HBITMAP hBitMap;
    BITMAP bm;
    PAINTSTRUCT ps;
    RECT rect;
    GetClientRect(hWnd, &rect);
    int client_width = (rect.right - rect.left);
    int client_height = (rect.bottom - rect.top);
    switch (message){
    case WM_CREATE:
        hInst = ((LPCREATESTRUCT)lParam)->hInstance;
        break;
    case WM_LBUTTONDOWN:
        //以下检测区域
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        if (page == 0 || page == 9 || page == 10) {
            int tmp = 0;
            if (page == 9) tmp = 10;
            else if (page == 10) tmp = 12;
            if (pt.y > client_height / 2) page = 1 + tmp;
            else page = 2 + tmp;
        }else if (page == 1 || page == 2) {
            int tmp = 0;
            if (page == 2) tmp = 4;
            if (pt.y > client_height * 3 / 4) page = 3 + tmp;
            else if (pt.y > client_height / 2) page = 4 + tmp;
            else if (pt.y > client_height / 4) page = 5 + tmp;
            else page = 6 + tmp;
        }else if (page == 8) page = 15;
        SendMessage(hWnd, WM_PAINT, 0, 0);
        InvalidateRect(hWnd, &rect, false);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            if (page == 15) page = 8;
            else if (page >= 13) page = 10;
            else if (page >= 11) page = 9;
            else if (page >= 7) page = 2;
            else if (page >= 3) page = 1;
            else page = 0;
        }
        SendMessage(hWnd, WM_PAINT, 0, 0);
        InvalidateRect(hWnd, &rect, false);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        int res;
        switch (page) {
        case 0: res = IDB_INDEX; break;
        case 1: res = IDB_NORTH; break;
        case 2: res = IDB_SOUTH; break;
        case 3: res = IDB_N3; break;
        case 4: res = IDB_N4; break;
        case 5: res = IDB_N5; break;
        case 6: res = IDB_N6; break;
        case 7: res = IDB_S1; break;
        case 8: res = IDB_S2; break;
        case 9: res = IDB_S3; break;
        case 10: res = IDB_S4; break;
        case 11: res = IDB_S3_2; break;
        case 12: res = IDB_S3_1; break;
        case 13: res = IDB_S4_2; break;
        case 14: res = IDB_S4_1; break;
        case 15: res = IDB_S2_1; break;
        default: res = IDB_DEFAULT;
        }
        hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(res));
        hdc = BeginPaint(hWnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        SetStretchBltMode(hdc, COLORONCOLOR);
        SelectObject(hdcMem, hBitMap);
        GetObject(hBitMap, sizeof(BITMAP), &bm);
        StretchBlt(hdc, 0, 0, client_width, client_height, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        DeleteObject(hBitMap);
            // 释放内存DC
        DeleteDC(hdcMem);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
