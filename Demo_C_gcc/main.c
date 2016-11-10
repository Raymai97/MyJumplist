#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "../C_DLL_vs2015/MyJumplist_C.h"

/* Use NULL unless you have called this:
   SetCurrentProcessExplicitAppUserModelID() */
#define JUMPLIST_APPID	NULL

int main()
{
    HRESULT hr = 0;
	MYJUMPLIST_TASK tasks[3] = {0};
	tasks[0].szTitle = L"Jumplist from pure C!";
	tasks[0].szPath = L"C:\\Windows\\System32\\cmd.exe";
	tasks[0].szArgs = L"/k echo Now everyone can use Jumplist";
	tasks[0].szIconPath = L"C:\\Windows\\System32\\cmd.exe";
	tasks[0].iIconIndex = 0;
	tasks[1].isSeparator = TRUE;
	tasks[2].szTitle = L"Magnify!!";
	tasks[2].szPath = L"C:\\Windows\\System32\\Magnify.exe";
	tasks[2].szIconPath = L"C:\\Windows\\System32\\Magnify.exe";
	tasks[2].iIconIndex = 0;	
	printf("Trying to update Jumplist...\n");
	hr = MyJumplist_SetTasks(JUMPLIST_APPID, tasks, 3);
    if (SUCCEEDED(hr)) {
        printf("Succeeded! Press any key to clear Jumplist. \n");
		_getch();
		hr = MyJumplist_SetTasks(JUMPLIST_APPID, tasks, 0);
		if (SUCCEEDED(hr)) {
			printf("Operation successfully.");
		}
    }
    if (FAILED(hr)) {
        printf("HRESULT error: 0x%.8lX \n", hr);
    }
    _getch();
    return 0;
}
