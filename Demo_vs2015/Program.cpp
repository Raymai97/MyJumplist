#include <Windows.h>
#include <WindowsX.h>
#include <wchar.h>
#include <string>
#include <sstream>
#include <vector>
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
std::vector<JumplistTask> tasks;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnCommand(HWND, int, HWND, UINT);
void OnActivate(HWND, UINT, HWND, BOOL);

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

	HWND hwnd = CreateWindowExW(0,
		CLASS_NAME,
		WND_TITLE,
		WND_STYLE,
		CW_USEDEFAULT, 0,
		600, 400,
		nullptr, nullptr,
		nullptr, nullptr);
	if (!hwnd) { return 2; }

	CreateWindowExW(0,
		L"BUTTON", L"Update Jumplist",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP,
		20, 10, 200, 32,
		hwnd, (HMENU)BTN_UPDATE_JUMPLIST,
		nullptr, nullptr);

	CreateWindowExW(0,
		L"BUTTON", L"Clear Jumplist",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP,
		20, 50, 200, 32,
		hwnd, (HMENU)BTN_CLEAR_JUMPLIST,
		nullptr, nullptr);

	CreateWindowExW(0,
		L"BUTTON", L"Append Jumplist",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP,
		20, 90, 200, 32,
		hwnd, (HMENU)BTN_APPEND_JUMPLIST,
		nullptr, nullptr);

	ShowWindow(hwnd, nCmdShow);
	MSG msg = { 0 };
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		if (!IsDialogMessage(hwnd, &msg)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_ACTIVATE:
		HANDLE_WM_ACTIVATE(hwnd, w, l, OnActivate);
		break;
	case WM_COMMAND:
		HANDLE_WM_COMMAND(hwnd, w, l, OnCommand);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hwnd, msg, w, l);
	}
	return 0;
}

void OnCommand(HWND hwnd, int id, HWND, UINT) {
	try {
		if (id == BTN_UPDATE_JUMPLIST) {
			tasks.clear();
			tasks.push_back(
				JumplistTask()
				.Title(L"Command Prompt")
				.Desc(L"Open command prompt")
				.IconPath(L"C:\\Windows\\System32\\cmd.exe")
				.Path(L"C:\\Windows\\System32\\cmd.exe")
				.Args(L"/k echo Opened by JumplistDemo")
			);
			tasks.push_back(
				JumplistTask().IsSeparator(true)
			);
			tasks.push_back(
				JumplistTask()
				.Title(L"About Windows")
				.Path(L"C:\\Windows\\System32\\winver.exe")
				.IconPath(L"C:\\Windows\\explorer.exe")
			);
		}
		else if (id == BTN_CLEAR_JUMPLIST) {
			tasks.clear();
		}
		else if (id == BTN_APPEND_JUMPLIST) {
			tasks.push_back(
				JumplistTask()
				.Title(L"Append item test")
			);
		}
		jumplist.Commit(tasks);
		MessageBoxW(hwnd, L"Succeeded! Check the Jumplist now!", L"OK", MB_ICONINFORMATION);
	}
	catch (AutoHR const &info) {
		std::wstringstream ss;
		ss << L"Error occured at:" << std::endl;
		ss << info.GetTaskName() << std::endl << std::endl;
		ss << L"HRESULT: ";
		ss << std::hex << std::uppercase << std::showbase << info.GetHr();
		MessageBoxW(hwnd, ss.str().c_str(), L"Error!", MB_ICONERROR);
	}
}

void OnActivate(HWND hwnd, UINT state, HWND, BOOL) {
	static HWND hwndFocus{};
	if (state == WA_INACTIVE) { hwndFocus = GetFocus(); }
	else { SetFocus(hwndFocus); }
}
