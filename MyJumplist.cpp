#include "MyJumplist.h"

AutoHR hr;

// Jumplist ...

HRESULT Jumplist::MakeShellLinkFromTask(JumplistTask const & task, IShellLinkW **ppsl) const {
	try {
		IShellLinkW *psl = nullptr;
		hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&psl));
		PROPVARIANT pv;
		AutoReleasePV arpv(&pv);
		IPropertyStore *pps = nullptr;
		hr = psl->QueryInterface<IPropertyStore>(&pps);
		if (task.IsSeparator()) {
			hr = InitPropVariantFromBoolean(TRUE, &pv);
			hr = pps->SetValue(PKEY_AppUserModel_IsDestListSeparator, pv);
		}
		else {
			hr = psl->SetArguments(task.Args().c_str());
			hr = psl->SetDescription(task.Desc().c_str());
			hr = psl->SetHotkey(task.Hotkey());
			hr = psl->SetIconLocation(task.IconPath().c_str(), task.IconIndex());
			hr = psl->SetPath(task.Path().c_str());
			hr = psl->SetShowCmd(task.ShowCmd());
			hr = psl->SetWorkingDirectory(task.WorkingDir().c_str());
			hr = InitPropVariantFromString(task.Title().c_str(), &pv);
			hr = pps->SetValue(PKEY_Title, pv);
		}
		hr = pps->Commit();
		*ppsl = psl;
	}
	catch (AutoHR const & info) {
		return info.GetHr();
	}
	return S_OK;
}

Jumplist::Jumplist(std::wstring const &appId) : m_appId(appId) {
	hr = L"Jumplist init";
	// No need to check CoInit
	// It returns error if current thread had CoInit before, which is fine
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	hr += L"CoCreateInstance m_pcdl";
	hr = CoCreateInstance(
		CLSID_DestinationList, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pcdl));
	if (appId.length() > 0) {
		hr += L"m_pcdl->SetAppID";
		hr = m_pcdl->SetAppID(appId.c_str());
	}
}

Jumplist::~Jumplist() {
	m_pcdl->Release();
	CoUninitialize();
}

void Jumplist::Clear() {
	Commit(std::vector<JumplistGroup>());
}

void Jumplist::Commit(std::vector<JumplistTask> const & tasks) {
	std::vector<JumplistGroup> groups;
	groups.push_back(JumplistGroup()
		.SetType(JumplistGroup::Types::Tasks)
		.SetName(L"")
		.Tasks(tasks)
	);
	Commit(groups);
}

void Jumplist::Commit(std::vector<JumplistGroup> const & groups) {
	hr = L"Jumplist.Commit:";
	if (m_hadCommit) {
		m_hadCommit = false;
		hr += L"m_poc->DeleteList";
		hr = m_pcdl->DeleteList(m_appId.c_str());
	}
	IObjectArray *poaRemoved = nullptr;
	UINT cMinSlots;
	hr += L"m_pcdl->BeginList";
	hr = m_pcdl->BeginList(&cMinSlots, IID_PPV_ARGS(&poaRemoved));
	AutoRelease ar(poaRemoved);
	for (auto group : groups) {
		switch (group.GetType()) {
		case JumplistGroup::Types::Frequent:
			hr += L"m_pcdl->AppendKnown... KDC_FREQUENT";
			m_pcdl->AppendKnownCategory(KDC_FREQUENT);
			break;
		case JumplistGroup::Types::Recent:
			hr += L"m_pcdl->AppendKnown... KDC_RECENT";
			m_pcdl->AppendKnownCategory(KDC_RECENT);
			break;
		default:
			if (group.Tasks().size() > 0) {
				// Make poc from group.Tasks
				IObjectCollection *poc = nullptr;
				hr += L"CoCreateInstance poc";
				hr = CoCreateInstance(
					CLSID_EnumerableObjectCollection, nullptr, CLSCTX_INPROC, IID_PPV_ARGS(&poc));
				AutoRelease ar(poc);
				hr += L"tasks.forEach poc->AddObject";
				for (auto task : group.Tasks()) {
					IShellLinkW *psl = nullptr;
					hr = MakeShellLinkFromTask(task, &psl);
					AutoRelease ar2(psl);
					poc->AddObject(psl);
				}
				// Make poa from poc
				IObjectArray *poa = nullptr;
				hr += L"poc->Query... &poa";
				hr = poc->QueryInterface<IObjectArray>(&poa);
				AutoRelease ar3(poa);
				// Append the poa
				if (group.GetName().length() > 0) {
					hr += L"m_pcdl->AppendCat...";
					hr = m_pcdl->AppendCategory(group.GetName().c_str(), poa);
				}
				else {
					hr += L"m_pcdl->AddUserTasks";
					hr = m_pcdl->AddUserTasks(poa);
				}
			}
			break;
		}
	}
	hr += L"m_pcdl->CommitList";
	hr = m_pcdl->CommitList();
	m_hadCommit = true;
}

// ... Jumplist

// JumplistTask ...

bool JumplistTask::IsHidden() const {
	return m_isHidden;
}

JumplistTask & JumplistTask::IsHidden(bool val) {
	m_isHidden = val;
	return *this;
}

bool JumplistTask::IsSeparator() const {
	return m_isSeparator;
}

JumplistTask & JumplistTask::IsSeparator(bool val) {
	m_isSeparator = val;
	return *this;
}

std::wstring const & JumplistTask::Path() const {
	return m_path;
}

JumplistTask & JumplistTask::Path(std::wstring const & val) {
	m_path = val;
	return *this;
}

std::wstring const & JumplistTask::Args() const {
	return m_args;
}

JumplistTask & JumplistTask::Args(std::wstring const & val) {
	m_args = val;
	return *this;
}

std::wstring const & JumplistTask::IconPath() const {
	return m_iconPath;
}

JumplistTask & JumplistTask::IconPath(std::wstring const & val) {
	m_iconPath = val;
	return *this;
}

std::wstring const & JumplistTask::Title() const {
	return m_title;
}

JumplistTask & JumplistTask::Title(std::wstring const & val) {
	m_title = val;
	return *this;
}

std::wstring const & JumplistTask::Desc() const {
	return m_desc;
}

JumplistTask & JumplistTask::Desc(std::wstring const & val) {
	m_desc = val;
	return *this;
}

std::wstring const & JumplistTask::WorkingDir() const {
	return m_workingDir;
}

JumplistTask & JumplistTask::WorkingDir(std::wstring const & val) {
	m_workingDir = val;
	return *this;
}

int JumplistTask::IconIndex() const {
	return m_iconIndex;
}

JumplistTask & JumplistTask::IconIndex(int val) {
	m_iconIndex = val;
	return *this;
}

int JumplistTask::ShowCmd() const {
	return m_showCmd;
}

JumplistTask & JumplistTask::ShowCmd(int val) {
	m_showCmd = val;
	return *this;
}

WORD JumplistTask::Hotkey() const {
	return m_hotkey;
}

JumplistTask & JumplistTask::Hotkey(int val) {
	m_hotkey = val;
	return *this;
}

// ... JumplistTask

// JumplistGroup ...

bool JumplistGroup::IsHidden() const {
	return m_isHidden;
}

JumplistGroup & JumplistGroup::IsHidden(bool val) {
	m_isHidden = val;
	return *this;
}

JumplistGroup::Types JumplistGroup::GetType() const {
	return m_type;
}

JumplistGroup & JumplistGroup::SetType(JumplistGroup::Types val) {
	m_type = val;
	return *this;
}

std::wstring JumplistGroup::GetName() const {
	return m_name;
}

JumplistGroup & JumplistGroup::SetName(std::wstring val) {
	m_name = val;
	return *this;
}

std::vector<JumplistTask>& JumplistGroup::Tasks() {
	return m_tasks;
}

JumplistGroup & JumplistGroup::Tasks(std::vector<JumplistTask> tasks) {
	m_tasks = tasks;
	return *this;
}
