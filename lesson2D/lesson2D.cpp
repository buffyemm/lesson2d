
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//
//}

#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")
#include<windows.h>
#include <iostream>
#include <vector>


enum class item_{

	Sword,
	Bow, 
	Axe,
	block

};


struct sprite {
	float x, y, width, height, speed;
	bool inJump = false;
};


struct {

	HWND hWnd;
	int width, height;
	//HDC context, device_context;

}window;

struct object_ {

	sprite pl;
	HBITMAP picture;
	item_ ID;
};


class ar {

public:

	sprite model;
	HBITMAP picture;
	bool activ;
	float directionX, directionY;

	/*~ar() {
		
		if (picture != NULL) {
			DeleteObject(picture);
			picture = NULL;
		}
	
	}*/

	ar(float x, float y) {

		model.x = x;
		model.y = y;
		model.speed = 20;
		model.width = 40;
		model.height = 40;
		picture = (HBITMAP)LoadImageW(NULL, L"ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		activ = false;
	}

	bool isOutOfScreen() {

		return (model.x + model.width > window.width ||
			model.x < 0 ||
			model.y < 0 ||
			model.y + model.height > window.height);
	}

};

//float dx, dy;
//POINT mouse;

struct Character {

sprite model;
std::vector<HBITMAP> anim[4];
std::vector<object_> item;
//item_ current_item = item_::Sword;
int currentFrame = 0;
bool activ = false;
int HP = 100;

int get_anim_index() {

	if (item.empty()) return 0;

	else return (int)item[0].ID + 1;

}


void HelpAnim() {

	//int count = hero.item;
	if (item.empty()) {

		currentFrame++;

		if (currentFrame >= anim[0].size()) currentFrame = 0;

		activ = false;

	}
	else {

		int index = get_anim_index();

		currentFrame++;

		if(currentFrame >= anim[index].size()) currentFrame = 0;

	}



	}

	void set_parameters(float x, float y, float width, float height, float speed, int hp) {

		model.x = x;
		model.y = y;
		model.width = width;
		model.height = height;
		model.speed = speed;
		HP = hp;

	}


	void collise(std::vector<object_> platform) {

		for (auto p : platform) {

			if (model.x + model.width >= p.pl.x &&
				model.x <= p.pl.x + p.pl.width &&
				model.y < p.pl.y + p.pl.height &&
				model.y + model.height > p.pl.y) {


				float UP = abs(p.pl.y - (model.y + model.height));
				float DOWN = abs((p.pl.y + p.pl.height) - model.y);

				//float RIGHT = ();
				int over_Y = min(UP, DOWN);

				float LEFT = abs(p.pl.x - (model.x + model.width));
				float RIGHT = abs(p.pl.x + p.pl.width - model.x);
				float over_X = min(LEFT, RIGHT);

				if (over_X < over_Y)
				{
					if (LEFT < RIGHT)
					{
						model.x = p.pl.x - model.width;
					}
					else
					{
						model.x = p.pl.x + p.pl.width;
					}
				}
				else {

					if (UP < DOWN)
					{
						model.y = p.pl.y - model.height;
						model.inJump = false;
					}
					else
					{
						model.y = p.pl.y + p.pl.height;
					}
				}




			}



		}

	}

};



std::vector<object_> platform;
std::vector<object_> item;

Character hero;
Character enemy;
std::vector<ar>arrow;


struct {

	POINT p;

	bool collise_mouse() {

		if (p.x >= hero.model.x &&
			p.x <= hero.model.x + hero.model.width &&
			p.y <= hero.model.y + hero.model.height &&
			p.y >= hero.model.y)
			return true;

		else
			return false;

	}


}mouse;


HBITMAP hBack = NULL;
HBITMAP test = NULL;

DWORD currentTime = timeGetTime();
DWORD LastTime = 0;



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


	hero.set_parameters(10, 10, 100, 150, 30, 100);

	

	enemy.set_parameters(window.width / 2, window.height - 150, 100, 150, 10, 150);

	enemy.anim[0].push_back((HBITMAP)LoadImageW(NULL, L"E0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	enemy.anim[0].push_back((HBITMAP)LoadImageW(NULL, L"E1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	enemy.anim[0].push_back((HBITMAP)LoadImageW(NULL, L"E2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));


	hero.anim[0].push_back((HBITMAP)LoadImageW(NULL, L"A0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	hero.anim[1].push_back((HBITMAP)LoadImageW(NULL, L"A1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[1].push_back((HBITMAP)LoadImageW(NULL, L"A2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[1].push_back((HBITMAP)LoadImageW(NULL, L"A3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[1].push_back((HBITMAP)LoadImageW(NULL, L"A4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[1].push_back((HBITMAP)LoadImageW(NULL, L"A5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	hero.anim[2].push_back((HBITMAP)LoadImageW(NULL, L"B1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[2].push_back((HBITMAP)LoadImageW(NULL, L"B2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[2].push_back((HBITMAP)LoadImageW(NULL, L"B3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[2].push_back((HBITMAP)LoadImageW(NULL, L"B4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[2].push_back((HBITMAP)LoadImageW(NULL, L"B5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	hero.anim[3].push_back((HBITMAP)LoadImageW(NULL, L"S1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[3].push_back((HBITMAP)LoadImageW(NULL, L"S2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[3].push_back((HBITMAP)LoadImageW(NULL, L"S3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[3].push_back((HBITMAP)LoadImageW(NULL, L"S4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hero.anim[3].push_back((HBITMAP)LoadImageW(NULL, L"S5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));


	item.push_back({ {100,  1300, 70, 70, 20}, (HBITMAP)LoadImageW(NULL, L"sword.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), item_::Sword });
	item.push_back({ {300,  1300, 70, 70, 30}, (HBITMAP)LoadImageW(NULL, L"axe.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), item_::Axe });
	item.push_back({ {500,  1300, 70, 70, 15}, (HBITMAP)LoadImageW(NULL, L"bow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), item_::Bow });


	platform.push_back({ {100,  700, 500, 70, 0}, (HBITMAP)LoadImageW(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), item_::block });
	platform.push_back({ {500,  1050, 500, 70, 0}, (HBITMAP)LoadImageW(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), item_::block });
	platform.push_back({ {1000,  300, 500, 70, 0}, (HBITMAP)LoadImageW(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), item_::block });

	
}


void Enemy_Fight() {

	if (enemy.model.x <= hero.model.x + hero.model.width && +
		enemy.model.x + enemy.model.width >= hero.model.x &&
		enemy.model.y <= hero.model.y + hero.model.height &&
		enemy.model.y + enemy.model.height >= hero.model.y &&
		hero.activ) {


		if (!hero.item.empty() && enemy.HP > 0) {

			enemy.HP -= hero.item[0].pl.speed;
			enemy.model.y -= 200;

		}

		if (enemy.HP <= 0) {

			enemy.set_parameters(rand() % window.width, window.height / 2, 100, 150, 10, 100);

		}


	}

}


void EnemyMove() {

	float dist = 500;


	float distToHero = abs(enemy.model.x - hero.model.x);


	if (distToHero < dist && enemy.model.y - hero.model.y < 200) {

		if (enemy.model.x < hero.model.x) { // right enemy

			enemy.HP > 20 ? enemy.model.x += 10 : enemy.model.x -=10;
				


		}
		else {

			enemy.HP > 20 ? enemy.model.x -= 10 : enemy.model.x += 10;

		}

		Enemy_Fight();
		

	}

	enemy.model.y += 30;
	enemy.model.y = min((enemy.model.y), (window.height - enemy.model.height));

}


void ProcesBall() {

	for (int i = 0; i < arrow.size(); i++) {

		if (arrow[i].activ) {
			arrow[i].model.x += arrow[i].directionX * arrow[i].model.speed;
			arrow[i].model.y += arrow[i].directionY * arrow[i].model.speed;
		}
	}
}

void ProcesImput() {

	

	static bool drop = false;
	static float jump = 0;

	float gravity = 30;
	if (GetAsyncKeyState('A')) hero.model.x -= hero.model.speed;
	if (GetAsyncKeyState('D')) hero.model.x += hero.model.speed;
	if (GetAsyncKeyState(VK_SPACE) && !hero.model.inJump) {

		drop = true;
		jump = 90;
		hero.model.inJump = true;

	}

	if (GetAsyncKeyState(VK_CONTROL)) {
		if (!hero.item.empty() && drop) {

			item.push_back(hero.item[0]);

			hero.item.erase(hero.item.begin());
			item[item.size() - 1].pl.x = hero.model.x + hero.model.width * 2;
			item[item.size() - 1].pl.y = hero.model.y;
			drop = false;
		}
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
	
	
		if (enemy.model.x <= window.width - window.width) enemy.model.x = 0;


		if (enemy.model.x >= window.width - enemy.model.width) enemy.model.x = window.width - enemy.model.width;

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
	
	static DWORD lasttime = 0;

	if (currentTime - lasttime > 500) {

		lasttime = currentTime;
		return true;
	}

	return false;
}




void ShowObject(HDC hMemDC) {

	for (auto pl : platform) {

	DrawBitmap(hMemDC,pl.pl.x, pl.pl.y, pl.pl.width, pl.pl.height, pl.picture, false);

	}

	if (hero.item.empty()) {

		if(!hero.anim[hero.get_anim_index()].empty() && hero.currentFrame < hero.anim[hero.get_anim_index()].size())

		DrawBitmap(hMemDC, hero.model.x, hero.model.y, hero.model.width, hero.model.height, hero.anim[hero.get_anim_index()][hero.currentFrame], true);

	}
	else {

		if (hero.get_anim_index() < 4 && !hero.anim[hero.get_anim_index()].empty() && hero.currentFrame < hero.anim[hero.get_anim_index()].size()) {

			DrawBitmap(hMemDC, hero.model.x, hero.model.y, hero.model.width, hero.model.height, hero.anim[hero.get_anim_index()][hero.currentFrame], true);

		}
	}

	if (!enemy.anim[0].empty() && enemy.currentFrame < enemy.anim[0].size()) {

		DrawBitmap(hMemDC, enemy.model.x, enemy.model.y, enemy.model.width, enemy.model.height, enemy.anim[0][enemy.currentFrame], true);

	}

	for (auto stuff : item) {

	DrawBitmap(hMemDC, stuff.pl.x, stuff.pl.y, stuff.pl.width, stuff.pl.height, stuff.picture, true);

	}

		int dist = 70;
	for (auto i : hero.item) {
		DrawBitmap(hMemDC, 100 + dist, 100, i.pl.width, i.pl.height, i.picture, true);
		dist += dist;
	}

	for (auto a : arrow) {

		if (a.activ)

			DrawBitmap(hMemDC, a.model.x, a.model.y, a.model.width, a.model.height, a.picture, true);

	}
	
	
}

void Colise_item() {

	for (int i = 0; i < item.size(); i++) {

		
		if (hero.model.x + hero.model.width >= item[i].pl.x &&
			hero.model.x <= item[i].pl.x + item[i].pl.width &&
			hero.model.y < item[i].pl.y + item[i].pl.height &&
			hero.model.y + hero.model.height > item[i].pl.y) {

			
			hero.item.push_back(item[i]);
			item.erase(item.cbegin() + i);
			hero.activ = true;

			break;



		}


		
	}

}

//bool DrawLine(HDC ) {
//
//}


void Mouse_Action() {


	arrow.push_back(ar(hero.model.x + hero.model.width, hero.model.y + hero.model.height / 2));
	/*arrow.model.x = hero.model.x + hero.model.width;
	arrow.model.y = hero.model.y + hero.model.height / 2;*/

	// Вычисляем вектор направления (НЕ абсолютные значения!)
	int targetX = mouse.p.x;
	int targetY = mouse.p.y;

	for (int i = 0; i < arrow.size(); i++) {
		if (!arrow[i].activ) {

			// Вычисляем разницы
			int diffX = targetX - arrow[i].model.x;
			int diffY = targetY - arrow[i].model.y;

			// Вычисляем длину вектора (расстояние)
			float distance = sqrt(diffX * diffX + diffY * diffY);

			// Нормализуем вектор (делаем длину = 1)
			if (distance > 0) {
				arrow[i].directionX = diffX / distance;
				arrow[i].directionY = diffY / distance;
			}
			else {
				arrow[i].directionX = 0;
				arrow[i].directionY = 0;
			}

			arrow[i].activ = true;
		}
	}


}


void Clean_arrows() {

	for (int i = arrow.size() - 1; i >= 0; i--) {

		if (arrow[i].isOutOfScreen()) {

			if (arrow[i].picture != NULL) {
				DeleteObject(arrow[i].picture);
				arrow[i].picture = NULL;
			}

			arrow.erase(arrow.begin() + i);
		}

	}

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hI, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	//InitGame();
	srand(time(NULL));

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
	SetTimer(window.hWnd, 2, 140, NULL);


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





		hBack = (HBITMAP)LoadImageW(NULL, L"les.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		//InitGame();

		//if (!hBack || !test) MessageBoxW(hwnd, L"Не удалось!", L"ОШИБКА", MB_ICONERROR);
		break;
	}

	case WM_TIMER:

		if (wParam == 1) {

			GetCursorPos(&mouse.p);
			InvalidateRect(hwnd, NULL, FALSE);
			Proces_room();
			hero.collise(platform);
			enemy.collise(platform);
			Colise_item();
			EnemyMove();
			ProcesImput();
			
			ProcesBall();

			


		}
		if (wParam == 2) {

			
			hero.HelpAnim();
			enemy.HelpAnim();
		
		}

		if (wParam == 3) {

			KillTimer(hwnd, 3);
			Clean_arrows();
			Mouse_Action();
		}

		break;


	case WM_LBUTTONDOWN:
	{
		KillTimer(hwnd, 3);
		SetTimer(hwnd, 3, 300, NULL);

		break;
	}

	case WM_SIZE:
		InitWindow();
		break;

	case WM_DESTROY: {

		PostQuitMessage(0);
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);

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

		ShowObject(hMemDC);


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
