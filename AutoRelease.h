#pragma once
#include <Windows.h>

class AutoRelease {
	IUnknown *m_p;
public:
	AutoRelease(IUnknown *p) : m_p(p) {}
	~AutoRelease() {
		if (m_p) { m_p->Release(); }
	}
};

class AutoReleasePV {
	PROPVARIANT *m_ppv;
public:
	AutoReleasePV(PROPVARIANT *ppv) : m_ppv(ppv) {}
	~AutoReleasePV() {
		PropVariantClear(m_ppv);
	}
};
