
// TA ����Ʒ.h : TA ����Ʒ Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTA����ƷApp:
// �йش����ʵ�֣������ TA ����Ʒ.cpp
//

class CTA����ƷApp : public CWinAppEx
{
public:
	CTA����ƷApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
protected:
	HMENU  m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFILEOPEN();

	DECLARE_MESSAGE_MAP()
};

extern CTA����ƷApp theApp;
