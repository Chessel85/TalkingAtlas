
// SFRoamer.h : main header file for the SFRoamer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSFRoamerApp:
// See SFRoamer.cpp for the implementation of this class
//

class CSFRoamerDoc;

class CSFRoamerApp : public CWinApp
{
public:
	CSFRoamerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
		virtual BOOL OnIdle(LONG lCount);
private:
	CTime m_LastIdleTime;
};

extern CSFRoamerApp theApp;
