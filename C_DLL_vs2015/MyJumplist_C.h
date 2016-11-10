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
HRESULT MyJumplist_SetTasks(LPCWSTR szAppId, MYJUMPLIST_TASK *pTasks, size_t nTasks);

#endif/*MYJUMPLIST_C_H*/
