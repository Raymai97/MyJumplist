#include "MyJumplist_C.h"
#include "../AutoHR.h"
#include "../MyJumplist.h"
#include <stdio.h>

MYJUMPLIST_API HRESULT MyJumplist_SetTasks(LPCWSTR szAppId, MYJUMPLIST_TASK * pTasks, size_t nTasks) {
	try {
		Jumplist list(szAppId ? szAppId : L"");
		for (size_t i = 0; i < nTasks; i++) {
			if (pTasks[i].isHidden) { continue; }
			list.Tasks.Add(
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
		list.Tasks.Commit();
	}
	catch (AutoHR const &info) {
		return info.GetHr();
	}
	return 0;
}
