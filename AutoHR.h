#pragma once
#include <string>
#include <winerror.h>

class AutoHR {
	std::wstring m_taskName;
	HRESULT m_hr;
public:
	inline void operator=(HRESULT hr) {
		m_hr = hr;
		if (FAILED(hr)) { throw *this; }
	}
	inline void operator=(std::wstring const &taskName) {
		m_taskName = taskName;
	}
	inline void operator+=(std::wstring const &nextLine) {
		m_taskName += L"\n" + nextLine;
	}
	std::wstring const & GetTaskName() const {
		return m_taskName;
	}
	HRESULT GetHr() const {
		return m_hr;
	}
};
