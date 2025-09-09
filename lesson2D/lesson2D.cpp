
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//
//}

#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")
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

struct {

sprite model;
std::vector<HBITMAP> anim;
int currentFrame = anim.size() - anim.size();
int lastFrame = 0;
}hero;


std::vector<Platform> platform;
std::vector<Platform> item;

sprite enemy;
HBITMAP hBack = NULL;
HBITMAP test = NULL;

DWORD currentTime = timeGetTime();
DWORD LastTime = 0;


//void ShowBit(HWND hw, int x, int y, int width, int height, HBITMAP name) {
//
//	if (!name)return;
//
//	HDC hdc = GetDC(hw);
//	HDC hMemDC = CreateCompatibleDC(hdc); //создаю окно в памяти
//
//	HBITMAP temp = (HBITMAP)SelectObject(hMemDC, name); // помещаем картинку во временный сегмент
//	BITMAP bmp;
//	GetObject(name, sizeof(BITMAP), &bmp); // узнаем размер изображения
//
//	
//
//	StretchBlt(hdc, x, y, width, height, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
//	
//
//	SelectObject(hMemDC, temp);
//	DeleteDC(hMemDC);
//	ReleaseDC(hw, hdc);
//}



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

	hero.model.x = 10;
	hero.model.y = 10;
	hero.model.height = 148;
	hero.model.width = 110;
	hero.model.speed = 30;
	enemy.width = 30;
	enemy.height = 30;
	enemy.speed = 30;
	enemy.x = window.width / 2;
	enemy.y = window.height - enemy.height;

	hero.anim.push_back((HBITMAP)LoadImageW(NULL, L"hero.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim.push_back((HBITMAP)LoadImageW(NULL, L"h.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim.push_back((HBITMAP)LoadImageW(NULL, L"h1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));



	item.push_back({ {100,  1300, 70, 70, 0}, (HBITMAP)LoadImageW(NULL, L"sword.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) });
	platform.push_back({ {100,  700, 500, 70, 0}, (HBITMAP)LoadImageW(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) });
}

void EnemyMove() {

	float dist = 500;


	float distToHero = abs(enemy.x - hero.model.x);


	if (distToHero < dist) {

		if (enemy.x < hero.model.x) { // right enemy

			enemy.x += 10;
		}
		else {

			enemy.x -= 10;

		}
	}

}



	float jump = 0;

void ProcesImput() {

	

	float gravity = 30;
	if (GetAsyncKeyState(VK_LEFT)) hero.model.x -= hero.model.speed;
	if (GetAsyncKeyState(VK_RIGHT)) hero.model.x += hero.model.speed;
	if (GetAsyncKeyState(VK_SPACE) && !hero.model.inJump) {

		jump = 90;
		hero.model.inJump = true;

	}

	hero.model.y += gravity - jump;
	jump *= .9;
	hero.model.y = min((hero.model.y), (window.height - hero.model.height));

	if(hero.model.y + hero.model.height >= window.height)
	hero.model.inJump = false;


}


void Proces_room() {

	if (hero.model.x <= window.width - window.width)
		hero.model.x = 0;

	if (hero.model.x >= window.width - hero.model.width)
		hero.model.x = window.width - hero.model.width;
	
		


}

void collise() {

	for (auto p : platform) {

		if (hero.model.x + hero.model.width >= p.pl.x && hero.model.x <= p.pl.x + p.pl.width &&
			hero.model.y < p.pl.y + p.pl.height && hero.model.y + hero.model.height > p.pl.y) {


			float UP = abs(p.pl.y - (hero.model.y + hero.model.height));
			float DOWN = abs((p.pl.y + p.pl.height) - hero.model.y);

			//float RIGHT = ();
			int over_Y = min(UP, DOWN);

			float LEFT = abs(p.pl.x - (hero.model.x + hero.model.width));
			float RIGHT = abs(p.pl.x + p.pl.width - hero.model.x);
			float over_X = min(LEFT, RIGHT);

			if (over_X < over_Y)
			{
				if (LEFT < RIGHT)
				{
					hero.model.x = p.pl.x - hero.model.width;
				}
				else
				{
					hero.model.x = p.pl.x + p.pl.width;
				}
			}
			else {

				if (UP < DOWN)
				{
					hero.model.y = p.pl.y - hero.model.height;
					hero.model.inJump = false;
				}
				else
				{
					hero.model.y = p.pl.y + p.pl.height;
				}
			}




		}



	}

}
		

auto DrawBitmap = [](HDC hdcDest, int x, int y, int w, int h, HBITMAP hBmp, bool transparent) {
	if (!hBmp) return;
	HDC hMemDC = CreateCompatibleDC(hdcDest);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);
	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	if (transparent) {

		TransparentBlt(hdcDest, x, y, w, h, hMemDC, 0, 0, w, h, RGB(0, 0, 0));
	}
	else {

	StretchBlt(hdcDest, x, y, w, h, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}
	SelectObject(hMemDC, hOldBmp);
	DeleteDC(hMemDC);
	};


bool AnimTime() {

	//DWORD t = timeGetTime();
	
	static DWORD lasttime;

	if (currentTime - lasttime > 500) {

		LastTime = currentTime;
		return true;
	}

	return false;
}

//void Colise_item() {
//
//	for (int i = 0; i < item.size();i++) {
//
//		auto plat = item[i].pl;
//
//		if (hero.x + hero.width > plat.x &&
//			hero.x < plat.x + plat.width) {
//
//			item.emplace_back(item.cbegin() + i);
//
//		}
//	}
//
//}


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


		test = (HBITMAP)LoadImageW(NULL, L"h.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

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
		EnemyMove();
		//Colise_item();

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

	case WM_PAINT: {

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// 1. Создаём буфер в памяти
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hMemBmp = CreateCompatibleBitmap(hdc, window.width, window.height);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hMemBmp);

		// 2. Рисуем ВСЁ в буфер
		// --- Фон ---
		if (hBack) {
			HDC hBackDC = CreateCompatibleDC(hMemDC);
			HBITMAP hOldBackBmp = (HBITMAP)SelectObject(hBackDC, hBack);
			BITMAP bmp;
			GetObject(hBack, sizeof(BITMAP), &bmp);
			StretchBlt(hMemDC, 0, 0, window.width, window.height, hBackDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			SelectObject(hBackDC, hOldBackBmp);
			DeleteDC(hBackDC);
		}

		// --- Платформа и герой ---

		DrawBitmap(hMemDC, platform[0].pl.x, platform[0].pl.y, platform[0].pl.width, platform[0].pl.height, platform[0].picture, false);

		if (!hero.anim.empty() && hero.currentFrame < hero.anim.size()) {
			DrawBitmap(hMemDC, hero.model.x, hero.model.y, hero.model.width, hero.model.height,
				hero.anim[hero.currentFrame], false);
		}

		// Обновляем кадр анимации по таймеру
		if (AnimTime()) {
			hero.currentFrame++;
			if (hero.currentFrame >= hero.anim.size()) {
				hero.currentFrame = 0; // Зацикливаем анимацию
			}
		}

		//}
		DrawBitmap(hMemDC, enemy.x, enemy.y, enemy.width, enemy.height, hBack, false);
		DrawBitmap(hMemDC, item[0].pl.x, item[0].pl.y, item[0].pl.width, item[0].pl.height, item[0].picture, true);


		// 3. Копируем готовый буфер на экран
		BitBlt(hdc, 0, 0, window.width, window.height, hMemDC, 0, 0, SRCCOPY);

		// 4. Очистка
		SelectObject(hMemDC, hOldBmp);
		DeleteObject(hMemBmp);
		DeleteDC(hMemDC);
		EndPaint(hwnd, &ps);
		break;
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
