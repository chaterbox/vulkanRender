#include <windows.h>
#include "vulkanRender.h"
#include "macro.h"
#include <corecrt_io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);

vulkanRender vkRender;
HMENU hMenu;
HINSTANCE hinst;
int main() {
	vkRender.initInstance();
	vkRender.createSurface(hinst);
	vkRender.pickPhysicalDevice();
	vkRender.CreateLogicalDevice();
	//vkRender.renderLoop();
	vkRender.VkCleanup();
	return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInst, LPSTR args, int ncmdshow)
{
	WNDCLASSW wc = {  };
	HWND hwnd = {};
	vkRender.initInstance();
	vkRender.createSurface(hInst);

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;
	
	if (!RegisterClassW(&wc))
		return -1;
	
	CreateWindowW(L"myWindowClass", L"jovk Engine", WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX, 100, 100, 800, 700, NULL, NULL, NULL, NULL);
	
	MSG msg = { 0 };

	while (GetMessage(&msg,NULL,NULL,NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	vkRender.renderLoop();

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			vkRender.VkCleanup();
			break;
		case FILE_MENU_NEW:
			break;
		case FILE_MENU_LOADMODEL:
			MessageBox(hWnd, "load model","load model", MB_OK);
			break;
		case HELP_MENU_ABOUT:
			MessageBox(hWnd,"     to contact me email \n chatbox8558@gmail.com", "help", MB_OK);
			break;
		}
		break;
	case WM_CREATE:
		AddMenus(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		vkRender.VkCleanup();
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hHelpMenu = CreateMenu();
	//creates the file menu
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "file");
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, "new file");
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_OPEN, "open file");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING,FILE_MENU_SAVE, "save file");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_LOADMODEL, "load model");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "exit");
	//creates the help menu
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, "help");
	AppendMenu(hHelpMenu, MF_STRING, HELP_MENU_ABOUT, "contact");
	SetMenu(hWnd, hMenu);
}

