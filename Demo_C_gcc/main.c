#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "../C_DLL_vs2015/MyJumplist_C.h"

int main()
{
    HRESULT hr = 0;
    printf("Trying to add something into my Jumplist...\n");
    hr = MyJumplist_Init(NULL);
    if (SUCCEEDED(hr)) {
        MYJUMPLIST_TASK tasks[3] = {0};
        tasks[0].szTitle = L"Jumplist from pure C!";
        tasks[0].szPath = L"C:\\Windows\\System32\\cmd.exe";
        tasks[0].szArgs = L"/k echo This is how it should have done";
        tasks[0].szIconPath = L"C:\\Windows\\System32\\cmd.exe";
        tasks[0].iIconIndex = 0;
        tasks[1].isSeparator = TRUE;
        tasks[2].szTitle = L"Magnify!!";
        tasks[2].szPath = L"C:\\Windows\\System32\\Magnify.exe";
        tasks[2].szIconPath = L"C:\\Windows\\System32\\Magnify.exe";
        tasks[2].iIconIndex = 0;
        hr = MyJumplist_SetTasks(tasks, 3);
    }
    MyJumplist_Uninit();
    if (SUCCEEDED(hr)) {
        printf("Done...\n");
    }
    else {
        printf("HRESULT error: 0x%.8lX \n", hr);
    }
    _getch();
    return 0;
}
