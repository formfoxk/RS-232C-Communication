
// Emulator-MFC.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CEmulatorMFCApp:
// �� Ŭ������ ������ ���ؼ��� Emulator-MFC.cpp�� �����Ͻʽÿ�.
//

class CEmulatorMFCApp : public CWinApp
{
public:
	CEmulatorMFCApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CEmulatorMFCApp theApp;