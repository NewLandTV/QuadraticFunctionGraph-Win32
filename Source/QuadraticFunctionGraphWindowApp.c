#include <stdio.h>
#include <Windows.h>

#define GRAPH_WIDTH 800
#define GRAPH_HEIGHT 800
#define GRAPH_CENTER_X (GRAPH_WIDTH >> 1)
#define GRAPH_CENTER_Y (GRAPH_HEIGHT >> 1)
#define GRAPH_ZOOM_LEVEL 20
#define GRAPH_AXIS_THICKNESS 3

// Inputs
#define a 1
#define b 0
#define c 0
#define INPUT y = a * x * x + b * x + c

// Global variables
const char* className = "QuadraticFunctionGraphWindowApp";

// Functions
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

inline void MathMoveTo(HDC hdc, double x, double y)
{
	MoveToEx(hdc, (int)(x * GRAPH_ZOOM_LEVEL + GRAPH_CENTER_X), (int)-(y * GRAPH_ZOOM_LEVEL - GRAPH_CENTER_Y), NULL);
}

inline void MathLineTo(HDC hdc, double x, double y)
{
	LineTo(hdc, (int)(x * GRAPH_ZOOM_LEVEL + GRAPH_CENTER_X), (int)-(y * GRAPH_ZOOM_LEVEL - GRAPH_CENTER_Y));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int nCmdShow)
{
	// Window setup
	HWND hWnd;
	MSG message;
	WNDCLASSEXA wndClassEx;
	
	wndClassEx.hInstance = hInstance;
	wndClassEx.lpszClassName = className;
	wndClassEx.lpfnWndProc = WindowProcedure;
	wndClassEx.style = CS_DBLCLKS;
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	if (!RegisterClassEx(&wndClassEx))
	{
		return 0;
	}
	
	// Create and show window
	hWnd = CreateWindowEx(0, className, "Quadratic Function Graph", WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, GRAPH_WIDTH, GRAPH_HEIGHT, HWND_DESKTOP, NULL, hInstance, NULL);
	
	ShowWindow(hWnd, nCmdShow);
	
	// Message loop
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		
		DispatchMessage(&message);
	}
	
	return message.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static double x = -GRAPH_WIDTH;
	static double y;
	
	int i;
	
	HDC hdc;
	PAINTSTRUCT paintStruct;
	HPEN hPen;
	HPEN hOldPen;
	
	switch (message)
	{
	case WM_PAINT:
		INPUT;
		
		hdc = BeginPaint(hWnd, &paintStruct);
		
		// Draw X axis
		hPen = CreatePen(PS_SOLID, GRAPH_AXIS_THICKNESS, RGB(255, 0, 0));
		hOldPen = (HPEN)SelectObject(hdc, hPen);
		
		MoveToEx(hdc, 0, GRAPH_CENTER_Y, NULL);
		LineTo(hdc, GRAPH_WIDTH, GRAPH_CENTER_Y);
		
		for (i = 0; i < GRAPH_HEIGHT / GRAPH_ZOOM_LEVEL; i++)
		{
			MoveToEx(hdc, i * GRAPH_ZOOM_LEVEL, GRAPH_CENTER_Y - 2, NULL);
			LineTo(hdc, i * GRAPH_ZOOM_LEVEL, GRAPH_CENTER_Y + 3);
		}
		
		DeleteObject(SelectObject(hdc, hOldPen));
		
		// Draw Y axis
		hPen = CreatePen(PS_SOLID, GRAPH_AXIS_THICKNESS, RGB(0, 255, 0));
		hOldPen = (HPEN)SelectObject(hdc, hPen);
		
		MoveToEx(hdc, GRAPH_CENTER_X, 0, NULL);
		LineTo(hdc, GRAPH_CENTER_X, GRAPH_HEIGHT);
		
		for (i = 0; i < GRAPH_HEIGHT / GRAPH_ZOOM_LEVEL; i++)
		{
			MoveToEx(hdc, GRAPH_CENTER_X - 2, i * GRAPH_ZOOM_LEVEL, NULL);
			LineTo(hdc, GRAPH_CENTER_X + 3, i * GRAPH_ZOOM_LEVEL);
		}
		
		DeleteObject(SelectObject(hdc, hOldPen));
		
		// Draw graph
		hPen = CreatePen(PS_SOLID, GRAPH_AXIS_THICKNESS, RGB(0, 0, 0));
		hOldPen = (HPEN)SelectObject(hdc, hPen);
		
		MathMoveTo(hdc, x, y);
		
		for (x += 0.2; x <= GRAPH_WIDTH; x += 0.2)
		{
			INPUT;
			
			MathLineTo(hdc, x, y);
		}
		
		DeleteObject(SelectObject(hdc, hOldPen));
		EndPaint(hWnd, &paintStruct);
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	
	return 0;
}
