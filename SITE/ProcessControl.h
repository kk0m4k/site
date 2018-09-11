/*
  �� ���α׷��� MS-SQL���� �����ϴ� �� �������� ���� �޽��� ���
  SQL ������ ������� ������ ��쿡 �ش� DBMS ����(DB��,���̺��,
  �ʵ��, ������ ��)�� �̾�(select)���� ���� ���α׷��Դϴ�.

  �� ���α׷��� �����ϱ� ���ؼ� ª�� �ð��� MFC�� �н��Ͽ��⿡ ��
  �� �ϼ����� ���� �������� C++ ��Ÿ�� ���� ���� �� �ֽ��ϴ�.

  �ش� �ҽ��� ������ �����Ӱ� ������ �� ������, ������ �����մϴ�.
  
  ��, �ǿ��Ͽ��� ��쿡�� ��ŷ ���α׷��� ����� �� �ֱ� ������ �н�
  �뵵�θ� ����ؾ� �մϴ�.

  �ǿ��Ͽ��� ��쿡�� ������ å���� ���� �մϴ�.

  �ۼ��� : ������(franc3sco@paran.com)

*/
#ifndef __PROCESSCONTROL_H__
#define __PROCESSCONTROL_H__

#pragma once
#include <afxinet.h>
#include "SITEDoc.h"
#include <shlwapi.h>
#include "MssqlCode.h"
#include "SITEHeader.h"


// CProcessControl
class CSITEDoc;
class CProcessControl : public CWnd
{
	DECLARE_DYNAMIC(CProcessControl)

public:
	CProcessControl();
	virtual ~CProcessControl();
	CSITEDoc	*GetDocument();

protected:
	DECLARE_MESSAGE_MAP()

public:
	bool StartSQLInjectionAudit(DWORD category_type, DWORD category_flag=CATEGORY_COUNT, DWORD count = 1);
	bool HttpConnectionInitial(void);

public:
	CString m_szServerName;
	CString m_szObject;
	INTERNET_PORT m_dstPort;
	DWORD m_dwServiceType;
	DWORD m_dwHttpRequestFlags;


public:
	bool HttpParameterReconstruction(void);
	void InitialReferenceData(void);
	void HttpConnectionClose(void);

};

#endif

