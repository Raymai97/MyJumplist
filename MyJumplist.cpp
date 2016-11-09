#include "MyJumplist.h"

AutoHR hr;

// Jumplist ...

Jumplist::Jumplist(std::wstring const &appId) : Tasks(appId) {
}

Jumplist::~Jumplist() {
}

// Jumplist::Tasks ...

Jumplist::Tasks::Tasks(std::wstring const &appId) : m_appId(appId) {
	hr = L"Jumplist.Tasks init";
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
	hr += L"CoCreateInstance m_poc";
	hr = CoCreateInstance(
		CLSID_EnumerableObjectCollection, nullptr, CLSCTX_INPROC, IID_PPV_ARGS(&m_poc));
}

Jumplist::Tasks::~Tasks() {
	m_pcdl->Release();
	m_poc->Release();
	CoUninitialize();
}

void Jumplist::Tasks::Add(JumplistTask const & task) {
	task.AddIntoObjectCollection(m_poc);
}

void Jumplist::Tasks::Clear() {
	hr = L"Jumplist.Tasks.Clear";
	hr = m_poc->Clear();
}

void Jumplist::Tasks::Commit() {
	hr = L"Jumplist.Tasks.Commit:";
	if (m_hadCommit) {
		m_hadCommit = false;
		hr += L"m_poc->DeleteList";
		hr = m_pcdl->DeleteList(m_appId.c_str());
	}
	IObjectArray *poaRemoved = nullptr;
	AutoRelease ar(poaRemoved);
	UINT cMinSlots;
	hr += L"m_pcdl->BeginList";
	hr = m_pcdl->BeginList(&cMinSlots, IID_PPV_ARGS(&poaRemoved));
	if (this->GetCount() > 0) {
		IObjectArray *poa;
		hr += L"m_poc->Query... &poa";
		hr = m_poc->QueryInterface<IObjectArray>(&poa);
		hr += L"m_pcdl->AddUserTasks";
		hr = m_pcdl->AddUserTasks(poa);
	}
	hr += L"m_pcdl->CommitList";
	hr = m_pcdl->CommitList();
	m_hadCommit = true;
}

void Jumplist::Tasks::GetAt(UINT i, JumplistTask *pTask) {
	hr = L"Jumplist.Tasks.GetAt:";
	IShellLinkW *psl = nullptr;
	hr += L"CoCreateInstance psl";
	hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&psl));
	hr += L"m_poc->GetAt";
	hr = m_poc->GetAt(i, IID_PPV_ARGS(&psl));
	*pTask = JumplistTask(psl);
}

UINT Jumplist::Tasks::GetCount() {
	hr = L"Jumplist.Tasks.GetCount";
	UINT count = 0;
	hr = m_poc->GetCount(&count);
	return count;
}

void Jumplist::Tasks::RemoveAt(UINT i) {
	hr = L"Jumplist.Tasks.RemoveAt";
	hr = m_poc->RemoveObjectAt(i);
}

// ... Jumplist::Tasks

// JumplistTask ...

JumplistTask::JumplistTask() {
	hr = L"JumplistTask init";
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_psl));
}

JumplistTask::JumplistTask(IShellLinkW *psl) : m_psl(psl) {}

JumplistTask::~JumplistTask() {
	m_psl->Release();
}

void JumplistTask::AddIntoObjectCollection(IObjectCollection *poc) const {
	hr = L"JumplistTask.AddIntoObjectCollection";
	hr = poc->AddObject(m_psl);
}

void JumplistTask::GetPath(LPWSTR pszFile, int cch, WIN32_FIND_DATAW *pfd, DWORD fFlags) {
	hr = L"JumplistTask.GetPath";
	hr = m_psl->GetPath(pszFile, cch, pfd, fFlags);
}

JumplistTask & JumplistTask::SetPath(LPCWSTR pszFile) {
	hr = L"JumplistTask.SetPath";
	hr = m_psl->SetPath(pszFile);
	return *this;
}

void JumplistTask::GetArguments(LPWSTR pszArgs, int cch) {
	hr = L"JumplistTask.GetArg...";
	hr = m_psl->GetArguments(pszArgs, cch);
}

JumplistTask & JumplistTask::SetArguments(LPCWSTR pszArgs) {
	hr = L"JumplistTask.SetArg...";
	hr = m_psl->SetArguments(pszArgs);
	return *this;
}

void JumplistTask::GetIconLocation(LPWSTR pszIconPath, int cch, int *piIcon) {
	hr = L"JumplistTask.GetIcon...";
	hr = m_psl->GetIconLocation(pszIconPath, cch, piIcon);
}

JumplistTask & JumplistTask::SetIconLocation(LPCWSTR pszIconPath, int iIcon) {
	hr = L"JumplistTask.SetIcon...";
	hr = m_psl->SetIconLocation(pszIconPath, iIcon);
	return *this;
}

void JumplistTask::GetTitle(LPWSTR pszTitle, UINT cch) {
	hr = L"JumplistTask.GetTitle:";
	IPropertyStore *pps = nullptr;
	AutoRelease ar(pps);
	hr += L"m_psl->Query... &pps";
	hr = m_psl->QueryInterface<IPropertyStore>(&pps);
	PROPVARIANT pv;
	AutoReleasePV arpv(&pv);
	hr += L"pps->GetValue";
	hr = pps->GetValue(PKEY_Title, &pv);
	hr += L"Prop...ToString pv";
	hr = PropVariantToString(pv, pszTitle, cch);
	hr += L"Prop...Clear pv";
	hr = PropVariantClear(&pv);
}

JumplistTask & JumplistTask::SetTitle(LPCWSTR pszTitle) {
	hr = L"JumplistTask.SetTitle:";
	IPropertyStore *pps = nullptr;
	AutoRelease ar(pps);
	hr += L"m_psl->Query... &pps";
	hr = m_psl->QueryInterface<IPropertyStore>(&pps);
	PROPVARIANT pv;
	AutoReleasePV arpv(&pv);
	hr += L"Init...FromString &pv";
	hr = InitPropVariantFromString(pszTitle, &pv);
	hr += L"pps->SetValue";
	hr = pps->SetValue(PKEY_Title, pv);
	hr += L"Prop...Clear pv";
	hr = PropVariantClear(&pv);
	hr += L"pps->Commit";
	hr = pps->Commit();
	return *this;
}

void JumplistTask::GetDescription(LPWSTR pszDesc, int cch) {
	hr = L"JumplistTask.GetDesc...";
	hr = m_psl->GetDescription(pszDesc, cch);
}

JumplistTask & JumplistTask::SetDescription(LPCWSTR pszDesc) {
	hr = L"JumplistTask.SetDesc...";
	hr = m_psl->SetDescription(pszDesc);
	return *this;
}

void JumplistTask::GetWorkingDirectory(LPWSTR pszDir, int cch) {
	hr = L"JumplistTask.GetWorkingDir...";
	hr = m_psl->GetWorkingDirectory(pszDir, cch);
}

JumplistTask & JumplistTask::SetWorkingDirectory(LPCWSTR pszDir) {
	hr = L"JumplistTask.SetWorkingDir...";
	hr = m_psl->SetWorkingDirectory(pszDir);
	return *this;
}

void JumplistTask::GetHotkey(WORD *pwHotkey) {
	hr = L"JumplistTask.GetHotkey";
	hr = m_psl->GetHotkey(pwHotkey);
}

JumplistTask & JumplistTask::SetHotkey(WORD wHotkey) {
	hr = L"JumplistTask.SetHotkey";
	hr = m_psl->SetHotkey(wHotkey);
	return *this;
}

void JumplistTask::GetShowCmd(int *piShowCmd) {
	hr = L"JumplistTask.GetShowCmd";
	hr = m_psl->GetShowCmd(piShowCmd);
}

JumplistTask & JumplistTask::SetShowCmd(int iShowCmd) {
	hr = L"JumplistTask.SetShowCmd";
	hr = m_psl->SetShowCmd(iShowCmd);
	return *this;
}

void JumplistTask::GetIDList(PIDLIST_ABSOLUTE *ppidl) {
	hr = L"JumplistTask.GetIDList";
	hr = m_psl->GetIDList(ppidl);
}

JumplistTask & JumplistTask::SetIDList(PCIDLIST_ABSOLUTE pidl) {
	hr = L"JumplistTask.SetIDList";
	hr = m_psl->SetIDList(pidl);
	return *this;
}

BOOL JumplistTask::IsSeparator() {
	hr = L"JumplistTask.IsSeparator(get):";
	IPropertyStore *pps = nullptr;
	AutoRelease ar(pps);
	hr += L"m_psl->Query... &pps";
	hr = m_psl->QueryInterface<IPropertyStore>(&pps);
	PROPVARIANT pv;
	AutoReleasePV arpv(&pv);
	hr += L"pps->GetValue";
	hr = pps->GetValue(PKEY_AppUserModel_IsDestListSeparator, &pv);
	hr += L"Prop...ToBoolean pv";
	BOOL ret;
	hr = PropVariantToBoolean(pv, &ret);
	hr += L"Prop...Clear pv";
	hr = PropVariantClear(&pv);
	return ret;
}

JumplistTask & JumplistTask::IsSeparator(BOOL val) {
	hr = L"JumplistTask.IsSeparator(set):";
	IPropertyStore *pps = nullptr;
	AutoRelease ar(pps);
	hr += L"m_psl->Query... &pps";
	hr = m_psl->QueryInterface<IPropertyStore>(&pps);
	PROPVARIANT pv;
	AutoReleasePV arpv(&pv);
	hr += L"Init...FromBoolean &pv";
	hr = InitPropVariantFromBoolean(val, &pv);
	hr += L"pps->SetValue";
	hr = pps->SetValue(PKEY_AppUserModel_IsDestListSeparator, pv);
	hr += L"Prop...Clear pv";
	hr = PropVariantClear(&pv);
	hr += L"pps->Commit";
	hr = pps->Commit();
	return *this;
}
