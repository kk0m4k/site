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
// SITEDoc.h : CSITEDoc 클래스의 인터페이스
//
#ifndef __SITEDOC_H__
#define __SITEDOC_H__


#pragma once
#include "SITE.h"
#include "MainFrm.h"
#include "SITEHeader.h"
#include <shlwapi.h>
#include <afxinet.h>

class CMainFrame;
class CSITEDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CSITEDoc();
	DECLARE_DYNCREATE(CSITEDoc)

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);


// 구현입니다.
public:
	virtual ~CSITEDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	CMainFrame* GetMainFrame(void);
	DWORD m_DBMSApp;
	DWORD m_GetPost;
	DWORD m_VarType;
	DWORD m_Domain;
	DWORD m_dwDBrange;
	DWORD m_InjectionCategory;

	CString m_HttpUrlAddress;
	CString m_HttpParameter;
	CString m_HttpGetParameter;
	CString m_InjectionData;
	CString m_HttpHeader;

	CInternetSession *m_pHTTPSession;
	CHttpConnection * m_pServer;
	CHttpFile * m_pFile;

public:
	void HttpClose(void);
};

#endif