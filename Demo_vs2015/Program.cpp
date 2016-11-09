#include <Windows.h>
#include <WindowsX.h>
#include <wchar.h>
#include <string>
#include <sstream>
#include "..\MyJumplist.h"
#include "..\AutoHR.h"
#include "..\AutoRelease.h"

wchar_t const *	CLASS_NAME = L"JumplistDemo Win32";
wchar_t const *	WND_TITLE = CLASS_NAME;
DWORD const		WND_STYLE = (WS_OVERLAPPEDWINDOW);
wchar_t const * JUMPLIST_APP_ID = L"MaiSoft.JumplistDemo";

enum MYID {
	MYID_FIRST = WM_USER,
	BTN_UPDATE_JUMPLIST,
	BTN_CLEAR_JUMPLIST,
	BTN_APPEND_JUMPLIST
};

Jumplist jumplist(JUMPLIST_APP_ID);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnCommand(HWND, int, HWND, UINT);

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int nCmdShow) {
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassExW(&wcex)) { return 1; }
	SetCurrentProcessExplicitAppUserModelID(JUMPLIST_APP_ID);

	HWND hWnd = CreateWindowExW(0,
		CLASS_NAME,
		WND_TITLE,
		WND_STYLE,
		CW_USEDEFAULT, 0,
		600, 400,
		nullptr, nullptr,
		nullptr, nullptr);
	if (!hWnd) { return 2; }

	CreateWindowExW(0,
		L"BUTTON", L"Update Jumplist",
		WS_VISIBLE | WS_CHILD,
		20, 10, 200, 32,
		hWnd, (HMENU)BTN_UPDATE_JUMPLIST,
		nullptr, nullptr);

	CreateWindowExW(0,
		L"BUTTON", L"Clear Jumplist",
		WS_VISIBLE | WS_CHILD,
		20, 50, 200, 32,
		hWnd, (HMENU)BTN_CLEAR_JUMPLIST,
		nullptr, nullptr);

	CreateWindowExW(0,
		L"BUTTON", L"Append Jumplist",
		WS_VISIBLE | WS_CHILD,
		20, 90, 200, 32,
		hWnd, (HMENU)BTN_APPEND_JUMPLIST,
		nullptr, nullptr);

	ShowWindow(hWnd, nCmdShow);
	MSG msg = { 0 };
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_COMMAND:
		HANDLE_WM_COMMAND(hWnd, w, l, OnCommand);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, w, l);
	}
	return 0;
}

void OnCommand(HWND hWnd, int id, HWND, UINT) {
	try {
		if (id == BTN_UPDATE_JUMPLIST) {
			jumplist.Tasks.Clear();
			jumplist.Tasks.Add(
				JumplistTask()
				.SetTitle(L"Command Prompt")
				.SetDescription(L"Open command prompt")
				.SetPath(L"C:\\Windows\\System32\\cmd.exe")
				.SetIconLocation(L"C:\\Windows\\System32\\cmd.exe", 0)
				.SetArguments(L"/k echo Opened by JumplistDemo")
			);
			jumplist.Tasks.Add(
				JumplistTask().IsSeparator(true)
			);
			jumplist.Tasks.Add(
				JumplistTask()
				.SetTitle(L"About Windows")
				.SetPath(L"C:\\Windows\\System32\\winver.exe")
				.SetIconLocation(L"C:\\Windows\\explorer.exe", 0)
			);
			jumplist.Tasks.Commit();
		}
		else if (id == BTN_CLEAR_JUMPLIST) {
			jumplist.Tasks.Clear();
			jumplist.Tasks.Commit();
		}
		else if (id == BTN_APPEND_JUMPLIST) {
			jumplist.Tasks.Add(
				JumplistTask()
				.SetTitle(L"Append item test")
			);
			jumplist.Tasks.Commit();
		}
		MessageBoxW(hWnd, L"Succeeded! Check the Jumplist now!", L"OK", MB_ICONINFORMATION);
	}
	catch (AutoHR const &info) {
		std::wstringstream ss;
		ss << L"Error occured at:" << std::endl;
		ss << info.GetTaskName() << std::endl << std::endl;
		ss << L"HRESULT: ";
		ss << std::hex << std::uppercase << std::showbase << info.GetHr();
		MessageBoxW(hWnd, ss.str().c_str(), L"Error!", MB_ICONERROR);
	}
}
