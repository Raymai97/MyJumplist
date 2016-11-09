#ifndef MYJUMPLIST_C_H
#define MYJUMPLIST_C_H

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#ifdef MYJUMPLIST_EXPORT
#define MYJUMPLIST_API EXTERN_C __declspec(dllexport)
#else
#define MYJUMPLIST_API EXTERN_C __declspec(dllimport)
#endif

#include <Windows.h>

typedef struct {
	BOOL isSeparator;
	LPCWSTR szPath, szArgs, szIconPath;
	LPCWSTR szTitle, szDesc, szWorkingDir;
	int iIconIndex, iShowCmd;
	WORD wHotkey;
} MYJUMPLIST_TASK;

MYJUMPLIST_API
HRESULT MyJumplist_Init(LPCWSTR szAppId);

MYJUMPLIST_API
void MyJumplist_Uninit();

MYJUMPLIST_API
HRESULT MyJumplist_SetTasks(MYJUMPLIST_TASK *pTasks, size_t count);

#endif/*MYJUMPLIST_C_H*/
