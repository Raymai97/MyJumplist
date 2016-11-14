// This class throws AutoHR on error and requires
// kernel32.lib, user32.lib, shell32.lib, ole32.lib, uuid.lib, shlwapi.lib, Propsys.lib

#pragma once
#include <vector>
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
class JumplistGroup;

class JumplistTask {
	bool m_isHidden, m_isSeparator;
	std::wstring m_path, m_args, m_iconPath;
	std::wstring m_title, m_desc, m_workingDir;
	int m_iconIndex, m_showCmd;
	WORD m_hotkey;

public:
	bool IsHidden() const;
	JumplistTask & IsHidden(bool);

	bool IsSeparator() const;
	JumplistTask & IsSeparator(bool);

	std::wstring const & Path() const;
	JumplistTask & Path(std::wstring const &);

	std::wstring const & Args() const;
	JumplistTask & Args(std::wstring const &);

	std::wstring const & IconPath() const;
	JumplistTask & IconPath(std::wstring const &);

	std::wstring const & Title() const;
	JumplistTask & Title(std::wstring const &);

	std::wstring const & Desc() const;
	JumplistTask & Desc(std::wstring const &);

	std::wstring const & WorkingDir() const;
	JumplistTask & WorkingDir(std::wstring const &);

	int IconIndex() const;
	JumplistTask & IconIndex(int);

	int ShowCmd() const;
	JumplistTask & ShowCmd(int);

	WORD Hotkey() const;
	JumplistTask & Hotkey(int);
};

class JumplistGroup {
public:
	enum class Types {
		Tasks = 0,
		Frequent = 1,
		Recent = 2
	};

private:
	bool m_isHidden{ false };
	Types m_type{ Types::Tasks };
	std::wstring m_name{ L"" };
	std::vector<JumplistTask> m_tasks;

public:
	bool IsHidden() const;
	JumplistGroup & IsHidden(bool);

	Types GetType() const;
	JumplistGroup & SetType(Types);

	std::wstring GetName() const;
	JumplistGroup & SetName(std::wstring);

	// Method 1: .Tasks.push_back(the_task);
	// Method 2: .Tasks(vector_of_tasks);
	// Note: Don't pass by ref, we want copy.

	std::vector<JumplistTask> & Tasks();
	JumplistGroup & Tasks(std::vector<JumplistTask> tasks);
};

class Jumplist {
	bool m_hadCommit{ false };
	std::wstring const &m_appId;
	ICustomDestinationList *m_pcdl{ nullptr };

	HRESULT MakeShellLinkFromTask(JumplistTask const &, IShellLinkW**) const;

public:
	Jumplist(std::wstring const &appId = L"");
	~Jumplist();

	void Clear();
	void Commit(std::vector<JumplistTask> const &);
	void Commit(std::vector<JumplistGroup> const &);

};
