﻿
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//
//}

#include<windows.h>
#include <iostream>
#include <vector>

struct sprite {
	float x, y, width, height, speed;
	bool inJump = false;
};


struct {

	HWND hWnd;
	int width, height;
	//HDC context, device_context;

}window;

struct Platform {

	sprite pl;
	HBITMAP picture;

};


std::vector<Platform> platform;
sprite hero;
HBITMAP hBack = NULL;
HBITMAP test = NULL;


void ShowBit(HWND hw, int x, int y, int width, int height, HBITMAP name) {

	if (!name)return;

	HDC hdc = GetDC(hw);
	HDC hMemDC = CreateCompatibleDC(hdc); //создаю окно в памяти

	HBITMAP temp = (HBITMAP)SelectObject(hMemDC, name); // помещаем картинку во временный сегмент
	BITMAP bmp;
	GetObject(name, sizeof(BITMAP), &bmp); // узнаем размер изображения

	

	StretchBlt(hdc, x, y, width, height, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	

	SelectObject(hMemDC, temp);
	DeleteDC(hMemDC);
	ReleaseDC(hw, hdc);
}



void InitWindow() {

	RECT r;
	GetClientRect(window.hWnd, &r);
	window.width = r.right - r.left;
	window.height = r.bottom - r.top;


}

void Cleanup() {

	if (platform[0].picture) {
		DeleteObject(platform[0].picture);
		platform[0].picture = NULL;
	}

	if (hBack) {
		DeleteObject(hBack);
		hBack = NULL;
	}
	if (test) {
		DeleteObject(test);
		test = NULL;
	}
}

void InitGame() {

	//hBack = (HBITMAP)LoadImageW(NULL, L"les.pmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	hero.x = 0;
	hero.y = window.height / 2;
	hero.height = 50;
	hero.width = 50;
	hero.speed = 30;
	platform.push_back({ {500,  1200, 600, 100, 0}, (HBITMAP)LoadImageW(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) });
}

void EnemyMove() {
	bool temp = true;

	if (hero.x + hero.width >= (window.width - hero.width) && temp ) {

		hero.x -= hero.speed;
		temp = false;
	}
	else {

	hero.x += hero.speed;
	}


}



	float jump = 0;

void ProcesImput() {

	

	float gravity = 30;
	if (GetAsyncKeyState(VK_LEFT)) hero.x -= hero.speed;
	if (GetAsyncKeyState(VK_RIGHT)) hero.x += hero.speed;
	if (GetAsyncKeyState(VK_SPACE) && !hero.inJump) {

		jump = 90;
		hero.inJump = true;

	}

	hero.y += gravity - jump;
	jump *= .9;
	hero.y = min((hero.y), (window.height - hero.height));

	if(hero.y + hero.height >= window.height)
	hero.inJump = false;


}


void Proces_room() {

	if (hero.x <= window.width - window.width)
		hero.x = 0;

	if (hero.x >= window.width - hero.width)
		hero.x = window.width - hero.width;
	
		
		
		//если коодината левого угла ракетки меньше нуля, присвоим ей ноль
	//аналогично для правого угла


}

void collise() {
	for (auto p : platform) {

		if (hero.x + hero.width >= p.pl.x && hero.x <= p.pl.x + p.pl.width &&
			hero.y < p.pl.y + p.pl.height && hero.y + hero.height > p.pl.y) {


			float UP = abs(p.pl.y - (hero.y + hero.height));
			float DOWN = abs((p.pl.y + p.pl.height) - hero.y);

			//float RIGHT = ();
			int over_Y = min(UP, DOWN);

			float LEFT = abs(p.pl.x - (hero.x + hero.width));
			float RIGHT = abs(p.pl.x + p.pl.width - hero.x);
			float over_X = min(LEFT, RIGHT);

			if (over_X < over_Y)
			{
				if (LEFT < RIGHT)
				{
					hero.x = p.pl.x - hero.width;
				}
				else
				{
					hero.x = p.pl.x + p.pl.width;
				}
			}
			else {

				if (UP < DOWN)
				{
					hero.y = p.pl.y - hero.height;
					//isJumping = false;
				}
				else
				{
					hero.y = p.pl.y + p.pl.height;
				}
			}




		}



	}

}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hI, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	//InitGame();


	const wchar_t CLASS_NAME[] = L"KAMEN";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hI;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);
	

	 window.hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"JOB IS DONE",
		 WS_POPUP
		/*WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED*/,
		CW_USEDEFAULT, CW_USEDEFAULT, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hI,
		NULL
	);


	if (window.hWnd == NULL) return 0;

	InitWindow();
	InitGame();

	ShowWindow(window.hWnd, nCmdShow);

	SetTimer(window.hWnd, 1, 16, NULL);

	MSG msg = { };

	while (GetMessage(&msg, NULL, 0, 0) > 0) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Cleanup();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {


	switch (uMsg) {


	case WM_CREATE: {


		test = (HBITMAP)LoadImageW(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		hBack = (HBITMAP)LoadImageW(NULL, L"les.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		//InitGame();

		if (!hBack || !test) MessageBoxW(hwnd, L"Не удалось!", L"ОШИБКА", MB_ICONERROR);
		break;
	}

	case WM_TIMER:
		
		InvalidateRect(hwnd, NULL, FALSE);
		ProcesImput();
		Proces_room();
		collise();
		//EnemyMove();


	case WM_LBUTTONDOWN:

		if (test) {

		}


		break;

	case WM_SIZE:
		InitWindow();
		break;

	case WM_DESTROY: {

		PostQuitMessage(0);
		KillTimer(hwnd, 1);
		return 0;
	}

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {

			/*if (hBack) {

				DeleteObject(hBack);
				hBack = NULL;
			}*/

			DestroyWindow(hwnd);
			
		}
		//PostQuitMessage(0);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		if (hBack) {


			HDC hMemDC = CreateCompatibleDC(hdc);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBack);

			BITMAP bmp;
			GetObject(hBack, sizeof(BITMAP), &bmp);

			//BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0, 0, window.width, window.height, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

			SelectObject(hMemDC, hOldBitmap);

			DeleteDC(hMemDC);

		}
		else {

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW +1));

		}
			
			ShowBit(hwnd, platform[0].pl.x, platform[0].pl.y, platform[0].pl.width, platform[0].pl.height, platform[0].picture);
			ShowBit(hwnd,hero.x, hero.y, hero.width, hero.height, test);

		EndPaint(hwnd, &ps);
		return 0;

	}

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}











//#include <windows.h>
//#include <iostream>
//
//struct {
//	HWND hWnd;
//	int width, height;
//} window;
//
//HBITMAP hBack = NULL;
//HBITMAP test = NULL;
//
//void ShowBit(HWND hw, int x, int y, int width, int height, HBITMAP name) {
//
//	if (!name) return; // Проверка на валидность битмапа
//
//	HDC hdc = GetDC(hw);
//	HDC hMemDC = CreateCompatibleDC(hdc);
//
//	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, name);
//	BITMAP bmp;
//	GetObject(name, sizeof(BITMAP), &bmp);
//
//	StretchBlt(hdc, x, y, width, height, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
//
//	SelectObject(hMemDC, hOldBmp);
//	DeleteDC(hMemDC);
//	ReleaseDC(hw, hdc);
//}
//
//void InitWindow() {
//	RECT r;
//	GetClientRect(window.hWnd, &r);
//	window.width = r.right - r.left;
//	window.height = r.bottom - r.top;
//}
//
//void Cleanup() {
//	if (hBack) DeleteObject(hBack);
//	if (test) DeleteObject(test);
//}
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
//int WINAPI wWinMain(HINSTANCE hI, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//	const wchar_t CLASS_NAME[] = L"KAMEN";
//
//	WNDCLASS wc = { };
//	wc.lpfnWndProc = WindowProc;
//	wc.hInstance = hI;
//	wc.lpszClassName = CLASS_NAME;
//	RegisterClass(&wc);
//
//	window.hWnd = CreateWindowEx(
//		0,
//		CLASS_NAME,
//		L"JOB IS DONE",
//		WS_POPUP,
//		CW_USEDEFAULT, CW_USEDEFAULT,
//		GetSystemMetrics(SM_CXSCREEN),
//		GetSystemMetrics(SM_CYSCREEN),
//		NULL,
//		NULL,
//		hI,
//		NULL
//	);
//
//	if (!window.hWnd) return 0;
//
//	InitWindow();
//	ShowWindow(window.hWnd, nCmdShow);
//
//	MSG msg = { };
//	while (GetMessage(&msg, NULL, 0, 0) > 0) {
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//
//	Cleanup();
//	return 0;
//}
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	switch (uMsg) {
//	case WM_CREATE: {
//		test = (HBITMAP)LoadImageW(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//		hBack = (HBITMAP)LoadImageW(NULL, L"les.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//
//		if (!hBack || !test) {
//			MessageBoxW(hwnd, L"Ошибка загрузки изображений!", L"Ошибка", MB_ICONERROR);
//		}
//		break;
//	}
//
//	case WM_LBUTTONDOWN:
//		if (test) {
//			ShowBit(hwnd, 50, 50, 100, 100, test);
//		}
//		break;
//
//	case WM_SIZE:
//		InitWindow();
//		break;
//
//	case WM_KEYDOWN:
//		if (wParam == VK_ESCAPE) {
//			DestroyWindow(hwnd);
//		}
//		break;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//
//	case WM_PAINT: {
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hwnd, &ps);
//
//		if (hBack) {
//			HDC hMemDC = CreateCompatibleDC(hdc);
//			HBITMAP hOld = (HBITMAP)SelectObject(hMemDC, hBack);
//
//			BITMAP bmp;
//			GetObject(hBack, sizeof(BITMAP), &bmp);
//
//			StretchBlt(hdc, 0, 0, window.width, window.height,
//				hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
//
//			SelectObject(hMemDC, hOld);
//			DeleteDC(hMemDC);
//		}
//		else {
//			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
//		}
//
//		EndPaint(hwnd, &ps);
//		return 0;
//	}
//	}
//
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}
//




//// lesson2D.cpp : Defines the entry point for the application.
////
//
//#include "framework.h"
//#include "lesson2D.h"
//
//#define MAX_LOADSTRING 100
//
//// Global Variables:
//HINSTANCE hInst;                                // current instance
//WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
//WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//
//// Forward declarations of functions included in this code module:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: Place code here.
//
//    // Initialize global strings
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_LESSON2D, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // Perform application initialization:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LESSON2D));
//
//    MSG msg;
//
//    // Main message loop:
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//    return (int) msg.wParam;
//}
//
//
//
////
////  FUNCTION: MyRegisterClass()
////
////  PURPOSE: Registers the window class.
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEXW wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//
//    wcex.style          = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc    = WndProc;
//    wcex.cbClsExtra     = 0;
//    wcex.cbWndExtra     = 0;
//    wcex.hInstance      = hInstance;
//    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LESSON2D));
//    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
//    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LESSON2D);
//    wcex.lpszClassName  = szWindowClass;
//    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassExW(&wcex);
//}
//
////
////   FUNCTION: InitInstance(HINSTANCE, int)
////
////   PURPOSE: Saves instance handle and creates main window
////
////   COMMENTS:
////
////        In this function, we save the instance handle in a global variable and
////        create and display the main program window.
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   hInst = hInstance; // Store instance handle in our global variable
//
//   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  PURPOSE: Processes messages for the main window.
////
////  WM_COMMAND  - process the application menu
////  WM_PAINT    - Paint the main window
////  WM_DESTROY  - post a quit message and return
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_COMMAND:
//        {
//            int wmId = LOWORD(wParam);
//            // Parse the menu selections:
//            switch (wmId)
//            {
//            case IDM_ABOUT:
//                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//                break;
//            case IDM_EXIT:
//                DestroyWindow(hWnd);
//                break;
//            default:
//                return DefWindowProc(hWnd, message, wParam, lParam);
//            }
//        }
//        break;
//    case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hWnd, &ps);
//            // TODO: Add any drawing code that uses hdc here...
//            EndPaint(hWnd, &ps);
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//// Message handler for about box.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
