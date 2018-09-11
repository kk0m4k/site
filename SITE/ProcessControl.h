/*
  본 프로그램은 MS-SQL서버 연동하는 웹 페이지에 에러 메시지 기반
  SQL 인젝션 취약점이 존재할 경우에 해당 DBMS 정보(DB명,테이블명,
  필드명, 데이터 값)를 뽑아(select)내는 점검 프로그램입니다.

  이 프로그램을 구현하기 위해서 짧은 시간에 MFC를 학습하였기에 코
  드 완성도가 많이 떨어지고 C++ 스타일 답지 않을 수 있습니다.

  해당 소스는 누구나 자유롭게 배포할 수 있으며, 수정도 가능합니다.
  
  단, 악용하였을 경우에는 해킹 프로그램을 오용될 수 있기 때문에 학습
  용도로만 사용해야 합니다.

  악용하였을 경우에는 본인이 책임을 져야 합니다.

  작성자 : 김태훈(franc3sco@paran.com)

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

