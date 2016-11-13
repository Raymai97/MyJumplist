// This class throws AutoHR on error and requires
// kernel32.lib, user32.lib, shell32.lib, ole32.lib, uuid.lib, shlwapi.lib, Propsys.lib

#pragma once
#include <Windows.h>
#include <WindowsX.h>
#include <wchar.h>
#include <propkey.h>
#include <propvarutil.h>
#include <shlobj.h>

#include "AutoHR.h"
#include "AutoRelease.h"

class Jumplist;
class JumplistTask;

class JumplistTask {
	IShellLinkW *m_psl{ nullptr };

public:
	JumplistTask();
	JumplistTask(IShellLinkW*);
	~JumplistTask();

	void AddIntoObjectCollection(IObjectCollection*) const;

	void GetPath(LPWSTR, int, WIN32_FIND_DATAW *pfd, DWORD fFlags) const;
	JumplistTask & SetPath(LPCWSTR);

	void GetArguments(LPWSTR, int) const;
	JumplistTask & SetArguments(LPCWSTR);

	void GetIconLocation(LPWSTR, int, int *piIconIndex) const;
	JumplistTask & SetIconLocation(LPCWSTR, int iIconIndex);

	void GetTitle(LPWSTR, UINT) const;
	JumplistTask & SetTitle(LPCWSTR);

	void GetDescription(LPWSTR, int) const;
	JumplistTask & SetDescription(LPCWSTR);

	void GetWorkingDirectory(LPWSTR, int) const;
	JumplistTask & SetWorkingDirectory(LPCWSTR);

	void GetHotkey(WORD*) const;
	JumplistTask & SetHotkey(WORD);

	void GetShowCmd(int*) const;
	JumplistTask & SetShowCmd(int);

	void GetIDList(PIDLIST_ABSOLUTE*) const;
	JumplistTask & SetIDList(PCIDLIST_ABSOLUTE);

	BOOL IsSeparator() const;
	JumplistTask & IsSeparator(BOOL);
};

class Jumplist {
public:
	Jumplist(std::wstring const &appId = L"");
	~Jumplist();

	class Tasks {
		bool m_hadCommit{ false };
		std::wstring const &m_appId;
		ICustomDestinationList *m_pcdl{ nullptr };
		IObjectCollection *m_poc{ nullptr };
	public:
		Tasks(std::wstring const &appId);
		~Tasks();

		void Add(JumplistTask const &);
		void Clear();
		void Commit();
		void GetAt(UINT, JumplistTask*) const;
		UINT GetCount() const;
		void RemoveAt(UINT);
	} Tasks;

};
