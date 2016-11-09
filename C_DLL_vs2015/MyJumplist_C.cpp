#include "MyJumplist_C.h"
#include "../AutoHR.h"
#include "../MyJumplist.h"
#include <stdio.h>

Jumplist *pList;

MYJUMPLIST_API HRESULT MyJumplist_Init(LPCWSTR szAppId) {
	try {
		pList = new Jumplist(szAppId ? szAppId : L"");
	}
	catch (AutoHR const &info) {
		return info.GetHr();
	}
	return 0;
}

MYJUMPLIST_API void MyJumplist_Uninit() {
	if (pList) { delete pList; }
}

MYJUMPLIST_API HRESULT MyJumplist_SetTasks(MYJUMPLIST_TASK * pTasks, size_t count) {
	try {
		if (!pList) { return ERROR_NOT_READY; }
		pList->Tasks.Clear();
		for (size_t i = 0; i < count; i++) {
			pList->Tasks.Add(
				pTasks[i].isSeparator ?
				JumplistTask().IsSeparator(TRUE) :
				JumplistTask()
				.SetPath(pTasks[i].szPath)
				.SetArguments(pTasks[i].szArgs)
				.SetIconLocation(pTasks[i].szIconPath, pTasks[i].iIconIndex)
				.SetTitle(pTasks[i].szTitle)
				.SetDescription(pTasks[i].szDesc)
				.SetWorkingDirectory(pTasks[i].szWorkingDir)
				.SetHotkey(pTasks[i].wHotkey)
				.SetShowCmd(pTasks[i].iShowCmd)
			);
		}
		pList->Tasks.Commit();
	}
	catch (AutoHR const &info) {
		return info.GetHr();
	}
	return 0;
}
