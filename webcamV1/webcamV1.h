
// webcamV1.h : webcamV1 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CwebcamV1App:
// �� Ŭ������ ������ ���ؼ��� webcamV1.cpp�� �����Ͻʽÿ�.
//

class CwebcamV1App : public CWinAppEx
{
public:
	CwebcamV1App();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CwebcamV1App theApp;
